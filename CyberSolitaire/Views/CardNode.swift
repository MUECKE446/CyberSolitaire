//
//  CardNode.swift
//  Cybersol_SpriteKit
//
//  Created by Christian Muth on 06.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import UIKit
import SpriteKit

// MARK: Konstanten
let cAdditionalTime = 0.1
let cHalfFlipDuration = 0.2
let cWaitBetweenMoveAndFlip = 0.2
let cWaitBetweenFlipAndMove = 0.2
let cColorizeWithColorBlendFactorDuration = 0.1
let cShakeTotalDuration = 1.0
let cShakeDuration = 0.1
let cCheatDuration = 0.75

//let cAdditionalTime = 0.2
//let cHalfFlipDuration = 0.5
//let cWaitBetweenMoveAndFlip = 0.5
//let cWaitBetweenFlipAndMove = 0.5
//let cColorizeWithColorBlendFactorDuration = 0.1
//let cShakeTotalDuration = 2.0
//let cShakeDuration = 0.2
//let cCheatDuration = 3.0


class CardNode: SKSpriteNode {

    struct statics {
        static var waitForDurationAction = SKAction.wait(forDuration: 0.0)
        static var waitForDuration = 0.0
    }
    

    weak var touchesProtocolDelegate: TouchesProtocolDelegate? = nil
    weak var userInteractionProtocolDelegate: UserInteractionProtocolDelegate? = nil
    
    var lastTouchType = TouchType.touchesBegan
    
    /*! cardID = Kartenwert + Deck-Nummer * 52 */
    let cardId: Int
    
    /*! Kartenwert 1  = Kreuz As 14 = Karo As 27 = Herz As 40 = Pik As */
    let value: Int
    
    let frontTexture: SKTexture
    let backTexture = SKTexture(imageNamed: "Back1.png")
    
    var faceUp: Bool
    
    var scaleFactor = CGFloat(1.0)
    
    
    init(cardID: Int, faceUp: Bool) {
        
        /*! über eine cardId wird eine Karte angelegt und initialisiert */
        assert(cardID >= 1 && cardID <= 104, "error: wrong cardId!")
        self.cardId = cardID
        
        // maximal 2 Kartenspiele werden berücksichtigt
        // der Kartenwert liegt zwischen 1 und 52
        if self.cardId > 52 {
            value = self.cardId - 52
        }
        else {
            value = self.cardId
        }
        
        var cardImage = "Card"
        cardImage += String(value) + ".png"
        frontTexture = SKTexture(imageNamed: cardImage)
        self.faceUp = faceUp
        if self.faceUp {
            super.init(texture: frontTexture, color: SKColor.black, size: frontTexture.size())
        }
        else {
            super.init(texture: backTexture, color:  SKColor.black, size: frontTexture.size())
        }
        isUserInteractionEnabled = true
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    // MARK: class functions
    
    class func setWaitForDuration(_ duration: TimeInterval) {
        statics.waitForDurationAction = SKAction.wait(forDuration: duration)
        statics.waitForDuration = duration
        //log.debug("\(duration)")
    }
    
    // MARK: instance functions
    
    // moveCard
    func moveCardToPosition(_ position: CGPoint) {
        let oldPosition = self.position
        let distance = (position - oldPosition).length()
        let moveDuration = Double(distance * speedFactor)
        let move = SKAction.move(to: position, duration: moveDuration)
        
        let sequence = SKAction.sequence([statics.waitForDurationAction,move])
        run(sequence)
        
        //log.debug("moveCard")
    }
    
    // moveAndTurnCard
    func moveAndTurnCardToPosition(_ position: CGPoint) {
        let oldPosition = self.position
        let distance = (position - oldPosition).length()
        let moveDuration = Double(distance * speedFactor)
        let yScale = self.scaleFactor
        
        let move = SKAction.move(to: position, duration: moveDuration)
        let firstHalfFlip = SKAction.scaleY(to: 0.0, duration: cHalfFlipDuration)
        let secondHalfFlip = SKAction.scaleY(to: yScale, duration: cHalfFlipDuration)
        let wait = SKAction.wait(forDuration: cWaitBetweenMoveAndFlip)
        
        let flip = SKAction.run() {
            if self.faceUp {
                self.run(firstHalfFlip, completion: {
                    self.texture = self.backTexture
                    self.faceUp = false
                    self.run(secondHalfFlip)
                }) 
            }
            else {
                self.run(firstHalfFlip, completion: {
                    self.texture = self.frontTexture
                    self.faceUp = true
                    self.run(secondHalfFlip)
                }) 
            }
        }
        let sequence = SKAction.sequence([move,wait,flip])
        self.run(sequence)
        //log.debug("moveAndTurnCard")
    }
    
    // turnAndMoveCard
    func turnAndMoveCardToPosition(_ position: CGPoint) {
        let oldPosition = self.position
        let distance = (position - oldPosition).length()
        let moveDuration = Double(distance * speedFactor)
        let yScale = self.scaleFactor
        
        let move = SKAction.move(to: position, duration: moveDuration)
        let firstHalfFlip = SKAction.scaleY(to: 0.0, duration: cHalfFlipDuration)
        let secondHalfFlip = SKAction.scaleY(to: yScale, duration: cHalfFlipDuration)
        let wait = SKAction.wait(forDuration: 2*cHalfFlipDuration + cWaitBetweenFlipAndMove)
        
        let flip = SKAction.run() {
            if self.faceUp {
                self.run(firstHalfFlip, completion: {
                    self.texture = self.backTexture
                    self.faceUp = false
                    self.run(secondHalfFlip)
                }) 
            }
            else {
                self.run(firstHalfFlip, completion: {
                    self.texture = self.frontTexture
                    self.faceUp = true
                    self.run(secondHalfFlip)
                }) 
            }
        }
        // die wait action läuft hier parallel zum flip, deshalb muss sie um die Dauer des flip länger sein
        let sequence = SKAction.sequence([flip,wait,move])
        self.run(sequence)
        //log.debug("turnAndMoveCard")
    }
    
    // turnCard
    func turnCard() {
        let yScale = self.scaleFactor
        //log.debug("size:\(self.size) xScale:\(xScale) yScale:\(yScale)")
        let firstHalfFlip = SKAction.scaleY(to: 0.0, duration: cHalfFlipDuration)
        let secondHalfFlip = SKAction.scaleY(to: yScale, duration: cHalfFlipDuration)
        let flip = SKAction.run() {
            if self.faceUp {
                self.run(firstHalfFlip, completion: {
                    self.texture = self.backTexture
                    self.faceUp = false
                    self.run(secondHalfFlip)
                }) 
            }
            else {
                self.run(firstHalfFlip, completion: {
                    self.texture = self.frontTexture
                    self.faceUp = true
                    self.run(secondHalfFlip)
                }) 
            }
        }
        self.run(flip)
    }
    
    // shakeCard
    func shakeCard() {
        let rotR = SKAction.rotate(byAngle: CGFloat(15.0).degreesToRadians(), duration: cShakeDuration/2.0)
        let rotL = SKAction.rotate(byAngle: CGFloat(-15.0).degreesToRadians(), duration: cShakeDuration/2.0)
        let cycle = SKAction.sequence([rotL,rotR,rotR,rotL])
        let shakes = SKAction.repeat(cycle, count: 2)
        let shake = SKAction.sequence([shakes,SKAction.rotate(toAngle: 0.0, duration: cShakeDuration/2.0)])
        run(shake)
    }
   
    // cheatCard
    func cheatCard() {
        let yScale = self.scaleFactor
        let firstHalfFlip = SKAction.scaleY(to: 0.0, duration: cHalfFlipDuration)
        let secondHalfFlip = SKAction.scaleY(to: yScale, duration: cHalfFlipDuration)
        let wait = SKAction.wait(forDuration: cCheatDuration)
        
        let flip = SKAction.run() {
            if self.faceUp {
                self.run(firstHalfFlip, completion: {
                    self.texture = self.backTexture
                    self.faceUp = false
                    self.run(secondHalfFlip)
                }) 
            }
            else {
                self.run(firstHalfFlip, completion: {
                    self.texture = self.frontTexture
                    self.faceUp = true
                    self.run(secondHalfFlip)
                }) 
            }
        }
        let sequence = SKAction.sequence([flip,wait,flip])
        self.run(sequence)
    }

    func changeZPositionCard() {
        self.run(SKAction.move(to: self.position, duration: TimeInterval.init()))
    }
    
    func selectCard(_ showSelected: Bool) {
        if showSelected {
            colorBlendFactor = 0.2
        }
        else {
            colorBlendFactor = 0.0
        }
    }
    
    func selectCardForSelectingPile(_ showSelected: Bool) {
        if showSelected {
            if colorBlendFactor == 0.0 {
                colorBlendFactor = 0.7
            }
        }
        else {
            colorBlendFactor = 0.0
        }
    }
    
    // MARK: touches
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        let touch: UITouch = touches.first! as UITouch
        let location = touch.location(in: self)
        let touchNode = atPoint(location)
        let locationInScene = touchNode.convert(location, to: touchNode.scene!)
        let _ = locationInScene
        //log.info("CardNode on: \(locationInScene)")
        lastTouchType = .touchesBegan
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        let touch: UITouch = touches.first! as UITouch
        let location = touch.location(in: self)
        let touchNode = atPoint(location)
        let locationInScene = touchNode.convert(location, to: touchNode.scene!)
        //log.info("CardNode on: \(locationInScene)")
        let dict = ["Card":(touchNode as! CardNode).cardId]
        touchesProtocolDelegate!.tapOnGameWithDictionary(dict, locationInScene: locationInScene)
        lastTouchType = .touchesEnded
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        let touch: UITouch = touches.first! as UITouch
        let location = touch.location(in: self)
        let touchNode = atPoint(location)
        let locationInScene = touchNode.convert(location, to: touchNode.scene!)
        let _ = locationInScene
        //log.info("CardNode on: \(locationInScene)")
        //let dict = ["Card":(touchNode as! CardNode).cardId]
//        switch lastTouchType {
//        case .touchesBegan:
//            //TODO: delegation
//            //touchesProtocolDelegate!.startMoveOnGameWithDictionary(dict, locationInScene: locationInScene)
//            break
//        case .touchesMoved:
//            //TODO: delagation
//            //touchesProtocolDelegate!.startMoveOnGameWithDictionary(dict, locationInScene: locationInScene)
//            break
//        default:
//            // nothing todo
//            break
//        }
    }

}
