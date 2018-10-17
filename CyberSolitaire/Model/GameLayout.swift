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
                tmpGameLayout.typeOfScoring = convertToTypeOfScoring(foundGame, key: "scoringType")
                tmpGameLayout.difficulty = foundGame["difficulty"] as! Int
                tmpGameLayout.maxPoints = foundGame["maxPoints"] as! Int
                tmpGameLayout.numberOfPiles = foundGame["numberOfPiles"] as! Int
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
        //return nil
    }
    
    class func gameLayoutPList(_ gameLayoutsPList : [Dictionary<String,Any>], _ gameName : String) -> GameLayout {
//        let tmpGameLayout = GameLayout()
//        let games = getAllGames()
//        for game in games {
//            if let foundGame = game[gameName] {
//                tmpGameLayout.gameLayoutVersion = foundGame["gameLayoutVersion"] as! Int
//                assert(tmpGameLayout.gameLayoutVersion == kGameLayoutVersion, "falsche gameLayout Version aktuell:\(kGameLayoutVersion) file:\(tmpGameLayout.gameLayoutVersion)")
//                tmpGameLayout.gameName = foundGame["gameName"] as! String
//                tmpGameLayout.gameGroup = foundGame["gameGroup"] as! String
//                tmpGameLayout.numberOfDecks = foundGame["numberOfDecks"] as! Int
//                tmpGameLayout.typeOfScoring = convertToTypeOfScoring(foundGame, key: "scoringType")
//                tmpGameLayout.difficulty = foundGame["difficulty"] as! Int
//                tmpGameLayout.maxPoints = foundGame["maxPoints"] as! Int
//                tmpGameLayout.numberOfPiles = foundGame["numberOfPiles"] as! Int
//                let pilesLayoutPList = foundGame["piles"] as! [Dictionary<String,Any>]
//                for pileLayoutPList in pilesLayoutPList {
//                    // PileLayouts hinzupacken
//                    let pileLayout = PileLayout.pileLayoutFromPList(pileLayoutPList: pileLayoutPList)
//                    tmpGameLayout.pileLayouts.append(pileLayout)
//                }
//                return tmpGameLayout
//            }
//        }
        let tmpGameLayout = GameLayout()

        for gameLayoutPList in gameLayoutsPList {
            if gameLayoutPList["gameName"] as! String == gameName {
               tmpGameLayout.gameLayoutVersion = gameLayoutPList["gameLayoutVersion"] as! Int
                 assert(tmpGameLayout.gameLayoutVersion == kGameLayoutVersion, "falsche gameLayout Version aktuell:\(kGameLayoutVersion) file:\(tmpGameLayout.gameLayoutVersion)")
                tmpGameLayout.gameName = gameLayoutPList["gameName"] as! String
                tmpGameLayout.gameGroup = gameLayoutPList["gameGroup"] as! String
                tmpGameLayout.numberOfDecks = gameLayoutPList["numberOfDecks"] as! Int
                tmpGameLayout.typeOfScoring = convertToTypeOfScoring(gameLayoutPList, key: "scoringType")
                tmpGameLayout.difficulty = gameLayoutPList["difficulty"] as! Int
                tmpGameLayout.maxPoints = gameLayoutPList["maxPoints"] as! Int
                tmpGameLayout.numberOfPiles = gameLayoutPList["numberOfPiles"] as! Int
                let pilesLayoutPList = gameLayoutPList["piles"] as! [Dictionary<String,Any>]
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
        let str = gameLayoutPList[key] as! String
        switch str {
        case "notAvaiable":
            return .scoringTypeNA
        case "scoringSequenceInSuitAndFoundation":
            return .scoringSequenceInSuitAndFoundation
        case "scoringSequenceNoColorAndFoundation":
            return .scoringSequenceNoColorAndFoundation
        case "scoringCardOnFoundation":
            return .scoringCardOnFoundation
        case "scoringSequenceInSuitAndKingUp":
            return .scoringSequenceInSuitAndKingUp
        default:
            fatalError("falscher TypeOfScoring")
        }
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
