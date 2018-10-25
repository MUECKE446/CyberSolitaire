//
//  GameSelectCellTableViewCell.swift
//  CyberSolitaire
//
//  Created by Christian Muth on 10.10.18.
//  Copyright © 2018 Christian Muth. All rights reserved.
//

import UIKit

class GameSelectCell: UITableViewCell {

    @IBOutlet weak var gameNameLabel: UILabel!
    @IBOutlet weak var disposalLabel: UILabel!
    @IBOutlet weak var targetLabel: UILabel!
    @IBOutlet weak var howToPlayLabel: UILabel!
    @IBOutlet weak var descriptionButton: UIButton!
    
    var withDescription = false
    
}
