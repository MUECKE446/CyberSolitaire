//
//  GameLayout.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 09.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import Foundation
import UIKit

let kGameLayoutVersion = 2

class GameLayout {
    var gameLayoutVersion = kGameLayoutVersion
    var gameName = ""
    var gameGroup = ""
    var numberOfDecks = 0
    var typeOfScoring = TypeOfScoring(rawValue: 0)
    var difficulty = 0
    var maxPoints = 0
    var numberOfPiles = 0
    var pileLayouts: [PileLayout] = []
    
    // MARK: Klassen - Methoden
    
    class func gameLayoutWithPropertyList(_ inArr: [AnyObject]) -> GameLayout {
        let tmpGameLayout = GameLayout()
        tmpGameLayout.gameLayoutVersion = inArr[0] as! Int
        assert(tmpGameLayout.gameLayoutVersion == kGameLayoutVersion, "falsche gameLayout Version aktuell:\(kGameLayoutVersion) file:\(tmpGameLayout.gameLayoutVersion)")
        tmpGameLayout.gameName = inArr[1] as! String
        tmpGameLayout.gameGroup = inArr[2] as! String
        tmpGameLayout.numberOfDecks = inArr[3] as! Int
        let tos = TypeOfScoring(rawValue: inArr[4] as! Int)
        tmpGameLayout.typeOfScoring = tos
        tmpGameLayout.difficulty = inArr[5] as! Int
        tmpGameLayout.maxPoints = inArr[6] as! Int
        tmpGameLayout.numberOfPiles = inArr[7] as! Int
        for i in 0 ..< tmpGameLayout.numberOfPiles {
            // PileLayouts hinzupacken
            tmpGameLayout.pileLayouts.append(PileLayout.pileLayoutWithPropertyList(inArr[i+8] as! [AnyObject]))
        }
        
        return tmpGameLayout
    }
    
    // MARK: Instance - Methoden
    
    
    func printForDebug() {
//        log.setupShowFuncName(false)
//        log.info("pileLayoutVersion: \(self.gameLayoutVersion)")
//        log.info("gameName: \(self.gameName)")
//        log.info("gameGroup: \(self.gameGroup)")
//        log.info("numberOfDecks: \(self.numberOfDecks)")
//        log.info("typeOfScoring: \(String(describing: self.typeOfScoring?.description()))")
//        log.info("difficulty: \(self.difficulty)")
//        log.info("maxPoints: \(self.maxPoints)")
//        log.info("numberOfPiles: \(self.numberOfPiles)")
//        for i in 0 ..< self.numberOfPiles {
//            // PileLayouts hinzupacken
//            log.info("Pile No (\(i)")
//            self.pileLayouts[i].printForDebug()
//        }
    }
    
    func convertToPropertyList() -> [AnyObject] {
        var propertyList: [AnyObject] = [gameLayoutVersion as AnyObject,gameName as AnyObject,
            gameGroup as AnyObject,numberOfDecks as AnyObject, typeOfScoring!.rawValue as AnyObject,difficulty as AnyObject, maxPoints as AnyObject, numberOfPiles as AnyObject]
        for pileLayout in pileLayouts {
            propertyList.append(pileLayout.convertToPropertyList() as AnyObject)
        }
        return propertyList
    }
}
