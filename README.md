# OnvifEventTopicParser
## Explanation

## Compilation
Open the solution in Visual Studio and build the required versions. If you wish to build the library from scratch, follow these instructions, https://github.com/rcahler/Onvif-Library. The Utility has been built and tested using Windows 10 and Visual Studio 2017.

## Usage
This utility is a command line tool which connects to an Onvif camera and returns its event triggers in json that can be read by the ExacqVision client. As of 7/9/2018 the client will return the name of all events on all onvif cameras, but is unable to get all of the data about the specific triggers on all cameras.

The json data can be sent to a file, but if a file is not specified then the data will be printed to the command line.

`Utility.exe [camera username] [camera password] [camera ip address] [optional: file path to send data]`
