//
//  ViewController.swift
//  ESP8266LED
//
//  Created by Francisco Zabala on 11/26/15.
//  Copyright © 2015 Francisco Zabala. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    @IBOutlet weak var web: UIWebView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    // URL paths including the ESP8266's IP address and arguments
    var led1on_URL = "http://192.168.0.107/setleds?led1=ON"
    // IBAction connected to the "LED1 ON" button
    @IBAction func led1on(sender: AnyObject) {
        // Instantiate an NSURL object using the
        // led1on_URL string
        let requestURL = NSURL(string: led1on_URL)
        // Instantiate an NSURLRequest object using the
        // requestURL NSURL
        let request = NSURLRequest(URL: requestURL!)
        // Use the webview to send the request to the
        // request NSURLRequest
        web.loadRequest(request)
    }

}

