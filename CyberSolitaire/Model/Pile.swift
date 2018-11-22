//
//  Pile.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 08.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import Foundation
import UIKit

let defaultFaceUpOverlapPercent:CGFloat = 30.0;
let defaultFaceDownOverlapPercent:CGFloat = 10.0;
let pileOverlapRatio = defaultFaceUpOverlapPercent / defaultFaceDownOverlapPercent;


// welche Karten sind zu Beginn des Spieles sichtbar
enum FaceAtStart: Int {
    case
    faceAtStartNA = 0,
    allFaceUp,                      // alle Karten des Stapels sind sichtbar
    allFaceDown,                    // keine Karte des Stapels ist sichtbar
    lastFaceUp,                     // nur die letzte Karte des Stapels ist sichtbar
    oddFaceUp,                      // jede ungerade (1.,3.,..) Karte ist sichtbar
    evenFaceUp                      // jede gerade (2.,4.,..) Karte ist sichtbar
    
    func description() -> String {
        switch self {
        case .faceAtStartNA:  return "notAvailable"
        case .allFaceUp: return "allFaceUp"
        case .allFaceDown: return "allFaceDown"
        case .lastFaceUp: return "lastFaceUp"
        case .oddFaceUp: return "oddFaceUp"
        case .evenFaceUp: return "evenFaceUp"
        }
    }
    
    
    static func valueFromDescription(_ description:String) -> FaceAtStart {
        switch description {
        case "notAvailable": return .faceAtStartNA
        case "allFaceUp": return .allFaceUp
        case "allFaceDown": return .allFaceDown
        case "lastFaceUp": return .lastFaceUp
        case "oddFaceUp": return .oddFaceUp
        case "evenFaceUp": return .evenFaceUp
        default: fatalError("falscher FaceAtStart")
        }
    }
}

// welche Art der Überlappung hat dieser Stapel
enum TypeOfOverlap: Int {
    case
    overlapModeNA = 0,
    notOverlapped,              // alle Karten liegen übereinander
    downOverlapped              // Karten überlappen nach unten
    
    func description() -> String {
        switch self {
        case .overlapModeNA:  return "notAvailable"
        case .notOverlapped: return "notOverlapped"
        case .downOverlapped: return "downOverlapped"
        }
    }
    
    
    static func valueFromDescription(_ description:String) -> TypeOfOverlap {
        switch description {
        case "notAvailable": return .overlapModeNA
        case "notOverlapped": return .notOverlapped
        case "downOverlapped": return .downOverlapped
        default: fatalError("falscher TypeOfOverlap")
        }
    }
    
}

enum TypeOfPile: Int {
    case
    typeOfPileNA = 0,
    stock,
    tableau,
    multiFoundation,
    foundation,
    waste
    
    func description() -> String {
        switch self {
        case .typeOfPileNA:  return "notAvailable"
        case .stock: return "Stock"
        case .tableau: return "Tableau"
        case .multiFoundation: return "MultiFoundation"
        case .foundation: return "Foundation"
        case .waste: return "Waste"
        }
    }
    
    static func valueFromDescription(_ description:String) -> TypeOfPile {
        switch description {
        case "notAvailable": return .typeOfPileNA
        case "Stock": return .stock
        case "Tableau": return .tableau
        case "MultiFoundation": return .multiFoundation
        case "Foundation": return .foundation
        case "Waste": return .waste
        default: fatalError("falscher TypeOfPileType")
        }
    }
    
}

// welche Sequenzen dürfen vom Spieler selektiert und bewegt werden
// Sequencen sind immer: abwärts vom höheren Kartenwert zu niedrigeren
enum TypeOfUserSelectAndMove: Int {
    case
    typeOfUserSelectAndMoveNA = 0,
    // beliebige Sequenz (also auch Stücke einer längeren)
    anySequenceInSuit,              // beliebige Sequenz Farbfolge
    anySeqAlternate,                // beliebige Sequenz Farbwechsel
    anySeqNoColor,                  // beliebige Sequenz ohne Farbfolge
    // gesamte   Sequenz -> keine Stücke einer längeren Sequenz
    allSeqNoColorOrLast,            // gesamte   Sequenz ohne Farbfolge oder letzte Karte
    allSeqInSuitOrLast,             // gesamte   Sequenz Farbfolge      oder letzte Karte
    allSeqAlternateOrLast,          // gesamte   Sequenz Farbwechsel    oder letzte Karte
    all,                            // alle Karten des Stapels
    onlyLast,                       // nur letzte Karte
    exactOne,                       // genau eine Karte des Stapels
    allFaceUpCanSelect,             // alle sichtbaren Karten
    pairsEqualValueCanSelect,                // Paare gleichen Werts
    pairsValue13                    // Paare mit Wert 13
    
    func description() -> String {
        switch self {
        case .typeOfUserSelectAndMoveNA:  return "notAvailable"
        case .anySequenceInSuit: return "anySequenceInSuit"
        case .anySeqAlternate: return "anySeqAlternate"
        case .anySeqNoColor: return "anySeqNoColor"
        case .allSeqNoColorOrLast: return "allSeqNoColorOrLast"
        case .allSeqInSuitOrLast: return "allSeqInSuitOrLast"
        case .allSeqAlternateOrLast: return "allSeqAlternateOrLast"
        case .all: return "all"
        case .onlyLast: return "onlyLast"
        case .exactOne: return "exactOne"
        case .allFaceUpCanSelect: return "allFaceUpCanSelect"
        case .pairsEqualValueCanSelect: return "pairsEqualValueCanSelect"
        case .pairsValue13: return "pairsValue13"
        }
    }
    
    
    static func valueFromDescription(_ description:String) -> TypeOfUserSelectAndMove {
        switch description {
        case "notAvailable": return .typeOfUserSelectAndMoveNA
        case "anySequenceInSuit": return .anySequenceInSuit
        case "anySeqAlternate": return .anySeqAlternate
        case "anySeqNoColor": return .anySeqNoColor
        case "allSeqNoColorOrLast": return .allSeqNoColorOrLast
        case "allSeqInSuitOrLast": return .allSeqInSuitOrLast
        case "allSeqAlternateOrLast": return .allSeqAlternateOrLast
        case "all": return .all
        case "onlyLast": return .onlyLast
        case "exactOne": return .exactOne
        case "allFaceUpCanSelect": return .allFaceUpCanSelect
        case "pairsEqualValueCanSelect": return .pairsEqualValueCanSelect
        case "pairsValue13": return .pairsValue13
        default: fatalError("falscher TypeOfUserSelectAndMove")
        }
    }
    
}

// wie ist die Reihenfolge beim Ablegen durch den Benutzer
enum TypeOfDepositFromUser: Int {
    case
    typeOfDepositFromUserNA = 0,
    downInSuit,                     // abwärts  Farbfolge
    downAlternate,                  // abwärts  Farbwechsel
    downNoColor,                    // abwärts  ohne Farbfolge
    upInSuit,                       // aufwärts Farbfolge
    upAlternate,                    // aufwärts Farbwechsel
    upNoColor,                      // aufwärts ohne Farbfolge
    withGaps,                       // mit Lücken
    onlyFromStock,                  // beliebig, aber nur vom Stock
    nothing,                        // auf diesen Stapel kann nicht abgelegt werden
    downInSuitRolling,              // abwärts  Farbfolge (mit ‹berlauf As->Kˆnig)
    upInSuitRolling,                // aufwärts Farbfolge (mit ‹berlauf Kˆnig->As)
    downAlternateRolling,           // abwärts  Farbwechsel (mit ‹berlauf As->Kˆnig)
    upAlternateRolling,             // aufwärts Farbwechsel (mit ‹berlauf Kˆnig->As)
    pairsEqualValue,                // Paare gleichen Wertes
    pairsValue13OrKing,             // Paare mit Wert 13 oder einzelner Kˆnig
    upDownNoColorRolling,           // aufwärts oder abwärts mit ‹berlauf ohne Farbfolge
    oneFromTableauOrWaste,          // nur 1 Karte vom Tableau oder Waste
    onlyFromTableau,                // beliebig, aber nur von Tableaus
    upSameColor,                    // aufwärts gleiche Farbe wie Basiskarte
    downSameColor,                  // abwärts gleiche Farbe wie Basiskarte
    upDiff2InSuit                   // aufwärts im Abstand von 2 in Farbfolge
    
    func description() -> String {
        switch self {
        case .typeOfDepositFromUserNA: return "notAvailable"
        case .downInSuit: return "downInSuit"
        case .downAlternate: return "downAlternate"
        case .downNoColor: return "downNoColor"
        case .upInSuit: return "upInSuit"
        case .upAlternate: return "upAlternate"
        case .upNoColor: return "upNoColor"
        case .withGaps: return "withGaps"
        case .onlyFromStock: return "onlyFromStock"
        case .nothing: return "nothing"
        case .downInSuitRolling: return "downInSuitRolling"
        case .upInSuitRolling: return "upInSuitRolling"
        case .downAlternateRolling: return "downAlternateRolling"
        case .upAlternateRolling: return "upAlternateRolling"
        case .pairsEqualValue: return "pairsEqualValue"
        case .pairsValue13OrKing: return "pairsValue13OrKing"
        case .upDownNoColorRolling: return "upDownNoColorRolling"
        case .oneFromTableauOrWaste: return "oneFromTableauOrWaste"
        case .onlyFromTableau: return "onlyFromTableau"
        case .upSameColor: return "upSameColor"
        case .downSameColor: return "downSameColor"
        case .upDiff2InSuit: return "upDiff2InSuit"
        }
    }
    
    
    static func valueFromDescription(_ description:String) -> TypeOfDepositFromUser {
        switch description {
        case "notAvailable": return .typeOfDepositFromUserNA
        case "downInSuit": return .downInSuit
        case "downAlternate": return .downAlternate
        case "downNoColor": return .downNoColor
        case "upInSuit": return .upInSuit
        case "upAlternate": return .upAlternate
        case "upNoColor": return .upNoColor
        case "withGaps": return .withGaps
        case "onlyFromStock": return .onlyFromStock
        case "nothing": return .nothing
        case "downInSuitRolling": return .downInSuitRolling
        case "upInSuitRolling": return .upInSuitRolling
        case "downAlternateRolling": return .downAlternateRolling
        case "upAlternateRolling": return .upAlternateRolling
        case "pairsEqualValue": return .pairsEqualValue
        case "pairsValue13OrKing": return .pairsValue13OrKing
        case "upDownNoColorRolling": return .upDownNoColorRolling
        case "oneFromTableauOrWaste": return .oneFromTableauOrWaste
        case "onlyFromTableau": return .onlyFromTableau
        case "upSameColor": return .upSameColor
        case "downSameColor": return .downSameColor
        case "upDiff2InSuit": return .upDiff2InSuit
        default: fatalError("falscher TypeOfDepositFromUser")
        }
    }
    
}

// womit kann ein leerer Stapel belegt werden
enum TypeOfDepositIfEmpty: Int {
    case
    typeOfDepositIfEmptyNA	= 0,
    noCard,                         // nicht mehr belegbar
    basicCard,                      // mit einer Basiskarte
    basicCardMinus1,                // mit Basiskarte - 1
    ace,                            // As
    king,                           // König
    fromDealOrAny,                  // wird automatisch belegt oder beliebige Karte, wenn keine Karten zum Geben
    anyCard,                        // beliebiger Karte
    onlyFromStockCanDepositIfEmpty,	// darf nur vom Stock belegt werden
    onlyFromAutoDealCanDepositIfEmpty,	// wird ausschlieﬂlich automatisch von den Deal-Piles belegt
    onlyFromTableauCanDepositIfEmpty	// beliebig, aber nur von Tableaus
    
    func description() -> String {
        switch self {
        case .typeOfDepositIfEmptyNA: return "notAvailable"
        case .noCard: return "noCard"
        case .basicCard: return "basicCard"
        case .basicCardMinus1: return "basicCardMinus1"
        case .ace: return "ace"
        case .king: return "king"
        case .fromDealOrAny: return "fromDealOrAny"
        case .anyCard: return "anyCard"
        case .onlyFromStockCanDepositIfEmpty: return "onlyFromStockCanDepositIfEmpty"
        case .onlyFromAutoDealCanDepositIfEmpty: return "onlyFromAutoDealCanDepositIfEmpty"
        case .onlyFromTableauCanDepositIfEmpty: return "onlyFromTableauCanDepositIfEmpty"
        }
    }
    
    static func valueFromDescription(_ description:String) -> TypeOfDepositIfEmpty {
        switch description {
        case "notAvailable": return .typeOfDepositIfEmptyNA
        case "noCard": return .noCard
        case "basicCard": return .basicCard
        case "basicCardMinus1": return .basicCardMinus1
        case "ace": return .ace
        case "king": return .king
        case "fromDealOrAny": return .fromDealOrAny
        case "anyCard": return .anyCard
        case "onlyFromStockCanDepositIfEmpty": return .onlyFromStockCanDepositIfEmpty
        case "onlyFromAutoDealCanDepositIfEmpty": return .onlyFromAutoDealCanDepositIfEmpty
        case "onlyFromTableauCanDepositIfEmpty": return .onlyFromTableauCanDepositIfEmpty
        default: fatalError("falscher TypeOfDepositIfEmpty")
        }
    }
    
}

// auf welche Piles darf von diesem Stapel gespielt werden (durch eine Aktion des Users)
// hat nichts mit dem geben zu tun
enum TypeOfPermittedToPlay: Int {
    case
    typeOfPermittedToPlayNA	= 0,
    tableauPermittedToPlay,                 // auf Tableau
    wastePermittedToPlay,                   // auf Waste
    foundationPermittedToPlay,              // auf Foundation
    tableauOrFoundationPermittedToPlay,		// auf Tableau or Foundation
    waste_Tableau_FoundationPermittedToPlay	// auf Waste, Tableau or Foundation
    
    func description() -> String {
        switch self {
        case .typeOfPermittedToPlayNA: return "notAvailable"
        case .tableauPermittedToPlay: return "tableauPermittedToPlay"
        case .wastePermittedToPlay: return "wastePermittedToPlay"
        case .foundationPermittedToPlay: return "foundationPermittedToPlay"
        case .tableauOrFoundationPermittedToPlay: return "tableauOrFoundationPermittedToPlay"
        case .waste_Tableau_FoundationPermittedToPlay: return "waste_Tableau_FoundationPermittedToPlay"
        }
    }
    
    static func valueFromDescription(_ description:String) -> TypeOfPermittedToPlay {
        switch description {
        case "notAvailable": return .typeOfPermittedToPlayNA
        case "tableauPermittedToPlay": return .tableauPermittedToPlay
        case "wastePermittedToPlay": return .wastePermittedToPlay
        case "foundationPermittedToPlay": return .foundationPermittedToPlay
        case "tableauOrFoundationPermittedToPlay": return .tableauOrFoundationPermittedToPlay
        case "waste_Tableau_FoundationPermittedToPlay": return .waste_Tableau_FoundationPermittedToPlay
        default: fatalError("falscher TypeOfPermittedToPlay")
        }
    }
    
}

// welcher Art ist die Basiskarte eines Foundations
enum TypeOfBasicCard: Int {
    case
    typeOfBasicCardNA	= 0,
    firstPlayed,                    // erste abgelegte Karte wird Basiskarte
    fixedButNotLie,                 // vorgegeben, liegt aber nicht
    fixedLie,                       // vorgegeben, liegt zu Beginn
    fromStock                       // wird bei Start vom Stock gezogen
    
    func description() -> String {
        switch self {
        case .typeOfBasicCardNA: return "notAvailable"
        case .firstPlayed: return "firstPlayed"
        case .fixedButNotLie: return "fixedButNotLie"
        case .fixedLie: return "fixedLie"
        case .fromStock: return "fromStock"
        }
    }
    
    
    static func valueFromDescription(_ description:String) -> TypeOfBasicCard {
        switch description {
        case "notAvailable": return .typeOfBasicCardNA
        case "firstPlayed": return .firstPlayed
        case "fixedButNotLie": return .fixedButNotLie
        case "fixedLie": return .fixedLie
        case "fromStock": return .fromStock
        default: fatalError("falscher TypeOfBasicCard")
        }
    }
    
}

enum TypeOfDealingFromStock: Int {
    case
    typeOfDealingFromStockNA	= 0,
    dealToTableaus,                 // auf Spielstapel
    dealToWaste                     // in den Waste	(dann ist die Anzahl interessant)
    
    func description() -> String {
        switch self {
        case .typeOfDealingFromStockNA: return "notAvailable"
        case .dealToTableaus: return "dealToTableaus"
        case .dealToWaste: return "dealToWaste"
        }
    }
    
    
    static func valueFromDescription(_ description:String) -> TypeOfDealingFromStock {
        switch description {
        case "notAvailable": return .typeOfDealingFromStockNA
        case "dealToTableaus": return .dealToTableaus
        case "dealToWaste": return .dealToWaste
        default: fatalError("falscher TypeOfDealingFromStock")
        }
    }
    
}

enum TypeOfRedeal: Int {
    case
    typeOfRedealNA	= 0,
    redealToTableaus,               // Spielstapel einsammeln
    redealToTableausMix,            // Spielstapel einsammeln unsd mischen
    redealToWaste,                  // Waste einsammeln
    redealToWasteMix                // Waste einsammeln unsd mischen
    
    func description() -> String {
        switch self {
        case .typeOfRedealNA: return "notAvailable"
        case .redealToTableaus: return "redealToTableaus"
        case .redealToTableausMix: return "redealToTableausMix"
        case .redealToWaste: return "redealToWaste"
        case .redealToWasteMix: return "redealToWasteMix"
        }
    }
    
    
    static func valueFromDescription(_ description:String) -> TypeOfRedeal {
        switch description {
        case "notAvailable": return .typeOfRedealNA
        case "redealToTableaus": return .redealToTableaus
        case "redealToTableausMix": return .redealToTableausMix
        case "redealToWaste": return .redealToWaste
        case "redealToWasteMix": return .redealToWasteMix
        default: fatalError("falscher TypeOfRedeal")
        }
    }
    
}

class Pile: NSObject {
    
    struct statics {
        static var gamePile : Pile? = nil
    }

    var undoManager = UndoManager()
    var cardIdsBeforeShuffle: [Int] = []

    var faceDownOverlapPercent = defaultFaceDownOverlapPercent
    var faceUpOverlapPercent = defaultFaceUpOverlapPercent
    
    var selectedCards: [Card] = []
    
    let pileId : Int
    let pileType: TypeOfPile
    /*! enthält alle Karten des Stapels */
    var cards: [Card] = []
    let overlapType : TypeOfOverlap
    let userSelectAndMoveType: TypeOfUserSelectAndMove
    
    let indexForEmptyPileImage: Int
    let numberOfCardsAtStart: Int
    let dealOrderAtStart: Int
    let faceAtStart : FaceAtStart
    let depositFromUserType: TypeOfDepositFromUser
    let depositIfEmptyType: TypeOfDepositIfEmpty
    let permittedToPlayType: TypeOfPermittedToPlay
    let basicCardType: TypeOfBasicCard
    let dealingFromStockType: TypeOfDealingFromStock
    let redealType: TypeOfRedeal
    let numberPerMove: Int
    
    var pileEmptySize = CGSize.zero
    var pileSize = CGSize.zero
    var oldPileSize = CGSize.zero
    var pilePosition = CGPoint.zero
    var maxPileHeight: CGFloat = 0.0
    
    var numberOfCards : Int {
        get {
            return cards.count
        }
    }
    
    var autoLayDown : Bool {
        get {
            return pileType == .foundation
        }
    }
    
    var pileSizeHasChanged: Bool {
        get {
            return (pileSize.width != oldPileSize.width) || (pileSize.height != oldPileSize.height)
        }
    }
    
    var memoCardsRepositioned = false   // memo bleibt gesetzt, bis es von aussen mit resetCardsRepositioned() zurückgesetzt wird
    
    // MARK: initialisierer
    
    init(pileID: Int,
        pileType: TypeOfPile,
        overlapType: TypeOfOverlap,
        userSelectAndMoveType: TypeOfUserSelectAndMove,
        depositFromUserType: TypeOfDepositFromUser,
        depositIfEmptyType: TypeOfDepositIfEmpty,
        permittedToPlayType: TypeOfPermittedToPlay,
        basicCardType: TypeOfBasicCard,
        dealingFromStockType: TypeOfDealingFromStock,
        redealType: TypeOfRedeal,
        numberPerMove: Int,
        indexForEmptyPileImage: Int,
        numberOfCardsAtStart: Int,
        dealOrderAtStart: Int,
        faceAtStart: FaceAtStart) {
        // der Initialisierer enthält alle Komponenten aus dem PileLayout ausser der Position
        self.pileId = pileID
        self.pileType = pileType
        self.overlapType = overlapType
        self.userSelectAndMoveType = userSelectAndMoveType
        self.depositFromUserType = depositFromUserType
        self.depositIfEmptyType = depositIfEmptyType
        self.permittedToPlayType = permittedToPlayType
        self.basicCardType = basicCardType
        self.dealingFromStockType = dealingFromStockType
        self.redealType = redealType
        self.numberPerMove = numberPerMove
        self.indexForEmptyPileImage = indexForEmptyPileImage
        self.numberOfCardsAtStart = numberOfCardsAtStart
        self.dealOrderAtStart = dealOrderAtStart
        self.faceAtStart = faceAtStart
        super.init()
    }
    
    deinit {
        log.verbose("Pile deinit")
    }
    
    // MARK: Aktionen, die in den UndoManager eingespeist werden
    
    @objc func shuffle() {
        // MARK: sendet die Notification: playSoundNotification
        // mischt den Pile
        // TODO: muss wieder raus
        //srandom(time(NULL));
        srandom(0);
        
        
        // wenn kein redo mehr existiert, gehört das Mischen auch nicht mehr in den undoManager
        // sofern shuffle ganz normal im Programm ausgeführt wird, merke dir die cardId aller Karten vor dem Mischen in der Instanzvariablen cardIdBeforeShuffle
        cardIdsBeforeShuffle.removeAll(keepingCapacity: false)
        for card in cards {
            cardIdsBeforeShuffle.append(card.cardId)
        }
        // jetzt werden die Karten x-mal zufällig getauscht
        let mutableCards = NSMutableArray(array: cards)
        for _ in 0 ..< 1000 {
            let index1 = arc4random() % UInt32(cards.count)
            let index2 = arc4random() % UInt32(cards.count)
            if index1 != index2 {
                mutableCards.exchangeObject(at: Int(index1), withObjectAt: Int(index2))
            }
        }
        cards = mutableCards as AnyObject as! [Card]
        // unterrichte den Controller, damit der den Sound abspielen kann
        NotificationCenter.default.post(name: Notification.Name(rawValue: playSoundNotification), object: self, userInfo: (NSDictionary(object: "shuffle", forKey: "soundName" as NSCopying) as! [AnyHashable: Any]))

/*  siehe oben
        // falls kein Undo oder Redo durchgeführt wird
        if !(undoManager.isUndoing || undoManager.isRedoing) {
            // sofern shuffle ganz normal im Programm ausgeführt wird, merke dir die cardId aller Karten vor dem Mischen in der Instanzvariablen cardIdBeforeShuffle
            cardIdsBeforeShuffle.removeAll(keepingCapacity: false)
            for card in cards {
                cardIdsBeforeShuffle.append(card.cardId)
            }
        }
        let theProxy: (AnyObject) = undoManager.prepare(withInvocationTarget: self) as (AnyObject)
        // falls ein Undo oder Redo durchgeführt wird
        if (undoManager.isUndoing || undoManager.isRedoing) {
            // sofern shuffle beim Undo/Redo ausgeführt wird, tausche die Elemente des aktuellen Zustands mit denen des gespeicherten aus und merke dir danach wieder den aktuellen Zustand in der Instanzvariablen
            var tmpCardIds: [Int] = []
            for card in cards {
                tmpCardIds.append(card.cardId)
            }
            let mutableCards = NSMutableArray(array: cards)
            for i in 0 ..< cards.count {
                let IDBeforeShuffle = cardIdsBeforeShuffle[i]
                for index in 0 ..< cards.count {
                    if (i != index) && ((mutableCards.object(at: index) as! Card).cardId == IDBeforeShuffle) {
                        mutableCards.exchangeObject(at: i, withObjectAt: index)
                        //log.debug("exchanged i:\(i) with index:\(index)")
                        break
                    }
                }
            }
            cards = mutableCards as AnyObject as! [Card]
            // merke dir wieder den jetzt aktuellen Zustand
            cardIdsBeforeShuffle = tmpCardIds
        }
        else {
            let mutableCards = NSMutableArray(array: cards)
            for _ in 0 ..< 1000 {
                let index1 = arc4random() % UInt32(cards.count)
                let index2 = arc4random() % UInt32(cards.count)
                if index1 != index2 {
                    mutableCards.exchangeObject(at: Int(index1), withObjectAt: Int(index2))
                }
            }
            cards = mutableCards as AnyObject as! [Card]
            // unterrichte den Controller, damit der den Sound abspielen kann
            NotificationCenter.default.post(name: Notification.Name(rawValue: playSoundNotification), object: self, userInfo: (NSDictionary(object: "shuffle", forKey: "soundName" as NSCopying) as! [AnyHashable: Any]))
        }
        theProxy.shuffle()
 */
    }
    
    // MARK: Selektierer Aktionen (senden Notifikations)
    
    func selectCardOrSequenceWithCard(_ card: Card) {
        // MARK: sendet die Notification: selectCardsNotification
        // Achtung: hier wird nicht mehr die Konformität einer Sequenz überprüft, sondern es werden beginnend mit inCard alle nachfolgenden Karten des Stapels als selektiert (für den Spielzug) gekennzeichnet
        selectedCards.removeAll(keepingCapacity: false)
        // finde die Karte im Stapel
        let startIndex = findIndexForCard(card)
        for i in startIndex ..< cards.count {
            cards[i].selected = true
            selectedCards.append(cards[i])
        }
        // unterrichte den Controller, damit der den Sound abspielen kann
        NotificationCenter.default.post(name: Notification.Name(rawValue: playSoundNotification), object: self, userInfo: (NSDictionary(object: "select", forKey: "soundName" as NSCopying) as! [AnyHashable: Any]))
        // es wird eine Benachrichtigung verschickt, damit die selektierten Karten optisch im View verändert werden können
        NotificationCenter.default.post(name: Notification.Name(rawValue: selectCardsNotification), object: self, userInfo: (NSDictionary(object: selectedCards, forKey: "selectCards" as NSCopying) as! [AnyHashable: Any]))
    }
    
    func unselectCards() {
        // MARK: sendet die Notification: selectCardsNotification
        var tmpSelectedCards: [Card] = []
        for card in cards {
            if card.selected {
                card.selected = false
                tmpSelectedCards.append(card)
            }
        }
        // es wird eine Benachrichtigung verschickt, damit die selektierten Karten optisch im View verändert werden können
        NotificationCenter.default.post(name: Notification.Name(rawValue: selectCardsNotification), object: self, userInfo: (NSDictionary(object: tmpSelectedCards, forKey: "unselectCards" as NSCopying) as! [AnyHashable: Any]))
    }
    
    // MARK: Aktionen, die die Geometrie des Stapels betreffen
    
    func updatePileGeometryWithResize(_ resize: Bool) {
        // MARK: Observable: moveCard
        
        // merke die ursprüngliche Größe des Stapels
        oldPileSize = pileSize
        // die Positionen aller Karten im Pile werden gesetzt
        var tmpOverlapValue: CGFloat = 0.0
        switch overlapType {
        case .notOverlapped:
            pileSize = pileEmptySize
            for card in cards {
                card.position = pilePosition
            }
        case .downOverlapped:
            if isPileEmpty() {
                pileSize = pileEmptySize
            }
            else {
                // die erste Karte liegt immer an der Pile Position
                var cardBefore = cards.first!
                cardBefore.position = pilePosition
                // falls noch weitere Karten existieren
                for i in 1 ..< numberOfCards {
                    let card: Card! = cards[i]
                    if cardBefore.faceUp || cardBefore.markedForCheat {
                        tmpOverlapValue += faceUpOverlapPercent * pileEmptySize.height / 100.0
                        card.position = CGPoint(x: pilePosition.x, y: pilePosition.y + tmpOverlapValue)
                    }
                    else {
                        tmpOverlapValue += faceDownOverlapPercent * pileEmptySize.height / 100.0
                        card.position = CGPoint(x: pilePosition.x, y: pilePosition.y + tmpOverlapValue)
                    }
                    cardBefore = card
                }
                pileSize = CGSize(width: pileSize.width, height: tmpOverlapValue + pileEmptySize.height)
                if resize {
                    // falls die Höhe jetzt zu groß geworden ist, muss der Stapel als Ganzes kleiner werden
                    if pileSize.height > maxPileHeight {
                        decreasePileSize()
                        updatePileGeometryWithResize(false)
                        // verschiebe alle Karten, denn ausser der ersten Karte haben alle anderen die Position verändert
                        memoCardsRepositioned = true
                    }
                    else {
                        // falls die berechnete Standardgröße den Maximalwert unterschreitet, kann ich den Stapel vergrößern
                        //let tmpSize = getPileSizeWithDefaults()
                        //TODO: erst mal ein Versuch
                        //if (tmpSize.height <= maxPileHeight) &&
                        if (pileSize.height <= maxPileHeight) &&
                            ((faceUpOverlapPercent != defaultFaceUpOverlapPercent) || (faceDownOverlapPercent != defaultFaceDownOverlapPercent)) {
                                increasePileSize()
                                updatePileGeometryWithResize(false)
                                // verschiebe alle Karten, denn ausser der ersten Karte haben alle anderen die Position verändert
                                memoCardsRepositioned = true

                        }
                    }
                }
            }
        default:
            log.error("irgendwas ist wieder richtig schief gelaufen")
            break
        }
    }
    
    func getPileSizeWithDefaults() -> CGSize {
        // liefert die pileSize bei Verwendung der Standardwerte
        // berechne die Stapelgröße mit den Standardwerten
        var retSize = pileEmptySize
        var tmpOverlapValue:CGFloat = 0.0
        switch overlapType {
        case .notOverlapped:
            return retSize
        case .downOverlapped:
            var cardBefore = cards.first
            for i in 1 ..< numberOfCards {
                let card = cards[i]
                if cardBefore!.faceUp || cardBefore!.markedForCheat {
                    tmpOverlapValue += defaultFaceUpOverlapPercent * pileEmptySize.height / 100.0
                }
                else {
                    tmpOverlapValue += defaultFaceDownOverlapPercent * pileEmptySize.height / 100.0
                }
                cardBefore = card
            }
            retSize = CGSize(width: pileSize.width, height: tmpOverlapValue + pileEmptySize.height)
        default:
            log.error("Fehler aufgetreten")
        }
        return retSize
    }
    
    func decreasePileSize () {
        // verringert die Höhe des Stapels, sodass die maximal Höhe eingehalten wird
        var countBiggerCards = 0
        var countSmallerCards = 0
        switch overlapType {
        case .notOverlapped:
            break
        case .downOverlapped:
            // berechne die neuen Werte für die Überlappungen
            var cardBefore = cards.first
            for i in 1 ..< numberOfCards {
                let card = cards[i]
                if cardBefore!.faceUp || cardBefore!.markedForCheat {
                    countBiggerCards += 1
                }
                else {
                    countSmallerCards += 1
                }
                cardBefore = card
            }
        default:
            log.error("das darf nicht geschehen")
        }
        let newFaceOverlapPercent = ((maxPileHeight-pileEmptySize.height)*100.0/pileEmptySize.height)/(CGFloat(countBiggerCards) + CGFloat(countSmallerCards)/pileOverlapRatio)
        faceUpOverlapPercent = newFaceOverlapPercent
        faceDownOverlapPercent = faceUpOverlapPercent/pileOverlapRatio
    }
    
    func increasePileSize () {
        // vergrößert die Höhe des Stapels, sodass die maximale Höhe eingehalten wird
        // bis die Standardwerte erreicht sind
        var countBiggerCards = 0
        var countSmallerCards = 0
        switch overlapType {
        case .notOverlapped:
            break
        case .downOverlapped:
            // berechne die neuen Werte für die Überlappungen
            var cardBefore = cards.first
            for i in 1 ..< numberOfCards {
                let card = cards[i]
                if cardBefore!.faceUp || cardBefore!.markedForCheat {
                    countBiggerCards += 1
                }
                else {
                    countSmallerCards += 1
                }
                cardBefore = card
            }
        default:
            log.error("das darf nicht geschehen")
        }
        let newFaceOverlapPercent = ((maxPileHeight-pileEmptySize.height)*100.0/pileEmptySize.height)/(CGFloat(countBiggerCards) + CGFloat(countSmallerCards)/pileOverlapRatio)
        if newFaceOverlapPercent >= defaultFaceUpOverlapPercent {
            // größer als die Standardwerte ist nicht gut
            faceUpOverlapPercent = defaultFaceUpOverlapPercent
            faceDownOverlapPercent = defaultFaceDownOverlapPercent
        }
        else {
            faceUpOverlapPercent = newFaceOverlapPercent
            faceDownOverlapPercent = faceUpOverlapPercent/pileOverlapRatio
        }
    }
    
    func increasePileSizeToDefault() {
        // vergrößert die Größe des Stapels auf die Standardwerte
        switch overlapType {
        case .notOverlapped:
            break
        case .downOverlapped:
            faceUpOverlapPercent = defaultFaceUpOverlapPercent
            faceDownOverlapPercent = defaultFaceDownOverlapPercent
        default:
            log.error("übler Fehler")
        }
    }

    func repositionLastCard() {
        updatePileGeometryWithResize(true)
    }
    
    // MARK: Aktionen mit Karten
    
    func putCard(_ card: Card) {
        // legt eine neue Karte oben auf den Stapel
        cards.append(card)
        updatePileGeometryWithResize(true)
    }
    
    @discardableResult func removeLastCard() -> Card? {
        // entfernt die letzte Karte vom Stapel
        if let card = self.getLastCard() {
            cards.removeLast()
            updatePileGeometryWithResize(true)
            return card
        }
        return nil
    }
    
    func moveSequence(_ sequence: [Card], fromPile: Pile) {
        // verbringe alle Karten in den neuen Stapel
        if let firstCard = sequence.first {
            cards = cards + sequence
            if let startIndex = fromPile.cards.index(of: firstCard) {
//            if let startIndex = find(fromPile.cards,firstCard) {
                // entferne die Karten aus dem fromPile
                let range = (startIndex ..< fromPile.cards.count) // start ..< end
                fromPile.cards.removeSubrange(range)
                self.updatePileGeometryWithResize(true)
                fromPile.updatePileGeometryWithResize(true)
            }
            else {
                log.error("die firstCard der Sequenz gehört nicht zum fromPile")
            }
        }
        else {
            log.error("die Sequenz hat keine firstCard -> sie muss leer sein")
        }
    }
    
    // MARK: Getter
    
    func createSequenceFromSelection() -> [Card] {
        return self.selectedCards
    }
    
    
    func createRevertedSequenceFromSelection() -> [Card] {
        var tmpReverted : [Card] = []
        for card in selectedCards.reversed() {
            tmpReverted.append(card)
        }
        return tmpReverted
    }
    
    func getLastCard() -> Card? {
        // liefert die oberste Karte vom Stapel, falls der Stapel leer ist "nil" 
        // die Karte wird nicht vom Stapel entfernt
        return cards.last
    }

    func getCardBefore(_ card: Card) -> Card? {
        // liefert die Karte bevor card, falls diese existiert
        if let cardIndex = cards.index(of: card) {
//        if let cardIndex = find(cards, card) {
            // die Karte befindet sich im Stapel
            if cardIndex > 0 {
                // vor der Karte gibt es noch weitere
                // liefere die direkt bevor
                return cards[cardIndex-1]
            }
            else {
                // es gibt keine Karte davor
                return nil
            }
        }
        else {
            assert(false, "die Karte muss sich im Stapel befinden! tut sie aber nicht")
            return nil
        }
    }
    
    func firstSelectedCard() -> Card? {
        if selectedCards.count != 0 {
            return selectedCards[0]
        }
        return nil
    }
    
    // MARK: Prüfungen
    
    func isPileEmpty() -> Bool {
        return cards.isEmpty
    }
    
    func conformsWithSelectAndMoveMode(_ card: Card) -> Bool {
        switch self.userSelectAndMoveType {
        case .anySequenceInSuit:
            return isDownInSequenceAndSuitStartingAt(card)
        case .allSeqNoColorOrLast:
            // nur die letzte Karte oder alle Karten einer Sequenz (keine Stücke)
            if getLastCard() == card {
                // es ist die letzte Karte des Stapels
                return true
            }
            // prüfe, ob es sich um eine gesamte Sequenz handelt
            return isDownInWholeSequence(card)
        case .allSeqInSuitOrLast:
            // nur die letzte Karte oder alle Karten einer Sequenz (keine Stücke)
            if getLastCard() == card {
                // es ist die letzte Karte des Stapels
                return true
            }
            // prüfe, ob es sich um eine gesamte Sequenz handelt
            return isDownInSuitWholeSequence(card)
        case .anySeqNoColor:
            return isDownInSequenceStartingAt(card)
        default:
            log.error("für andere Spiele muss dies noch implementiert werden")
            return false
        }
    }
    
    func conformsToPlayOnTableau(_ card: Card, stockNotEmpty: Bool) -> Bool {
        assert(pileType == .tableau, "das hätte nicht passieren dürfen")
        // dürfen hier Karten abgelegt werden
        // zunächst gibt es die beiden Fälle leer oder nicht
        if isPileEmpty() {
            // leeren Pile bearbeiten
            switch self.depositIfEmptyType {
            case .anyCard:
                return true
            case .fromDealOrAny:
                // falls noch Karten im Stock, dann wird ein leerer Pile automatisch vom Stock belegt,
                // sonst beliebig
                if stockNotEmpty {
                    return false
                }
                else {
                    return true
                }
            default:
                //TODO: implementieren
                log.error("muss implementiert werden")
                return false
            }
        }
        else {
            // der Stapel enthält Karten
            //TODO: maximale Anzahl bearbeiten
            let lastCard = getLastCard()
            switch self.depositFromUserType {
            case .upInSuit:
                return lastCard!.isUpInSuitWithCard(card)
            case .downInSuit:
                return lastCard!.isDownInSuitWithCard(card)
            case .downNoColor:
                return lastCard!.isDownInSequenceWithCard(card)
            default:
                //TODO: implementieren
                log.error("muss implementiert werden")
                return false
            }
        }
    }
    
    func conformsToPlayOnFoundation(_ card: Card, fromPile: Pile) -> Bool {
        assert((pileType == .foundation) || (pileType == .multiFoundation), "das hätte nicht passieren dürfen")
        // wie viele Karten dürfen auf einmal abgelegt werden
        if numberPerMove == 1 {
            // es darf genau 1 Karte abgelegt werden
            // falls mehrere Karten selektiert sind, wird die auto - Ablage geprüft
            if fromPile.selectedCards.count != 1 {
                // bisher können sowieso nur Sequenzen abgelegt werden
                // deshalb wird zuerst geprüft, ob die Selektion eine Sequenz ist
                // in den bisher implementierten Spielen ist das immer eine Sequenz nach unten
                if !fromPile.isDownInSequenceStartingAt(card) {
                    // ist gar keine Sequenz
                    return false
                }
                // falls der Pile noch leer ist
                if self.isPileEmpty() {
                    // mit welcher Karte darf der leere Pile belegt werden
                    switch self.depositIfEmptyType {
                    case .ace:
                        if !fromPile.selectedCards.last!.isAce() {
                            return false
                        }
                    case .king:
                        if !fromPile.selectedCards.last!.isKing() {
                            return false
                        }
                    case .anyCard:
                        break
                    default:
                        //TODO: implementieren
                        log.error("muss implementiert werden")
                        return false
                    }
                }
                // wie kann abgelegt werden ?
                // leere Stapel wurden vorher untersucht! bei depositeIfEmptyType.any wird daher true zurückgegeben
                switch self.depositFromUserType {
                case .downInSuit:
                    // falls der Pile nicht leer ist
                    if !self.isPileEmpty() {
                        let lastCard = self.getLastCard()!
                        return lastCard.isDownInSuitWithCard(fromPile.selectedCards.last!)
                    }
                case .downNoColor:
                    // falls der Pile nicht leer ist
                    if !self.isPileEmpty() {
                        let lastCard = self.getLastCard()!
                        return lastCard.isDownInSequenceWithCard(fromPile.selectedCards.last!)
                    }
                case .upInSuit:
                    // falls der Pile nicht leer ist
                    if !self.isPileEmpty() {
                        let lastCard = self.getLastCard()!
                        return lastCard.isUpInSuitWithCard(fromPile.selectedCards.last!)
                    }
                default:
                    //TODO: implementieren
                    log.error("muss implementiert werden")
                    return false
                }
                return true
            }
            // es ist genau 1 Karte selektiert
            // falls der Pile noch leer ist
            if self.isPileEmpty() {
                // mit welcher Karte darf der leere Pile belegt werden
                switch self.depositIfEmptyType {
                case .ace:
                    if !card.isAce() {
                        return false
                    }
                case .king:
                    if !card.isKing() {
                        return false
                    }
                case .anyCard:
                    break
                default:
                    //TODO: implementieren
                    log.error("muss implementiert werden")
                    return false
               }
            }
            switch self.depositFromUserType {
            case .downInSuit:
                // falls der Pile nicht leer ist
                if !self.isPileEmpty() {
                    let lastCard = self.getLastCard()!
                    return lastCard.isDownInSuitWithCard(card)
                }
            case .downNoColor:
                // falls der Pile nicht leer ist
                if !self.isPileEmpty() {
                    let lastCard = self.getLastCard()!
                    return lastCard.isDownInSequenceWithCard(card)
                }
            case .upInSuit:
                // falls der Pile nicht leer ist
                if !self.isPileEmpty() {
                    let lastCard = self.getLastCard()!
                    return lastCard.isUpInSuitWithCard(card)
                }
            default:
                //TODO: implementieren
                log.error("muss implementiert werden")
                return false
            }
            return true
        }
        else {
            assert((numberPerMove == 13), "was anderes ist nicht bekannt")
            if pileType == .foundation {
                // für jede Sequenz mit 13 Karten muss ein eigener Foundation existieren
                // der Stapel muss leer sein
                if !isPileEmpty() {
                    return false
                }
            }
            // ein .MultiFoundation muss nicht leer sein
            if fromPile.selectedCards.count != 13 {
                return false
            }
            else {
                // es sind genau 13 Karten selektiert
                switch self.depositFromUserType {
                case .downInSuit:
                    return fromPile.isDownInSequenceAndSuitStartingAt(card)
                case .downNoColor:
                    return fromPile.isDownInSequenceStartingAt(card)
                default:
                    //TODO: implementieren
                    log.error("muss implementiert werden")
                    return false
                }
            }
        }
    }
    
    func isPermittedToPlayFromPile(_ forSelectedPile: Pile) -> Bool {
        // darf auf diesen Stapel vom forSelectedPile abgelegt werden?
        var destinationPilePermittedtoPlay = false
        switch forSelectedPile.permittedToPlayType {
        case .tableauPermittedToPlay:
            // der Zielstapel muss ein Tableau sein
            destinationPilePermittedtoPlay = pileType == .tableau
        case .wastePermittedToPlay:
            // der Zielstapel muss ein Waste sein
            destinationPilePermittedtoPlay = pileType == .waste
        case .foundationPermittedToPlay:
            // der Zielstapel muss ein Foundation sein
            destinationPilePermittedtoPlay = (pileType == .foundation) || (pileType == .multiFoundation)
        case .tableauOrFoundationPermittedToPlay:
            // der Zielstapel muss ein Tableau oder Foundation sein
            destinationPilePermittedtoPlay = (pileType == .foundation) || (pileType == .tableau) || (pileType == .multiFoundation)
        case .waste_Tableau_FoundationPermittedToPlay:
            // der Zielstapel muss ein Tableau oder Foundation oder Waste sein
            destinationPilePermittedtoPlay = (pileType == .foundation) || (pileType == .tableau) || (pileType == .waste) || (pileType == .multiFoundation)
        default:
            log.error("kein Zielstapel darf nicht vorkommen")
            destinationPilePermittedtoPlay = false
        }
        return destinationPilePermittedtoPlay
    }
    
    // MARK: Finder
    
    func findIndexForCard(_ card: Card) -> Int {
        // liefert den index in cards
        for i in 0 ..< cards.count {
            if cards[i] == card {
                return i
            }
        }
        return -1
    }
    
    // MARK: Sequence Überprüfungen
    
    func isDownInSequenceAndSuitStartingAt(_ card: Card) -> Bool {
        /*! prüft, ob alle folgenden Karten des Stapels in absteigender Folge sind und
        dieselbe Kartenkategorie haben */
        // wenn es die letzte Karte im Stapel ist, braucht es keiner weiteren Prüfung
        if card == cards.last {
            return true
        }
        let startIndex = findIndexForCard(card)
        // bilden alle folgenden Karten im Stapel eine Sequenz und
        // haben alle folgenden Karten im Stapel diegleiche Kartenkategorie (Herz,Karo,...)
        for i in startIndex ..< cards.count - 1 {
            if !cards[i].isDownInSuitWithCard(cards[i+1]) {
                return false
            }
        }
        return true
    }

    func isDownInSequenceStartingAt(_ card: Card) -> Bool {
        // prüft, ob alle folgenden Karten des Stapels in absteigender Folge sind (ohne Rücksicht auf Farbe oder Art
        if card == cards.last {
            return true
        }
        let startIndex = findIndexForCard(card)
        // bilden alle folgenden Karten im Stapel eine Sequenz und
        // haben alle folgenden Karten im Stapel diegleiche Kartenkategorie (Herz,Karo,...)
        for i in startIndex ..< cards.count - 1 {
            if !cards[i].isDownInSequenceWithCard(cards[i+1]) {
                return false
            }
        }
        return true
    }
    
    // prüft, ob es sich um eine gesamte Sequenz handelt oder nur um eine Teilsequenz
    func isDownInWholeSequence(_ card: Card) -> Bool {
        // überprüft, ob es sich um eine komplette Sequenz handelt
        // stelle fest, ob es noch eine Karte über card im Stapel gibt
        if let cardBefore = getCardBefore(card) {
            // es werden nur aufgedeckte Karten untersucht
            if cardBefore.faceUp && !cardBefore.isDownInSequenceWithCard(card) {
                // es ist keine ganze Sequenz
                return false
            }
            return isDownInSequenceStartingAt(card)
        }
        // es gibt keine Karte darüber
        return isDownInSequenceStartingAt(card)
    }
    
    
    // prüft, ob es sich um eine gesamte Sequenz in Farbe handelt oder nur um eine Teilsequenz
    func isDownInSuitWholeSequence(_ card: Card) -> Bool {
        // überprüft, ob es sich um eine komplette Sequenz handelt
        // stelle fest, ob es noch eine Karte über card im Stapel gibt
        if let cardBefore = getCardBefore(card) {
            // es werden nur aufgedeckte Karten untersucht
            if cardBefore.faceUp && cardBefore.isDownInSuitWithCard(card) {
                // es ist keine ganze Sequenz
                return false
            }
            return isDownInSequenceStartingAt(card)
        }
        // es gibt keine Karte darüber
        return isDownInSequenceAndSuitStartingAt(card)
    }
    
    func isSelectedCompleteDownInSequenceAndSuit() -> Bool {
        return isDownInSequenceAndSuitStartingAt(selectedCards.first!) && (selectedCards.count == 13)
    }
    

    
    func isUpInSequenceAndSuitStartingAt(_ card: Card) -> Bool {
        /*! prüft, ob alle folgenden Karten des Stapels in aufsteigender Folge sind und
         dieselbe Kartenkategorie haben */
        // wenn es die letzte Karte im Stapel ist, braucht es keiner weiteren Prüfung
        if card == cards.last {
            return true
        }
        let startIndex = findIndexForCard(card)
        // bilden alle folgenden Karten im Stapel eine Sequenz und
        // haben alle folgenden Karten im Stapel diegleiche Kartenkategorie (Herz,Karo,...)
        for i in startIndex ..< cards.count - 1 {
            if !cards[i].isUpInSuitWithCard(cards[i+1]) {
                return false
            }
        }
        return true
    }
    

    // MARK: logging activities
    
    func logMoveSequence(_ sequence: [Card], fromPile: Pile) {
//        let toPile = self
//        log.messageOnly("Sequenz von \(fromPile.pileType.description())(\(fromPile.pileId)) nach \(toPile.pileType.description())(\(toPile.pileId)):")
//        for card in sequence {
//            if card.faceUp {
//                log.messageOnly("\t  sichtbar: \(card.name)")
//            }
//            else {
//                log.messageOnly("\tunsichtbar: \(card.name)")
//            }
//        }
    }
    
    // MARK: Helper
    
    func isCardsRepositioned() -> Bool {
        return memoCardsRepositioned
    }
    
    func resetCardsRepositioned() {
        memoCardsRepositioned = false
    }
    
    func resetzPositions() {
        for card in cards {
            card.zPosition = highestzPosition
            highestzPosition += 1
            zPositionCard! <- card
        }
        //log.debug("high: \(highestzPosition)")
    }
    

}
