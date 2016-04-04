# minesweeper_cpp

Basic and Ugly Minesweeper done in C++

## Install

Just use the Makefile. It compiles on Linux and Mac OSX at the moment.
`$> make minesweeper; ./minesweeper SIZE_X SIZE_Y NB_MINES`

You *MUST* provides parameters or it won't work at the moment.

## Requirements

 - SFML 2.X
 - [sftools](http://mantognini.github.io/sftools/)
 - Boost (includes only)

## How To Play

 - Left click to discover a Cell.
 - Right click on a Cell to Flag it (mark that there is a mine here).
 - Right click on a discovered Cell with enough Flags near to complete the block.
 - Use <kbd>CTRL</kbd> to have a better visibility of the blocks.
 - Press <kbd>R</kbd> to restart
 - ECHAP or <kbd>CTRL</kbd> + <kbd>C</kbd> to Quit.

## Improvements

 - [X] Win / Loose Display
 - [X] Sprites for Mines and Flags
 - [X] Timer
 - [X] Display Header
 - [ ] Leaderboard
 - [ ] Default Parameters (easy / medium / advanced)
 - [ ] *(?)* Online leaderboard using Parse.com

## Credits

 - [IconFinder](https://www.iconfinder.com) for the Flag Sprite.
 - [IGN](http://microsites.ign.com/rayman_timeline/) for the Mine Sprite.
