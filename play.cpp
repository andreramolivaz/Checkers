#include "player.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::string;

int main(int argc, char **argv){


    try {

        Player p1(1);

        p1.init_board("board_0.txt");
       /* Player p2(2);
        p1.load_board("board_0.txt");
        p1.move();
        p1.store_board("board_1.txt");
        p2.load_board("board_1.txt");
        p2.move();
        p2.store_board("board_2.txt");
        int round =2;
        while(p1.valid_move() && p2.valid_move() && !p1.wins() && !p2.wins()){
            p1.load_board("board_" + std::to_string(round) + ".txt");
            p1.move();
            p1.store_board("board_" + std::to_string(++round) + ".txt");
            p2.load_board("board_" + std::to_string(round) + ".txt");
            p2.move();
            p2.store_board("board_" + std::to_string(++round) + ".txt");
        }

        (p1.wins())? cout << "Player 1 wins!" << endl : cout << "Player 2 wins!" << endl;

        p2.store_board("board_exit.txt",0);

*/

    }
    catch(player_exception& e){
        std::cout << e.msg << std::endl;
    }



}

