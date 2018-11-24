//
//  SelectGameTableViewController.swift
//  CyberSolitaire
//
//  Created by Christian Muth on 05.10.18.
//  Copyright © 2018 Christian Muth. All rights reserved.
//

import UIKit
import SwiftRichString

// styles
let normal = Style {
    $0.font = SystemFonts.Helvetica.font(size: 15)
}

let bold = Style {
    $0.font = SystemFonts.Helvetica_Bold.font(size: 16)
    $0.color = UIColor.black
}

let styleGroup = StyleGroup(base: normal, ["bold": bold])

class SelectGameTableViewController: UITableViewController,UIPopoverPresentationControllerDelegate {

    struct GameWithDescription {
        var gameName : String
        var gameDescription : Dictionary<String,String>
    }
    
    var gamesWithDescriptionCanBeSelected : [GameWithDescription] = []

    @IBOutlet weak var settingsButton: UIButton!
    @IBOutlet weak var statisticButton: UIButton!
    
    @IBAction func descriptionButtonTapped(_ sender: UIButton) {
        var parent = sender.superview
        while ((parent != nil) && !(parent!.isKind(of: GameSelectCell.self))) {
            parent = parent?.superview
        }
        let cell = parent as! GameSelectCell
        _ = self.tableView.indexPath(for: cell)


        if sender.titleLabel?.text == "Beschreibung anzeigen" {
            sender.setTitle("Beschreibung verbergen", for: UIControl.State.normal)
            cell.withDescription = true
        }
        else {
            sender.setTitle("Beschreibung anzeigen", for: UIControl.State.normal)
            cell.withDescription = false
        }
        self.tableView.reloadData()

    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        settingsButton.backgroundColor = UIColor.white
        settingsButton.layer.cornerRadius = 5
        settingsButton.layer.borderWidth = 1
        settingsButton.layer.borderColor = UIColor.black.cgColor
        
        
        statisticButton.backgroundColor = UIColor.white
        statisticButton.layer.cornerRadius = 5
        statisticButton.layer.borderWidth = 1
        statisticButton.layer.borderColor = UIColor.black.cgColor
        
        // fülle die Tabelle
        let allGames = getAllGames()
        for game in allGames {
            for (name,value) in game {
                let gameDescription = value["gameDescription"] as! Dictionary<String,String>
                let gameWithDescription = GameWithDescription(gameName: name, gameDescription: gameDescription)
                gamesWithDescriptionCanBeSelected.append(gameWithDescription)
            }
        }
        
        // Uncomment the following line to preserve selection between presentations
        // self.clearsSelectionOnViewWillAppear = false

        // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
        // self.navigationItem.rightBarButtonItem = self.editButtonItem
    }

    // MARK: - Table view data source

    override func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return gamesWithDescriptionCanBeSelected.count
    }

    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        var cell : GameSelectCell = tableView.dequeueReusableCell(withIdentifier: "GameSelectCell", for: indexPath) as! GameSelectCell
        cell.descriptionButton.backgroundColor = UIColor.white
        cell.descriptionButton.layer.cornerRadius = 5
        cell.descriptionButton.layer.borderWidth = 1
        cell.descriptionButton.layer.borderColor = UIColor.black.cgColor

        if  cell.withDescription {
            fillTableViewCell(&cell, indexPath: indexPath, withDescription: true)
        }
        else {
            fillTableViewCell(&cell, indexPath: indexPath, withDescription: false)
        }
        return cell
    }
    
    func fillTableViewCell( _ cell : inout GameSelectCell, indexPath path : IndexPath, withDescription with: Bool) {
        if with {
            let gameName = gamesWithDescriptionCanBeSelected[path.row].gameName
            let disposal = gamesWithDescriptionCanBeSelected[path.row].gameDescription["disposal"]
            let target = gamesWithDescriptionCanBeSelected[path.row].gameDescription["target"]
            let howToPlay = gamesWithDescriptionCanBeSelected[path.row].gameDescription["howToPlay"]
            // Configure the cell...
            switch gamesWithDescriptionCanBeSelected[path.row].gameDescription["difficulty"] {
            case "leicht":
                cell.backgroundColor = UIColor.green
                
            case "mittel":
                cell.backgroundColor = UIColor.orange
                
            case "schwer":
                cell.backgroundColor = UIColor.red
                
            default:
                fatalError("darf nicht vorkommen")
            }
            cell.gameNameLabel?.text = gameName
            cell.disposalLabel?.attributedText = disposal?.set(style: styleGroup)
            cell.targetLabel?.attributedText = target?.set(style: styleGroup)
            cell.howToPlayLabel?.attributedText = howToPlay?.set(style: styleGroup)
        }
        else {
            let gameName = gamesWithDescriptionCanBeSelected[path.row].gameName
            let disposal = ""
            let target = ""
            let howToPlay = ""
            // Configure the cell...
            switch gamesWithDescriptionCanBeSelected[path.row].gameDescription["difficulty"] {
            case "leicht":
                cell.backgroundColor = UIColor.green
                
            case "mittel":
                cell.backgroundColor = UIColor.orange
                
            case "schwer":
                cell.backgroundColor = UIColor.red
                
            default:
                fatalError("darf nicht vorkommen")
            }
            cell.gameNameLabel?.text = gameName
            cell.disposalLabel?.attributedText = disposal.set(style: styleGroup)
            cell.targetLabel?.attributedText = target.set(style: styleGroup)
            cell.howToPlayLabel?.attributedText = howToPlay.set(style: styleGroup)
        }
    }
    
    // MARK: - Navigation, Segue

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
        if segue.identifier == "PlayGame" {
            
            let tableView = self.tableView
            let pathOfTheCell = tableView?.indexPath(for: sender as! UITableViewCell)
            let row = pathOfTheCell?.row
            
            gameName = gamesWithDescriptionCanBeSelected[row!].gameName
        }
        
        if segue.identifier == "Settings" {
            let destinationVC = segue.destination as! SettingsTableViewController
            
            if let settingsPopoverPresentationController = destinationVC.popoverPresentationController {
                settingsPopoverPresentationController.delegate = self as UIPopoverPresentationControllerDelegate?
            }
            
        }
    }
    
    override func performSegue(withIdentifier identifier: String, sender: Any?) {
        if sender is UIButton {
            performSegue(withIdentifier: "Settings", sender: sender)
        }
        else {
            performSegue(withIdentifier: "PlayGame", sender: sender)
        }
    }

    func popoverPresentationControllerShouldDismissPopover(_ popoverPresentationController: UIPopoverPresentationController) -> Bool {
        return false
    }

}
