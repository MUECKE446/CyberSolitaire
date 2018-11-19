//
//  GameLayout.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 09.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import Foundation
import UIKit

let kGameLayoutVersion = 3

class GameLayout {
    var gameLayoutVersion = kGameLayoutVersion
    var gameName = ""
    var gameGroup = ""
    var numberOfDecks = 0
    var scoringType = TypeOfScoring(rawValue: 0)
    var maxPoints = 0
    var numberOfPiles = 0
    var pileLayouts: [PileLayout] = []
    
    // MARK: Klassen - Methoden
    
    class func getGameLayout(_ gameName:String) -> GameLayout? {
        let tmpGameLayout = GameLayout()
        let games = getAllGames()
        for game in games {
            if let foundGame = game[gameName] {
                tmpGameLayout.gameLayoutVersion = foundGame["gameLayoutVersion"] as! Int
                assert(tmpGameLayout.gameLayoutVersion == kGameLayoutVersion, "falsche gameLayout Version aktuell:\(kGameLayoutVersion) file:\(tmpGameLayout.gameLayoutVersion)")
                tmpGameLayout.gameName = foundGame["gameName"] as! String
                tmpGameLayout.gameGroup = foundGame["gameGroup"] as! String
                tmpGameLayout.numberOfDecks = foundGame["numberOfDecks"] as! Int
                tmpGameLayout.scoringType = convertToTypeOfScoring(foundGame, key: "scoringType")
                tmpGameLayout.maxPoints = foundGame["maxPoints"] as! Int
                let pilesLayoutPList = foundGame["piles"] as! [Dictionary<String,Any>]
                for pileLayoutPList in pilesLayoutPList {
                    // PileLayouts hinzupacken
                    let pileLayout = PileLayout.pileLayoutFromPList(pileLayoutPList: pileLayoutPList)
                    tmpGameLayout.pileLayouts.append(pileLayout)
                }
                return tmpGameLayout
            }
        }
        fatalError("dieses Game gibt es nicht! darf nicht vorkommen")
    }
    
    class func convertToTypeOfScoring(_ gameLayoutPList:Dictionary<String,Any>, key:String) -> TypeOfScoring {
        let description = gameLayoutPList[key] as! String
        return TypeOfScoring.valueFromDescription(description)
    }
    

    // MARK: Instance - Methoden
    
    
    func printForDebug() {
//        log.setupShowFuncName(false)
//        log.info("pileLayoutVersion: \(self.gameLayoutVersion)")
//        log.info("gameName: \(self.gameName)")
//        log.info("gameGroup: \(self.gameGroup)")
//        log.info("numberOfDecks: \(self.numberOfDecks)")
//        log.info("scoringType: \(String(describing: self.scoringType?.description()))")
//        log.info("maxPoints: \(self.maxPoints)")
//        log.info("numberOfPiles: \(self.numberOfPiles)")
//        for i in 0 ..< self.numberOfPiles {
//            // PileLayouts hinzupacken
//            log.info("Pile No (\(i)")
//            self.pileLayouts[i].printForDebug()
//        }
    }
    
}
