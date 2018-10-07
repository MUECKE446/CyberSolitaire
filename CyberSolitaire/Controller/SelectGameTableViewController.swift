//
//  SelectGameTableViewController.swift
//  CyberSolitaire
//
//  Created by Christian Muth on 05.10.18.
//  Copyright © 2018 Christian Muth. All rights reserved.
//

import UIKit

class SelectGameTableViewController: UITableViewController {

    struct Game
    {
        var id : Int
        var title : String
        var description : String
    }
    

    var gamesCanBeSelected = [
        Game(id: 1, title: "Black widow (entschärft)", description: "Ziel: Alle 8 Farbsequenzen mit allen 13 Karten, vom König bis zum As, als Einheit ablegen\nSpiel: Karten werden in absteigender Folge ohne Rücksicht auf Farbe gelegt. Jede Kartensequenz kann als Einheit bewegt werden. Freie Plätze können mit einer beliebigen Karte belegt werden\nZu jedem Zeitpunkt kann eine weitere Kartenreihe vom Talon auf das Kartenbild gezogen werden."),
        Game(id: 2, title: "Black widow", description: "Ziel: Alle 8 Farbsequenzen mit allen 13 Karten, vom König bis zum As, als Einheit ablegen\nSpiel: Karten werden in absteigender Folge ohne Rücksicht auf Farbe gelegt. Jede Kartensequenz gleicher Farbe kann als Einheit bewegt werden. Freie Plätze können mit einer beliebigen Karte belegt werden\nZu jedem Zeitpunkt kann eine weitere Kartenreihe vom Talon auf das Kartenbild gezogen werden."),
        Game(id: 3, title: "Spider", description: "Ziel: Alle 8 Farbsequenzen mit allen 13 Karten, vom König bis zum As, als Einheit ablegen\nSpiel: Karten werden in absteigender Folge ohne Rücksicht auf Farbe gelegt. Jede Kartensequenz oder Teile davon kann als Einheit bewegt werden. Freie Plätze können mit einer beliebigen Karte belegt werden\nZu jedem Zeitpunkt kann eine weitere Kartenreihe vom Talon auf das Kartenbild gezogen werden.")
    ]
    
    
    override func viewDidLoad() {
        super.viewDidLoad()

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

    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
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

}
