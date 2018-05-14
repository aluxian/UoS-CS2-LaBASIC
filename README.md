# LaBASIC

## Description

Now you can turn your LaFortuna into an 80s portable computer! What? It's already dumb like an 80s computer? Ah, ok, 70s! Or whenever BASIC was invented.

In case you haven't guessed, LaBASIC is a BASIC interpreter for the LaFortuna board. The reason I decided to create an interpreter is to make programming on the LaFortuna easier. I know nothing can match C, but sometimes you want to hack something quickly and a higher level language is perfect for this. Whether BASIC is the appropriate choice for an interpreted language will be left up for debate to the reader.

## Commands

Working:

```
PRINT 'some string'         # print any string
PRINT ENDL                  # print a new line
PRINT $varname              # print the value of a variable
LED ON                      # turn the LED on
LED OFF                     # turn the LED off
PAUSE 123                   # pause for 123 ms
LET varname = 42            # assign a value to a variable
GOTO 5                      # skip to the instruction on line 5 (1-based index)
READ input                  # read a number with the rotary encoder and store it in a variable
```

To be implemented:

* PINMODE 1, 1
* variable = `<expr>`
* IF `<expr>` THEN cmd
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

Based on FortunaOS which includes:

* The RIOS Scheduler
* Peter Dannegger’s Code for the Rotary Encoder
* Peter Dannegger’s Code for Switch debouncing
* Steve Gunn’s display driver
* ChanN’s FAT File System

## Usage

* Install the AVR toolchain and `make`
* Clone the GitHub repository
* Edit `main.c` and modify the hardcoded LaBASIC program
* Connect your LaFortuna board to your computer
* Run `make` to flash your board

## License

[GNU GPL](LICENSE)

Copyright (c) 2018 Alexandru Rosianu
