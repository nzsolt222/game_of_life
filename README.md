##Summary

This code is an implementation in C++11 of Game of Life.

## Usage
To compile and run (if you're on a GNU/Linux system):
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./game_of_life
```
In one line:
```bash
$ mkdir build && cd build && cmake .. && make && ./game_of_life
```
or use `cmake-gui`.

To compile and run (if you're on a Windows system):

1. use cmake-gui and Visual Studio 2013
2. run the compiled binrary version under bin/windows folder

##Controls

| Key             | Action                                      |
| ----------------| ------------------------------------------- |
| n               | Calculate the next iteration                |
| r               | Clear the grid                              |
| b               | Toggle boundary option                      |
| f               | Toggle fullscreen mode                      |
| KEYPAD+         | Speed up the iteration                      |
| KEYPAD-         | Slow down the iteration                     |
| SPACE           | Play/Stop the iteration                     |
| ESC             | Exit                                        |


| Mouse           | Action                                      |
| ----------------| ------------------------------------------- |
| Scroll          | Use the next/previous brush                 |
| CTRL+Scroll     | Resize the grid                             |
| Left Button     | Erase or Paint (depends on where click)     |

## Screenshots

![Alt text](example.gif?raw=true "Game of Life")


##License

This software is released under the MIT License, see license.txt
