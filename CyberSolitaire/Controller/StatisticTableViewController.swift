//
//  StatisticTableViewController.swift
//  CyberSolitaire
//
//  Created by Christian Muth on 01.11.18.
//  Copyright © 2018 Christian Muth. All rights reserved.
//

import UIKit

extension TimeInterval {
    func formattedString() -> String {
        let hours = Int(self) / 3600
        let minutes = Int(self) / 60 % 60
        let seconds = Int(self) % 60
        return String(format:"%02i:%02i:%02i", hours, minutes, seconds)
    }
}



class StatisticTableViewController: UITableViewController {

    
    @IBOutlet weak var totalGamesLabel: UILabel!
    @IBOutlet weak var totalWonLabel: UILabel!
    @IBOutlet weak var totalLostLabel: UILabel!
    @IBOutlet weak var totaltimeLabel: UILabel!
    
  
    @IBOutlet weak var goBackButton: UIButton!
    @IBOutlet weak var deleteStatisticsButton: UIButton!
    
    var totalGames = 0
    var totalWon = 0
    var totalLost = 0
    var totalTime : TimeInterval = 0.0
    
    @IBOutlet weak var deleteThisStatisticButton: UIButton!
    
    @IBAction func goBackButton(_ sender: Any) {
        self.dismiss(animated: true) {
            //self.view = nil
        }
    }
    
    @IBAction func deleteThisStatisticButtonTapped(_ sender: Any) {
        var parent = (sender as! UIButton).superview
        while ((parent != nil) && !(parent!.isKind(of: StatisticViewCell.self))) {
            parent = parent?.superview
        }
        let cell = parent as! StatisticViewCell
        let indexPath = self.tableView.indexPath(for: cell)

        let alert = UIAlertController(title: "Bitte bestätigen", message: "möchtest Du diese Statistik wirklich löschen?", preferredStyle: UIAlertController.Style.alert)
        alert.addAction(UIAlertAction(title: "OK", style: .destructive, handler: { action in
            var gameStatistic = gamesStatistics[(indexPath?.row)!]
            gameStatistic.totalPlayed = 0
            gameStatistic.won = 0
            gameStatistic.lost = 0
            gameStatistic.totalTime = 0.0
            gamesStatistics[(indexPath?.row)!] = gameStatistic
            (self.totalGames,self.totalWon,self.totalTime) = computeStatisticTotals()
            self.totalLost = self.totalGames - self.totalWon
            self.totalGamesLabel.text = String(self.totalGames)
            self.totalWonLabel.text = String(self.totalWon)
            self.totalLostLabel.text = String(self.totalLost)
            self.totaltimeLabel.text = self.totalTime.formattedString()
            
            self.tableView.reloadData()
            writeStatisticsList()
        }))
        alert.addAction(UIAlertAction(title: "Abbbruch", style: .cancel, handler: nil))
        self.present(alert, animated: true, completion: nil)
    }
    
    @IBAction func deleteAllStatistics(_ sender: Any) {
        let alert = UIAlertController(title: "Bitte bestätigen", message: "möchtest Du alle Statistiken wirklich löschen?", preferredStyle: UIAlertController.Style.alert)
        alert.addAction(UIAlertAction(title: "OK", style: .destructive, handler: { action in
            for (index, var gameStatistic) in gamesStatistics.enumerated() {
                gameStatistic.totalPlayed = 0
                gameStatistic.won = 0
                gameStatistic.lost = 0
                gameStatistic.totalTime = 0.0
                gamesStatistics[index] = gameStatistic
            }
            (self.totalGames,self.totalWon,self.totalTime) = computeStatisticTotals()
            self.totalLost = self.totalGames - self.totalWon
            self.totalGamesLabel.text = String(self.totalGames)
            self.totalWonLabel.text = String(self.totalWon)
            self.totalLostLabel.text = String(self.totalLost)
            self.totaltimeLabel.text = self.totalTime.formattedString()
            self.tableView.reloadData()
            writeStatisticsList()
        }))
        alert.addAction(UIAlertAction(title: "Abbbruch", style: .cancel, handler: nil))
        self.present(alert, animated: true, completion: nil)
    }
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        deleteStatisticsButton.backgroundColor = UIColor.white
        deleteStatisticsButton.layer.cornerRadius = 5
        deleteStatisticsButton.layer.borderWidth = 1
        deleteStatisticsButton.layer.borderColor = UIColor.black.cgColor

        goBackButton.backgroundColor = UIColor.white
        goBackButton.layer.cornerRadius = 5
        goBackButton.layer.borderWidth = 1
        goBackButton.layer.borderColor = UIColor.black.cgColor
        
        (totalGames,totalWon,totalTime) = computeStatisticTotals()
        totalLost = totalGames - totalWon
        
        totalGamesLabel.text = String(totalGames)
        totalWonLabel.text = String(totalWon)
        totalLostLabel.text = String(totalLost)
        totaltimeLabel.text = totalTime.formattedString()
        
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
        return gamesStatistics.count
    }

    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: "statisticViewCell", for: indexPath) as! StatisticViewCell
        cell.deleteThisStatisticButton.backgroundColor = UIColor.white
        cell.deleteThisStatisticButton.layer.cornerRadius = 5
        cell.deleteThisStatisticButton.layer.borderWidth = 1
        cell.deleteThisStatisticButton.layer.borderColor = UIColor.black.cgColor
        
        //fillStaticViewCell
        let gameName = gamesStatistics[indexPath.row].gameName
        let totalGames = gamesStatistics[indexPath.row].totalPlayed
        let totalWon = gamesStatistics[indexPath.row].won
        let totalLost = gamesStatistics[indexPath.row].lost
        let totalTime = gamesStatistics[indexPath.row].totalTime
        
        cell.gameNameLabel.text = gameName
        cell.totalGamesLabel.text = String(totalGames)
        cell.totalWonLabel.text = String(totalWon)
        cell.totalLostLabel.text = String(totalLost)
        cell.totalTimeLabel.text = totalTime.formattedString()
        
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

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destination.
        // Pass the selected object to the new view controller.
    }
    */

}
