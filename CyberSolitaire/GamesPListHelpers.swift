//
//  GamesPListHelpers.swift
//  CyberSolitaire
//
//  Created by Christian Muth on 16.10.18.
//  Copyright © 2018 Christian Muth. All rights reserved.
//

import Foundation
import SwiftyPlistManager

struct GamePList {
    
    let name:String
    
    var sourcePath:String? {
        guard let path = Bundle.main.path(forResource: name, ofType: "plist") else { return .none }
        return path
    }
}


// liefert ein Array mit allen URLs der Spiele
func getAllGamePListURLs() -> [URL] {
    let mainBundle = Bundle.main
    let urls = mainBundle.urls(forResourcesWithExtension: "plist", subdirectory: nil)
    var gameUrls : [URL] = []
    for url in urls! {
        let strUrl = url.relativeString
        if strUrl.hasPrefix("game(") {
            gameUrls.append(url)
        }
    }
    return gameUrls
}

// liefert ein Array mit allen plist Namen der Spiele
func getAllGamePListNames() -> [String] {
    let gameUrls = getAllGamePListURLs()
    var gamePListNames : [String] = []
    for url in gameUrls {
        let strUrl = url.relativeString
        var strComponents = strUrl.split(separator: ".")
        strComponents.removeLast()
        let str = String(strComponents[0])
        let replaced = str.replacingOccurrences(of: "%20", with: " ")
        gamePListNames.append(replaced)
    }
    return gamePListNames
}

// liefert ein Array mit allen Namen der Spiele
func getAllGameNames() -> [String] {
    let gamePListNames = getAllGamePListNames()
    var gameNames : [String] = []
    for plist in gamePListNames {
        let game = SwiftyPlistManager.shared.fetchValue(for: "game", fromPlistWithName: plist) as! Dictionary<String,Any>
        let gameName = game["gameName"] as! String
        gameNames.append(gameName)
    }
    return gameNames
}

func getGameName(plistName:String) -> String {
    let game = SwiftyPlistManager.shared.fetchValue(for: "game", fromPlistWithName: plistName) as! Dictionary<String,Any>
    let gameName = game["gameName"] as! String
    return gameName
}

func getAllGames() -> [Dictionary<String,Dictionary<String,Any>>] {
    let gamePListNames = getAllGamePListNames()
    var games : [Dictionary<String,Dictionary<String,Any>>] = []
    for plist in gamePListNames {
        let game = SwiftyPlistManager.shared.fetchValue(for: "game", fromPlistWithName: plist) as! Dictionary<String,Any>
        let dictItem = [getGameName(plistName: plist):game]
        games.append(dictItem)
    }
    return games
}

