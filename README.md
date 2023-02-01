# Checkers

The project is about defining a class for the checkers game (with slightly simplified rules). The game is between two players: player1 (represented by x) and player2 (represented by o). The game takes place on an 8x8 chessboard. At any moment, pieces are only on alternate cells. Pieces on the chessboard are of two types:
- "Pawn": These are the pieces initially available to each player (12 pieces per player) o/x
- "Queen": These are pieces with special moves. When a player's pawn reaches the opposite row (row 7 for player1 or row 0 for player2), it automatically becomes a Queen. O/X

## Features:

- Player1 pawns can only capture in the up/left or up/right direction. 
- Conversely, player2 pawns can only capture in the down/left or down/right direction.
- Pawns can only capture other pawns (not Queens).
- Both player's Queens can capture in any direction (up/left, up/right, down/left, down/right).
- Queens can capture both pawns and Queens.
- If a pawn reaches the opposite row, it is promoted to a Queen.

## Gameplay

The code can play against itself using the [minimax AI algorithm](https://github.com/andreramolivaz/Checkers/blob/77c635ec66a2123d45b7b4e445b28d11db4f1f44/player2.cpp#L361)/random move or against other code that implements different strategies against each other in a tournament. The class Player is implemented whose declaration is contained in the player.hpp header.

### A game will take place as follows:
At each moment of the game, three executables are running: two binaries compiled from "play.cpp" with your players (player1 and player2 - the turn is assigned randomly by us), and the binary compiled from "verify.cpp" (the verifier, containing our correct implementation of Player).
- Initially, the verifier creates an initial chessboard and saves it in "board_1.txt".
- Player1 loads "board_1.txt", makes a move, and saves the updated chessboard in "board_2.txt".
- Player2 loads "board_2.txt", makes a move, and saves the updated chessboard in "board_3.txt".
- The verifier loads "board_3.txt" and checks the legality of the moves made by the players.
T- he process repeats until one player wins or the game is a draw.

## Getting Started

To play Checkers, follow these steps:

1. Clone or download this repository
2. Compile the source code with a C compiler using the [CMakeLists.txt file](https://github.com/andreramolivaz/Checkers/blob/77c635ec66a2123d45b7b4e445b28d11db4f1f44/CMakeLists.txt)



<div align="center">
 <table>
   <tr>
<td><img src="http://vdapoi.altervista.org/ezgif.com-gif-maker-6.gif" width="250" height="200" /><br>
</td> 
   </tr>
  </table>
</div>


## Requirements

- `#include <iostream>` The iostream library provides basic input/output functionality, including the standard input and output streams cin, cout, and cerr.
- `#include <fstream>` The fstream library provides support for reading and writing to files using the ifstream and ofstream classes.
- `#include <string>` The string library provides a string class that can be used to store and manipulate strings.
- `#include <sstream>` The sstream library provides support for stringstream, which can be used to perform input and output operations on strings.
- `#include <cassert>` The cassert library provides the assert macro, which can be used to perform debugging checks in the code, such as checking if a pointer is null before dereferencing it.
- `#include <thread>` The thread library provides support for creating and managing threads, which are independent sequences of execution within a single program.


## Project Structure

    Checkers              
    ├── play.cpp                   
    ├── player.hpp
    ├── player.cpp
    ├── verify.cpp
    ├── board_0.txt
    ├── ...
    ├── board_n.txt
    └── CMakeList.txt

## Documentation

You can find the detailed documentation of the project [here](https://andreramolivaz.github.io/Checkers-documentation/).
