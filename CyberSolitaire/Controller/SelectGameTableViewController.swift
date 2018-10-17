//
//  SelectGameTableViewController.swift
//  CyberSolitaire
//
//  Created by Christian Muth on 05.10.18.
//  Copyright © 2018 Christian Muth. All rights reserved.
//

import UIKit

class SelectGameTableViewController: UITableViewController,UIPopoverPresentationControllerDelegate {

    struct Game
    {
        var id : Int
        var title : String
        var description : String
    }
    
    var gamesCanBeSelected : [Game] = []
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // fülle die Tabelle
        let allGames = getAllGames()
        for game in allGames {
            var id = 0
            for (name,value) in game {
                id += 1
                let description = value["description"] as! String
                let game = Game(id: id, title: name, description: description)
                gamesCanBeSelected.append(game)
            }
        }
        
        // Uncomment the following line to preserve selection between presentations
        // self.clearsSelectionOnViewWillAppear = false

        // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
        // self.navigationItem.rightBarButtonItem = self.editButtonItem
    }

    // MARK: - Table view data source

    override func numberOfSections(in tableView: UITableView) -> Int {
        // #warning Incomplete implementation, return the number of sections
        return 1
    }

    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        // #warning Incomplete implementation, return the number of rows
        return gamesCanBeSelected.count
    }

    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "GameSelectCell", for: indexPath)

        // Configure the cell...
        cell.textLabel?.text = gamesCanBeSelected[indexPath.row].title
        cell.detailTextLabel?.text = gamesCanBeSelected[indexPath.row].description
        return cell
    }
    
    /*
    // Override to support conditional editing of the table view.
    override func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the specified item to be editable.
        return true
    }
    */

    /*
    // Override to support editing the table view.
    override func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCellEditingStyle, forRowAt indexPath: IndexPath) {
        if editingStyle == .delete {
            // Delete the row from the data source
            tableView.deleteRows(at: [indexPath], with: .fade)
        } else if editingStyle == .insert {
            // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
        }    
    }
    */

    /*
    // Override to support rearranging the table view.
    override func tableView(_ tableView: UITableView, moveRowAt fromIndexPath: IndexPath, to: IndexPath) {

    }
    */

    /*
    // Override to support conditional rearranging of the table view.
    override func tableView(_ tableView: UITableView, canMoveRowAt indexPath: IndexPath) -> Bool {
        // Return false if you do not want the item to be re-orderable.
        return true
    }
    */

    // MARK: - Navigation, Segue

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
        if segue.identifier == "PlayGame" {
            
            let tableView = self.tableView
            let pathOfTheCell = tableView?.indexPath(for: sender as! UITableViewCell)
            let row = pathOfTheCell?.row
            
            switch gamesCanBeSelected[row!].id {
            case 1:
                gameName = "Black Widow weich"
            case 2:
                gameName = "Black Widow hart"
            case 3:
                gameName = "Spider"
            default:
                fatalError("kein Spiel für diese id definiert")
            }
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
