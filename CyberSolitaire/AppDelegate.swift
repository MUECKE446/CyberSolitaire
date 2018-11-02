//
//  AppDelegate.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 08.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import UIKit
import SwiftyPlistManager


let log = ActionLogger.defaultLogger()
var gameName = ""
// !!! Achtung: bei jeder Änderung der Version muss diese händisch auch in LaunchScreen.xib gemacht werden !!!
let CyberSolitaireVersion = "1.2.0"

// Konstanten für SwiftyPlistManger
let allGamePListNames = getAllGamePListNames()
var allPListNames : [String] = []

func logSwiftyPlistManager(_ error: SwiftyPlistManagerError?) {
    guard let err = error else {
        return
    }
    print("-------------> SwiftyPlistManager error: '\(err)'")
}

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?
    
    let activityLogIdent = "activityLog"
    var activityLogFileName: String!
    var activityLogURL: URL!
    var dateStr: String!
    
    var currentActiveGame : SolitaireGame?
    

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        // Override point for customization after application launch.
        log.setup(logLevel: .allLevels, showLogLevel: true, showFileName: false, showLineNumber: false, writeToFile: nil)
        
        // MARK: Start des SwiftyPlistManager
        allPListNames.append(settingsListName)
        allPListNames.append(statisticsListName)
        allPListNames.append(contentsOf: allPListNames)
        SwiftyPlistManager.shared.start(plistNames: allPListNames, logging: false)

        readSettingList()
        readStatisticsList()
        return true
    }
    
    func applicationWillResignActive(_ application: UIApplication) {
        // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
        // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
        log.verbose("")
        if let game = currentActiveGame {
            // wenn das Spiel fertig ist, dann wurden alle Statistik Einträge erledigt
            if game.gameState != .completedState {
                game.gameState = .pausingState
                game.cumulatePlayTime()
            }
        }
    }

    func applicationDidEnterBackground(_ application: UIApplication) {
        // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
        // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
        log.verbose("")
    }

    func applicationWillEnterForeground(_ application: UIApplication) {
        // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
        log.verbose("")
    }

    func applicationDidBecomeActive(_ application: UIApplication) {
        // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
        log.verbose("")
        if let game = currentActiveGame {
            // wenn das Spiel fertig ist, dann wurden alle Statistik Einträge erledigt
            if game.gameState == .pausingState {
                game.gameState = .runningState      // reaktiviere Spiel
                game.lastTimeCumulated = Date()     // Zeitrechnung wieder starten
            }
        }
    }

    func applicationWillTerminate(_ application: UIApplication) {
        // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
        log.verbose("es ist vorbei")
    }
    
    func applicationDocumentsDirectory() -> URL {
        let fileManager = FileManager.default
        let urls = fileManager.urls(for: .documentDirectory,in: .userDomainMask) 
        return urls[0]
    }
    
    func createActivityLogURL() -> URL {
        let documentsURL = applicationDocumentsDirectory()
        let currentDate = Date()
        let formatter = DateFormatter()
        formatter.locale =  Locale.current
        formatter.dateFormat = "-dd-MM-yyyy"
        dateStr = formatter.string(from: currentDate)
        let versionNumber = 1
        let versionNumberStr = NSString(format: "%d", versionNumber)
        activityLogFileName = activityLogIdent + dateStr + "_" + (versionNumberStr as String) + ".TXT"
        return documentsURL.appendingPathComponent(activityLogFileName)
    }
    
    func incrementActivityLogFileVersion(_ logFile: inout String!) {
        // finde die die Nummer im activityLogFileName
        //let index1 = find(logFile,"_")!
        let index1 = logFile.index((logFile.index(of: "_"))!, offsetBy: 1)
        let index2 = logFile.index(of: ".")
        //!!! Achtung !!!
        let versionNumberStr = logFile[index1 ..< index2!]  // this is now a Substring type under Swift 4
        //var versionNumberStr = logFile.substring(with: (index1 ..< index2!))
        var versionNumber = Int(versionNumberStr)
        versionNumber! += 1
        //versionNumberStr = NSString(format: "%d", versionNumber!) as String
        //activityLogFileName = activityLogIdent + dateStr + "_" + \versionNumber + ".TXT"
        //activityLogFileName = activityLogIdent + dateStr + "_" + "\(versionNumber ?? 0)" + ".TXT"
        logFile = activityLogIdent + dateStr + "_" + String(describing: versionNumber) + ".TXT"

    }
}
