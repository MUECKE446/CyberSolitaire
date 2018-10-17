//
//  Helper.swift
//  CyberSolitaire_SpriteKit
//
//  Created by Christian Muth on 12.03.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

import Foundation

// MARK: Helperfunktionen

func delay(_ delay:Double, closure:@escaping ()->()) {
    DispatchQueue.main.asyncAfter(
        deadline: DispatchTime.now() + Double(Int64(delay * Double(NSEC_PER_SEC))) / Double(NSEC_PER_SEC), execute: closure)
}


