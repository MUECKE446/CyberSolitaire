//
//  SelectGameViewController.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 14.12.17.
//  Copyright © 2017 Christian Muth. All rights reserved.
//

import UIKit

class SelectGameViewController: UIViewController {
    
    @IBAction func BlackWidowWeich(_ sender: UIButton) {
        gameName = "Black Widow weich"
    }
    
    @IBAction func Spider(_ sender: UIButton) {
        gameName = "Spider"
    }
    
    @IBAction func BlackWidowHart(_ sender: UIButton) {
        gameName = "Black Widow hart"
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */


}
