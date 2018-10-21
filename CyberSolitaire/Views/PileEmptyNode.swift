//
//  PileEmptyNode.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 10.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import UIKit
import SpriteKit

class PileEmptyNode: SKSpriteNode {

    weak var delegate: TouchesProtocolDelegate? = nil
    
    let pileEmptyId: Int
    
    var pileId: Int
    
    let frontTexture: SKTexture?
    
    init(pileEmptyID: Int, pileID: Int) {
        
        /*! über eine cardId wird eine Karte angelegt und initialisiert */
        assert(pileEmptyID >= 0 && pileEmptyID <= 7, "error: wrong pileEmptyId!")
        self.pileEmptyId = pileEmptyID
        // zur Identifizierung des zugehörigen Piles
        self.pileId = pileID
        
        var pileEmptyImage = "PileEmpty-"
        pileEmptyImage += String(pileEmptyId) + ".png"
        frontTexture = SKTexture(imageNamed: pileEmptyImage)
        
        super.init(texture: frontTexture, color: UIColor.clear, size: frontTexture!.size())
       
        isUserInteractionEnabled = true
        
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        let touch: AnyObject = touches.first!
        let location = touch.location(in: self)
        let touchNode = atPoint(location)
        //let locationInScene = touchNode.convert(location, to: touchNode.scene!)
        //log.info("touch on: \(locationInScene)")
        let dict = ["PileEmpty":(touchNode as! PileEmptyNode).pileId]
        delegate!.tapOnGameWithDictionary(dict, locationInScene: location)
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        for touch: AnyObject in touches {
            let location = touch.location(in: self)
            let touchNode = atPoint(location)
            let locationInScene = touchNode.convert(location, to: touchNode.scene!)
            //log.info("touch on: \(locationInScene)")
            let dict = ["PileEmpty":(touchNode as! PileEmptyNode).pileId]
            delegate!.tapOnGameWithDictionary(dict, locationInScene: locationInScene)
        }
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
//        for touch: AnyObject in touches {
//            let location = touch.location(in: self)
//            let touchNode = atPoint(location)
//            let locationInScene = touchNode.convert(location, to: touchNode.scene!)
//            //log.info("touch on: \(locationInScene)")
//            let dict = ["PileEmpty":(touchNode as! PileEmptyNode).pileId]
//            //TODO: delagation
//        }
    }
    
    


}
