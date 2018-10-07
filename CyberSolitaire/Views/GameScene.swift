//
//  GameScene.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 08.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import UIKit
import SpriteKit


var playableRect = CGRect.zero
var longestDistance = CGFloat(0.0)
let durationForLongestDistance: CGFloat = 1.0
var speedFactor = CGFloat(0.0)

class GameScene: SKScene {

    var sceneDelegate: TouchesProtocolDelegate? = nil
    
    var cardNodes: [CardNode] = []
    var pileEmptyNodes: [PileEmptyNode] = []
    
    override func didMove(to view: SKView) {
        //let background = SKSpriteNode(imageNamed: "CyberSol_Background1")
        let background = SKSpriteNode(imageNamed: "BackgroundWildWest")
//        background.setScale(0.5)
        background.yScale = size.height/background.size.height
        background.xScale = size.width/background.size.width
        background.position = CGPoint(x: size.width/2, y: size.height/2)
        background.anchorPoint = CGPoint(x: 0.5, y: 0.5)
        background.zPosition = -1
        addChild(background)
        
        //log.verbose("add Nodes cards:\(cardNodes.count) piles:\(pileEmptyNodes.count)")
        for node in cardNodes {
            addChild(node)
        }
        for node in pileEmptyNodes {
            addChild(node)
        }
        
    }
    
//    override func touchesBegan(touches: Set<NSObject>, withEvent event: UIEvent) {
//        let a=1
//        //log.info("touch on: \(locationInScene)")
//
//    }
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        let touch: UITouch = touches.first! as UITouch
        let location = touch.location(in: self)
        let touchNode = atPoint(location)
        let locationInScene = touchNode.convert(location, to: touchNode.scene!)
        let _ = locationInScene
        //log.info("touch on: \(locationInScene)")
    }
   
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        let touch: UITouch = touches.first! as UITouch
        let location = touch.location(in: self)
        let touchNode = atPoint(location)
        let locationInScene = touchNode.convert(location, to: touchNode.scene!)
        //log.info("touch on: \(locationInScene)")
        let dict = ["Game":-1]
        sceneDelegate!.tapOnGameWithDictionary(dict, locationInScene: locationInScene)
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        let touch: UITouch = touches.first! as UITouch
        let location = touch.location(in: self)
        let touchNode = atPoint(location)
        let locationInScene = touchNode.convert(location, to: touchNode.scene!)
        let _ = locationInScene
        //log.info("touch on: \(locationInScene)")
    }
    
    override func update(_ currentTime: TimeInterval) {
        /* Called before each frame is rendered */
    }
    
}
