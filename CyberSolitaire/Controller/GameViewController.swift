//
//  GameViewController.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 08.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import UIKit
import SpriteKit
import AVFoundation
//import Observable


let cardCreatedNotification = "cardCreatedNotification"
let pileCreatedNotification = "pileCreatedNotification"
let playSoundNotification   = "playSoundNotification"
let selectCardsNotification = "selectCardsNotification"
let selectPilesNotification = "selectPilesNotification"


class GameViewController: UIViewController, TouchesProtocolDelegate, UserInteractionProtocolDelegate {
    struct statics {
        static var cardAndEmptyPileSize = CGSize.zero
        static var timer: Timer? = nil
        static var durationTimer: TimeInterval = 0.0
        static var lastStarted: TimeInterval = 0.0
        static var restTime: TimeInterval = 0.0
        static var listOfActions = [Int]()
    }
    
    var scene: GameScene? = nil
    var scaleFactorForView: CGFloat = 1.0
    var audioPlayer: AVAudioPlayer = AVAudioPlayer()
    
    var game: SolitaireGame? = nil
    var lastPoint: CGPoint? = nil
    
    // UserInteraction control
    var listOfUserInteractions = [Int]()
    var userInteractionId = 0

    var zaehler = 0

    @IBOutlet weak var undoButton: UIButton!
    @IBOutlet weak var redoButton: UIButton!
    @IBOutlet weak var chooseAnotherGameButton: UIButton!
    @IBOutlet weak var ScoreValueLabel: UILabel!
    @IBOutlet weak var playableAreaView: UIView!
    
    
    @IBAction func ChooseAnotherGameButton(_ sender: Any) {
        log.verbose("GameVC dismiss")
        self.dismiss(animated: true) {
            self.view = nil
            self.scene = nil
            self.game = nil
        }
   }
    
    // überschreiben der (read-only) property undoManager
    // Hilfsvariable
    var myUndoManager: UndoManager!
    override var undoManager: UndoManager {
        get {
            return myUndoManager
        }
        set {
            myUndoManager = newValue
        }
    }
    
    override func awakeFromNib() {
        super.awakeFromNib()
        self.undoManager = UndoManager()
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Notifications einrichten
        let notificationCenter = NotificationCenter.default
        notificationCenter.addObserver(self, selector: #selector(GameViewController.createNodeForCard(_:)), name: NSNotification.Name(rawValue: cardCreatedNotification), object: nil)
        notificationCenter.addObserver(self, selector: #selector(GameViewController.createNodeForEmptyPile(_:)), name: NSNotification.Name(rawValue: pileCreatedNotification), object: nil)
        notificationCenter.addObserver(self, selector: #selector(GameViewController.playSound(_:)), name: NSNotification.Name(rawValue: playSoundNotification), object: nil)
        notificationCenter.addObserver(self, selector: #selector(GameViewController.selectCardsInView(_:)), name: NSNotification.Name(rawValue: selectCardsNotification), object: nil)
        notificationCenter.addObserver(self, selector: #selector(GameViewController.selectPilesInView(_:)), name: NSNotification.Name(rawValue: selectPilesNotification), object: nil)
        
        // ich fand es besser so
//        chooseAnotherGameButton.backgroundColor = UIColor.white
//        chooseAnotherGameButton.layer.cornerRadius = 5
//        chooseAnotherGameButton.layer.borderWidth = 1
//        chooseAnotherGameButton.layer.borderColor = UIColor.black.cgColor
        
        // redo ist gar nicht nötig
        redoButton.isHidden = true
        
        
        // die erste Scene einrichten
        // die Scene füllt den gesamten View aus
        let viewFrame = view.frame
        let sceneSize = CGSize(width: viewFrame.width, height: viewFrame.height)
        // nun bestimme ich die eigentliche Spielfläche
        // so bekomme ich die richtigen Größen raus
        playableAreaView.setNeedsLayout()
        playableAreaView.layoutIfNeeded()
        playableRect = playableAreaView.bounds
        // das der Ursprung des Koordinatensystems ist unten links
        longestDistance = CGPoint(x: playableRect.width, y: playableRect.height).length()
        speedFactor = durationForLongestDistance / longestDistance
        
        // das ist die Fläche in dots nicht in pixel
        // Achtung: bei SpriteKit ist der Koordinatenursprung in der linken unteren Ecke
        scene = GameScene(size:sceneSize)
        scene?.sceneDelegate = self

        // nachdem die zu bespielende Fläche festgelegt wurde, kann ein Spiel ausgewählt werden
        game = SolitaireGame(gameName: gameName, playingAreaRect: playableRect, undoManager: self.undoManager, userInteractionProtocolDelegate: self)

        // das Layout des Spiels ist jetzt fertig
        // die CardNodes und EmptyPileNodes wurden erzeugt und sind in den Arrays der scene abgelegt !!! deshalb scene vor game erzeugern !!!
        // nun kann die scene angezeigt werden
        
        let skView = self.view as! SKView

        // für den Test
//        skView.showsFPS = true
//        skView.showsNodeCount = true
        skView.ignoresSiblingOrder = true
        scene!.scaleMode = .aspectFill
        scene!.scaleMode = .fill
        //log.verbose("present Scene")
        
        // hiermit bekommt self.view eine strong reference auf den SKView
        // deshalb in viewDidDisAppear: self.view = nil, damit GameScene deallocated werden kann
        skView.presentScene(scene)
        
        // MARK: add Observer handler
        
        moveCard!.afterChange += {
            let cardNew = $1 as Card
            // finde den entsprechenden CardNode
            for cardNode in self.scene!.cardNodes! {
                if cardNode.cardId == cardNew.cardId {
                    cardNode.zPosition = cardNew.zPosition
                    let newPosition = self.convertPointFromModelToView(cardNew.position)
                    let oldPosition = cardNode.position
                    let distance = (newPosition - oldPosition).length()
                    let moveDuration = Double(distance * speedFactor)
                    // verbiete User Interaktionen
                    self.setUserInteractionDisabledForDuration(moveDuration+CardNode.statics.waitForDuration+cAdditionalTime, actionId: self.userInteractionId)
                    self.userInteractionId += 1
                    cardNode.moveCardToPosition(newPosition)
                    //log.verbose("card \(cardNew.cardId) moved to position: \(newPosition)")
                }
            }
        }
        
        moveAndTurnCard!.afterChange += {
            let cardNew = $1 as Card
            // finde den entsprechenden CardNode
            for cardNode in self.scene!.cardNodes! {
                if cardNode.cardId == cardNew.cardId {
                    cardNode.zPosition = cardNew.zPosition
                    let newPosition = self.convertPointFromModelToView(cardNew.position)
                    let oldPosition = cardNode.position
                    let distance = (newPosition - oldPosition).length()
                    let moveDuration = Double(distance * speedFactor)
                    // verbiete User Interaktionen
                    self.setUserInteractionDisabledForDuration(moveDuration + cWaitBetweenMoveAndFlip + 2*cHalfFlipDuration + cAdditionalTime, actionId: self.userInteractionId)
                    self.userInteractionId += 1
                    cardNode.moveAndTurnCardToPosition(newPosition)
                    //log.verbose("card \(cardNew.cardId) moved and turned to position: \(newPosition)")
                }
            }
        }
        
        
        turnAndMoveCard!.afterChange += {
            let cardNew = $1 as Card
            // finde den entsprechenden CardNode
            for cardNode in self.scene!.cardNodes! {
                if cardNode.cardId == cardNew.cardId {
                    cardNode.zPosition = cardNew.zPosition
                    let newPosition = self.convertPointFromModelToView(cardNew.position)
                    let oldPosition = cardNode.position
                    let distance = (newPosition - oldPosition).length()
                    let moveDuration = Double(distance * speedFactor)
                    // verbiete User Interaktionen
                    self.setUserInteractionDisabledForDuration(moveDuration + cWaitBetweenMoveAndFlip + 2*cHalfFlipDuration + cAdditionalTime, actionId: self.userInteractionId)
                    self.userInteractionId += 1
                    cardNode.turnAndMoveCardToPosition(newPosition)
                    //log.verbose("card \(cardNew.cardId) turned and moved to position: \(newPosition)")
                }
            }
        }
        
        turnCard!.afterChange += {
            let cardNew = $1 as Card
            // finde den entsprechenden CardNode
            for cardNode in self.scene!.cardNodes! {
                if cardNode.cardId == cardNew.cardId {
                    cardNode.zPosition = cardNew.zPosition
                    // verbiete User Interaktionen
                    self.setUserInteractionDisabledForDuration(2*cHalfFlipDuration + cAdditionalTime, actionId: self.userInteractionId)
                    self.userInteractionId += 1
                    cardNode.turnCard()
                    //log.verbose("card \(cardNew.cardId) turned")
                }
            }
        }
        
        repositionCard!.afterChange += {
            let cardNew = $1 as Card
            // finde den entsprechenden CardNode
            for cardNode in self.scene!.cardNodes! {
                if cardNode.cardId == cardNew.cardId {
                    cardNode.zPosition = cardNew.zPosition
                    let newPosition = self.convertPointFromModelToView(cardNew.position)
                    // verbiete User Interaktionen
                    self.setUserInteractionDisabledForDuration(cAdditionalTime, actionId: self.userInteractionId)
                    self.userInteractionId += 1
                    cardNode.position = newPosition
                    //log.verbose("card \(cardNew.cardId) repositioned")
                }
            }
        }
        
        
        shakeCard!.afterChange += {
            // als Gedankenstütze
            //let cardOld = $0 as Card
            let cardNew = $1 as Card
            // finde den entsprechenden CardNode
            for cardNode in self.scene!.cardNodes! {
                if cardNode.cardId == cardNew.cardId {
                    cardNode.zPosition = cardNew.zPosition
                    // verbiete User Interaktionen
                    self.setUserInteractionDisabledForDuration(4 * cShakeDuration + cShakeDuration/2.0, actionId: self.userInteractionId)
                    self.userInteractionId += 1
                    cardNode.shakeCard()
                    // unterrichte den Controller, damit der den Sound abspielen kann
                    NotificationCenter.default.post(name: Notification.Name(rawValue: playSoundNotification), object: self, userInfo: (NSDictionary(object: "incorrect", forKey: "soundName" as NSCopying) as! [AnyHashable: Any]))

                    //log.verbose("card \(cardNew.cardId) shaked")
                }
            }
        }
        
        
        cheatCard!.afterChange += {
            let cardNew = $1 as Card
            // finde den entsprechenden CardNode
            for cardNode in self.scene!.cardNodes! {
                if cardNode.cardId == cardNew.cardId {
                    cardNode.zPosition = cardNew.zPosition
                    // verbiete User Interaktionen
                    self.setUserInteractionDisabledForDuration(2*cHalfFlipDuration + cCheatDuration + cAdditionalTime, actionId: self.userInteractionId)
                    self.userInteractionId += 1
                    cardNode.cheatCard()
                    //log.verbose("card \(cardNew.cardId) cheated")
                }
            }
        }
        
        someChangedOnCard!.afterChange += {
            let cardNew = $1 as Card
            // finde den entsprechenden CardNode
            for cardNode in self.scene!.cardNodes! {
                if cardNode.cardId == cardNew.cardId {
                    cardNode.zPosition = cardNew.zPosition
                    //log.verbose("card \(cardNew.cardId) someChangedOn")
                }
            }
        }
        
        waitForDuration!.afterChange += {
            let newDuration = $1
            CardNode.setWaitForDuration(newDuration)
        }
 
        scoreValue! += {
            let newScore = $1
            let valueText = "\(newScore)"
            if let l = self.ScoreValueLabel {
               l.text = valueText
            }
        }
        
        // MARK: Ende Observer Handler

        self.game!.dealoutStartFormation()
        game!.gameState = .runningState
        
        log.verbose("ab jetzt kann gespielt werden. userInteraction = \(view.isUserInteractionEnabled) Zaehler = \(zaehler)")
        //logGameStart()
    }

    deinit {
        log.verbose("GameVC deinit")
    }
    
    override func viewWillDisappear(_ animated: Bool) {
        log.verbose("ich verschwinde")
        // Statistik verarbeiten
        if game!.isGameWon() {
            game!.gameStatistic.won += 1
        }
        else {
            game!.cumulatePlayTime()
            game!.gameStatistic.lost += 1
        }
        game!.gameStatistic.totalTime = game!.totalTimeGame
        updateStatisticsListFor(game!.gameName, with: game!.gameStatistic)
        
        let notificationCenter = NotificationCenter.default
        notificationCenter.removeObserver(self, name: NSNotification.Name(rawValue: cardCreatedNotification), object: nil)
        notificationCenter.removeObserver(self, name: NSNotification.Name(rawValue: pileCreatedNotification), object: nil)
        notificationCenter.removeObserver(self, name: NSNotification.Name(rawValue: playSoundNotification), object: nil)
        notificationCenter.removeObserver(self, name: NSNotification.Name(rawValue: selectCardsNotification), object: nil)
        notificationCenter.removeObserver(self, name: NSNotification.Name(rawValue: selectPilesNotification), object: nil)
        moveCard = nil
        moveAndTurnCard = nil
        turnAndMoveCard = nil
        turnCard = nil
        repositionCard = nil
        shakeCard = nil
        cheatCard = nil
        someChangedOnCard = nil
        waitForDuration = nil
    }
    
    // MARK: Notification Methoden
    
    @objc func createNodeForCard(_ notification:Notification) {
        let card = notification.object as! Card
        statics.cardAndEmptyPileSize = card.size
        //log.info("id: \(card.cardId)")
        let cardNode = CardNode(cardID: card.cardId, faceUp: card.faceUp)
        cardNode.touchesProtocolDelegate = self
        cardNode.userInteractionProtocolDelegate = self
        cardNode.position = convertPointFromModelToView(card.position)
        let scaleFactor = (playableRect.width * kCardWidthRTW/100.0) / cardNode.size.width
        cardNode.scaleFactor = scaleFactor
        cardNode.setScale(scaleFactor)
        cardNode.zPosition = CGFloat(card.zPosition)
        scene!.cardNodes!.append(cardNode)
    }
    
    @objc func createNodeForEmptyPile(_ notification:Notification) {
        let pile = notification.object as! Pile
        statics.cardAndEmptyPileSize = pile.pileEmptySize
        let pileEmptyNode = PileEmptyNode(pileEmptyID: pile.indexEmptyPile, pileID: pile.pileId)
        pileEmptyNode.delegate = self
        pileEmptyNode.position = convertPointFromModelToView(pile.pilePosition)
        let scaleFactor = (playableRect.width * kCardWidthRTW/100.0) / pileEmptyNode.size.width
        pileEmptyNode.setScale(scaleFactor)
        pileEmptyNode.zPosition = 0
        //log.info("pile: \(pile.pileId) indexEmptyPile: \(pile.indexEmptyPile) location: \(pileEmptyNode.position)")
        scene!.pileEmptyNodes!.append(pileEmptyNode)
    }
    
    @objc func playSound(_ notification:Notification) {
        let dict = notification.userInfo as Dictionary?
        let soundName = dict?["soundName"] as! String
        var soundURL: URL? = nil
        switch soundName {
        case "shuffle":
            soundURL = URL(fileURLWithPath: Bundle.main.path(forResource: "shuffle", ofType: "caf")!)
        case "plus":
            soundURL = URL(fileURLWithPath: Bundle.main.path(forResource: "plus", ofType: "caf")!)
        case "minus":
                soundURL = URL(fileURLWithPath: Bundle.main.path(forResource: "minus", ofType: "caf")!)
        case "select":
            soundURL = URL(fileURLWithPath: Bundle.main.path(forResource: "select", ofType: "caf")!)
        case "bing":
            soundURL = URL(fileURLWithPath: Bundle.main.path(forResource: "bing", ofType: "caf")!)
        case "clapping":
            soundURL = URL(fileURLWithPath: Bundle.main.path(forResource: "clapping", ofType: "caf")!)
        case "gallop":
            soundURL = URL(fileURLWithPath: Bundle.main.path(forResource: "gallop", ofType: "caf")!)
        case "put":
            soundURL = URL(fileURLWithPath: Bundle.main.path(forResource: "put", ofType: "caf")!)
        case "incorrect":
            soundURL = URL(fileURLWithPath: Bundle.main.path(forResource: "incorrect", ofType: "caf")!)
        default:
            log.error("sound not implemented")
        }
        if soundURL != nil && playTones {
            // Achtung: der audioPlayer darf nicht in dieser Methode lokal ezeugt werden, sonst wird er deallociiert
            // bevor er den Sound abspielen kann !!!
            audioPlayer = try! AVAudioPlayer(contentsOf: soundURL!)
            audioPlayer.prepareToPlay()
            audioPlayer.play()
            //log.info("sound: \(soundName) played")
        }
    }
    
    @objc func selectCardsInView(_ notification:Notification) {
        let dict = notification.userInfo as Dictionary?
        for key in (dict?.keys)! {
            let keyStr = key as! String
            if keyStr == "selectCards" {
                let selectedCards = dict?[key] as! [Card]
                for card in selectedCards {
                    selectCardInView(card, showSelected: true)
                }
                break
             }
            if keyStr == "unselectCards" {
                let selectedCards = dict?[key] as! [Card]
                for card in selectedCards {
                    selectCardInView(card, showSelected: false)
                }
                break
            }
        }
    }
    
    func selectCardInView(_ card: Card, showSelected: Bool) {
        // finde den entsprechenden CardNode
        for cardNode in self.scene!.cardNodes! {
            if cardNode.cardId == card.cardId {
                cardNode.selectCard(showSelected) 
            }
        }

    }
    
    @objc func selectPilesInView(_ notification:Notification) {
        let dict = notification.userInfo as Dictionary?
        for key in (dict?.keys)! {
            let keyStr = key as! String
            if keyStr == "selectPiles" {
                let selectedPiles = dict?[key] as! [Pile]
                for pile in selectedPiles {
                    selectPileInView(pile, showSelected: true)
                }
                break
            }
            if keyStr == "unselectPiles" {
                let selectedPiles = dict?[key] as! [Pile]
                for pile in selectedPiles {
                    selectPileInView(pile, showSelected: false)
                }
                break
            }
        }
    }

    func selectPileInView(_ pile: Pile, showSelected: Bool) {
        if pile.isPileEmpty() {
            for pileEmptyNode in scene!.pileEmptyNodes! {
                if pileEmptyNode.pileId == pile.pileId {
                    //TODO: implementieren
//                    pileEmptyNode.selectEmptyPile(showSelected) {
//                        // erlaube wieder User Interaktionen
//                        self.setUserInteractionEnabled(true)
//                    }
                }
            }
        }
        else {
            for card in pile.cards {
                // finde den entsprechenden CardNode
                for cardNode in self.scene!.cardNodes! {
                    if cardNode.cardId == card.cardId {
                        cardNode.selectCardForSelectingPile(showSelected)
                    }
                }
            }
        }
    }
    
    func convertPointFromModelToView(_ point: CGPoint) -> CGPoint {
        // das gesamte Spiel ist auf das playableRect (die Spielfläche) fokusiert
        // alle Koordinaten im Model beziehen sich auf diese Spielfläche mit dem Nullpunkt in der linken oberen Ecke
        // mit wachsendem x nach links und wachsendem y nach unten
        // dagegen hat die Spielfläche im SpriteKit ihren Ursprung in der linken unteren Ecke und es geht mit wachsendem y nach oben
        // ausserdem ist die Spielfläche nur ein Teil der Scene
        // Außerdem wird der AnchorPoint in die Mitte der Karte verlegt
        let dx = statics.cardAndEmptyPileSize.width / 2.0
        let dy = statics.cardAndEmptyPileSize.height / 2.0
        return CGPoint(x: point.x + dx, y: playableRect.height - point.y - dy)
    }

    // MARK: TouchesProtocolDelegate
    
    func tapOnGameWithDictionary(_ dict: Dictionary<String, Int>, locationInScene: CGPoint) {
        var card: Card? = nil
        var pile: Pile? = nil
        //log.debug("tapOnGame")
        // was wurde angetippt?
        if let index = dict.index(forKey: "Card") {
            let keyValuePair = dict[index]
            card = game!.findCardForId(keyValuePair.1)
            game!.playMove!.invokeForTapOnCard(card!)
            return
        }
        if let index = dict.index(forKey: "PileEmpty") {
            let keyValuePair = dict[index]
            pile = game!.findPileForId(keyValuePair.1)
            game!.playMove!.invokeForTapOnEmptyPile(pile!)
            return
        }
        if let index = dict.index(forKey: "Game") {
            let keyValuePair = dict[index]
            assert(keyValuePair.1 == -1, "dumm gelaufen")
            game!.playMove!.invokeForTapInsidePlayArea()
            return
        }
    }
    
    // MARK: UserInteractionDelegate
    
    func setUserInteractionDisabledForDuration(_ duration: TimeInterval, actionId: Int) {
        zaehler = zaehler + 1
//        if zaehler % 105 == 0 {
//            log.verbose("Zaehler = \(zaehler)")
//        }
        // userInteraction soll verboten werden
        // es könnte sein, dass dies schon von anderer Stelle gefordert wurde
        if let runningTimer = statics.timer {
            // es läuft noch ein Timer, wir müssen prüfen, ob die verbleibende Zeit ausreicht oder ob wir die Zeit verlängern müssen
            // berechne die Restzeit
            let currentTime = Date.timeIntervalSinceReferenceDate
            let restTime = statics.restTime - (currentTime - statics.lastStarted)
            // falls die Restzeit kleiner als die neue Zeit ist
            if restTime < duration {
                // halte den timer an
                runningTimer.invalidate()
                statics.timer = nil
                // der Timer muss mit der neuen Zeit erneut gestartet werden
            }
            else {
                // andernfalls kann der Timer weiterlaufen
                statics.lastStarted = Date.timeIntervalSinceReferenceDate
                statics.restTime = restTime
                //log.debug("disabled for \(restTime) invoked with \(duration)")
                return
            }
        }
        // verbiete userInteractions
        disableUndoRedo()
        view.isUserInteractionEnabled = false
        // es läuft definitiv kein Timer; starte Timer
        // merke die Zeit
        statics.lastStarted = Date.timeIntervalSinceReferenceDate
        statics.durationTimer = duration
        statics.restTime = duration
        // starte den Timer; der Handler muss versuchen die Interactions wieder zu erlauben
        statics.timer = Timer.scheduledTimer(timeInterval: duration, target: self, selector: #selector(GameViewController.setUserInteractionEnabled), userInfo: nil, repeats: false)
        //log.debug("disabled for \(duration)")
    }
    
    @objc func setUserInteractionEnabled() {
        // der Timer muss abgelaufen sein
        if let timer = statics.timer {
            if timer.isValid {
                timer.invalidate()
                statics.timer = nil
            }
            else {
                log.error("timer not valid")
            }
        }
        // erlaube userInteractions
        enableUndoRedo()
        view.isUserInteractionEnabled = true
        //log.debug("userInteractions enabled. Zaehler = \(zaehler)")
    }
    
    func enableUndoRedo() {
        if permitUndoRedo {
            undoButton.isEnabled = undoManager.canUndo
            redoButton.isEnabled = undoManager.canRedo
            //log.debug("UndoButton isEnabled = \(undoButton.isEnabled)")
        }
    }
    
    func disableUndoRedo() {
        undoButton.isEnabled = false
        redoButton.isEnabled = false
        //log.debug("")
    }
    
    // MARK: Button Handler
    
    
    @IBAction func Undo(_ sender: UIButton) {
        if permitUndoRedo {
            //log.messageOnly("Undo")
            if let tmpGame = game {
                tmpGame.resetzPositions()
                self.undoManager.undo()
                game!.evaluateScore()
            }
        }
    }
    
    @IBAction func Redo(_ sender: UIButton) {
        if permitUndoRedo {
            //log.messageOnly("Redo")
            if let tmpGame = game {
                tmpGame.resetzPositions()
                self.undoManager.redo()
                game!.evaluateScore()
            }
        }
    }
    
    // MARK: overrides
    
    override var shouldAutorotate : Bool {
        return true
    }

    override var supportedInterfaceOrientations : UIInterfaceOrientationMask {
        return UIInterfaceOrientationMask.landscape
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Release any cached data, images, etc that aren't in use.
    }

    override var prefersStatusBarHidden : Bool {
        return true
    }
    
    // MARK: logging activities

    func logGameStart() {
        //log.verbose("\(self.game!.gameName) started")
        //log.messageOnly("Stapel und Karten nach Auslegen")
        for pile in game!.gamePiles! {
            //log.messageOnly("Stapel \(pile.pileType.description())(\(pile.pileId)):")
            for card in pile.cards {
                if card.faceUp {
                    //log.messageOnly("\t  sichtbar: \(card.name)")
                }
                else {
                    //log.messageOnly("\tunsichtbar: \(card.name)")
                }
            }
        }
        //log.messageOnly("Ende: Auslegen")
    }

}

