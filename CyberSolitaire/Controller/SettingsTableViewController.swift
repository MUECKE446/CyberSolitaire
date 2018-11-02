//
//  SettingsTableViewController.swift
//  CyberSolitaire
//
//  Created by Christian Muth on 08.10.18.
//  Copyright © 2018 Christian Muth. All rights reserved.
//

import UIKit
import SwiftyPlistManager


// Konstanten für SwiftyPlistManger
let settingsListName = "SettingsList"

let playTonesKey = "playTones"
let permitUndoRedoKey = "permitUndoRedo"
let permitCheatingKey = "permitCheating"

// settings variables, die alle Spiele betreffen
var playTones = true
var permitUndoRedo = true
var permitCheating = true


class SettingsTableViewController: UITableViewController,UIPopoverControllerDelegate {
 
    @IBOutlet weak var settingsTonesSwitch: UISwitch!
    @IBOutlet weak var settingsPermitCheating: UISwitch!
    @IBOutlet weak var settingsPermitUndoRedo: UISwitch!
    
    @IBAction func settingsTonesChanged(_ sender: Any) {
        playTones = !playTones
    }
    
    @IBAction func settingsPermitCheating(_ sender: Any) {
        permitCheating = !permitCheating
    }

    @IBAction func settingsPermitUndoRedo(_ sender: Any) {
        permitUndoRedo = !permitUndoRedo
    }
    
    @IBAction func settingsDone(_ sender: Any) {
        writeSettingsList()
        dismiss(animated: true, completion: nil)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        readSettingList()
        settingsTonesSwitch.setOn(playTones, animated: false)
        settingsPermitCheating.setOn(permitCheating, animated: false)
        settingsPermitUndoRedo.setOn(permitUndoRedo, animated: false)

        // Uncomment the following line to preserve selection between presentations
        // self.clearsSelectionOnViewWillAppear = false

    }

    // MARK: - Table view data source

    override func numberOfSections(in tableView: UITableView) -> Int {
        return 2
    }

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        switch section {
        case 0:
            return 1
        case 1:
            return 2
        default:
            fatalError("darf nicht vorkommen")
        }
    }

}


func readSettingList() {
    playTones = SwiftyPlistManager.shared.fetchValue(for: playTonesKey, fromPlistWithName: settingsListName) as! Bool
    permitCheating = SwiftyPlistManager.shared.fetchValue(for: permitCheatingKey, fromPlistWithName: settingsListName) as! Bool
    permitUndoRedo = SwiftyPlistManager.shared.fetchValue(for: permitUndoRedoKey, fromPlistWithName: settingsListName) as! Bool
}

func writeSettingsList() {
    SwiftyPlistManager.shared.save(playTones, forKey: playTonesKey, toPlistWithName: settingsListName) { (err) in
        if err != nil {
            logSwiftyPlistManager(err)
        }
    }
    SwiftyPlistManager.shared.save(permitCheating, forKey: permitCheatingKey, toPlistWithName: settingsListName) { (err) in
        if err != nil {
            logSwiftyPlistManager(err)
        }
    }
    SwiftyPlistManager.shared.save(permitUndoRedo, forKey: permitUndoRedoKey, toPlistWithName: settingsListName) { (err) in
        if err != nil {
            logSwiftyPlistManager(err)
        }
    }
}


