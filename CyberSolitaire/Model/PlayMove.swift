//
//  PlayMove.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 22.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import UIKit
import SpriteKit

enum PlayMoveResult {
    case
    executed,
    canceled
}

enum PlayMoveStatus {
    case
    idle,
    awaitFurtherAction
}

class PlayMove {
    let game: SolitaireGame
    //let controller: GameViewController
    var lastSuccess: PlayMoveResult = .executed
    var status: PlayMoveStatus = .idle
    
    var selectedPile: Pile? = nil
    var secondSelectedPile: Pile? = nil
    var selectedCard: Card? = nil
    var secondSelectedCard: Card? = nil

    
    init( game: SolitaireGame) {
        self.game = game
    }
    
    func setInnerState(status: PlayMoveStatus, result: PlayMoveResult,
                        selectedPile: Pile?, secondSelectedPile: Pile?,
                        selectedCard: Card?, secondSelectedCard: Card?) {
                            self.status = status; self.lastSuccess = result
                            self.selectedPile = selectedPile; self.secondSelectedPile = secondSelectedPile
                            self.selectedCard = selectedCard; self.secondSelectedCard = secondSelectedCard
    }
    
    func setInnerState(status: PlayMoveStatus,
        selectedPile: Pile?, secondSelectedPile: Pile?,
        selectedCard: Card?, secondSelectedCard: Card?) {
            self.status = status
            self.selectedPile = selectedPile; self.secondSelectedPile = secondSelectedPile
            self.selectedCard = selectedCard; self.secondSelectedCard = secondSelectedCard
    }
    
    func invokeForTapOnCard(_ card: Card) {
        // erfrage vom Spiel den Stapel, in dem die Karte liegt, die angeklickt wurde
        let tappedPile = game.findPileForCard(card)!
        
        // in Abhängigkeit des internen Zustands des PlayMove wird reagiert
        switch status {
        case .idle:
            // es muss geprüft werden, ob ein neuer Zug begonnen werden kann
            // frage das game, ob mit der gewählten Karte ein Zug möglich ist
            let movePossible = game.isPlayMovePossibleForFirstActionWith(card, pile: tappedPile)
            if movePossible {
                // starte den Spielzug
                game.startPlayMoveFor(tappedPile, withCard: card)
            }
            else {
                shakeCard <- card
                setInnerState(status: .idle, result: .canceled, selectedPile: nil, secondSelectedPile: nil, selectedCard: nil, secondSelectedCard: nil)            }
        case .awaitFurtherAction:
            // der Spielzug muss fortgesetzt werden und es wurde eine Karte angetippt
            if (selectedPile == nil) || (tappedPile == selectedPile) {
                // es ist aber kein Stapel selektiert oder der selektierte wurde erneut gewählt
                // beende den Zug
                game.playMoveCanceled()
                return
            }
            // frage das Game, ob mit der gewählten Karte der Spielzug fortgeführt werden kann
            let movePossible = game.isPlayMovePossibleForFurtherActionWithCard(card, forSelectedPile: selectedPile!)
            if movePossible {
                // führe den Spielzug fort
                game.continuePlayMoveForPile(tappedPile, withCard: selectedCard!, forSelectedPile: selectedPile!)
            }
            else {
                game.playMoveCanceled()
            }
//        default:
//            assert(false, "darf nicht vorkommen")
//            break
        }
    }

    func invokeForTapOnEmptyPile(_ pile: Pile) {
        // in Abhängigkeit des internen Zustands des PlayMove wird reagiert
        switch status {
        case .idle:
            // einen leeren Stapel macht keinen Sinn
            return
        case .awaitFurtherAction:
            let movePossible = game.isPlayMovePossibleForFurtheActionWithEmptyPile(pile,forSelectedPile:selectedPile!)
            if movePossible {
                // führe den Spielzug fort
                game.continuePlayMoveForPile(pile, withCard: selectedCard!, forSelectedPile: selectedPile!)
            }
            else {
                game.playMoveCanceled()
            }
        }
    }
    
    func invokeForTapInsidePlayArea() {
        // der Zug wird abgebrochen
        game.playMoveCanceled()
    }
    
}
