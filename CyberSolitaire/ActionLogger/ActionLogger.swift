//
//  ActionLogger.swift
//
//  Created by Christian Muth on 12.04.15.
//  Copyright (c) 2015 Christian Muth. All rights reserved.
//

/**
    Version 1.0.0:  the starter with outputs to the Xcode console and/or files

    Version 1.1.0:  add a new ActionLogdestination -> ActionLogTextViewdestination writes output into (NS/UI)TextView control

    Version 1.1.1:  add the new ActionLogDestination to some functions (bug fix)
                    add outputLogLevel to ActionLogDestinatianProtocoll -> some functions come very easier
                    add Quick Help descriptions, which are also used for documentation with jazzy

    Version 1.1.2:  setting showFileName = false && showLineNumber = true is forbidden
                    if this detected, showLineNumber is set false
    
    Version 1.1.3:  error in Xcode 7.3 in function logSetupValues

    Version 1.1.4:  changes for use with Swift 4

*/


import Foundation
#if os(OSX)
    import AppKit
#endif
#if os(iOS)
    import UIKit
#endif

// Version see at the begin of class ActionLogger


// MARK: - ActionLogDetails
// - Data structure to hold all info about a log message, passed to log destination classes

/// this structure holds all informations of a log message
/// and is passed to the log destination classes
///
/// **properties:**
///
/// * `var logLevel: ActionLogger.LogLevel` -> the LogLevel of this message
/// * `var date: NSDate` -> date and time of this message
/// * `var logMessage: String` -> the pure text message
/// * `var functioneName: String`-> the function name, where the message is generated
/// * `var fileName: String`-> the file name, where the message is created
/// * `var linNumber: Int` -> the line number in the file, where the message is generated
///
public struct ActionLogDetails {
    /// the LogLevel of this message
    var logLevel: ActionLogger.LogLevel
    /// date and time of this message
    var date: Date
    /// the pure text message
    var logMessage: String
    /// the function name, where the message is generated
    var functionName: String
    /// the file name, where the message is created
    var fileName: String
    /// the line number in the file, where the message is generated
    var lineNumber: Int
    
    /// initialize an ActionLogDetails struct
    ///
    /// **parameters** see on struct ActionLogDetails
    ///
    init(logLevel: ActionLogger.LogLevel, date: Date, logMessage: String, functionName: String, fileName: String, lineNumber: Int) {
        self.logLevel = logLevel
        self.date = date
        self.logMessage = logMessage
        self.functionName = functionName
        self.fileName = fileName
        self.lineNumber = lineNumber
    }
}

// MARK: - ActionLogger
/**
####Overview:

The Action Logger class sends messages to the Xcode console and / or a file or a NSTextView object (very new!!!). The messages are processed accordingly before they are issued.
If the Xcode Plugin XcodeColors is installed, the messages in the Xcode console are displayed in color.
The aim of the development was to implement a very easy-to-use method that can be generated with the reports to track program run during software testing. On the other hand has been achieved that through a variety of options, the class can also be used for any other protocol tasks.
See two example outputs

![alt tag](https://cloud.githubusercontent.com/assets/6715559/11895791/a38a1694-a581-11e5-8a0f-b244118d45a2.png)

![alt tag](https://cloud.githubusercontent.com/assets/6715559/11895795/ab8e6598-a581-11e5-8da4-bc2c59592943.png)

####general description:

A message line consists of individually assembled components:

{Date+Time}▫︎[{LogLevel}]▫︎[{Filename}:{LineNumber}]▫︎{Functionname}▫︎:▫︎{Messagetext}

Components are enclosed in {},
▫︎ means space

By default, all components are selected. Each of these components can be turned off (not the message text).

####Use:

The use of Action Logger class in your project is very simple:

Drag the file ActionLoggerComplete.swift file from Finder directly in Xcode project.
in AppDelegate.swift immediately after the imports, insert the following:

```
// Get a reference to defaultLoggerlet
log = ActionLogger.defaultLogger ()
```

Once the Action Logger class is referenced once, a default logger is automatically created. The default logger specifies the Xcode Debug Console.

The logger log can now be used in all other .swift files in your project.

```
log.verbose("this message is generated in ViewController")
ActionLogger.info("here is an info for you")
```

![alt tag](https://cloud.githubusercontent.com/assets/6715559/11776753/f29b0af8-a249-11e5-983a-ffb788dc4892.png)

As the above example shows, it's even easier: all outputs can also be done via class methods. Step 2 of this procedure could also be left out! If you will use the class methods used internally also the default logger automatically generated.
*/
open class ActionLogger : CustomDebugStringConvertible {
    
    // MARK: - class wide vars
    open class var dateFormatterGER: DateFormatter {
        let formatter = DateFormatter()
        formatter.locale =  Locale.current
        formatter.dateFormat = "dd-MM-yyyy HH:mm:ss.SSS"
        return formatter
    }
    
    open class var dateFormatterUSA: DateFormatter {
        let formatter = DateFormatter()
        formatter.locale =  Locale.current
        formatter.dateFormat = "yyyy-MM-dd HH:mm:ss.SSS"
        return formatter
    }
    
    // MARK: - Version
    let integerCharSet = CharacterSet(charactersIn: "+-0123456789")
    
    // read only computed properties
    /// most importent number of version **X**.Y.Z
    open var ActionLoggerVersionX: Int {
        let scanner = Scanner(string: constants.ActionLoggerVersion)
        while let _ = scanner.scanUpToCharactersFromSet(integerCharSet) {}
        return scanner.scanInteger()!
    }
    
    /// middle number of version X.**Y**.Z
    open var ActionLoggerVersionY: Int {
        let scanner = Scanner(string: constants.ActionLoggerVersion)
        while let _ = scanner.scanUpToCharactersFromSet(integerCharSet) {}
        _ = scanner.scanInteger()!
        while let _ = scanner.scanUpToCharactersFromSet(integerCharSet) {}
        return scanner.scanInteger()!
    }
    
    /// least importent number of version X.Y.**Z**
    open var ActionLoggerVersionZ: Int {
        let scanner = Scanner(string: constants.ActionLoggerVersion)
        while let _ = scanner.scanUpToCharactersFromSet(integerCharSet) {}
        _ = scanner.scanInteger()!
        while let _ = scanner.scanUpToCharactersFromSet(integerCharSet) {}
        _ = scanner.scanInteger()!
        while let _ = scanner.scanUpToCharactersFromSet(integerCharSet) {}
        return scanner.scanInteger()!
    }
    
    
    // MARK: - Constants
    struct constants {
        static let defaultLoggerIdentifier = "de.muecke-software.ActionLogger.defaultLogger"
        static let baseConsoleDestinationIdentifier = "de.muecke-software.ActionLogger.logdestination.console"
        static let logQueueIdentifier = "de.muecke-software.ActionLogger.queue"
        /**
         bei
         Veränderung der Schnittstelle                       X
         Starker Veränderung der Funktionalität				X
         
         Erweiterung der Funktionalität (alte bleibt aber erhalten)	Y
         Veränderung der Funktionalität wegen Bug Fixing               Y
         
         Veränderung des internen Codes ohne die Funktionalität
         zu verändern (CodeLifting, interne Schönheit)			Z
         
         X	die verwendenden Applikationen müssen hinsichtlich der vorgenommenen Veränderungen oder auf Grund der geänderten Schnittstellen hin untersucht werden.
         
         Y	Veränderungen in Applikation überprüfen
         
         Z	nur Austausch der Datei ActionLogger.swift nötig
         
         ** !!! Achtung: die Version als String befindet sich bei constants! **
         */
        static let ActionLoggerVersion: String = "1.1.3"
    }
    
    struct statics {
        static var loggerDict = [String: ActionLogger]()
        static let defaultLogger: ActionLogger! = ActionLogger(identifier:ActionLogger.constants.defaultLoggerIdentifier)
        static var logQueue = DispatchQueue(label: ActionLogger.constants.logQueueIdentifier, attributes: [])
        static let standardLogConsoleDestination: ActionLogDestinationProtocol =  ActionLogConsoleDestination(identifier: ActionLogger.constants.baseConsoleDestinationIdentifier)
    }
    
    open var dateFormatter: DateFormatter
    
    // MARK: - Enums
    /// the possible values of LogLevel for a log message
    ///
    /// it depends on the objects outputLogLevel and the LogLevel of the message whether a message is really written to the output
    ///
    /// only log messages with a LogLevel >= outputLogLevel are written out
    public enum LogLevel: Int, Comparable {
        case allLevels = 0,
        messageOnly,
        comment,
        verbose,
        info,
        debug,
        warning,
        error,
        severe
        
        func description() -> String {
            switch self {
            case .allLevels:
                return "AllLevels"
            case .messageOnly:
                return "MessageOnly"
            case .comment:
                return "Comment"
            case .verbose:
                return "Verbose"
            case .debug:
                return "Debug"
            case .info:
                return "Info"
            case .warning:
                return "Warning"
            case .error:
                return "Error"
            case .severe:
                return "Severe"
            }
        }
    }
    
    // MARK: - Properties
    /// the (unique) identifier for an ActionLogger object
    public let identifier: String
    
    /// the current outputLogLevel for the ActionLogger object
    ///
    /// only log messages with a LogLevel >= outputLogLevel are written out
    open var outputLogLevel: LogLevel = .allLevels {
        didSet {
            for logDestination in logDestinations {
                if logDestination is ActionLogConsoleDestination {
                    let tmpDestination = logDestination as! ActionLogConsoleDestination
                    tmpDestination.outputLogLevel = outputLogLevel
                    return
                }
                if logDestination is ActionLogFileDestination {
                    let tmpDestination = logDestination as! ActionLogFileDestination
                    tmpDestination.outputLogLevel = outputLogLevel
                    return
                }
            }
        }
    }
    
    /// an array with all logDestinations of this ActionLogger object
    ///
    /// an ActionLogger can have 1, 2 or more logDestinations e.g.: console and file
    ///
    /// - Note: this var is not public
    var logDestinations = [ActionLogDestinationProtocol]()
    
    // MARK: - initializer
    init?(id: String, withStandardConsole: Bool = true) {
        self.dateFormatter = ActionLogger.dateFormatterGER
        self.identifier = id
        if let _ = statics.loggerDict[identifier] {
            ActionLogger.defaultLogger().error("unable to initialize ActionLogger instance with identifier: \"\(identifier)\" allways exists")
            return nil
        }
        statics.loggerDict[identifier] = self
        if withStandardConsole {
            _ = addLogDestination(ActionLogger.statics.standardLogConsoleDestination)
        }
    }
    
    convenience init?(identifier: String, logDestinations: [ActionLogDestinationProtocol]? = nil) {
        self.init(id: identifier, withStandardConsole: false)
        if let logDests = logDestinations {
            for logDest in logDests {
                _ = addLogDestination(logDest)
            }
        }
        else {
            _ = addLogDestination(ActionLogger.statics.standardLogConsoleDestination)
        }
    }
    
    public convenience init?() {
        self.init(identifier:Bundle.main.bundleIdentifier!)
    }
    
    public convenience init?(logFile withLogFile: String) {
        self.init(id: withLogFile, withStandardConsole: false)
        if let logFileDestination = ActionLogFileDestination(writeToFile: withLogFile as AnyObject) {
            _ = self.addLogDestination(logFileDestination)
        }
        else {
            ActionLogger.defaultLogger().error("could not instantiate ActionLogger instance")
            return nil
        }
    }
    
    deinit {
        // remove Logger from dict
        statics.loggerDict[self.identifier] = nil
    }
    
    // MARK: - DefaultLogger
    /// the defaultLogger is created with the first reference to the class ActionLogger
    ///
    /// if you need only the defaultLogger, you don't need instantiating an ActionLogger object.
    ///
    /// all public class functions work with this defaultLogger
    ///
    /// - returns: `the static defaultLogger`
    open class func defaultLogger() -> ActionLogger {
        return statics.defaultLogger
    }
    
    // MARK: - DebugPrintable
    open var debugDescription: String {
        get {
            var description: String = "ActionLogger: \(identifier) - logDestinations: \r"
            for logDestination in logDestinations {
                description += "\t \(logDestination.debugDescription)\r"
            }
            
            return description
        }
    }
    
    // MARK: - Setup methods
    /// use this class function to setup the defaultLogger
    ///
    /// for description of parameters see the instance function
    open class func setup(logLevel: LogLevel = .allLevels, showDateAndTime: Bool = true, showLogLevel: Bool = true, showFileName: Bool = true, showLineNumber: Bool = true, showFuncName: Bool = true, dateFormatter: DateFormatter = ActionLogger.dateFormatterGER, writeToFile: AnyObject? = nil) {
        defaultLogger().setup(logLevel: logLevel, showDateAndTime: showDateAndTime, showLogLevel: showLogLevel, showFileName: showFileName, showLineNumber: showLineNumber, showFuncName: showFuncName, dateFormatter: dateFormatter, writeToFile: writeToFile)
    }
    
    /// use this function to setup properties of the ActionLogger object
    ///
    /// - Parameters:
    ///   - logLevel: setup the outputLogLevel  default = .AllLevels
    ///   - showDateAndTime: shows the date and time in the message default = true
    ///   - showLogLevel: shows the LogLevel of the message default = true
    ///   - showFileName: shows the filename where the message is generated default = true
    ///   - showLineNumber: shows the linenumber in the file where the message is generated (only if showFileName is true)  default = true
    ///   - showFuncName: shows the func name where the message is generated    default = true
    ///   - dateFormatter: the dateFormatter which is used (ActionLogger has implemented dateFormatterGER and dateFormatterUSA, but you can also use your own)  default = ActionLogger.dateFormatterGER
    ///   - writeToFile: a file to which the messages are written
    open func setup(logLevel: LogLevel = .allLevels, showDateAndTime: Bool = true,  showLogLevel: Bool = true, showFileName: Bool = true, showLineNumber: Bool = true, showFuncName: Bool = true, dateFormatter: DateFormatter = ActionLogger.dateFormatterGER, writeToFile: AnyObject? = nil) {
        outputLogLevel = logLevel;
        
        if let unwrappedWriteToFile : AnyObject = writeToFile {
            // We've been passed a file to use for logging, set up a file logger
            if let logFileDestination: ActionLogFileDestination = ActionLogFileDestination(writeToFile: unwrappedWriteToFile) {
                _ = addLogDestination(logFileDestination)
            }
            else {
                // melde den Fehler
                self.error("could not create ActionLogDestination for \(String(describing: writeToFile))")
            }
        }
        
        for var logDestination in logDestinations {
            logDestination.outputLogLevel = logLevel
            logDestination.showDateAndTime = showDateAndTime
            logDestination.showLogLevel = showLogLevel
            logDestination.showFileName = showFileName
            logDestination.showLineNumber = showLineNumber
            logDestination.showFuncName = showFuncName
            logDestination.dateFormatter = dateFormatter
            if !logDestination.showFileName && logDestination.showLineNumber {
                ActionLogger.defaultLogger().warning("setting showFileName = false AND showLineNumber = true is forbidden! shoeLineNumber is now set to false")
                logDestination.showLineNumber = false
            }
        }
    }
    
    /// use this class function to setup the outputLogLevel of the defaultLogger
    ///
    /// for description of parameters see the instance function
    open class func setupLogLevel(_ logLevel: ActionLogger.LogLevel) {
        defaultLogger().setupLogLevel(logLevel)
    }
    
    /// use this function to setup the outputLogLevel of the ActionLogger object
    ///
    /// - Parameters:
    ///   - logLevel: setup the outputLogLevel
    open func setupLogLevel(_ logLevel: ActionLogger.LogLevel) {
        outputLogLevel = logLevel;
        
        for var logDestination in logDestinations {
            logDestination.outputLogLevel = outputLogLevel
        }
    }
    
    
    /// use this class function to setup the showDateAndTime property of the defaultLogger
    ///
    /// for description of parameters see the instance function
    open class func setupShowDateAndTime(_ showDateAndTime: Bool) {
        defaultLogger().setupShowDateAndTime(showDateAndTime)
    }
    
    /// use this function to setup the showDateAndTime property of the ActionLogger object
    ///
    /// - Parameters:
    ///   - showDateAndTime: shows the date and time in the message
    open func setupShowDateAndTime(_ showDateAndTime: Bool) {
        for var logDestination in logDestinations {
            logDestination.showDateAndTime = showDateAndTime
        }
    }
    
    /// use this class function to setup the showLogLevel property of the defaultLogger
    ///
    /// for description of parameters see the instance function
    open class func setupShowLogLevel(_ showLogLevel: Bool) {
        defaultLogger().setupShowLogLevel(showLogLevel)
    }
    
    /// use this function to setup the showLogLevel property of the ActionLogger object
    ///
    /// - Parameters:
    ///   - showLogLevel: shows the LogLevel of the message
    open func setupShowLogLevel(_ showLogLevel: Bool) {
        for var logDestination in logDestinations {
            logDestination.showLogLevel = showLogLevel
        }
    }
    
    /// use this class function to setup the showFileName property of the defaultLogger
    ///
    /// for description of parameters see the instance function
    open class func setupShowFileName(_ showFileName: Bool) {
        defaultLogger().setupShowFileName(showFileName)
    }
    
    /// use this function to setup the showFileName property of the ActionLogger object
    ///
    /// - Parameters:
    ///   - showFileName: shows the filename where the message is generated
    open func setupShowFileName(_ showFileName: Bool) {
        for var logDestination in logDestinations {
            logDestination.showFileName = showFileName
            if showFileName == false {
                logDestination.showLineNumber = false
            }
        }
    }
    
    /// use this class function to setup the showFileNumber property of the defaultLogger
    ///
    /// for description of parameters see the instance function
    open class func setupShowLineNumber(_ showLineNumber: Bool) {
        defaultLogger().setupShowLineNumber(showLineNumber)
    }
    
    /// use this function to setup the showFilenumber property of the ActionLogger object
    ///
    /// - Parameters:
    ///   - showLineNumber: shows the linenumber in the file where the message is generated (only if showFileName is true)
    open func setupShowLineNumber(_ showLineNumber: Bool) {
        for var logDestination in logDestinations {
            if logDestination.showFileName == true {
                logDestination.showLineNumber = showLineNumber
            }
            else {
                if showLineNumber == true {
                    ActionLogger.error("showLineNumber cannot set true, if showFileName is false")
                }
            }
        }
    }
    
    /// use this class function to setup the showFuncName property of the defaultLogger
    ///
    /// for description of parameters see the instance function
    open class func setupShowFuncName(_ showFuncName: Bool) {
        defaultLogger().setupShowFuncName(showFuncName)
    }
    
    /// use this function to setup the showFuncName property of the ActionLogger object
    ///
    /// - Parameters:
    ///   - showFuncName: shows the func name where the message is generated
    open func setupShowFuncName(_ showFuncName: Bool) {
        for var logDestination in logDestinations {
            logDestination.showFuncName = showFuncName
        }
    }
    
    
    /// use this class function to setup the dateFormatter property of the defaultLogger
    ///
    /// for description of parameters see the instance function
    open class func setupDateFormatter(_ dateFormatter: DateFormatter) {
        defaultLogger().setupDateFormatter(dateFormatter)
    }
    
    /// use this function to setup the dateFormatter property of the ActionLogger object
    ///
    /// - Parameters:
    ///   - dateFormatter: the dateFormatter which is used (ActionLogger has implemented dateFormatterGER and dateFormatterUSA, but you can also use your own)
    open func setupDateFormatter(_ dateFormatter: DateFormatter) {
        for var logDestination in logDestinations {
            logDestination.dateFormatter = dateFormatter
        }
    }
    
    /// logs all properties values to the output
    open func logSetupValues() {
        // log the setup values
        var message =   "setupValues for ActionLogger object\n" +
            "ActionLogger Version: \(constants.ActionLoggerVersion)\n" +
            "Identifier          : \(identifier)\n" +
            "outputLogLevel      : \(outputLogLevel.description())\n" +
            "with logDestinations:\n"
        
        for logDestination in logDestinations {
            //let typeLongName = _stdlib_getDemangledTypeName(logDestination)   // error since Xcode 7.3
            let typeLongName = String(describing: type(of: self))
            let tokens = typeLongName.split(separator: ".")
            //let tokens = typeLongName.characters.split(separator: { $0 == "." }).map { String($0) }
            let typeName = tokens.last!
            message += "\n" +
                "Type of logDestination: \(typeName)\n" +
                "Identifier            : \(logDestination.identifier)\n" +
                "showLogLevel          : \(logDestination.showLogLevel)\n" +
                "showFileName          : \(logDestination.showFileName)\n" +
                "showLineNumber        : \(logDestination.showLineNumber)\n" +
                "showFuncName          : \(logDestination.showFuncName)\n" +
            "date & time format    : \(String(describing: logDestination.dateFormatter.dateFormat))\n"
            
            if logDestination.hasFile() {
                message +=
                "writeToFile           : \((logDestination as! ActionLogFileDestination).getLogFileURL())\n"
            }
        }
        message += "\nend of setupValues\n"
        logLine(message, logLevel: LogLevel.info, functionName: "", fileName: "", lineNumber: 0, withFileLineFunctionInfo: false)
    }
    
    // MARK: - Logging methods
    open class func logLine(_ logMessage: String, logLevel: LogLevel = .debug, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line, withFileLineFunctionInfo: Bool = true) {
        self.defaultLogger().logLine(logMessage, logLevel: logLevel, functionName: functionName, fileName: fileName, lineNumber: lineNumber, withFileLineFunctionInfo: withFileLineFunctionInfo)
    }
    
    
    open func logLine(_ logMessage: String, logLevel: LogLevel = .debug, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line, withFileLineFunctionInfo: Bool = true) {
        let date = Date()
        let logDetails = ActionLogDetails(logLevel: logLevel, date: date, logMessage: logMessage, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
        for logDestination in self.logDestinations {
            logDestination.processLogDetails(logDetails,withFileLineFunctionInfo: withFileLineFunctionInfo)
        }
    }
    
    open class func exec(_ logLevel: LogLevel = .debug, closure: () -> () = {}) {
        self.defaultLogger().exec(logLevel, closure: closure)
    }
    
    open func exec(_ logLevel: LogLevel = .debug, closure: () -> () = {}) {
        if (!isEnabledForLogLevel(logLevel)) {
            return
        }
        
        closure()
    }
    
    //    func logLogDetails(logDetails: [ActionLogDetails], selectedLogDestination: ActionLogDestinationProtocol? = nil) {
    //        for logDestination in (selectedLogDestination != nil ? [selectedLogDestination!] : logDestinations) {
    //            for logDetail in logDetails {
    //                logDestination.processLogDetails(logDetail,withFileLineFunctionInfo: false)
    //            }
    //        }
    //    }
    
    
    
    // MARK: - Convenience logging methods
    open class func messageOnly(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.defaultLogger().messageOnly(logMessage, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open func messageOnly(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.logLine(logMessage, logLevel: .messageOnly, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open class func comment(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.defaultLogger().comment(logMessage, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open func comment(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.logLine(logMessage, logLevel: .comment, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    open class func verbose(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.defaultLogger().verbose(logMessage, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open func verbose(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.logLine(logMessage, logLevel: .verbose, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open class func debug(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.defaultLogger().debug(logMessage, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open func debug(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.logLine(logMessage, logLevel: .debug, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open class func info(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.defaultLogger().info(logMessage, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open func info(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.logLine(logMessage, logLevel: .info, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open class func warning(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.defaultLogger().warning(logMessage, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open func warning(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.logLine(logMessage, logLevel: .warning, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open class func error(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.defaultLogger().error(logMessage, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open func error(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.logLine(logMessage, logLevel: .error, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open class func severe(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.defaultLogger().severe(logMessage, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open func severe(_ logMessage: String, functionName: String = #function, fileName: String = #file, lineNumber: Int = #line) {
        self.logLine(logMessage, logLevel: .severe, functionName: functionName, fileName: fileName, lineNumber: lineNumber)
    }
    
    open class func messageOnlyExec(_ closure: () -> () = {}) {
        self.defaultLogger().exec(ActionLogger.LogLevel.messageOnly, closure: closure)
    }
    
    open func messageOnlyExec(_ closure: () -> () = {}) {
        self.exec(ActionLogger.LogLevel.messageOnly, closure: closure)
    }
    
    open class func commentExec(_ closure: () -> () = {}) {
        self.defaultLogger().exec(ActionLogger.LogLevel.comment, closure: closure)
    }
    
    open func commentExec(_ closure: () -> () = {}) {
        self.exec(ActionLogger.LogLevel.comment, closure: closure)
    }
    
    open class func verboseExec(_ closure: () -> () = {}) {
        self.defaultLogger().exec(ActionLogger.LogLevel.verbose, closure: closure)
    }
    
    open func verboseExec(_ closure: () -> () = {}) {
        self.exec(ActionLogger.LogLevel.verbose, closure: closure)
    }
    
    open class func debugExec(_ closure: () -> () = {}) {
        self.defaultLogger().exec(ActionLogger.LogLevel.debug, closure: closure)
    }
    
    open func debugExec(_ closure: () -> () = {}) {
        self.exec(ActionLogger.LogLevel.debug, closure: closure)
    }
    
    open class func infoExec(_ closure: () -> () = {}) {
        self.defaultLogger().exec(ActionLogger.LogLevel.info, closure: closure)
    }
    
    open func infoExec(_ closure: () -> () = {}) {
        self.exec(ActionLogger.LogLevel.info, closure: closure)
    }
    
    open class func warningExec(_ closure: () -> () = {}) {
        self.defaultLogger().exec(ActionLogger.LogLevel.warning, closure: closure)
    }
    
    open func warningExec(_ closure: () -> () = {}) {
        self.exec(ActionLogger.LogLevel.warning, closure: closure)
    }
    
    open class func errorExec(_ closure: () -> () = {}) {
        self.defaultLogger().exec(ActionLogger.LogLevel.error, closure: closure)
    }
    
    open func errorExec(_ closure: () -> () = {}) {
        self.exec(ActionLogger.LogLevel.error, closure: closure)
    }
    
    open class func severeExec(_ closure: () -> () = {}) {
        self.defaultLogger().exec(ActionLogger.LogLevel.severe, closure: closure)
    }
    
    open func severeExec(_ closure: () -> () = {}) {
        self.exec(ActionLogger.LogLevel.severe, closure: closure)
    }
    
    // MARK: - Misc methods
    open func isEnabledForLogLevel (_ logLevel: ActionLogger.LogLevel) -> Bool {
        return logLevel >= self.outputLogLevel
    }
    
    open func logDestination(_ identifier: String) -> ActionLogDestinationProtocol? {
        for logDestination in logDestinations {
            if logDestination.identifier == identifier {
                return logDestination
            }
        }
        return nil
    }
    
    open func getLogDestinations() -> [ActionLogDestinationProtocol] {
        return logDestinations
    }
    
    open func addLogDestination(_ logDestination: ActionLogDestinationProtocol) -> Bool {
        let existingLogDestination: ActionLogDestinationProtocol? = self.logDestination(logDestination.identifier)
        if existingLogDestination != nil {
            return false
        }
        logDestinations.append(logDestination)
        return true
    }
    
    open func removeLogDestination(_ logDestination: ActionLogDestinationProtocol) {
        removeLogDestination(logDestination.identifier)
    }
    
    open func removeLogDestination(_ identifier: String) {
        logDestinations = logDestinations.filter({$0.identifier != identifier})
    }
    
}


// Implement Comparable for ActionLogger.LogLevel
public func < (lhs:ActionLogger.LogLevel, rhs:ActionLogger.LogLevel) -> Bool {
    return lhs.rawValue < rhs.rawValue
}


// MARK: - ActionLogXcodeColorProfile
/// ColorProfile which is used with the XcodeColor plugin for coloring Xcode console outputs
struct ActionLogXcodeColorProfile {
    
    var fg_Red:UInt8 = 0
    var fg_Green: UInt8 = 0
    var fg_Blue: UInt8 = 0
    
    var fg_code: String = ""
    
    var bg_Red:UInt8 = 255
    var bg_Green: UInt8 = 255
    var bg_Blue: UInt8 = 255
    
    var bg_code: String = ""
    
    let fg_reset_code: String = "\u{001B}[fg;"
    let bg_reset_code: String = "\u{001B}[bg;"
    let all_reset_code: String = "\u{001B}[;"
    
    let xcodeColorEscape = "\u{001B}["
    
    init (fg_Red: UInt8=0,fg_Green: UInt8=0,fg_Blue: UInt8=0,bg_Red: UInt8=255,bg_Green: UInt8=255,bg_Blue: UInt8=255) {
        self.fg_Red = fg_Red; self.fg_Green = fg_Green; self.fg_Blue = fg_Blue
        self.bg_Red = bg_Red; self.bg_Green = bg_Green; self.bg_Blue = bg_Blue
        
        self.fg_code = xcodeColorEscape + (NSString(format: "fg%d,%d,%d;", self.fg_Red,self.fg_Green,self.fg_Blue) as String)
        self.bg_code = xcodeColorEscape + (NSString(format: "bg%d,%d,%d;", self.bg_Red,self.bg_Green,self.bg_Blue) as String)
        
    }
    
    #if os(OSX)
    
    init(foregroundColor fg_color: NSColor, backgroundColor bg_color: NSColor) {
        var fg_red: CGFloat = 0, fg_green: CGFloat = 0, fg_blue: CGFloat = 0
        var bg_red: CGFloat = 0, bg_green: CGFloat = 0, bg_blue: CGFloat = 0
        var alpha: CGFloat = 0
        let fg_c = fg_color.usingColorSpace(NSColorSpace.deviceRGB)
        fg_c!.getRed(&fg_red, green: &fg_green, blue: &fg_blue, alpha: &alpha)
        let bg_c = bg_color.usingColorSpace(NSColorSpace.deviceRGB)
        bg_c!.getRed(&bg_red, green: &bg_green, blue: &bg_blue, alpha: &alpha)
        
        self.fg_Red = UInt8(fg_red * 255.0); self.fg_Green = UInt8(fg_green * 255.0); self.fg_Blue = UInt8(fg_blue * 255.0)
        self.bg_Red = UInt8(bg_red * 255.0); self.bg_Green = UInt8(bg_green * 255.0); self.bg_Blue = UInt8(bg_blue * 255.0)
        
        self.fg_code = xcodeColorEscape + (NSString(format: "fg%d,%d,%d;", self.fg_Red,self.fg_Green,self.fg_Blue) as String)
        self.bg_code = xcodeColorEscape + (NSString(format: "bg%d,%d,%d;", self.bg_Red,self.bg_Green,self.bg_Blue) as String)
        
    }
    
    #endif
    
    
    #if os(iOS)
    
    init(foregroundColor fg_color: UIColor, backgroundColor bg_color: UIColor) {
    var fg_red: CGFloat = 0, fg_green: CGFloat = 0, fg_blue: CGFloat = 0
    var bg_red: CGFloat = 0, bg_green: CGFloat = 0, bg_blue: CGFloat = 0
    var alpha: CGFloat = 0
    fg_color.getRed(&fg_red, green: &fg_green, blue: &fg_blue, alpha: &alpha)
    bg_color.getRed(&bg_red, green: &bg_green, blue: &bg_blue, alpha: &alpha)
    
    self.fg_Red = UInt8(fg_red * 255.0); self.fg_Green = UInt8(fg_green * 255.0); self.fg_Blue = UInt8(fg_blue * 255.0)
    self.bg_Red = UInt8(bg_red * 255.0); self.bg_Green = UInt8(bg_green * 255.0); self.bg_Blue = UInt8(bg_blue * 255.0)
    
    self.fg_code = xcodeColorEscape + (NSString(format: "fg%d,%d,%d;", self.fg_Red,self.fg_Green,self.fg_Blue) as String)
    self.bg_code = xcodeColorEscape + (NSString(format: "bg%d,%d,%d;", self.bg_Red,self.bg_Green,self.bg_Blue) as String)
    
    }
    
    #endif
    
    mutating func buildForegroundCode() {
        self.fg_code = xcodeColorEscape + (NSString(format: "fg%d,%d,%d;", self.fg_Red,self.fg_Green,self.fg_Blue) as String)
    }
    
    mutating func buildBackgroundCode() {
        self.bg_code = xcodeColorEscape + (NSString(format: "bg%d,%d,%d;", self.bg_Red,self.bg_Green,self.bg_Blue) as String)
    }
    
}

// MARK: - common functions
public func preProcessLogDetails(_ logDetails: ActionLogDetails, showDateAndTime: Bool, showLogLevel: Bool, showFileName: Bool, showLineNumber: Bool, showFuncName: Bool, dateFormatter: DateFormatter,
    withFileLineFunctionInfo: Bool = true) -> String {
        // create extended details
        var extendedDetails: String = ""
        if showLogLevel && (logDetails.logLevel > .messageOnly) {
            extendedDetails += "[" + logDetails.logLevel.description() + "] "
        }
        
        if withFileLineFunctionInfo {
            // showLineNumber is only relevant with showFileName
            if showFileName {
                let url = URL(fileURLWithPath:logDetails.fileName)
                extendedDetails += "[" + url.lastPathComponent + (showLineNumber ? ":" + String(logDetails.lineNumber) : "") + "] "
            }
            
            if showFuncName {
                extendedDetails += "\(logDetails.functionName) "
            }
        }
        let formattedDate: String = dateFormatter.string(from: logDetails.date)
        
        var logMessage = logDetails.logMessage
        
        if logDetails.logLevel == ActionLogger.LogLevel.comment {
            logMessage = "// " + logMessage
        }
        
        if showDateAndTime == true {
            return  "\(formattedDate) \(extendedDetails): \(logMessage)\n"
        }
        else {
            return  "\(extendedDetails): \(logMessage)\n"
        }
}

// MARK: - ActionLogDestinationProtocol
/// - Protocol for output classes to conform to
public protocol ActionLogDestinationProtocol: CustomDebugStringConvertible {
    var identifier: String {get set}
    var showDateAndTime: Bool {get set}
    var showLogLevel: Bool {get set}
    var showFileName: Bool {get set}
    var showLineNumber: Bool {get set}
    var showFuncName: Bool {get set}
    var dateFormatter: DateFormatter {get set}
    var outputLogLevel: ActionLogger.LogLevel {get set}
    
    func processLogDetails(_ logDetails: ActionLogDetails, withFileLineFunctionInfo: Bool)
    func isEnabledForLogLevel(_ logLevel: ActionLogger.LogLevel) -> Bool
    func hasFile() -> Bool
    // color enhancement
    func isEnabledForColor() -> Bool
}

// MARK: - ActionLogConsoleDestination
/// - A standard log destination that outputs log details to the console
open class ActionLogConsoleDestination : ActionLogDestinationProtocol, CustomDebugStringConvertible {
    //    var owner: ActionLogger
    open var identifier: String
    
    open var showDateAndTime: Bool = true
    open var showLogLevel: Bool = true
    open var showFileName: Bool = true
    open var showLineNumber: Bool = true
    open var showFuncName: Bool = true
    open var dateFormatter = ActionLogger.dateFormatterGER
    open var outputLogLevel: ActionLogger.LogLevel = .allLevels
    
    // color enhancement
    var colorProfiles = Dictionary<ActionLogger.LogLevel,ActionLogXcodeColorProfile>()
    
    
    init(identifier: String = "") {
        //        self.owner = owner
        self.identifier = identifier
        
        // color enhancement
        if isEnabledForColor() {
            // setting default color values
            #if os(OSX)
                self.colorProfiles[.allLevels]    = ActionLogXcodeColorProfile(foregroundColor: NSColor.white,backgroundColor: NSColor.white)
                self.colorProfiles[.messageOnly]  = ActionLogXcodeColorProfile(foregroundColor: NSColor.lightGray,backgroundColor: NSColor.white)
                self.colorProfiles[.comment]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.gray,backgroundColor: NSColor.white)
                self.colorProfiles[.verbose]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.darkGray,backgroundColor: NSColor.white)
                self.colorProfiles[.info]         = ActionLogXcodeColorProfile(foregroundColor: NSColor.blue,backgroundColor: NSColor.white)
                self.colorProfiles[.debug]        = ActionLogXcodeColorProfile(foregroundColor: NSColor.green,backgroundColor: NSColor.white)
                self.colorProfiles[.warning]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.orange,backgroundColor: NSColor.white)
                self.colorProfiles[.error]        = ActionLogXcodeColorProfile(foregroundColor: NSColor.red,backgroundColor: NSColor.white)
                self.colorProfiles[.severe]       = ActionLogXcodeColorProfile(foregroundColor: NSColor.magenta,backgroundColor: NSColor.white)
            #endif
            
            #if os(iOS)
                self.colorProfiles[.allLevels]    = ActionLogXcodeColorProfile(foregroundColor: UIColor.white,backgroundColor: UIColor.white)
                self.colorProfiles[.messageOnly]  = ActionLogXcodeColorProfile(foregroundColor: UIColor.lightGray,backgroundColor: UIColor.white)
                self.colorProfiles[.comment]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.gray,backgroundColor: UIColor.white)
                self.colorProfiles[.verbose]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.darkGray,backgroundColor: UIColor.white)
                self.colorProfiles[.info]         = ActionLogXcodeColorProfile(foregroundColor: UIColor.blue,backgroundColor: UIColor.white)
                self.colorProfiles[.debug]        = ActionLogXcodeColorProfile(foregroundColor: UIColor.green,backgroundColor: UIColor.white)
                self.colorProfiles[.warning]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.orange,backgroundColor: UIColor.white)
                self.colorProfiles[.error]        = ActionLogXcodeColorProfile(foregroundColor: UIColor.red,backgroundColor: UIColor.white)
                self.colorProfiles[.severe]       = ActionLogXcodeColorProfile(foregroundColor: UIColor.magenta,backgroundColor: UIColor.white)
            #endif
        }
    }
    
    open func setDefaultLogLevelColors() {
        // color enhancement
        if isEnabledForColor() {
            // setting default color values
            #if os(OSX)
                self.colorProfiles[.allLevels]    = ActionLogXcodeColorProfile(foregroundColor: NSColor.white,backgroundColor: NSColor.white)
                self.colorProfiles[.messageOnly]  = ActionLogXcodeColorProfile(foregroundColor: NSColor.lightGray,backgroundColor: NSColor.white)
                self.colorProfiles[.comment]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.gray,backgroundColor: NSColor.white)
                self.colorProfiles[.verbose]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.darkGray,backgroundColor: NSColor.white)
                self.colorProfiles[.info]         = ActionLogXcodeColorProfile(foregroundColor: NSColor.blue,backgroundColor: NSColor.white)
                self.colorProfiles[.debug]        = ActionLogXcodeColorProfile(foregroundColor: NSColor.green,backgroundColor: NSColor.white)
                self.colorProfiles[.warning]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.orange,backgroundColor: NSColor.white)
                self.colorProfiles[.error]        = ActionLogXcodeColorProfile(foregroundColor: NSColor.red,backgroundColor: NSColor.white)
                self.colorProfiles[.severe]       = ActionLogXcodeColorProfile(foregroundColor: NSColor.magenta,backgroundColor: NSColor.white)
            #endif
            
            #if os(iOS)
                self.colorProfiles[.allLevels]    = ActionLogXcodeColorProfile(foregroundColor: UIColor.white,backgroundColor: UIColor.white)
                self.colorProfiles[.messageOnly]  = ActionLogXcodeColorProfile(foregroundColor: UIColor.lightGray,backgroundColor: UIColor.white)
                self.colorProfiles[.comment]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.gray,backgroundColor: UIColor.white)
                self.colorProfiles[.verbose]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.darkGray,backgroundColor: UIColor.white)
                self.colorProfiles[.info]         = ActionLogXcodeColorProfile(foregroundColor: UIColor.blue,backgroundColor: UIColor.white)
                self.colorProfiles[.debug]        = ActionLogXcodeColorProfile(foregroundColor: UIColor.green,backgroundColor: UIColor.white)
                self.colorProfiles[.warning]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.orange,backgroundColor: UIColor.white)
                self.colorProfiles[.error]        = ActionLogXcodeColorProfile(foregroundColor: UIColor.red,backgroundColor: UIColor.white)
                self.colorProfiles[.severe]       = ActionLogXcodeColorProfile(foregroundColor: UIColor.magenta,backgroundColor: UIColor.white)
            #endif
        }
    }
    
    open func processLogDetails(_ logDetails: ActionLogDetails, withFileLineFunctionInfo: Bool = true) {
        var fullLogMessage = preProcessLogDetails(logDetails, showDateAndTime: showDateAndTime, showLogLevel: showLogLevel, showFileName: showFileName, showLineNumber: showLineNumber, showFuncName: showFuncName, dateFormatter: dateFormatter, withFileLineFunctionInfo: withFileLineFunctionInfo)
        
        // color enhancement
        if let cp = self.colorProfiles[logDetails.logLevel] {
            fullLogMessage = cp.fg_code + cp.bg_code + fullLogMessage + cp.all_reset_code
        }
        
        // print it, only if the LogDestination should print this
        if isEnabledForLogLevel(logDetails.logLevel) {
            ActionLogger.statics.logQueue.async {
                print(fullLogMessage, terminator: "")
            }
        }
    }
    
    // MARK: - Misc methods
    open func isEnabledForLogLevel (_ logLevel: ActionLogger.LogLevel) -> Bool {
        return logLevel >= self.outputLogLevel
    }
    
    // color enhancement
    open func isEnabledForColor() -> Bool {
        let dict = ProcessInfo.processInfo.environment
        if let env = dict["XcodeColors"] as String? {
            return env == "YES"
        }
        return false
    }
    
    open func hasFile() -> Bool {
        return false
    }
    
    // MARK: - DebugPrintable
    open var debugDescription: String {
        get {
            return "ActionLogConsoleDestination: \(identifier) - LogLevel: \(outputLogLevel.description()) showLogLevel: \(showLogLevel) showFileName: \(showFileName) showLineNumber: \(showLineNumber) date & time format: \(String(describing: dateFormatter.dateFormat))"
        }
    }
    
    // color enhancement
    // MARK: - color enhancement
    open func setLogColors(foregroundRed fg_red: UInt8 = 0, foregroundGreen fg_green: UInt8 = 0, foregroundBlue fg_blue: UInt8 = 0, backgroundRed bg_red: UInt8 = 255, backgroundGreen bg_green: UInt8 = 255, backgroundBlue bg_blue: UInt8 = 255, forLogLevel logLevel: ActionLogger.LogLevel) {
        if var cp = self.colorProfiles[logLevel] {
            cp.fg_Red = fg_red; cp.fg_Green = fg_green; cp.fg_Blue = fg_blue
            cp.bg_Red = bg_red; cp.bg_Green = bg_green; cp.bg_Blue = bg_blue
            cp.buildForegroundCode()
            cp.buildBackgroundCode()
            self.colorProfiles[logLevel] = cp
        }
    }
    
    /*! using setLogColors:
    setLogColor(foregroundRed:0,foregroundGreen:0,foregroundBlue:0,forLogLevel:.Verbose)        means: resetForegroundColor of logLevel .Verbose to black
    setLogColor(backgroundRed:255,backgroundGreen:255,backgroundBlue:255,forLogLevel:.Debug)    means: resetBackgroundColor of logLevel .Debug   to white
    */
    
    open func resetAllLogColors() {
        for (logLevel, var colorProfile) in colorProfiles {
            colorProfile.fg_Red = 0; colorProfile.fg_Green = 0; colorProfile.fg_Blue = 0
            colorProfile.bg_Red = 255; colorProfile.bg_Green = 255; colorProfile.bg_Blue = 255
            colorProfile.buildForegroundCode()
            colorProfile.buildBackgroundCode()
            self.colorProfiles[logLevel] = colorProfile
        }
    }
    
    #if os(OSX)
    
    open func setForegroundColor(_ color: NSColor, forLogLevel logLevel: ActionLogger.LogLevel) {
        var fg_red: CGFloat = 0, fg_green: CGFloat = 0, fg_blue: CGFloat = 0
        var alpha: CGFloat = 0
        let c = color.usingColorSpace(NSColorSpace.deviceRGB)
        c!.getRed(&fg_red, green: &fg_green, blue: &fg_blue, alpha: &alpha)
        
        if var cp = self.colorProfiles[logLevel] {
            cp.fg_Red = UInt8(fg_red * 255.0); cp.fg_Green = UInt8(fg_green * 255.0); cp.fg_Blue = UInt8(fg_blue * 255.0)
            cp.buildForegroundCode()
            cp.buildBackgroundCode()
            self.colorProfiles[logLevel] = cp
        }
    }
    
    
    open func setBackgroundColor(_ color: NSColor, forLogLevel logLevel: ActionLogger.LogLevel) {
        var bg_red: CGFloat = 0, bg_green: CGFloat = 0, bg_blue: CGFloat = 0
        var alpha: CGFloat = 0
        
        let c = color.usingColorSpace(NSColorSpace.deviceRGB)
        c!.getRed(&bg_red, green: &bg_green, blue: &bg_blue, alpha: &alpha)
        
        if var cp = self.colorProfiles[logLevel] {
            cp.bg_Red = UInt8(bg_red * 255.0); cp.bg_Green = UInt8(bg_green * 255.0); cp.bg_Blue = UInt8(bg_blue * 255.0)
            cp.buildForegroundCode()
            cp.buildBackgroundCode()
            self.colorProfiles[logLevel] = cp
        }
    }
    
    #endif
    
    #if os(iOS)
    
    public func setForegroundColor(color: UIColor, forLogLevel logLevel: ActionLogger.LogLevel) {
    var fg_red: CGFloat = 0, fg_green: CGFloat = 0, fg_blue: CGFloat = 0
    var alpha: CGFloat = 0
    color.getRed(&fg_red, green: &fg_green, blue: &fg_blue, alpha: &alpha)
    
    if var cp = self.colorProfiles[logLevel] {
    cp.fg_Red = UInt8(fg_red * 255.0); cp.fg_Green = UInt8(fg_green * 255.0); cp.fg_Blue = UInt8(fg_blue * 255.0)
    cp.buildForegroundCode()
    cp.buildBackgroundCode()
    self.colorProfiles[logLevel] = cp
    }
    }
    
    
    public func setBackgroundColor(color: UIColor, forLogLevel logLevel: ActionLogger.LogLevel) {
    var bg_red: CGFloat = 0, bg_green: CGFloat = 0, bg_blue: CGFloat = 0
    var alpha: CGFloat = 0
    
    color.getRed(&bg_red, green: &bg_green, blue: &bg_blue, alpha: &alpha)
    
    if var cp = self.colorProfiles[logLevel] {
    cp.bg_Red = UInt8(bg_red * 255.0); cp.bg_Green = UInt8(bg_green * 255.0); cp.bg_Blue = UInt8(bg_blue * 255.0)
    cp.buildForegroundCode()
    cp.buildBackgroundCode()
    self.colorProfiles[logLevel] = cp
    }
    }
    
    #endif
}

// MARK: - ActionLogFileDestination
/// - A standard log destination that outputs log details to a file
open class ActionLogFileDestination : ActionLogDestinationProtocol, CustomDebugStringConvertible {
    //    var owner: ActionLogger
    open var identifier: String = ""
    
    open var showDateAndTime: Bool = true
    open var showLogLevel: Bool = true
    open var showFileName: Bool = true
    open var showLineNumber: Bool = true
    open var showFuncName: Bool = true
    open var dateFormatter = ActionLogger.dateFormatterGER
    open var outputLogLevel: ActionLogger.LogLevel = .allLevels
    
    fileprivate var writeToFileURL : URL? = nil {
        didSet {
            _ = openFile()
        }
    }
    fileprivate var logFileHandle: FileHandle? = nil
    
    init?(writeToFile: AnyObject) {
        
        if writeToFile is NSString {
            writeToFileURL = URL(fileURLWithPath: writeToFile as! String)
            self.identifier = writeToFile as! String
        }
        else if writeToFile is URL {
            writeToFileURL = writeToFile as? URL
            if !writeToFileURL!.isFileURL
            {
                ActionLogger.defaultLogger().error("no fileURL is given!")
                return nil
            }
            else {
                self.identifier = writeToFileURL!.absoluteString
            }
        }
        else {
            ActionLogger.defaultLogger().error("unable to open file: \"\(writeToFile as! String)\"")
            writeToFileURL = nil
            return nil
        }
        
        if !openFile() {
            ActionLogger.defaultLogger().error("unable to open file: \"\(String(describing: writeToFileURL))\"")
            return nil
        }
        closeFile()
    }
    
    deinit {
        // close file stream if open
        closeFile()
    }
    
    // MARK: - Logging methods
    open func processLogDetails(_ logDetails: ActionLogDetails, withFileLineFunctionInfo: Bool = true) {
        let fullLogMessage = preProcessLogDetails(logDetails, showDateAndTime: showDateAndTime, showLogLevel: showLogLevel, showFileName: showFileName, showLineNumber: showLineNumber, showFuncName: showFuncName,  dateFormatter: dateFormatter, withFileLineFunctionInfo: withFileLineFunctionInfo)
        
        // print it, only if the LogDestination should print this
        if isEnabledForLogLevel(logDetails.logLevel) {
            if let encodedData = fullLogMessage.data(using: String.Encoding.utf8) {
                _ = reopenFile()
                logFileHandle?.write(encodedData)
                closeFile()
            }
        }
    }
    
    open func getLogFileName() -> String {
        return writeToFileURL!.lastPathComponent
    }
    
    open func getLogFileURL() -> String {
        return writeToFileURL!.absoluteString
    }
    
    
    // MARK: - Misc methods
    open func isEnabledForLogLevel (_ logLevel: ActionLogger.LogLevel) -> Bool {
        return logLevel >= self.outputLogLevel
    }
    
    open func isEnabledForColor() -> Bool {
        return false
    }
    
    open func hasFile() -> Bool {
        return true
    }
    
    fileprivate func openFile() -> Bool {
        if logFileHandle != nil {
            closeFile()
        }
        
        if let unwrappedWriteToFileURL = writeToFileURL {
            if let path = unwrappedWriteToFileURL.path as String? {
                FileManager.default.createFile(atPath: path, contents: nil, attributes: nil)
                var fileError : NSError? = nil
                do {
                    logFileHandle = try FileHandle(forWritingTo: unwrappedWriteToFileURL)
                } catch let error as NSError {
                    fileError = error
                    logFileHandle = nil
                }
                if logFileHandle == nil {
                    ActionLogger.defaultLogger().logLine("Attempt to open log file for writing failed: \(String(describing: fileError?.localizedDescription))", logLevel: .error, withFileLineFunctionInfo: false)
                    return false
                }
                else {
                    ActionLogger.defaultLogger().logLine("ActionLogger writing log to: \(unwrappedWriteToFileURL)", logLevel: .info, withFileLineFunctionInfo: false)
                    return true
                }
            }
        }
        return false
    }
    
    fileprivate func reopenFile() -> Bool {
        if logFileHandle != nil {
            closeFile()
        }
        
        if let unwrappedWriteToFileURL = writeToFileURL {
            if let _ = unwrappedWriteToFileURL.path as String? {
                var fileError : NSError? = nil
                do {
                    logFileHandle = try FileHandle(forWritingTo: unwrappedWriteToFileURL)
                } catch let error as NSError {
                    fileError = error
                    logFileHandle = nil
                }
                if logFileHandle == nil {
                    ActionLogger.defaultLogger().logLine("Attempt to open log file for writing failed: \(String(describing: fileError?.localizedDescription))", logLevel: .error, withFileLineFunctionInfo: false)
                    return false
                }
                else {
                    logFileHandle?.seekToEndOfFile()
                    return true
                }
            }
        }
        return false
    }
    
    
    fileprivate func closeFile() {
        logFileHandle?.closeFile()
        logFileHandle = nil
    }
    
    // MARK: - DebugPrintable
    open var debugDescription: String {
        get {
            return "ActionLogFileDestination: \(identifier) - LogLevel: \(outputLogLevel.description()) showLogLevel: \(showLogLevel) showFileName: \(showFileName) showLineNumber: \(showLineNumber) date & time format: \(String(describing: dateFormatter.dateFormat))"
        }
    }
}

// color enhancement
// MARK: - ActionLogTextViewColorProfile
/// ColorProfile used with (NS/UI)TextView controls for coloring text outputs
struct ActionLogTextViewColorProfile {
    
    #if os(OSX)
    var foregroundColor: NSColor = NSColor.black
    var backgroundColor: NSColor = NSColor.white
    #endif
    
    #if os(iOS)
    var foregroundColor: UIColor = UIColor.black
    var backgroundColor: UIColor = UIColor.white
    #endif
    
    #if os(OSX)
    init(foregroundColor fg_color: NSColor, backgroundColor bg_color: NSColor) {
        self.foregroundColor = fg_color
        self.backgroundColor = bg_color
    }
    #endif
    
    #if os(iOS)
    init(foregroundColor fg_color: UIColor, backgroundColor bg_color: UIColor) {
    self.foregroundColor = fg_color
    self.backgroundColor = bg_color
    }
    #endif

}

// MARK: - ActionLogTextViewDestination
/// - A log destination that outputs log details to a NSTextView
open class ActionLogTextViewDestination : ActionLogDestinationProtocol, CustomDebugStringConvertible {
    /// the TextView
    #if os(OSX)
    open var textView: NSTextView
    #endif
    
    #if os(iOS)
    public var textView: UITextView
    #endif
    
    open var identifier: String
    
    open var showDateAndTime: Bool = true
    open var showLogLevel: Bool = true
    open var showFileName: Bool = true
    open var showLineNumber: Bool = true
    open var showFuncName: Bool = true
    open var dateFormatter = ActionLogger.dateFormatterGER
    open var outputLogLevel: ActionLogger.LogLevel = .allLevels
    
    // color enhancement
    var colorProfiles = Dictionary<ActionLogger.LogLevel,ActionLogTextViewColorProfile>()
    
    
    #if os(OSX)
    init(identifier: String = "", textView: NSTextView) {
        self.identifier = identifier
        self.textView = textView
        
        // color enhancement
        if isEnabledForColor() {
            // setting default color values
            self.colorProfiles[.allLevels]    = ActionLogTextViewColorProfile(foregroundColor: NSColor.white,backgroundColor: NSColor.white)
            self.colorProfiles[.messageOnly]  = ActionLogTextViewColorProfile(foregroundColor: NSColor.lightGray,backgroundColor: NSColor.white)
            self.colorProfiles[.comment]      = ActionLogTextViewColorProfile(foregroundColor: NSColor.gray,backgroundColor: NSColor.white)
            self.colorProfiles[.verbose]      = ActionLogTextViewColorProfile(foregroundColor: NSColor.darkGray,backgroundColor: NSColor.white)
            self.colorProfiles[.info]         = ActionLogTextViewColorProfile(foregroundColor: NSColor.blue,backgroundColor: NSColor.white)
            self.colorProfiles[.debug]        = ActionLogTextViewColorProfile(foregroundColor: NSColor.green,backgroundColor: NSColor.white)
            self.colorProfiles[.warning]      = ActionLogTextViewColorProfile(foregroundColor: NSColor.orange,backgroundColor: NSColor.white)
            self.colorProfiles[.error]        = ActionLogTextViewColorProfile(foregroundColor: NSColor.red,backgroundColor: NSColor.white)
            self.colorProfiles[.severe]       = ActionLogTextViewColorProfile(foregroundColor: NSColor.magenta,backgroundColor: NSColor.white)
        }
    }
    #endif
    
    #if os(iOS)
    init(identifier: String = "", textView: UITextView) {
        self.identifier = identifier
        self.textView = textView
        //        self.textView.delegate = self as! NSTextViewDelegate

        // color enhancement
        if isEnabledForColor() {
        // setting default color values
    
        #if os(iOS)
        self.colorProfiles[.allLevels]    = ActionLogTextViewColorProfile(foregroundColor: UIColor.white,backgroundColor: UIColor.white)
        self.colorProfiles[.messageOnly]  = ActionLogTextViewColorProfile(foregroundColor: UIColor.lightGray,backgroundColor: UIColor.white)
        self.colorProfiles[.comment]      = ActionLogTextViewColorProfile(foregroundColor: UIColor.gray,backgroundColor: UIColor.white)
        self.colorProfiles[.verbose]      = ActionLogTextViewColorProfile(foregroundColor: UIColor.darkGray,backgroundColor: UIColor.white)
        self.colorProfiles[.info]         = ActionLogTextViewColorProfile(foregroundColor: UIColor.blue,backgroundColor: UIColor.white)
        self.colorProfiles[.debug]        = ActionLogTextViewColorProfile(foregroundColor: UIColor.green,backgroundColor: UIColor.white)
        self.colorProfiles[.warning]      = ActionLogTextViewColorProfile(foregroundColor: UIColor.orange,backgroundColor: UIColor.white)
        self.colorProfiles[.error]        = ActionLogTextViewColorProfile(foregroundColor: UIColor.red,backgroundColor: UIColor.white)
        self.colorProfiles[.severe]       = ActionLogTextViewColorProfile(foregroundColor: UIColor.magenta,backgroundColor: UIColor.white)
        #endif
        }
    }
    #endif
    
    required public init(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    open func setDefaultLogLevelColors() {
        // color enhancement
        if isEnabledForColor() {
            // setting default color values
            #if os(OSX)
                self.colorProfiles[.allLevels]    = ActionLogTextViewColorProfile(foregroundColor: NSColor.white,backgroundColor: NSColor.white)
                self.colorProfiles[.messageOnly]  = ActionLogTextViewColorProfile(foregroundColor: NSColor.lightGray,backgroundColor: NSColor.white)
                self.colorProfiles[.comment]      = ActionLogTextViewColorProfile(foregroundColor: NSColor.gray,backgroundColor: NSColor.white)
                self.colorProfiles[.verbose]      = ActionLogTextViewColorProfile(foregroundColor: NSColor.darkGray,backgroundColor: NSColor.white)
                self.colorProfiles[.info]         = ActionLogTextViewColorProfile(foregroundColor: NSColor.blue,backgroundColor: NSColor.white)
                self.colorProfiles[.debug]        = ActionLogTextViewColorProfile(foregroundColor: NSColor.green,backgroundColor: NSColor.white)
                self.colorProfiles[.warning]      = ActionLogTextViewColorProfile(foregroundColor: NSColor.orange,backgroundColor: NSColor.white)
                self.colorProfiles[.error]        = ActionLogTextViewColorProfile(foregroundColor: NSColor.red,backgroundColor: NSColor.white)
                self.colorProfiles[.severe]       = ActionLogTextViewColorProfile(foregroundColor: NSColor.magenta,backgroundColor: NSColor.white)
            #endif
            
            #if os(iOS)
                self.colorProfiles[.allLevels]    = ActionLogTextViewColorProfile(foregroundColor: UIColor.white,backgroundColor: UIColor.white)
                self.colorProfiles[.messageOnly]  = ActionLogTextViewColorProfile(foregroundColor: UIColor.lightGray,backgroundColor: UIColor.white)
                self.colorProfiles[.comment]      = ActionLogTextViewColorProfile(foregroundColor: UIColor.gray,backgroundColor: UIColor.white)
                self.colorProfiles[.verbose]      = ActionLogTextViewColorProfile(foregroundColor: UIColor.darkGray,backgroundColor: UIColor.white)
                self.colorProfiles[.info]         = ActionLogTextViewColorProfile(foregroundColor: UIColor.blue,backgroundColor: UIColor.white)
                self.colorProfiles[.debug]        = ActionLogTextViewColorProfile(foregroundColor: UIColor.green,backgroundColor: UIColor.white)
                self.colorProfiles[.warning]      = ActionLogTextViewColorProfile(foregroundColor: UIColor.orange,backgroundColor: UIColor.white)
                self.colorProfiles[.error]        = ActionLogTextViewColorProfile(foregroundColor: UIColor.red,backgroundColor: UIColor.white)
                self.colorProfiles[.severe]       = ActionLogTextViewColorProfile(foregroundColor: UIColor.magenta,backgroundColor: UIColor.white)
            #endif
        }
    }
    
    open func processLogDetails(_ logDetails: ActionLogDetails, withFileLineFunctionInfo: Bool = true) {
        let fullLogMessage = preProcessLogDetails(logDetails, showDateAndTime: showDateAndTime, showLogLevel: showLogLevel, showFileName: showFileName, showLineNumber: showLineNumber, showFuncName: showFuncName, dateFormatter: dateFormatter, withFileLineFunctionInfo: withFileLineFunctionInfo)
        
        let textViewMessage = NSMutableAttributedString(string: fullLogMessage)
        let messageRange = NSRange.init(location: 0, length: textViewMessage.length)
        
        // color enhancement
        if let cp = self.colorProfiles[logDetails.logLevel] {
            // set fore- and backgroundColor
            textViewMessage.addAttribute(NSAttributedString.Key.foregroundColor, value: cp.foregroundColor, range: messageRange)
            textViewMessage.addAttribute(NSAttributedString.Key.backgroundColor, value: cp.backgroundColor, range: messageRange)
        }
        
        // print it, only if the LogDestination should print this
        if isEnabledForLogLevel(logDetails.logLevel) {
            #if os(OSX)
                textView.textStorage!.append(textViewMessage)
            #endif

            #if os(iOS)
                textView.textStorage.append(textViewMessage)
            #endif
        }
    }
    
    // MARK: - Misc methods
    open func isEnabledForLogLevel (_ logLevel: ActionLogger.LogLevel) -> Bool {
        return logLevel >= self.outputLogLevel
    }
    
    // color enhancement
    open func isEnabledForColor() -> Bool {
        // is allways enabled for this Destination
        return true
    }
    
    open func hasFile() -> Bool {
        return false
    }
    
    // MARK: - DebugPrintable
    open var debugDescription: String {
        get {
            return "ActionLogTextViewDestination: \(identifier) - LogLevel: \(outputLogLevel.description()) showLogLevel: \(showLogLevel) showFileName: \(showFileName) showLineNumber: \(showLineNumber) date & time format: \(String(describing: dateFormatter.dateFormat))"
        }
    }
    
    // color enhancement
    open func setLogColors(foregroundRed fg_red: UInt8 = 0, foregroundGreen fg_green: UInt8 = 0, foregroundBlue fg_blue: UInt8 = 0, backgroundRed bg_red: UInt8 = 255, backgroundGreen bg_green: UInt8 = 255, backgroundBlue bg_blue: UInt8 = 255, forLogLevel logLevel: ActionLogger.LogLevel) {
        if var cp = self.colorProfiles[logLevel] {
            let fg_color = CIColor(red: CGFloat(fg_red)/255.0, green: CGFloat(fg_green)/255.0, blue: CGFloat(fg_blue)/255.0)
            #if os(OSX)
                cp.foregroundColor = NSColor(ciColor: fg_color)
            #endif
            #if os(iOS)
                cp.foregroundColor = UIColor(ciColor: fg_color)
            #endif
            let bg_color = CIColor(red: CGFloat(bg_red)/255.0, green: CGFloat(bg_green)/255.0, blue: CGFloat(bg_blue)/255.0)
            #if os(OSX)
                cp.backgroundColor = NSColor(ciColor: bg_color)
            #endif
            #if os(iOS)
                cp.backgroundColor = UIColor(ciColor: bg_color)
            #endif
            self.colorProfiles[logLevel] = cp
        }
    }
    
    open func resetAllLogColors() {
        for (logLevel, var colorProfile) in colorProfiles {
            
            #if os(OSX)
                colorProfile.foregroundColor = NSColor.black
                colorProfile.backgroundColor = NSColor.white
            #endif
            
            #if os(iOS)
                colorProfile.foregroundColor = UIColor.black
                colorProfile.backgroundColor = UIColor.white
            #endif
            
            self.colorProfiles[logLevel] = colorProfile
        }
    }
    
    #if os(OSX)
    open func setForegroundColor(_ color: NSColor, forLogLevel logLevel: ActionLogger.LogLevel) {
        if var cp = self.colorProfiles[logLevel] {
            cp.foregroundColor = color
            self.colorProfiles[logLevel] = cp
        }
    }
    
    open func setBackgroundColor(_ color: NSColor, forLogLevel logLevel: ActionLogger.LogLevel) {
        if var cp = self.colorProfiles[logLevel] {
            cp.backgroundColor = color
            self.colorProfiles[logLevel] = cp
        }
    }
    #endif
    
    #if os(iOS)
    public func setForegroundColor(color: UIColor, forLogLevel logLevel: ActionLogger.LogLevel) {
        if var cp = self.colorProfiles[logLevel] {
        cp.foregroundColor = color
        self.colorProfiles[logLevel] = cp
        }
    }
    
    public func setBackgroundColor(color: UIColor, forLogLevel logLevel: ActionLogger.LogLevel) {
        if var cp = self.colorProfiles[logLevel] {
        cp.backgroundColor = color
        self.colorProfiles[logLevel] = cp
        }
    }
    #endif
}

// MARK: - ActionLogXcodeConsoleSimulationDestination
/// - A standard log destination that outputs log details to the console
open class ActionLogXcodeConsoleSimulationDestination : ActionLogDestinationProtocol, CustomDebugStringConvertible {
    /// the TextView
    #if os(OSX)
    open var textView: NSTextView
    #endif
    
    #if os(iOS)
    open var textView: UITextView
    #endif
    
    //    var owner: ActionLogger
    open var identifier: String
    
    open var showDateAndTime: Bool = true
    open var showLogLevel: Bool = true
    open var showFileName: Bool = true
    open var showLineNumber: Bool = true
    open var showFuncName: Bool = true
    open var dateFormatter = ActionLogger.dateFormatterGER
    open var outputLogLevel: ActionLogger.LogLevel = .allLevels
    
    // color enhancement
    var colorProfiles = Dictionary<ActionLogger.LogLevel,ActionLogXcodeColorProfile>()
    
#if os(OSX)
    init(identifier: String = "", textView: NSTextView) {
            self.identifier = identifier
            self.textView = textView
            
        // color enhancement
        if isEnabledForColor() {
            // setting default color values
            #if os(OSX)
                self.colorProfiles[.allLevels]    = ActionLogXcodeColorProfile(foregroundColor: NSColor.white,backgroundColor: NSColor.white)
                self.colorProfiles[.messageOnly]  = ActionLogXcodeColorProfile(foregroundColor: NSColor.lightGray,backgroundColor: NSColor.white)
                self.colorProfiles[.comment]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.gray,backgroundColor: NSColor.white)
                self.colorProfiles[.verbose]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.darkGray,backgroundColor: NSColor.white)
                self.colorProfiles[.info]         = ActionLogXcodeColorProfile(foregroundColor: NSColor.blue,backgroundColor: NSColor.white)
                self.colorProfiles[.debug]        = ActionLogXcodeColorProfile(foregroundColor: NSColor.green,backgroundColor: NSColor.white)
                self.colorProfiles[.warning]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.orange,backgroundColor: NSColor.white)
                self.colorProfiles[.error]        = ActionLogXcodeColorProfile(foregroundColor: NSColor.red,backgroundColor: NSColor.white)
                self.colorProfiles[.severe]       = ActionLogXcodeColorProfile(foregroundColor: NSColor.magenta,backgroundColor: NSColor.white)
            #endif
            
            #if os(iOS)
                self.colorProfiles[.AllLevels]    = ActionLogXcodeColorProfile(foregroundColor: UIColor.white,backgroundColor: UIColor.white)
                self.colorProfiles[.MessageOnly]  = ActionLogXcodeColorProfile(foregroundColor: UIColor.lightGray,backgroundColor: UIColor.white)
                self.colorProfiles[.Comment]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.gray,backgroundColor: UIColor.white)
                self.colorProfiles[.Verbose]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.darkGray,backgroundColor: UIColor.white)
                self.colorProfiles[.Info]         = ActionLogXcodeColorProfile(foregroundColor: UIColor.blue,backgroundColor: UIColor.white)
                self.colorProfiles[.Debug]        = ActionLogXcodeColorProfile(foregroundColor: UIColor.green,backgroundColor: UIColor.white)
                self.colorProfiles[.Warning]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.orange,backgroundColor: UIColor.white)
                self.colorProfiles[.Error]        = ActionLogXcodeColorProfile(foregroundColor: UIColor.red,backgroundColor: UIColor.white)
                self.colorProfiles[.Severe]       = ActionLogXcodeColorProfile(foregroundColor: UIColor.magenta,backgroundColor: UIColor.white)
            #endif
        }
    }
#endif

#if os(iOS)
    init(identifier: String = "", textView: UITextView) {
        self.identifier = identifier
        self.textView = textView
        
        // color enhancement
        if isEnabledForColor() {
            // setting default color values
            #if os(OSX)
                self.colorProfiles[.allLevels]    = ActionLogXcodeColorProfile(foregroundColor: NSColor.white,backgroundColor: NSColor.white)
                self.colorProfiles[.messageOnly]  = ActionLogXcodeColorProfile(foregroundColor: NSColor.lightGray,backgroundColor: NSColor.white)
                self.colorProfiles[.comment]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.gray,backgroundColor: NSColor.white)
                self.colorProfiles[.verbose]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.darkGray,backgroundColor: NSColor.white)
                self.colorProfiles[.info]         = ActionLogXcodeColorProfile(foregroundColor: NSColor.blue,backgroundColor: NSColor.white)
                self.colorProfiles[.debug]        = ActionLogXcodeColorProfile(foregroundColor: NSColor.green,backgroundColor: NSColor.white)
                self.colorProfiles[.warning]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.orange,backgroundColor: NSColor.white)
                self.colorProfiles[.error]        = ActionLogXcodeColorProfile(foregroundColor: NSColor.red,backgroundColor: NSColor.white)
                self.colorProfiles[.severe]       = ActionLogXcodeColorProfile(foregroundColor: NSColor.magenta,backgroundColor: NSColor.white)
            #endif
            
            #if os(iOS)
                self.colorProfiles[.allLevels]    = ActionLogXcodeColorProfile(foregroundColor: UIColor.white,backgroundColor: UIColor.white)
                self.colorProfiles[.messageOnly]  = ActionLogXcodeColorProfile(foregroundColor: UIColor.lightGray,backgroundColor: UIColor.white)
                self.colorProfiles[.comment]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.gray,backgroundColor: UIColor.white)
                self.colorProfiles[.verbose]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.darkGray,backgroundColor: UIColor.white)
                self.colorProfiles[.info]         = ActionLogXcodeColorProfile(foregroundColor: UIColor.blue,backgroundColor: UIColor.white)
                self.colorProfiles[.debug]        = ActionLogXcodeColorProfile(foregroundColor: UIColor.green,backgroundColor: UIColor.white)
                self.colorProfiles[.warning]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.orange,backgroundColor: UIColor.white)
                self.colorProfiles[.error]        = ActionLogXcodeColorProfile(foregroundColor: UIColor.red,backgroundColor: UIColor.white)
                self.colorProfiles[.severe]       = ActionLogXcodeColorProfile(foregroundColor: UIColor.magenta,backgroundColor: UIColor.white)
            #endif
        }
    }
#endif


    open func setDefaultLogLevelColors() {
        // color enhancement
        if isEnabledForColor() {
            // setting default color values
            #if os(OSX)
                self.colorProfiles[.allLevels]    = ActionLogXcodeColorProfile(foregroundColor: NSColor.white,backgroundColor: NSColor.white)
                self.colorProfiles[.messageOnly]  = ActionLogXcodeColorProfile(foregroundColor: NSColor.lightGray,backgroundColor: NSColor.white)
                self.colorProfiles[.comment]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.gray,backgroundColor: NSColor.white)
                self.colorProfiles[.verbose]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.darkGray,backgroundColor: NSColor.white)
                self.colorProfiles[.info]         = ActionLogXcodeColorProfile(foregroundColor: NSColor.blue,backgroundColor: NSColor.white)
                self.colorProfiles[.debug]        = ActionLogXcodeColorProfile(foregroundColor: NSColor.green,backgroundColor: NSColor.white)
                self.colorProfiles[.warning]      = ActionLogXcodeColorProfile(foregroundColor: NSColor.orange,backgroundColor: NSColor.white)
                self.colorProfiles[.error]        = ActionLogXcodeColorProfile(foregroundColor: NSColor.red,backgroundColor: NSColor.white)
                self.colorProfiles[.severe]       = ActionLogXcodeColorProfile(foregroundColor: NSColor.magenta,backgroundColor: NSColor.white)
            #endif
            
            #if os(iOS)
                self.colorProfiles[.allLevels]    = ActionLogXcodeColorProfile(foregroundColor: UIColor.white,backgroundColor: UIColor.white)
                self.colorProfiles[.messageOnly]  = ActionLogXcodeColorProfile(foregroundColor: UIColor.lightGray,backgroundColor: UIColor.white)
                self.colorProfiles[.comment]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.gray,backgroundColor: UIColor.white)
                self.colorProfiles[.verbose]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.darkGray,backgroundColor: UIColor.white)
                self.colorProfiles[.info]         = ActionLogXcodeColorProfile(foregroundColor: UIColor.blue,backgroundColor: UIColor.white)
                self.colorProfiles[.debug]        = ActionLogXcodeColorProfile(foregroundColor: UIColor.green,backgroundColor: UIColor.white)
                self.colorProfiles[.warning]      = ActionLogXcodeColorProfile(foregroundColor: UIColor.orange,backgroundColor: UIColor.white)
                self.colorProfiles[.error]        = ActionLogXcodeColorProfile(foregroundColor: UIColor.red,backgroundColor: UIColor.white)
                self.colorProfiles[.severe]       = ActionLogXcodeColorProfile(foregroundColor: UIColor.magenta,backgroundColor: UIColor.white)
            #endif
        }
    }
    
    open func processLogDetails(_ logDetails: ActionLogDetails, withFileLineFunctionInfo: Bool = true) {
        var fullLogMessage = preProcessLogDetails(logDetails, showDateAndTime: showDateAndTime, showLogLevel: showLogLevel, showFileName: showFileName, showLineNumber: showLineNumber, showFuncName: showFuncName, dateFormatter: dateFormatter, withFileLineFunctionInfo: withFileLineFunctionInfo)
        
        // color enhancement
        if let cp = self.colorProfiles[logDetails.logLevel] {
            fullLogMessage = cp.fg_code + cp.bg_code + fullLogMessage + cp.all_reset_code
        }
        
        
        let textViewMessage = NSMutableAttributedString(string: fullLogMessage)
//        let messageRange = NSRange.init(location: 0, length: textViewMessage.length)
        
        // print it, only if the LogDestination should print this
        if isEnabledForLogLevel(logDetails.logLevel) {
            #if os(OSX)
                textView.textStorage!.append(textViewMessage)
            #endif
            
        }
    }
    
    // MARK: - Misc methods
    open func isEnabledForLogLevel (_ logLevel: ActionLogger.LogLevel) -> Bool {
        return logLevel >= self.outputLogLevel
    }
    
    // color enhancement
    open func isEnabledForColor() -> Bool {
        return true
//        let dict = NSProcessInfo.processInfo().environment
//        
//        if let env = dict["XcodeColors"] as String! {
//            return env == "YES"
//        }
//        return false
    }
    
    open func hasFile() -> Bool {
        return false
    }
    
    // MARK: - DebugPrintable
    open var debugDescription: String {
        get {
            return "ActionLogConsoleDestination: \(identifier) - LogLevel: \(outputLogLevel.description()) showLogLevel: \(showLogLevel) showFileName: \(showFileName) showLineNumber: \(showLineNumber) date & time format: \(String(describing: dateFormatter.dateFormat))"
        }
    }
    
    // color enhancement
    // MARK: - color enhancement
    open func setLogColors(foregroundRed fg_red: UInt8 = 0, foregroundGreen fg_green: UInt8 = 0, foregroundBlue fg_blue: UInt8 = 0, backgroundRed bg_red: UInt8 = 255, backgroundGreen bg_green: UInt8 = 255, backgroundBlue bg_blue: UInt8 = 255, forLogLevel logLevel: ActionLogger.LogLevel) {
        if var cp = self.colorProfiles[logLevel] {
            cp.fg_Red = fg_red; cp.fg_Green = fg_green; cp.fg_Blue = fg_blue
            cp.bg_Red = bg_red; cp.bg_Green = bg_green; cp.bg_Blue = bg_blue
            cp.buildForegroundCode()
            cp.buildBackgroundCode()
            self.colorProfiles[logLevel] = cp
        }
    }
    
    /*! using setLogColors:
    setLogColor(foregroundRed:0,foregroundGreen:0,foregroundBlue:0,forLogLevel:.Verbose)        means: resetForegroundColor of logLevel .Verbose to black
    setLogColor(backgroundRed:255,backgroundGreen:255,backgroundBlue:255,forLogLevel:.Debug)    means: resetBackgroundColor of logLevel .Debug   to white
    */
    
    open func resetAllLogColors() {
        for (logLevel, var colorProfile) in colorProfiles {
            colorProfile.fg_Red = 0; colorProfile.fg_Green = 0; colorProfile.fg_Blue = 0
            colorProfile.bg_Red = 255; colorProfile.bg_Green = 255; colorProfile.bg_Blue = 255
            colorProfile.buildForegroundCode()
            colorProfile.buildBackgroundCode()
            self.colorProfiles[logLevel] = colorProfile
        }
    }
    
    #if os(OSX)
    
    open func setForegroundColor(_ color: NSColor, forLogLevel logLevel: ActionLogger.LogLevel) {
        var fg_red: CGFloat = 0, fg_green: CGFloat = 0, fg_blue: CGFloat = 0
        var alpha: CGFloat = 0
        let c = color.usingColorSpace(NSColorSpace.deviceRGB)
        c!.getRed(&fg_red, green: &fg_green, blue: &fg_blue, alpha: &alpha)
        
        if var cp = self.colorProfiles[logLevel] {
            cp.fg_Red = UInt8(fg_red * 255.0); cp.fg_Green = UInt8(fg_green * 255.0); cp.fg_Blue = UInt8(fg_blue * 255.0)
            cp.buildForegroundCode()
            cp.buildBackgroundCode()
            self.colorProfiles[logLevel] = cp
        }
    }
    
    
    open func setBackgroundColor(_ color: NSColor, forLogLevel logLevel: ActionLogger.LogLevel) {
        var bg_red: CGFloat = 0, bg_green: CGFloat = 0, bg_blue: CGFloat = 0
        var alpha: CGFloat = 0
        
        let c = color.usingColorSpace(NSColorSpace.deviceRGB)
        c!.getRed(&bg_red, green: &bg_green, blue: &bg_blue, alpha: &alpha)
        
        if var cp = self.colorProfiles[logLevel] {
            cp.bg_Red = UInt8(bg_red * 255.0); cp.bg_Green = UInt8(bg_green * 255.0); cp.bg_Blue = UInt8(bg_blue * 255.0)
            cp.buildForegroundCode()
            cp.buildBackgroundCode()
            self.colorProfiles[logLevel] = cp
        }
    }
    
    #endif
    
    #if os(iOS)
    
    public func setForegroundColor(color: UIColor, forLogLevel logLevel: ActionLogger.LogLevel) {
    var fg_red: CGFloat = 0, fg_green: CGFloat = 0, fg_blue: CGFloat = 0
    var alpha: CGFloat = 0
    color.getRed(&fg_red, green: &fg_green, blue: &fg_blue, alpha: &alpha)
    
    if var cp = self.colorProfiles[logLevel] {
    cp.fg_Red = UInt8(fg_red * 255.0); cp.fg_Green = UInt8(fg_green * 255.0); cp.fg_Blue = UInt8(fg_blue * 255.0)
    cp.buildForegroundCode()
    cp.buildBackgroundCode()
    self.colorProfiles[logLevel] = cp
    }
    }
    
    
    public func setBackgroundColor(color: UIColor, forLogLevel logLevel: ActionLogger.LogLevel) {
    var bg_red: CGFloat = 0, bg_green: CGFloat = 0, bg_blue: CGFloat = 0
    var alpha: CGFloat = 0
    
    color.getRed(&bg_red, green: &bg_green, blue: &bg_blue, alpha: &alpha)
    
    if var cp = self.colorProfiles[logLevel] {
    cp.bg_Red = UInt8(bg_red * 255.0); cp.bg_Green = UInt8(bg_green * 255.0); cp.bg_Blue = UInt8(bg_blue * 255.0)
    cp.buildForegroundCode()
    cp.buildBackgroundCode()
    self.colorProfiles[logLevel] = cp
    }
    }
    
    #endif
}



// some usefull extensions from
// NSScanner+Swift.swift
// A set of Swift-idiomatic methods for NSScanner
//
// (c) 2015 Nate Cook, licensed under the MIT license

extension Scanner {
    
    /// Returns a string, scanned until a character from a given character set are encountered, or the remainder of the scanner's string. Returns `nil` if the scanner is already `atEnd`.
    func scanUpToCharactersFromSet(_ set: CharacterSet) -> String? {
        var value: NSString? = ""
        if scanUpToCharacters(from: set, into: &value),
            let value = value as String? {
                return value
        }
        return nil
    }
    
    /// Returns an Int if scanned, or `nil` if not found.
    func scanInteger() -> Int? {
        var value = 0
        if scanInt(&value) {
            return value
        }
        return nil
    }
    
}

