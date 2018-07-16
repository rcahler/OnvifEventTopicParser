# OnvifEventTopicParser
## Explanation

## Compilation
Open the solution in Visual Studio and build the required versions. If you wish to build the library from scratch, follow these instructions, https://github.com/rcahler/Onvif-Library. The Utility has been built and tested using Windows 10 and Visual Studio 2017.

## Usage
This utility is a command line tool which connects to an Onvif camera and returns its event triggers in json that can be read by the ExacqVision client. As of 7/9/2018 the client will return the name of all events on all onvif cameras, but is unable to get all of the data about the specific triggers on all cameras.

The json data can be sent to a file, but if a file is not specified then the data will be printed to the command line.

### Commands:
        -h: Optional, Brings up all avaliable commands and usage information
        -v: Optional, Verbose output
        -u: Req, Username of the camera whose topics are being gotten
        -p: Optional, Password of the camera whose topics are being gotten, requiered on most cameras but not all
        -ip: Req, IP address of the camera whose topics are being gotten
        -f: Optional, Filepath to the json file (Whether it exists or needs to be created) where the data will be added. If no file is given, the data will be sent to the command line
        -o: Optional, Place at the end of the input parameters uses the built in order of [Utility.exe username password ip /filepath/]
