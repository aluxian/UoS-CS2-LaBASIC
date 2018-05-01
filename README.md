# LaBASIC

## Description

Now you can turn your LaFortuna into an 80s portable computer! What? It's already dumb like an 80s computer? Ah, ok, 70s! Or whenever BASIC was invented.

In case you haven't guessed, LaBASIC is a BASIC interpreter for the LaFortuna board. And when I say "BASIC", I literally mean basic because I haven't had time to implement that many commands.

The reason I decided to create an interpreter is to make programming on the LaFortuna easier. I know nothing can match C, but sometimes you want to hack something quickly and a higher level language is perfect for this.

## Commands

Working:

* PRINT "some string"
* PRINT ENDL

Almost working:

* LED_ON
* LED_OFF
* PIN 1, 1
* PINMODE 1, 1
* PAUSE 500

To be implemented:

* LET variable = <expr>
* variable = <expr>
* GOTO lineNumber
* IF <expr> THEN cmd
* FOR variable = start TO end STEP step
* LOAD "filename", SAVE "filename, DIR, DELETE "filename" 
* *and other commands from BASIC*

## Future plans

* Add support for mathematical expressions by implementing a better parser
* Add built-in APIs to allow LaBASIC programmers to access the file system, serial interface, etc
* Create an online documentation website with code examples and command reference
* Allow running LaBASIC programs from the filesystem instead of hardcoding them

## Sources

The code is available on GitHub: https://github.com/aluxian/UoS-CS2-LaBASIC

## Credits

I got the idea from Robin Edwards' ArduinoBASIC interpreter:

* https://github.com/robinhedwards/ArduinoBASIC

## Usage

* Install the AVR toolchain and `make`
* Clone the GitHub repository
* Edit `main.c` and modify the hardcoded LaBASIC program
* Connect your LaFortuna board to your computer
* Run `make` to flash your board

## License

[MIT License](LICENSE)

Copyright (c) 2018 Alexandru Rosianu
