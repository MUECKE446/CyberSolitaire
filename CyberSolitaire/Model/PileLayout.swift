//
//  PileLayout.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 09.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import Foundation
import UIKit

let kPileLayoutVersion = 2

class PileLayout {
    var pileLayoutVersion = kGameLayoutVersion
    var pileType = TypeOfPile(rawValue: 0)
    var numberOfCardsAtStart = 0
    var dealOrderAtStart = 0
    var faceAtStart = FaceAtStart(rawValue: 0)
    var typeOfOverlap = TypeOfOverlap(rawValue: 0)
    var typeOfUserSelectAndMove = TypeOfUserSelectAndMove(rawValue: 0)
    
    var typeOfDepositFromUser = TypeOfDepositFromUser(rawValue: 0)
    var typeOfDepositIfEmpty = TypeOfDepositIfEmpty(rawValue: 0)
    var typeOfPermittedToPlay = TypeOfPermittedToPlay(rawValue: 0)
    var typeOfBasicCard = TypeOfBasicCard(rawValue: 0)
    var typeOfDealingFromStock = TypeOfDealingFromStock(rawValue: 0)
    var typeOfRedeal = TypeOfRedeal(rawValue: 0)
    var numberPerMove = -1   // wieviele Karten können pro Zug abgelegt werden
    
    var indexForEmptyPileImage = 0
    var posX: CGFloat = 0.0
    var posY: CGFloat = 0.0
    
    // MARK: Klassen - Methoden
    
    class func pileLayoutWithPropertyList(_ inArr: [AnyObject]) -> PileLayout {
        let tmpPileLayout = PileLayout()
        tmpPileLayout.pileLayoutVersion = inArr[0] as! Int
        let pt = TypeOfPile(rawValue: inArr[1] as! Int)
        tmpPileLayout.pileType = pt
        tmpPileLayout.numberOfCardsAtStart = inArr[2] as! Int
        tmpPileLayout.dealOrderAtStart = inArr[3] as! Int
        let fas = FaceAtStart(rawValue: inArr[4] as! Int)
        tmpPileLayout.faceAtStart = fas
        let to = TypeOfOverlap(rawValue: inArr[5] as! Int)
        tmpPileLayout.typeOfOverlap = to
        let tus = TypeOfUserSelectAndMove(rawValue: inArr[6] as! Int)
        tmpPileLayout.typeOfUserSelectAndMove = tus
        
        let tdu = TypeOfDepositFromUser(rawValue: inArr[7] as! Int)
        tmpPileLayout.typeOfDepositFromUser = tdu
        let tde = TypeOfDepositIfEmpty(rawValue: inArr[8] as! Int)
        tmpPileLayout.typeOfDepositIfEmpty = tde
        let tpp = TypeOfPermittedToPlay(rawValue: inArr[9] as! Int)
        tmpPileLayout.typeOfPermittedToPlay = tpp
        let tbc = TypeOfBasicCard(rawValue: inArr[10] as! Int)
        tmpPileLayout.typeOfBasicCard = tbc
        let tds = TypeOfDealingFromStock(rawValue: inArr[11] as! Int)
        tmpPileLayout.typeOfDealingFromStock = tds
        let td = TypeOfRedeal(rawValue: inArr[12] as! Int)
        tmpPileLayout.typeOfRedeal = td
        tmpPileLayout.numberPerMove = inArr[13] as! Int
        
        tmpPileLayout.indexForEmptyPileImage = inArr[14] as! Int
        tmpPileLayout.posX = inArr[15] as! CGFloat
        tmpPileLayout.posY = inArr[16] as! CGFloat
        tmpPileLayout.printForDebug()
        return tmpPileLayout
    }
    
    // MARK: Instance - Methoden
    
    func printForDebug() {
        log.setupShowFuncName(false)
        log.info("pileLayoutVersion: \(self.pileLayoutVersion)")
        log.info("pileType: \(String(describing: self.pileType?.description()))")
        log.info("numberOfCardsAtStart: \(self.numberOfCardsAtStart)")
        log.info("dealOrderAtStart: \(self.dealOrderAtStart)")
        log.info("faceAtStart: \(String(describing: self.faceAtStart?.description()))")
        log.info("typeOfOverlap: \(String(describing: self.typeOfOverlap?.description()))")
        log.info("typeOfUserSelectAndMove: \(String(describing: self.typeOfUserSelectAndMove?.description()))")
        log.info("typeOfDepositFromUser: \(String(describing: self.typeOfDepositFromUser?.description()))")
        log.info("typeOfDepositIfEmpty: \(String(describing: self.typeOfDepositIfEmpty?.description()))")
        log.info("typeOfPermittedToPlay: \(String(describing: self.typeOfPermittedToPlay?.description()))")
        log.info("typeOfBasicCard: \(String(describing: self.typeOfBasicCard?.description()))")
        log.info("typeOfDealingFromStock: \(String(describing: self.typeOfDealingFromStock?.description()))")
        log.info("typeOfRedeal: \(String(describing: self.typeOfRedeal?.description()))")
        log.info("numberPerMove: \(self.numberPerMove)")
        log.info("indexForEmptyPileImage: \(self.indexForEmptyPileImage)")
        log.info("posX: \(self.posX)")
        log.info("posY: \(self.posY)")
    }
    
    func convertToPropertyList() -> [AnyObject] {
        let propertyList: [AnyObject] = [pileLayoutVersion as AnyObject,
            pileType!.rawValue as AnyObject,
            numberOfCardsAtStart as AnyObject,
            dealOrderAtStart as AnyObject,
            faceAtStart!.rawValue as AnyObject,
            typeOfOverlap!.rawValue as AnyObject,
            typeOfUserSelectAndMove!.rawValue as AnyObject,
            
            typeOfDepositFromUser!.rawValue as AnyObject,
            typeOfDepositIfEmpty!.rawValue as AnyObject,
            typeOfPermittedToPlay!.rawValue as AnyObject,
            typeOfBasicCard!.rawValue as AnyObject,
            typeOfDealingFromStock!.rawValue as AnyObject,
            typeOfRedeal!.rawValue as AnyObject,
            numberPerMove as AnyObject,
            
            indexForEmptyPileImage as AnyObject,
            posX as AnyObject, posY as AnyObject]
        return propertyList
    }

}
