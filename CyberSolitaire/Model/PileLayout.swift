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
    
    
    class func pileLayoutFromPList(pileLayoutPList : Dictionary<String,Any>) -> PileLayout {
        let tmpPileLayout = PileLayout()

        tmpPileLayout.pileLayoutVersion = pileLayoutPList["pileLayoutVersion"] as! Int
        assert(tmpPileLayout.pileLayoutVersion == kPileLayoutVersion, "falsche gameLayout Version aktuell:\(kPileLayoutVersion) file:\(tmpPileLayout.pileLayoutVersion)")
        tmpPileLayout.pileType = convertToTypeOfPile(pileLayoutPList,key:"pileType")
        tmpPileLayout.numberOfCardsAtStart = pileLayoutPList["numberOfCardsAtStart"] as! Int
        tmpPileLayout.dealOrderAtStart = pileLayoutPList["dealOrderAtStart"] as! Int
        tmpPileLayout.faceAtStart = convertToFaceAtStart(pileLayoutPList, key: "faceAtStart")
        tmpPileLayout.typeOfOverlap = convertToTypeOfOverlap(pileLayoutPList, key: "overlapType")
        tmpPileLayout.typeOfUserSelectAndMove = convertToTypeOfUserSelectAndMove(pileLayoutPList, key: "userSelectAndMoveType")
        tmpPileLayout.typeOfDepositFromUser = convertToTypeOfDepositFromUser(pileLayoutPList, key: "depositFromUserType")
        tmpPileLayout.typeOfDepositIfEmpty = convertToTypeOfDepositIfEmpty(pileLayoutPList, key: "depositIfEmptyType")
        tmpPileLayout.typeOfPermittedToPlay = convertToTypeOfPermittedToPlay(pileLayoutPList, key: "permittedToPlayType")
        tmpPileLayout.typeOfBasicCard = convertToTypeOfBasicCard(pileLayoutPList, key: "basicCardType")
        tmpPileLayout.typeOfDealingFromStock = convertToTypeOfDealingFromStock(pileLayoutPList, key: "dealingFromStockType")
        tmpPileLayout.typeOfRedeal = convertToTypeOfRedeal(pileLayoutPList, key: "redealType")
        tmpPileLayout.numberPerMove = pileLayoutPList["numberPerMove"] as! Int
        tmpPileLayout.indexForEmptyPileImage = pileLayoutPList["indexForEmptyPileImage"] as! Int
        tmpPileLayout.posX = CGFloat(((pileLayoutPList["posX"] as! String) as NSString).floatValue)
        tmpPileLayout.posY = CGFloat(((pileLayoutPList["posY"] as! String) as NSString).floatValue)

        //tmpPileLayout.printForDebug()
        return tmpPileLayout
    }
    
    class func convertToTypeOfPile(_ pileLayoutPList:Dictionary<String,Any>, key:String) -> TypeOfPile {
        let str = pileLayoutPList[key] as! String
        switch str {
        case "notAvailable":
            return .typeOfPileNA
        case "Stock":
            return .stock
        case "Tableau":
            return .tableau
        case "MultiFoundation":
            return .multiFoundation
        case "Foundation":
            return .foundation
        case "Waste":
            return .waste
        default:
            fatalError("falscher TypeOfPileType")
        }
    }
    
    
    class func convertToFaceAtStart(_ pileLayoutPList:Dictionary<String,Any>, key:String) -> FaceAtStart {
        let str = pileLayoutPList[key] as! String
        switch str {
        case "notAvailable":
            return .faceAtStartNA
        case "allFaceUp":
            return .allFaceUp
        case "allFaceDown":
            return .allFaceDown
        case "lastFaceUp":
            return .lastFaceUp
        case "oddFaceUp":
            return .oddFaceUp
        case "evenFaceUp":
            return .evenFaceUp
        default:
            fatalError("falscher FaceAtStart")
        }
    }
    
    
    class func convertToTypeOfOverlap(_ pileLayoutPList:Dictionary<String,Any>, key:String) -> TypeOfOverlap {
        let str = pileLayoutPList[key] as! String
        switch str {
        case "notAvailable":
            return .overlapModeNA
        case "notOverlapped":
            return .notOverlapped
        case "downOverlapped":
            return .downOverlapped
        default:
            fatalError("falscher TypeOfOverlap")
        }
    }
    
    
    class func convertToTypeOfUserSelectAndMove(_ pileLayoutPList:Dictionary<String,Any>, key:String) -> TypeOfUserSelectAndMove {
        let str = pileLayoutPList[key] as! String
        switch str {
        case "notAvailable":
            return .typeOfUserSelectAndMoveNA
        case "anySeqInSuit":
            return .anySequenceInSuit
        case "tableauAnySeqAlternate":
            return .anySeqAlternate
        case "anySeqNoColor":
            return .anySeqNoColor
        case "allSeqNoColorOrLast":
            return .allSeqNoColorOrLast
        case "allSeqInSuitOrLast":
            return .allSeqInSuitOrLast
        case "allSeqAlternateOrLast":
            return .allSeqAlternateOrLast
        case "all":
            return .all
        case "onlyLast":
            return .onlyLast
        case "exactOne":
            return .exactOne
        case "allFaceUpCanSelect":
            return .allFaceUpCanSelect
        case "pairsEqualValueCanSelect":
            return .pairsEqualValueCanSelect
        case "pairsValue13":
            return .pairsValue13
default:
            fatalError("falscher TypeOfUserSelectAndMove")
        }
    }

    
    class func convertToTypeOfDepositFromUser(_ pileLayoutPList:Dictionary<String,Any>, key:String) -> TypeOfDepositFromUser {
        let str = pileLayoutPList[key] as! String
        switch str {
        case "notAvailable":
            return .typeOfDepositFromUserNA
        case "downInSuit":
            return .downInSuit
        case "downAlternate":
            return .downAlternate
        case "downNoColor":
            return .downNoColor
        case "upInSuit":
            return .upInSuit
        case "upAlternate":
            return .upAlternate
        case "upNoColor":
            return .upNoColor
        case "withGaps":
            return .withGaps
        case "onlyFromStock":
            return .onlyFromStock
        case "nothing":
            return .nothing
        case "downInSuitRolling":
            return .downInSuitRolling
        case "upInSuitRolling":
            return .upInSuitRolling
        case "downAlternateRolling":
            return .downAlternateRolling
        case "upAlternateRolling":
            return .upAlternateRolling
        case "pairsEqualValue":
            return .pairsEqualValue
        case "pairsValue13OrKing":
            return .pairsValue13OrKing
        case "upDownNoColorRolling":
            return .upDownNoColorRolling
        case "oneFromTableauOrWaste":
            return .oneFromTableauOrWaste
        case "onlyFromTableau":
            return .onlyFromTableau
        case "upSameColor":
            return .upSameColor
        case "downSameColor":
            return .downSameColor
        case "upDiff2InSuit":
            return .upDiff2InSuit
        default:
            fatalError("falscher TypeOfDepositFromUser")
        }
    }
    
    class func convertToTypeOfDepositIfEmpty(_ pileLayoutPList:Dictionary<String,Any>, key:String) -> TypeOfDepositIfEmpty {
        let str = pileLayoutPList[key] as! String
        switch str {
        case "notAvailable":
            return .typeOfDepositIfEmptyNA
        case "noCard":
            return .noCard
        case "basicCard":
            return .basicCard
        case "basicCardMinus1":
            return .basicCardMinus1
        case "ace":
            return .ace
        case "king":
            return .king
        case "fromDealOrAny":
            return .fromDealOrAny
        case "anyCard":
            return .anyCard
        case "onlyFromStockCanDepositIfEmpty":
            return .onlyFromStockCanDepositIfEmpty
        case "onlyFromAutoDealCanDepositIfEmpty":
            return .onlyFromAutoDealCanDepositIfEmpty
        case "onlyFromTableauCanDepositIfEmpty":
            return .onlyFromTableauCanDepositIfEmpty
        default:
            fatalError("falscher TypeOfDepositIfEmpty")
        }
    }
    
    class func convertToTypeOfPermittedToPlay(_ pileLayoutPList:Dictionary<String,Any>, key:String) -> TypeOfPermittedToPlay {
        let str = pileLayoutPList[key] as! String
        switch str {
        case "notAvailable":
            return .typeOfPermittedToPlayNA
        case "tableauPermittedToPlay":
            return .tableauPermittedToPlay
        case "wastePermittedToPlay":
            return .wastePermittedToPlay
        case "foundationPermittedToPlay":
            return .foundationPermittedToPlay
        case "tableauOrFoundationPermittedToPlay":
            return .tableauOrFoundationPermittedToPlay
        case "waste_Tableau_FoundationPermittedToPlay":
            return .waste_Tableau_FoundationPermittedToPlay
        default:
            fatalError("falscher TypeOfPermittedToPlay")
        }
    }
    
    class func convertToTypeOfBasicCard(_ pileLayoutPList:Dictionary<String,Any>, key:String) -> TypeOfBasicCard {
        let str = pileLayoutPList[key] as! String
        switch str {
        case "notAvailable":
            return .typeOfBasicCardNA
        case "firstPlayed":
            return .firstPlayed
        case "fixedButNotLie":
            return .fixedButNotLie
        case "fixedLie":
            return .fixedLie
        case "fromStock":
            return .fromStock
        default:
            fatalError("falscher TypeOfBasicCard")
        }
    }
    
    class func convertToTypeOfDealingFromStock(_ pileLayoutPList:Dictionary<String,Any>, key:String) -> TypeOfDealingFromStock {
        let str = pileLayoutPList[key] as! String
        switch str {
        case "notAvailable":
            return .typeOfDealingFromStockNA
        case "dealToTableaus":
            return .dealToTableaus
        case "dealToWaste":
            return .dealToWaste
        default:
            fatalError("falscher TypeOfDealingFromStock")
        }
    }
    
    class func convertToTypeOfRedeal(_ pileLayoutPList:Dictionary<String,Any>, key:String) -> TypeOfRedeal {
        let str = pileLayoutPList[key] as! String
        switch str {
        case "notAvailable":
            return .typeOfRedealNA
        case "redealToTableaus":
            return .redealToTableaus
        case "redealToTableausMix":
            return .redealToTableausMix
        case "redealToWaste":
            return .redealToWaste
        case "redealToWasteMix":
            return .redealToWasteMix
        default:
            fatalError("falscher TypeOfRedeal")
        }
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
