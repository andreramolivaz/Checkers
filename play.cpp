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
        Player p2(1);
        Player p4(2);
        Player p5(2);

        p1.init_board("./ciao.txt");
        p1.init_board("./ciao2.txt");
        p2.init_board("./ciao.txt");
        p1.load_board("./load_test.txt");
        p2.load_board("./ciao.txt");
        p2.load_board("./ciao.txt");
        cout<<p2.valid_move();
        p2.load_board("./load_test2.txt");

        p1.load_board("./load_test2.txt");
        Player p3(p1);
        p2=p1;
        p4.load_board("./load_test.txt");

        p2.load_board("./load_test2.txt");

        p1.store_board("store_test.txt" , 1 );


        p4.store_board("store_test.txt" , 0 );
        p4.move();
        p4.store_board("store_test2.txt" , 0 );


        p4.load_board("./load_test2.txt");
        cout<<p4.valid_move();
        cout<<p4.recurrence();
        p4.pop();
        p4.load_board("./load_test2.txt");
        cout<<"ply 2 win?"<<p4.wins(2)<<endl;
        cout<<"p4 win?"<<p4.wins()<<endl;
        cout<<"ply 2 lose?"<<p4.loses(2)<<endl;
        cout<<"p4 lose?"<<p4.loses()<<endl;

        cout<< p4(7,0,0);



    }
    catch(player_exception& e){
        std::cout << e.msg << std::endl;
    }



}

