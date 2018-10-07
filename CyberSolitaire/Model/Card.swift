//
//  Card.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 08.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import UIKit

enum SuitType: Int {
    case
    club = 0,   // Kreuz
    diamond,     // Karo
    heart,      // Herz
    spade      // Pik
}

enum ColorType: Int {
    case
    black = 0,  // schwarz
    red         // rot
}

class Card: NSObject {
    
    var selected = false
    var markedForCheat = false
    
    var faceUp = false
    var visible = true
    
    var size: CGSize = CGSize.zero
    var position: CGPoint = CGPoint.zero
    
    /*! cardID = Kartenwert + Deck-Nummer * 52 */
    let cardId: Int
    
    // Kartenwert 1  = Kreuz As 14 = Karo As 27 = Herz As 40 = Pik As
    let value: Int
    
    var zPosition = CGFloat(1.0)
    
    let name : String
    let suit : SuitType
    let color: ColorType
    
    // MARK: initialisierer
    
    init(cardID: Int) {
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
        
        // baue den Kartennamen zusammen
        // bestimme dabei gleichzeitig suit und color
        var tmpName: String
        
        let x = (value-1) / 13
        suit = SuitType(rawValue: x)!
        switch suit {
        case .club: tmpName = "Kreuz "; color = ColorType.black
        case .spade: tmpName = "Pik "; color = ColorType.red
        case .heart: tmpName = "Herz "; color = ColorType.black
        case .diamond: tmpName = "Karo "; color = ColorType.red
        }
        
        switch (value - 1) % 13 {
        case 0: tmpName += "As"
        case 1: tmpName += "2"
        case 2: tmpName += "3"
        case 3: tmpName += "4"
        case 4: tmpName += "5"
        case 5: tmpName += "6"
        case 6: tmpName += "7"
        case 7: tmpName += "8"
        case 8: tmpName += "9"
        case 9: tmpName += "10"
        case 10: tmpName += "Bube"
        case 11: tmpName += "Dame"
        case 12: tmpName += "König"
        default: tmpName += "Fehler !!!"
        }
        
        name = tmpName
        super.init()
        
    }
    
    // MARK: Aktionen
    
    func toggleMarkForCheat() {
        markedForCheat = !markedForCheat
    }
    
    // MARK: Getter
    
    func getSuit() -> SuitType {
        return suit
    }
    
    // MARK: Überprüfungen
    
    func isDownInSequenceWithCard(_ card: Card) -> Bool {
        // überpfrüft, ob card im Wert genau -1 hinter self liegt (z.B.: self = 10 inCard = 9)
        let firstCardValue = (self.value-1) % 13
        let secondCardValue = (card.value-1) % 13
        return (firstCardValue-1) == secondCardValue
    }
    
    func isDownInSuitWithCard(_ card: Card) -> Bool {
        // überprüft, ob inCard im Wert genau -1 hinter self liegt (z.B.: self = 10 inCard = 9) und dieselbe Kartenkategorie (Herz,Pik,..) besitzt
        return isDownInSequenceWithCard(card) && (self.getSuit() == card.getSuit())
    }
    
    func isUpInSequenceWithCard(_ card: Card) -> Bool {
        // überprüft, ob die Karte self im Wert genau 1 nach card liegt
        let firstCardValue = (self.value-1) % 13
        let secondCardValue = (card.value-1) % 13
        return (firstCardValue+1) == secondCardValue
    }
    
    func isUpInSuitWithCard(_ card: Card) -> Bool {
        // überprüft, ob card in aufsteigender Folge ist und beide Karten gleicher Art sind (z.B. beide Kreuz)
        let suit1 = self.suit
        let suit2 = card.suit
        return isUpInSequenceWithCard(card) && (suit1 == suit2)
    }
    
    func isKing() -> Bool {
        // Kartenwert       13 = Kreuz König
        //                  26 = Karo  König
        //                  39 = Herz  König
        //                  52 = Pik   König
        return (value % 13) == 0
    }
    
    func isAce() -> Bool {
        // Kartenwert 1  = Kreuz As 14 = Karo As 27 = Herz As 40 = Pik As
        return value % 13 == 1
    }
}
