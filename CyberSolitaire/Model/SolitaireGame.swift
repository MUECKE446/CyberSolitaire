//
//  SolitaireGame.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 08.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import Foundation
import UIKit
import SwiftyPlistManager

// FIXME: comparison operators with optionals were removed from the Swift Standard Libary.
// Consider refactoring the code to use the non-optional operators.
fileprivate func < <T : Comparable>(lhs: T?, rhs: T?) -> Bool {
  switch (lhs, rhs) {
  case let (l?, r?):
    return l < r
  case (nil, _?):
    return true
  default:
    return false
  }
}


enum SKCardMoves {
    case
    move,
    moveAndTurn,
    turnAndMove,
    turn,
    reposition,
    shake,
    cheat,
    someChanged
}

// Observables
var moveCard : Observable? = Observable(Card(cardID: 1))  // initialize with dummy Card
var moveAndTurnCard : Observable? = Observable(Card(cardID: 1))  // initialize with dummy Card
var turnAndMoveCard : Observable? = Observable(Card(cardID: 1))  // initialize with dummy Card
var turnCard : Observable? = Observable(Card(cardID: 1))  // initialize with dummy Card
var repositionCard : Observable? = Observable(Card(cardID: 1))  // initialize with dummy Card
var shakeCard : Observable? = Observable(Card(cardID: 1))  // initialize with dummy Card
var cheatCard : Observable? = Observable(Card(cardID: 1))  // initialize with dummy Card
var someChangedOnCard : Observable? = Observable(Card(cardID: 1))  // initialize with dummy Card
var waitForDuration : Observable? = Observable(Double(0.0))

var scoreValue : Observable? = Observable(Int(-1))

let kCardRatio: CGFloat = 1.4; // Verhältnis Höhe/Breite einer Karte

// RTW bedeutet: Relative To Width   RTH bedeutet: Relative To Height; alle Werte in %
let kCardWidthRTW: CGFloat = 8.7;
let kCardHeightRTH: CGFloat = 17.5;
let kLeftMarginRTW: CGFloat = 0.5;
let kRightMarginRTW: CGFloat = 0.5;
let kTopMarginRTH: CGFloat = 0.7;
let kBottomMarginRTH: CGFloat = 0.7;
let kxGap1RTW: CGFloat = 1.5;
let kxGap2RTW: CGFloat = 0.2;
let kyGap1RTH: CGFloat = 2.1;

let kMaxLastTimeCumulated = TimeInterval(exactly: 20)!        // time in seconds

var highestzPosition = CGFloat(1.0)

enum TypeOfScoring: Int {
    case
    scoringTypeNA = 0,
    scoringSequenceInSuitAndFoundation, // jede Karte in Farbfolge + ablegen zählt 1 Punkt + Ablage auf Foundation
    scoringSequenceNoColorAndFoundation, // jede Karte in Folge + ablegen zählt 1 Punkt + Ablage auf Foundation
    scoringCardOnFoundation,            // jede Karte auf Ablagestapel zählt 1 Punkt
    scoringSequenceInSuitAndKingUp      // jede Karte in Farbfolge und König nach oben zählt 1 Punkt
    
    func description() -> String {
        switch self {
        case .scoringTypeNA: return "notAvaiable"
        case .scoringSequenceInSuitAndFoundation: return "ScoringSequenceInSuitAndFoundation"
        case .scoringSequenceNoColorAndFoundation: return "ScoringSequenceNoColorAndFoundation"
        case .scoringCardOnFoundation: return "ScoringCardOnFoundation"
        case .scoringSequenceInSuitAndKingUp: return "ScoringSequenceInSuitAndKingUp"
        }
    }
}

enum GameState : Int {
    case
    layoutState = 0,        // das Spiel befindet sich in der Layoutphase
    runningState,           // das Spiel findet statt
    pausingState,           // das Spiel macht Pause
    completedState          // das Spiel wurde beendet (Spielstände, Statistik usw.)
    
    func description() -> String {
        switch self {
        case .layoutState:  return "layoutState"
        case .runningState: return "runningState"
        case .pausingState: return "pausingState"
        case .completedState: return "completedState"
        }
    }
    
    
}

@objc class CardMovesDescription : NSObject {
    weak var card: Card? = nil
    weak var fromPile: Pile? = nil
    weak var toPile: Pile? = nil
    var action: SKCardMoves? = nil
    
    init(card: Card, fromPile: Pile, toPile: Pile, action: SKCardMoves) {
        self.card = card
        self.fromPile = fromPile
        self.toPile = toPile
        self.action = action
    }
    deinit {
        log.verbose("CardMovesDescription deinit")
    }
    
}

let gameListName = "CyberSolitaireGames"


class SolitaireGame: NSObject {
    
    let appDelegate = UIApplication.shared.delegate as! AppDelegate

    weak var userInteractionProtocolDelegate: UserInteractionProtocolDelegate?

    var startPile: Pile? = nil          // der Kartenstapel, der beim Start erzeugt wird und von dem die Karten auf die einzelnen Kartenstapel beim Start verteilt werden
    var gamePiles: [Pile]? = []          // alle Kartenstapel des Spiels
    let gameName : String
    let gameGroup: String
    let scoringType: TypeOfScoring
    var maxPoints: Int
    
    var score = 0

    var undoManager: UndoManager
    
    //TODO: es muss überlegt werden, wie diese Eigenschaft gesteuert wird
    var cheatAllowed = permitCheating
    
    var gameState = GameState.layoutState
    var playMove: PlayMove? = nil
    
    var movements: [(SKCardMoves,Card,Double)] = []
    
    let playingAreaRect: CGRect
    let playingAreaSize: CGSize
    let cardBasicWidth: CGFloat
    let cardBasicHeight: CGFloat
    let cardSize: CGSize
    
    // MARK: Daten für die Statistik
    
    var gameStatistic: GameStatistic
    
    // Zeiten für das gesamte Spiel
    var totalTimeGame = TimeInterval()
    var lastTimeCumulated = Date()
    
    
// MARK: initializers
    
    init(gameName: String, playingAreaRect: CGRect, undoManager: UndoManager, userInteractionProtocolDelegate: UserInteractionProtocolDelegate) {
        self.gameName = gameName
        self.undoManager = undoManager
        
        // hole Statistik
        self.gameStatistic = getGameStatisticFor(gameName)!
        
        // ermittle die Geometrie
        self.playingAreaRect = playingAreaRect
        self.playingAreaSize = playingAreaRect.size
        
        self.cardBasicWidth = playingAreaSize.width*kCardWidthRTW/100.0
        self.cardBasicHeight = kCardRatio * cardBasicWidth
        self.cardSize = CGSize(width: cardBasicWidth, height: cardBasicHeight)
        
        self.userInteractionProtocolDelegate = userInteractionProtocolDelegate
        
        moveCard = Observable(Card(cardID: 1))  // initialize with dummy Card
        moveAndTurnCard = Observable(Card(cardID: 1))  // initialize with dummy Card
        turnAndMoveCard = Observable(Card(cardID: 1))  // initialize with dummy Card
        turnCard = Observable(Card(cardID: 1))  // initialize with dummy Card
        repositionCard = Observable(Card(cardID: 1))  // initialize with dummy Card
        shakeCard = Observable(Card(cardID: 1))  // initialize with dummy Card
        cheatCard = Observable(Card(cardID: 1))  // initialize with dummy Card
        someChangedOnCard = Observable(Card(cardID: 1))  // initialize with dummy Card
        waitForDuration = Observable(Double(0.0))
        scoreValue = Observable(Int(-1))
        
        let gameLayout = GameLayout.getGameLayout(gameName)
        // erzeuge die leeren Kartenstapel aus dem Layout
        var id = 0
        for pileLayout in gameLayout!.pileLayouts {
        //for i in 0 ..< gameLayout!.numberOfPiles {
        //    let pileLayout = gameLayout!.pileLayouts[i] as PileLayout
            let pile = Pile(pileID: id, pileType: pileLayout.pileType!, overlapType: pileLayout.overlapType!, userSelectAndMoveType: pileLayout.userSelectAndMoveType!,
                depositFromUserType: pileLayout.depositFromUserType!,
                depositIfEmptyType: pileLayout.depositIfEmptyType!,
                permittedToPlayType: pileLayout.permittedToPlayType!,
                basicCardType: pileLayout.basicCardType!,
                dealingFromStockType: pileLayout.dealingFromStockType!,
                redealType: pileLayout.redealType!,
                numberPerMove: pileLayout.numberPerMove,
                            indexForEmptyPileImage: pileLayout.indexForEmptyPileImage, numberOfCardsAtStart: pileLayout.numberOfCardsAtStart,
                            dealOrderAtStart: pileLayout.dealOrderAtStart, faceAtStart: pileLayout.faceAtStart!)
            
            // Achtung: die Positionswerte im Layout sind Prozentwerte
            let xPilePosition = playingAreaSize.width * pileLayout.posX / 100.0
            let yPilePosition = playingAreaSize.height * pileLayout.posY / 100.0
            pile.pilePosition = CGPoint(x: xPilePosition, y: yPilePosition)
            pile.pileSize = cardSize
            pile.pileEmptySize = cardSize
            pile.maxPileHeight = cardBasicHeight
            
            pile.undoManager = self.undoManager
            gamePiles!.append(pile)
            // unterrichte den Controller, damit der einen View für die leeren Piles (PileEmptyNode) anlegen kann
            NotificationCenter.default.post(name: Notification.Name(rawValue: pileCreatedNotification), object: pile)
            id += 1
        }
        
        self.gameGroup = gameLayout!.gameGroup
        self.scoringType = gameLayout!.scoringType!
        self.maxPoints = gameLayout!.maxPoints
        

        super.init()      // nur wenn von NSObject abgeleitet wurde
        
        appDelegate.currentActiveGame = self
        
        // jetzt werden die maximalen Höhen der Piles berechnet
        // bei jedem Pile wird untersucht (falls es ein Pile mit veränderlicher Größe ist ->overlapped)
        // ob sich ein anderer Pile unter ihm befindet.
        self.computeMaxPileHeights()
        
        // ein Kartenstapel im Off zum Austeilen der Karten ist bestimmt nicht schlecht
        startPile = createStartPile(gameLayout!.numberOfDecks)
        
        self.playMove = PlayMove(game: self)

    }
    
    deinit {
 //       appDelegate.currentActiveGame = nil

        undoManager.removeAllActions()
        gamePiles?.removeAll()
        startPile = nil
        gamePiles = nil
        log.verbose("SolitaireGame deinit")
    }
    
    // MARK: Aktionen während der LayoutPhase

    func computeMaxPileHeights() {
        // berechnet die maximalen Höhen über/untereinander liegender Piles
        // bei jedem Pile wird untersucht (falls es ein Pile mit veränderlicher Größe ist ->overlapped)
        // ob sich ein anderer Pile unter ihm befindet.
        for pile in gamePiles! {
            if pile.overlapType == TypeOfOverlap.downOverlapped {
                // stelle fest, ob sich ein Pile unterhalb befindet; durchsuche alle anderen Piles
                var pileShortestToPile: Pile? = nil
                for anotherPile in gamePiles! {
                    // schliesse den gerade betrachteten Pile aus
                    if anotherPile.isEqual(pile) {
                        continue
                    }
                    let anotherPileLeftX = anotherPile.pilePosition.x;
                    let anotherPileRightX = anotherPile.pilePosition.x + anotherPile.pileEmptySize.width;
                    let pileLeftX = pile.pilePosition.x;
                    if (pileLeftX >= anotherPileLeftX) && (pileLeftX <= anotherPileRightX) {
                        // dieser ist ein Anwärter: ist er aber der nächstgelegene
                        if pileShortestToPile == nil {
                            // merke diesen vor
                            pileShortestToPile = anotherPile
                        }
                        else {
                            // es gibt schon einen Anwärter
                            if anotherPile.pilePosition.y < pileShortestToPile?.pilePosition.y {
                                // dieser Pile ist näher dran; selbst wenn zwei Piles darunter auf gleicher Höhe liegen, ist es egal, welcher für die Berechnung genommen wird
                                pileShortestToPile = anotherPile
                            }
                        }
                    }
                }
                // hier haben wir einen darunter liegenden Pile gefunden oder auch nicht
                if pileShortestToPile != nil {
                    // die maximale Höhe des Piles reicht bis zum darunterliegenden - einer kleinen Lücke
                    let heightBetweenPiles = pileShortestToPile!.pilePosition.y - pile.pilePosition.y;
                    pile.maxPileHeight = heightBetweenPiles - playingAreaSize.height * kyGap1RTH/100.0;
                }
                else
                {
                    // die maximale Höhe des Piles reicht bis zur unteren Kante der Spielfläche - einer kleinen Lücke
                    let heightBetweenBottom = playingAreaSize.height - pile.pilePosition.y;
                    pile.maxPileHeight = heightBetweenBottom - playingAreaSize.height * kyGap1RTH/100.0;
                }
            }
        }
    }

    func createStartPile(_ numberOfDecks: Int) -> Pile {
        // erzeugt einen Pile mit allen Karten des Spiels; der Pile liegt ausserhalb der Spielfläche
        // der Controller wird informiert, damit er Views für die erzeugten Karten erzeugen kann
        let pile = Pile(pileID: -1, pileType: TypeOfPile.stock, overlapType: TypeOfOverlap.notOverlapped, userSelectAndMoveType: TypeOfUserSelectAndMove.typeOfUserSelectAndMoveNA ,
            depositFromUserType: TypeOfDepositFromUser.typeOfDepositFromUserNA,
            depositIfEmptyType: TypeOfDepositIfEmpty.typeOfDepositIfEmptyNA,
            permittedToPlayType: TypeOfPermittedToPlay.typeOfPermittedToPlayNA,
            basicCardType: TypeOfBasicCard.typeOfBasicCardNA,
            dealingFromStockType: TypeOfDealingFromStock.typeOfDealingFromStockNA,
            redealType: TypeOfRedeal.typeOfRedealNA,
            numberPerMove: -1,
            indexForEmptyPileImage: 2, numberOfCardsAtStart: 0, dealOrderAtStart: 0, faceAtStart: FaceAtStart.allFaceDown)
        
        pile.pilePosition = CGPoint(x: (playingAreaSize.width-cardSize.width)/2.0, y: playingAreaSize.height+cardSize.height)
        pile.pileSize = cardSize
        pile.pileEmptySize = cardSize
        pile.maxPileHeight = cardBasicHeight
        
        pile.undoManager = self.undoManager
        
        // erzeuge die Karten
        for i in 1 ... numberOfDecks * 52 {
            let card = Card(cardID: i)
            card.faceUp = false
            card.visible = true
            
            card.position = pile.pilePosition
            card.size = cardSize
            pile.putCard(card)
            // unterrichte den Controller, damit der einen View für die Karte anlegen kann
            NotificationCenter.default.post(name: Notification.Name(rawValue: cardCreatedNotification), object: card)
        }
        return pile
    }

    func dealoutStartFormation() {
        // das Austeilen der Karten soll nicht mehr rückgängig gemacht werden können, deshalb auskommentiert
        //self.undoManager.beginUndoGrouping()
        // startPile gut durchmischen
        startPile!.shuffle()
        //log.verbose("gemischt")
        delay(0.1, closure: {
            self.dealoutStartFormationNextStep()
        })

//        self.dealoutStartFormationNextStep()

    }
    func dealoutStartFormationNextStep() {
        //log.verbose("next Step")
        // auf welche Stapel muss beim Start gegeben werden
        // nachdem alle Stapel indentifiziert sind, müssen sie sortiert werden
        // diesmal versuche ich eine Lösung mit einem Dictionary; das erspart mit hoffentlich das Sortieren
        
        // Wartezeit beim Moven zurücksetzen
        waitForDuration! <- 0.0
        
        resetzPositions()
        
        var cardsWithAction : [CardMovesDescription]? = [CardMovesDescription]()
        
        var pilesWithDealOrderAtStart: [Int:Pile] = [:]
        for pile in self.gamePiles! {
            if pile.dealOrderAtStart > 0 {
                pilesWithDealOrderAtStart[pile.dealOrderAtStart] = pile
            }
        }
        let maxDealOrder = pilesWithDealOrderAtStart.count
        // solange geben, bis alle Stapel die vorgegebene Anzahl der Karten beim Start erreicht haben
        var cardsToDeal = 0
        for i in 1 ... maxDealOrder {
            let pile = pilesWithDealOrderAtStart[i] as Pile?
            cardsToDeal += (pile?.numberOfCardsAtStart)!
        }
        
        var dealOrder = 1
        for _ in 1 ... cardsToDeal {
            let pile = pilesWithDealOrderAtStart[dealOrder] as Pile?
            if pile?.numberOfCards < pile?.numberOfCardsAtStart {
                // gib die nächste Karte
                assert(pile!.pileType == TypeOfPile.tableau, "bisher wird nur auf Tableaus ausgeteilt")
                switch pile!.faceAtStart {
                case .lastFaceUp:
                    if (pile?.numberOfCards)! == (pile?.numberOfCardsAtStart)! - 1 {
                        // es ist die letzte Karte, die auf diesem Stapel gegeben wird
                        // die letzte Karte wird gegeben und muss umgedreht werden
                        let cardBefore = pile?.getLastCard()!
                        // die Karte davor zum Schummeln markieren
                        cardBefore?.toggleMarkForCheat()
                        cardsWithAction!.append(CardMovesDescription(card: cardBefore!, fromPile: startPile!, toPile: pile!, action: .cheat))
                        let card = startPile!.getLastCard()!
                        // drehe die Karte
                        card.faceUp = !card.faceUp
                        pile?.putCard(card)
                        startPile!.removeLastCard()
                        card.zPosition = highestzPosition
                        highestzPosition += 1
                        cardsWithAction!.append(CardMovesDescription(card: card, fromPile: startPile!, toPile: pile!, action: .moveAndTurn))
                        movements.append((SKCardMoves.moveAndTurn,card,0.0))
                    }
                    else {
                        // nehme die Karte vom startPile und verschiebe sie zum pile
                        let card = startPile!.getLastCard()!
                        pile?.putCard(card)
                        startPile!.removeLastCard()
                        card.zPosition = highestzPosition
                        highestzPosition += 1
                        cardsWithAction!.append(CardMovesDescription(card: card, fromPile: startPile!, toPile: pile!, action: .move))
                        movements.append((SKCardMoves.move,card,0.0))
                    }
                default:
                    //TODO:  die anderen Fälle müssen implementiert werden, falls sie auftreten
                    log.error("dieser Fall muss noch implementiert werden")
                }
            }
            dealOrder += 1
            if dealOrder > maxDealOrder {
                dealOrder = 1
            }
        }

        // Wartezeit beim Moven zurücksetzen
        waitForDuration! <- 0.0
        
        // alle verbleibenden Karten kommen auf den Stock
        let fromPile = startPile!
        let toPile = findStock()!
        let restOfCards = startPile!.numberOfCards
        for _ in 1 ... restOfCards {
            let card = startPile!.getLastCard()!
            findStock()!.putCard(card)
            startPile!.removeLastCard()
            card.zPosition = highestzPosition
            highestzPosition += 1
            cardsWithAction!.append(CardMovesDescription(card: card, fromPile: fromPile, toPile: toPile, action: .move))
            movements.append((SKCardMoves.move,card,0.0))
        }
        // das Austeilen der Karten soll nicht mehr rückgängig gemacht werden können, deshalb auskommentiert
        //undoRedoCardActions(cardsWithAction!)
        logGameStart()
        makeMovements()
        
        scoreValue! <- score
        evaluateScore()
        // das Austeilen der Karten soll nicht mehr rückgängig gemacht werden können, deshalb auskommentiert
        //self.undoManager.endUndoGrouping()
        cardsWithAction!.removeAll()
        cardsWithAction = nil
    }
    
    @objc func undoRedoCardActions(_ cardsWithAction: [CardMovesDescription]) {
        let theProxy: (AnyObject) = self.undoManager.prepare(withInvocationTarget: self) as (AnyObject)
        var tmpCardsWithAction = [CardMovesDescription]()
        
        if (undoManager.isUndoing || undoManager.isRedoing) {
            //for (var i = cardsWithAction.count - 1; i>=0; i -= 1) {
            for i in (0...cardsWithAction.count-1).reversed() {
                let card = cardsWithAction[i].card!
                let fromPile = cardsWithAction[i].toPile!
                let toPile = cardsWithAction[i].fromPile!
                var action = cardsWithAction[i].action!
                switch action {
                case .moveAndTurn:
                    action = .turnAndMove
                    card.faceUp = !card.faceUp
                    toPile.putCard(card)
                    fromPile.removeLastCard()
                    movements.append((SKCardMoves.turnAndMove,card,0.0))
                case .turnAndMove:
                    action = .moveAndTurn
                    card.faceUp = !card.faceUp
                    toPile.putCard(card)
                    fromPile.removeLastCard()
                    movements.append((SKCardMoves.moveAndTurn,card,0.0))
                case .move:
                    toPile.putCard(card)
                    fromPile.removeLastCard()
                    if undoManager.isUndoing {
                        movements.append((SKCardMoves.move,card,cWaitBetweenFlipAndMove + 2*cHalfFlipDuration + cAdditionalTime))
                    }
                    else {
                        movements.append((SKCardMoves.move,card,0.0))
                    }
                case .cheat:
                    card.toggleMarkForCheat()
                default:
                    assert(false, "nicht vorgesehen")
                    
                }
                tmpCardsWithAction.append(CardMovesDescription(card: card, fromPile: fromPile, toPile: toPile, action: action))
            }
            if undoManager.isUndoing {
                makeMovements(true)
            }
            else {
                makeMovements()
            }
            theProxy.undoRedoCardActions(tmpCardsWithAction)
            return
        }
        theProxy.undoRedoCardActions(cardsWithAction)
    }

    // MARK: Spielzug Aktionen
    // bei jeder Spielzug Aktion (start, finish, cancel) wird die Gesamtspielzeit erhöht
    // falls die Zeit bis zum nächsten Ereignis  größer als eine vorgegebene Zeit ist,
    // wird die Spielzeit entsprechend nach unten korrigiert
    
    func cumulatePlayTime() {
        let now = Date()
        let timeSinceLastTimeCumulated = now.timeIntervalSince(lastTimeCumulated)
        // falls die Zeit seit dem letzten update kleiner als die vorgegebene Zeit ist, wird totalGameTime erhöht
        if timeSinceLastTimeCumulated <= kMaxLastTimeCumulated {
            totalTimeGame += timeSinceLastTimeCumulated
        }
        log.verbose("totalGameTime ist: \(totalTimeGame)")
        // dann beginnt das ganze Spiel von Neuem
        lastTimeCumulated = now
    }
    
    func startPlayMoveFor(_ pile: Pile, withCard: Card) {
        cumulatePlayTime()
        // hier sollten alle View Aktionen beendet sein
        userInteractionProtocolDelegate!.disableUndoRedo()
        resetzPositions()
        // wurde vielleicht eine Schummelkarte gewählt
        if cheatAllowed {
            if withCard.markedForCheat {
                cheatCard! <- withCard
                // der PlayMove hat begonnen und wurde gleich danach beendet
                //log.messageOnly("geschummelt mit: \(withCard.name)")
                playMoveFinished()
                return
            }
        }
        switch pile.pileType {
        case .tableau:
            // selektiere die Karte(n) für diesen Spielzug
            pile.selectCardOrSequenceWithCard(withCard)
            // finde mögliche Ziele
            let targets = findTargetsForPlayMove(pile:pile, card:withCard)
            if targets.count != 0 {
                // behandle die Targets
                if targets.count == 1 {
                    // dorthin kann automatisch abgelegt werden
                    unselectCards()
                    moveSequence(pile.createSequenceFromSelection(), fromPile: pile, toPile: targets.first!)
                    playMoveFinished()
                    return
                }
                else {
                    // mehrere Zielstapel sind möglich
                    // verdunkle alle nicht möglichen Stapel
                    var notPossibleTargets: [Pile] = gamePiles!
                    for pile in targets {
//                        let index = find(notPossibleTargets,pile)
                        let index = notPossibleTargets.index(of: pile)
                        notPossibleTargets.remove(at: index!)
                    }
                    // es wird eine Benachrichtigung verschickt, damit die selektierten Piles optisch im View verändert werden können
                    NotificationCenter.default.post(name: Notification.Name(rawValue: selectPilesNotification), object: self, userInfo: (NSDictionary(object: notPossibleTargets, forKey: "selectPiles" as NSCopying) as! [AnyHashable: Any]))
                    // playMoveAwaitFurtherAction()
                    playMove!.setInnerState(status: .awaitFurtherAction, selectedPile: pile, secondSelectedPile: nil, selectedCard: withCard, secondSelectedCard: nil)
                }
            }
            else {
                // es gibt keine Möglichkeit zu Ablegen
                // der Zug wird abgebrochen
                shakeCard! <- withCard
                playMoveCanceled(withoutEnableUndoRedo: true)
            }
        case .stock:
            dealCardsFromStock()
            // das ist der ganze Spielzug
            playMoveFinished()
        default:
            //TODO: die anderen Fälle noch implementieren
            assert(false, "muss noch implementiert werden")
        }
    }
    
    func continuePlayMoveForPile(_ pile: Pile, withCard: Card, forSelectedPile: Pile) {
        // hier sollten alle View Aktionen beendet sein
        resetzPositions()

        // darf vom selektierten Stape auf den Stapel dieser Art abgelegt werden?
        if pile.isPermittedToPlayFromPile(forSelectedPile) {
            switch pile.pileType {
            case .tableau:
                let sequence = forSelectedPile.createSequenceFromSelection()
                moveSequence(sequence, fromPile: forSelectedPile, toPile: pile)
                playMoveFinished()
            case .multiFoundation:
                let sequence = forSelectedPile.createSequenceFromSelection()
                moveSequence(sequence, fromPile: forSelectedPile, toPile: pile)
                playMoveFinished()
            default:
                //TODO: die anderen Fälle noch implementieren
                assert(false, "muss noch implementiert werden")
            }
        }
        else {
            // es gibt keine Möglichkeit zu Ablegen
            // der Zug wird abgebrochen
            playMoveCanceled()
        }
    }

    func playMoveFinished() {
        // berechne die bisherige Spielzeit
        cumulatePlayTime()
        unselectPiles()
        unselectCards()
        evaluateScore()
        playMove!.setInnerState(status: .idle, result: .executed, selectedPile: nil, secondSelectedPile: nil, selectedCard: nil, secondSelectedCard: nil)
        
    }
    func playMoveCanceled(withoutEnableUndoRedo withoutEnable: Bool = false) {
        // berechne die bisherige Spielzeit
        cumulatePlayTime()
        unselectPiles()
        unselectCards()
        playMove!.setInnerState(status: .idle, result: .canceled, selectedPile: nil, secondSelectedPile: nil, selectedCard: nil, secondSelectedCard: nil)
        if !withoutEnable {
            userInteractionProtocolDelegate!.enableUndoRedo()
        }
    }
    
    func resetzPositions() {
        highestzPosition = 1.0
        if !startPile!.isPileEmpty() {
            for card in startPile!.cards {
                card.zPosition = highestzPosition
                highestzPosition += 1
                someChangedOnCard! <- card
            }
        }
        for pile in gamePiles! {
            for card in pile.cards {
                card.zPosition = highestzPosition
                highestzPosition += 1
                someChangedOnCard! <- card
            }
        }
        //log.debug("high: \(highestzPosition)")
    }
    
    func findTargetsForPlayMove(pile: Pile, card: Card) -> [Pile] {
        // es wird untersucht, ob für eine Selektion von Karten ein oder mehrere Ziele existieren
        // diese Kartenstapel werden in einem [Pile] zurückgeliefert
        var targetPiles : [Pile] = []
        switch pile.permittedToPlayType {
        case .foundationPermittedToPlay:
            //TODO: implementieren
            log.error("muss implementiert werden")
        case .tableauOrFoundationPermittedToPlay:
            // untersuche alle Stapel des Spiels
            for aPile in gamePiles! {
                if aPile == pile {
                    // ich kann nicht den zuvor gewählten Stapel betrachten
                    continue
                }
                switch aPile.pileType {
                case .tableau:
                    if aPile.conformsToPlayOnTableau(card) {
                        targetPiles.append(aPile)
                    }
                case .foundation, .multiFoundation:
                    if aPile.conformsToPlayOnFoundation(card, fromPile: pile) {
                        // falls es sich zusätzlich um ein Spiel handelt, indem nur eine ganze 13er Sequenz abgelegt werden kann, können wir direkt ablegen
                        if aPile.numberPerMove == 13 {
                            // lösche alle bisherigen targets
                            targetPiles.removeAll(keepingCapacity: false)
                            // das ist dann der Stapel, auf den automatisch abgelegt wird
                            targetPiles.append(aPile)    // verlasse die func
                            return targetPiles
                        }
                        // sonst nimm aPile in die Liste der targets auf
                        targetPiles.append(aPile)
                    }
                default:
                    continue
                }
            }
        case .tableauPermittedToPlay:
            //TODO: implementieren
            log.error("muss implementiert werden")
        case .typeOfPermittedToPlayNA:
            //TODO: implementieren
            log.error("muss implementiert werden")
        case .waste_Tableau_FoundationPermittedToPlay:
            //TODO: implementieren
            log.error("muss implementiert werden")
        case .wastePermittedToPlay:
            //TODO: implementieren
            log.error("muss implementiert werden")
        }
        return targetPiles
    }
    
    // MARK: Spielzug Überprüfungen
    
    func isPlayMovePossibleForFirstActionWith(_ card: Card, pile: Pile) -> Bool {
        // überprüft, ob mit der gewählten Karte ein Zug möglich ist. (erste Aktion)
        // vielleicht wurde eine Schummelkarte gewählt
        if cheatAllowed {
            if card.markedForCheat {
                return true
            }
        }
        // FirstAction möglich, wenn
        switch pile.pileType {
        case .tableau:
            // die Karte und der zugehörige Stapel müssen conform sein mit dem userSelectAndMoveType
            return pile.conformsWithSelectAndMoveMode(card)
        case .stock:
            // es wurde der Stock gewählt (zunächst reagiere ich nur auf die letzte Karte)
            // damit soll auch verhindert werden, dass beim versehentlichen Berühren des Stocks schon ein Geben ausgelöst wird
            return card == findStock()!.cards.last
        case .foundation, .multiFoundation:
            // diese Stapel können nur als Ziel dienen und sind deshalb nicht zu einer FirstAction fähig
            return false
        case .waste:
            //TODO: die anderen Fälle noch implementieren
            assert(false, "muss noch implementiert werden")
            return false
        default:
            assert(false, "dieser Fall darf nicht vorkommen")
            return false
        }
    }
    
    func isPlayMovePossibleForFurtherActionWithCard(_ card: Card, forSelectedPile: Pile) -> Bool {
        // überprüft, ob mit der gewählten Karte ein Zug möglich ist. (folgende Aktion)
        // forSelectedPile ist der Stapel, der zuvor selektiert wurde
        // card ist die aktuell angetippte Karte
        // pile ist der Zielstapel
        let firstCard = forSelectedPile.firstSelectedCard()
        let pile = findPileForCard(card)!
        // als Ablage kann niemals derselbe Stapel dienen
        if pile == forSelectedPile {
            return false
        }
        // darf vom selektierten Stape auf den Stapel dieser Art abgelegt werden?
        if pile.isPermittedToPlayFromPile(forSelectedPile) {
            switch pile.pileType {
            case .tableau:
                // es muss die letzte Karte des Zielstapels sein
                if !(pile.cards.last == card) {
                    return false
                }
                return pile.conformsToPlayOnTableau(firstCard!)
            case .multiFoundation:
                return pile.conformsToPlayOnFoundation(firstCard!, fromPile: forSelectedPile)
            default:
                //TODO: die anderen Fälle noch implementieren
                assert(false, "muss noch implementiert werden")
                return false
            }
        }
        return false
    }
    
    func isPlayMovePossibleForFurtheActionWithEmptyPile(_ emptyPile: Pile, forSelectedPile: Pile) -> Bool {
        // ein leerer Stapel wurde angetippt
        let firstCard = forSelectedPile.firstSelectedCard()
        // emptyPile ist hier der Zielstapel
        if emptyPile.isPermittedToPlayFromPile(forSelectedPile) {
            switch emptyPile.pileType {
            case .tableau:
                return emptyPile.conformsToPlayOnTableau(firstCard!)
            case .multiFoundation:
                return emptyPile.conformsToPlayOnFoundation(firstCard!, fromPile: forSelectedPile)
            default:
                //TODO: die anderen Fälle noch implementieren
                assert(false, "muss noch implementiert werden")
                return false
            }
        }
        return false
    }
    
    // MARK: Kartenaktionen, die in den UndoManager eingespeist werden
    
    @objc func moveSequence(_ sequence: [Card], fromPile: Pile, toPile: Pile) {
        self.undoManager.beginUndoGrouping()
        let theProxy: (AnyObject) = self.undoManager.prepare(withInvocationTarget: self) as (AnyObject)
        fromPile.resetCardsRepositioned()
        toPile.resetCardsRepositioned()
        toPile.moveSequence(sequence, fromPile: fromPile)
        toPile.logMoveSequence(sequence,fromPile:fromPile)
        if fromPile.isCardsRepositioned() {
            // alle Karten des Stapels müssen neu positioniert werden
            for card in fromPile.cards {
                movements.append((SKCardMoves.reposition,card,0.0))
            }
            //log.debug("fromPile \(fromPile.pileId) repositioned")
        }
        if toPile.isCardsRepositioned() {
            // die ürsprünglichen Karten neu positionieren
            let numberOfSequence = sequence.count
            let numberOfCardsInPile = toPile.cards.count
            let numberOfCardsToReposition = numberOfCardsInPile - numberOfSequence
            for card in toPile.cards[0 ..< numberOfCardsToReposition] {
                movements.append((SKCardMoves.reposition,card,0.0))
            }
            //log.debug("toPile \(toPile.pileId) repositioned")
        }
        for card in sequence {
            movements.append((SKCardMoves.move,card,0.0))
        }
        theProxy.moveSequence(sequence, fromPile: toPile, toPile: fromPile)
        if !(self.undoManager.isUndoing || self.undoManager.isRedoing) {
            // falls fromPile jetzt nicht leer ist und die letzte Karte verdeckt ist, muss diese jetzt umgedreht werden
            if !fromPile.isPileEmpty() {
                let card = fromPile.cards.last
                if !card!.faceUp {
                    handleLastCardAfterMoveSequence(fromPile)
                    turnTheCard(card!)
                }
            }
        }
        makeMovements()
        self.undoManager.endUndoGrouping()
    }
    
    @objc func handleLastCardAfterMoveSequence(_ pile: Pile) {
        let theProxy: (AnyObject) = self.undoManager.prepare(withInvocationTarget: self) as (AnyObject)
        if !pile.isPileEmpty() {
            // der Stapel ist nicht leer -> es gibt eine letzte Karte
            let card = pile.cards.last!
            if !card.faceUp {
                // die Karte ist noch nicht aufgedeckt
                if cheatAllowed {
                    // bei dieser Karte markedForCheat aufheben
                    card.toggleMarkForCheat()
                    // falls es noch eine Karte oberhalb gibt, dort markForCheat setzen
                    if pile.numberOfCards > 1 {
                        let newCardForCheat = pile.cards[pile.cards.count-2]
                        newCardForCheat.toggleMarkForCheat()
                        pile.repositionLastCard()
                        movements.append((SKCardMoves.reposition,pile.cards.last!,0.0))
                    }
                }
                makeMovements()
                logHandleLastCardAfterMoveSequence(card, fromPile: pile)
                theProxy.handleLastCardAfterMoveSequence(pile)
            }
        }
    }
    
    @objc func turnTheCard(_ card: Card) {
        let theProxy: (AnyObject) = self.undoManager.prepare(withInvocationTarget: self) as (AnyObject)
        card.faceUp = !card.faceUp
        movements.append((SKCardMoves.turn,card,0.0))
        makeMovements()
        theProxy.turnTheCard(card)
    }
    
    func makeMovements(_ inReverseOrder: Bool = false) {
        if inReverseOrder {
            movements = movements.reversed()
        }
        
        for movement in movements {
           switch movement.0 {
                case .move:
                    waitForDuration! <- movement.2
                    movement.1.zPosition = highestzPosition
                    highestzPosition += 1
                    moveCard! <- movement.1
                case .moveAndTurn:
                    movement.1.zPosition = highestzPosition
                    highestzPosition += 1
                    moveAndTurnCard! <- movement.1
                case .turnAndMove:
                    movement.1.zPosition = highestzPosition
                    highestzPosition += 1
                    turnAndMoveCard! <- movement.1
                case .turn:
                    turnCard! <- movement.1
                case .reposition:
                    repositionCard! <- movement.1
                case .shake:
                    shakeCard! <- movement.1
                case .cheat:
                    cheatCard! <- movement.1
                    //            case .SomeChanged:
                    //                someChangedOnCard! <- movement.1
                default:
                    log.error("muss noch implementiert werden")
                }
            }

        // Liste der Kartenaktionen löschen
        movements.removeAll(keepingCapacity: false)
    }
    
    // MARK: Kartenaktionen, die nicht in den UndoManager eingespeist werden
    
    func dealCardsFromStock() {
        let fromPile = findStock()!
        let fromPiles = [fromPile]
        var toPiles: [Pile] = []
        switch fromPile.dealingFromStockType {
        case .dealToTableaus:
            for pile in gamePiles! {
                if pile.pileType == .tableau {
                    toPiles.append(pile)
                }
            }
        default:
            assert(false, "muss noch implementiert werden")
        }
        dealCardsFrom(fromPiles, toPiles: toPiles)
    }
    
    @objc func dealCardsFrom(_ fromPiles: [Pile], toPiles: [Pile]) {
        let theProxy: (AnyObject) = self.undoManager.prepare(withInvocationTarget: self) as (AnyObject)
        var playedCards = [Card]()
        var memoCardsRepositioned = false
        var cardsPlayed = 0
        for fromPile in fromPiles {
            fromPile.resetCardsRepositioned()
            for toPile in toPiles {
                toPile.resetCardsRepositioned()
                // bewege eine Karte vom fromPile auf den toPile
                assert(fromPile.numberOfCards > 0 , "keine Karte mehr im fromPile")
                let card = fromPile.getLastCard()!
                // drehe die Karte
                card.faceUp = !card.faceUp
                toPile.putCard(card)
                fromPile.removeLastCard()
                playedCards.append(card)
                cardsPlayed += 1
                if !undoManager.isUndoing {
                    // es wird kein Undo durchgeführt
                    // die ursprünglichen Karten dieses Stapels wurden neu positioniert
                    if toPile.isCardsRepositioned() {
                        // positioniere alle bis auf die letzte Karte neu
                        if toPile.numberOfCards > 1 {
                            //log.debug("toPile \(toPile.pileId) repositioned")
                            for card in toPile.cards[0 ..< toPile.numberOfCards-1] {
                                movements.append((SKCardMoves.reposition,card,0.0))
                            }
                        }
                    }
                    // erst bewegen dann drehen
                    movements.append((SKCardMoves.moveAndTurn,card,0.0))
                }
                else {
                    // es wird ein Undo durchgeführt
                    // merke dir, dass der toPile (der in diesem Falle der Stock ist) repositioniert wurde
                    memoCardsRepositioned = memoCardsRepositioned || toPile.isCardsRepositioned()
                    // erst drehen dann bewegen
                    movements.append((SKCardMoves.turnAndMove,card,0.0))
                }
            }
            if !undoManager.isUndoing {
                // es ist kein Undo
                if fromPile.isCardsRepositioned() {
                    for card in fromPile.cards {
                        movements.append((SKCardMoves.reposition,card,0.0))
                    }
                    //log.debug("fromPile \(fromPile.pileId) repositioned")
                }
            }
            else {
                // es ist ein Undo
                if memoCardsRepositioned {
                    // der Stock wurde beim Undo repositioniert
                    // positioniere alle Karten, die nicht zurückgespelt wurden neu
                    let toPile = toPiles.first!
                    for card in toPile.cards[0 ..< toPile.cards.count - cardsPlayed] {
                        movements.append((SKCardMoves.reposition,card,0.0))
                    }
                    //log.debug("toPile \(toPile.pileId) repositioned (undo)")
                    // muss der fromPile (der in diesem Falle ja ein anderer als der Stock ist)
                    // durch die Entnahme der Karte eventuell vergrößert werden?
                    if fromPile.isCardsRepositioned() {
                        for card in fromPile.cards {
                            movements.append((SKCardMoves.reposition,card,0.0))
                        }
                        //log.debug("fromPile \(fromPile.pileId) repositioned (undo)")
                    }
                }
            }
        }
        logDealCards(playedCards, fromPiles: fromPiles, toPiles: toPiles)
        // führe alle Bewegungnen durch
        makeMovements()
        theProxy.dealCardsFrom(toPiles, toPiles: fromPiles)
    }
    
    // MARK: Selektionen
    
    func unselectCards() {
        // alle Karten des Spiels werden deselektiert
        for pile in gamePiles! {
            pile.unselectCards()
        }
    }
    func unselectPiles() {
        // es wird eine Benachrichtigung verschickt, damit die selektierten Piles optisch im View verändert werden können
        NotificationCenter.default.post(name: Notification.Name(rawValue: selectPilesNotification), object: self, userInfo: (NSDictionary(object: gamePiles!, forKey: "unselectPiles" as NSCopying) as! [AnyHashable: Any]))
    }
    
    // MARK: Scoring
    
    func evaluateScore() {
        var newScore = 0
        switch scoringType {
        case .scoringSequenceInSuitAndFoundation, .scoringSequenceInSuitAndKingUp:
            // alle Piles durchsuchen
            for pile in gamePiles! {
                if (pile.pileType == .stock) || pile.isPileEmpty() {
                    // der Stock wird nicht untersucht, auch kein leerer Stapel
                    continue
                }
                if !pile.isPileEmpty() {
                    // für jeden im Foundation abgelegten Stapel gibt es einen Extrapunkt
                    if pile.pileType == TypeOfPile.foundation && scoringType == TypeOfScoring.scoringSequenceInSuitAndFoundation {
                        newScore += 1
                    }
                    // für jeden im MultiFoundation abgelegten Stapel gibt es einen Extrapunkt
                    if pile.pileType == TypeOfPile.multiFoundation {
                        newScore += (pile.cards.count / 13)
                    }
                    // bei einem nicht leeren Stapel muss es diese Karte geben
                    var prevCard = pile.cards.first!
                    if prevCard.faceUp && prevCard.isKing() && scoringType == TypeOfScoring.scoringSequenceInSuitAndKingUp {
                        newScore += 1
                    }
                    var nextCard: Card!
                    for i in 1 ..<  pile.cards.count {
                        nextCard = pile.cards[i]
                        // nur offen sichtbare Karten werden gezählt
                        if prevCard.faceUp {
                            assert(nextCard.faceUp, "hier ist was faul")
                            if prevCard.isDownInSequenceWithCard(nextCard) && prevCard.isDownInSuitWithCard(nextCard) {
                                newScore += 1
                            }
                        }
                        prevCard = nextCard
                    }
                }
            }
        case .scoringSequenceNoColorAndFoundation:
            // alle Piles durchsuchen
            for pile in gamePiles! {
                if (pile.pileType == .stock) || pile.isPileEmpty() {
                    // der Stock wird nicht untersucht, auch kein leerer Stapel
                    continue
                }
                if !pile.isPileEmpty() {
                    // für jeden im Foundation abgelegten Stapel gibt es einen Extrapunkt
                    if pile.pileType == TypeOfPile.foundation {
                        newScore += 1
                    }
                    // für jeden im MultiFoundation abgelegten Stapel gibt es einen Extrapunkt
                    if pile.pileType == TypeOfPile.multiFoundation {
                        newScore += (pile.cards.count / 13)
                    }
                    // bei einem nicht leeren Stapel muss es diese Karte geben
                    var prevCard = pile.cards.first!
                    if prevCard.faceUp && prevCard.isKing() && scoringType == TypeOfScoring.scoringSequenceInSuitAndKingUp {
                        newScore += 1
                    }
                    var nextCard: Card!
                    for i in 1 ..<  pile.cards.count {
                        nextCard = pile.cards[i]
                        // nur offen sichtbare Karten werden gezählt
                        if prevCard.faceUp {
                            assert(nextCard.faceUp, "hier ist was faul")
                            if prevCard.isDownInSequenceWithCard(nextCard) {
                                newScore += 1
                            }
                        }
                        prevCard = nextCard
                    }
                }
            }
        case .scoringCardOnFoundation:
            // alle Piles durchsuchen
            for pile in gamePiles! {
                if (pile.pileType == .stock) || pile.isPileEmpty() {
                    // der Stock wird nicht untersucht, auch kein leerer Stapel
                    continue
                }
                if !pile.isPileEmpty() && pile.pileType == .foundation {
                    newScore += pile.cards.count
                }
            }
        default:
            log.error("muss noch implementiert werden")
        }
        if newScore != score {
            // Geräusche machen
            if newScore > score {
                // unterrichte den Controller, damit der den Sound abspielen kann
                NotificationCenter.default.post(name: Notification.Name(rawValue: playSoundNotification), object: self, userInfo: (NSDictionary(object: "plus", forKey: "soundName" as NSCopying) as! [AnyHashable: Any]))
            }
            else {
                // unterrichte den Controller, damit der den Sound abspielen kann
                NotificationCenter.default.post(name: Notification.Name(rawValue: playSoundNotification), object: self, userInfo: (NSDictionary(object: "minus", forKey: "soundName" as NSCopying) as! [AnyHashable: Any]))
            }
            // neuen Wert übernehmen
            score = newScore
            // informiere controller den Wert anzuzeigen
            scoreValue! <- score
        }
        if isGameWon() {
            // unterrichte den Controller, damit der den Sound abspielen kann
            NotificationCenter.default.post(name: Notification.Name(rawValue: playSoundNotification), object: self, userInfo: (NSDictionary(object: "clapping", forKey: "soundName" as NSCopying) as! [AnyHashable: Any]))
            gameState = .completedState
            cumulatePlayTime()
        }
    }
    
    @discardableResult func isGameWon() -> Bool {
        return score == maxPoints
    }
    
    // MARK: Such Aktionen
    
    func findStock() -> Pile? {
        for pile in gamePiles! {
            if pile.pileType == TypeOfPile.stock {
                return pile
            }
        }
        return nil
    }

    func findCardForId(_ cardId: Int) -> Card? {
        for pile in gamePiles! {
            for card in pile.cards {
                if cardId == card.cardId {
                    return card
                }
            }
        }
        return nil
    }
    
    func findPileForId(_ pileId: Int) -> Pile? {
        for pile in gamePiles! {
            if pile.pileId == pileId {
                return pile
            }
        }
        return nil
        
    }
    
    func findPileForCard(_ inCard: Card) -> Pile? {
        // liefert den Stapel, in dem card liegt
        // irgendwo in den gamePiles muss die Karte liegen
        for pile in gamePiles! {
            for card in pile.cards {
                if card.isEqual(inCard) {
                    return pile
                }
            }
        }
        return nil
    }
    
    // MARK: logging activities
    
    func logHandleLastCardAfterMoveSequence(_ card: Card, fromPile: Pile) {
//        log.messageOnly("letzte Karte: \(card.name) von \(fromPile.pileType.description())(\(fromPile.pileId)) umgedreht")
    }
    
    func logDealCards(_ cards: [Card], fromPiles: [Pile], toPiles: [Pile]) {
//        if fromPiles.count == 1 {
//            log.messageOnly("gebe Karten von \(fromPiles[0].pileType.description())(\(fromPiles[0].pileId))")
//            var counter = 0
//            for pile in toPiles {
//                log.messageOnly("\tauf \(pile.pileType.description())(\(pile.pileId)): \(cards[counter].name)")
//                counter += 1
//            }
//        }
//        else {
//            var counter = 0
//            for pile in fromPiles {
//                log.messageOnly("spiele von \(pile.pileType.description())(\(pile.pileId)): \(cards[counter].name) zurück auf \(toPiles[0].pileType.description())(\(toPiles[0].pileId))")
//                counter += 1
//            }
//        }
        
    }
    
    func logGameStart() {
//        log.verbose("\(self.gameName) started")
//        log.messageOnly("Stapel und Karten nach Auslegen")
//        for pile in gamePiles! {
//            log.messageOnly("Stapel \(pile.pileType.description())(\(pile.pileId)):")
//            for card in pile.cards {
//                if card.faceUp {
//                    log.messageOnly("\t  sichtbar: \(card.name)")
//                }
//                else {
//                    log.messageOnly("\tunsichtbar: \(card.name)")
//                }
//            }
//        }
//        log.messageOnly("Ende: Auslegen")
    }
    

        
}
