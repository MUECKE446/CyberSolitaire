//
//  Statistics.swift
//  CyberSolitaire
//
//  Created by Christian Muth on 30.10.18.
//  Copyright © 2018 Christian Muth. All rights reserved.
//

import Foundation
import SwiftyPlistManager

// Konstanten für SwiftyPlistManger
let statisticsListName = "statisticsList"

struct GameStatistic {
    var gameName = ""
    var totalPlayed = 0
    var won = 0
    var lost = 0
    var totalTime : TimeInterval = 0.0
}


let gamesKey = "games"

var totalPlayedGames  = 0
var totalPlayedTime : TimeInterval = 0.0

var gamesInStatisticsList = SwiftyPlistManager.shared.fetchValue(for: gamesKey, fromPlistWithName: statisticsListName) as! [Dictionary<String,Any>]
var gamesStatistics : [GameStatistic] = []

func computeStatisticTotals() -> (totalPlayed : Int, totalWon : Int, totalTime : TimeInterval) {
    var totalGames = 0
    var totalWon = 0
    var totalTime : TimeInterval = 0.0
    for gameStatistic in gamesStatistics {
        totalGames += gameStatistic.totalPlayed
        totalWon += gameStatistic.won
        totalTime += gameStatistic.totalTime
    }
    return (totalGames,totalWon,totalTime)
}

func getStatisticFor(_ name:String) -> GameStatistic? {
    for gameInStatistics in gamesInStatisticsList {
        if gameInStatistics["gameName"] as! String == name {
            var gameStatistic = GameStatistic()
            gameStatistic.gameName = gameInStatistics["gameName"] as! String
            gameStatistic.totalPlayed = gameInStatistics["totalPlayed"] as! Int
            gameStatistic.won = gameInStatistics["won"] as! Int
            gameStatistic.lost = gameStatistic.totalPlayed - gameStatistic.won
            gameStatistic.totalTime = gameInStatistics["totalTime"] as! TimeInterval
            gamesStatistics.append(gameStatistic)
            return gameStatistic
        }
    }
    return nil
}

func getGameStatisticFor(_ name: String) -> GameStatistic? {
    for gameStatistic in gamesStatistics {
        if gameStatistic.gameName == name {
            return gameStatistic
        }
    }
    assert(false,"darf nicht vorkommen")
    return nil
}

func updateStatisticsListFor(_ name:String, with:GameStatistic ) {
    for (index,var gameInStatistic) in gamesInStatisticsList.enumerated() {
        gameInStatistic["gameName"] = with.gameName
        gameInStatistic["totalPlayed"] = with.totalPlayed
        gameInStatistic["won"] = with.won
        gameInStatistic["totalTime"] = with.totalTime
        gamesInStatisticsList[index] = gameInStatistic
        SwiftyPlistManager.shared.save(gamesInStatisticsList, forKey: gamesKey, toPlistWithName: statisticsListName)  { (err) in
            if err != nil {
                logSwiftyPlistManager(err)
            }
        }
    }
}

func readStatisticsList() {
    let allGameNames = getAllGameNames()
    for gameName in allGameNames {
        if let gameStatistic = getStatisticFor(gameName) {
            gamesStatistics.append(gameStatistic)
        }
        else {
            // dieses Spiel gibt es noch nicht in der statisticsList
            let gameStatistic = GameStatistic(gameName: gameName, totalPlayed: 0, won: 0, lost: 0, totalTime: 0.0)
            gamesStatistics.append(gameStatistic)
            // erzeuge neuen Eintrag in der plist
            var newGameDict = Dictionary<String,Any>()
            newGameDict["gameName"] = gameStatistic.gameName
            newGameDict["totalPlayed"] = gameStatistic.totalPlayed
            newGameDict["won"] = gameStatistic.won
            newGameDict["totalTime"] = gameStatistic.totalTime
            gamesInStatisticsList.append(newGameDict)
            SwiftyPlistManager.shared.save(gamesInStatisticsList, forKey: gamesKey, toPlistWithName: statisticsListName)  { (err) in
                if err != nil {
                    logSwiftyPlistManager(err)
                }
            }
        }
    }
    // lösche Statistiken der nicht mehr vorhandenen Spiele
    for (index,gameInStatistic) in gamesInStatisticsList.enumerated() {
        if !allGameNames.contains(gameInStatistic["gameName"] as! String) {
            gamesInStatisticsList.remove(at: index)
            SwiftyPlistManager.shared.save(gamesInStatisticsList, forKey: gamesKey, toPlistWithName: statisticsListName)  { (err) in
                if err != nil {
                    logSwiftyPlistManager(err)
                }
            }
        }
    }
}

func writeStatisticsList() {
    SwiftyPlistManager.shared.save(gamesInStatisticsList, forKey: gamesKey, toPlistWithName: statisticsListName)  { (err) in
        if err != nil {
            logSwiftyPlistManager(err)
        }
    }
}

