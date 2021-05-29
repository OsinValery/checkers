#include "lib.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>
#include "position.hpp"
#include "searcher.hpp"

// w - simple white
// W - queen of white
// b - black
// . - empty
// _ - never step
std::string gen_start(){
    return "w_w_w_w__w_w_w_ww_w_w_w__._._._.._._._.__b_b_b_bb_b_b_b__b_b_b_b";
}


void print_help(){
    std::cout << "help                      - show this list\n";
    std::cout << "exit                      - leave this game and exit\n";
    std::cout << "pass                      - surrend\n";
    std::cout << "start                     - create the new game\n";
    std::cout << "restart                   - leave this game and create new\n";
    std::cout << "move x y x y...           - move figure by the way\n";
    std::cout << "show                      - draw the board with game\n";
    std::cout << "see                       - show 1 move(may be stupid)\n\n";
    std::cout << "Good game to you!\n\n>>";
}

int from_char(char digit){
        if (digit == '1') return 1;
        if (digit == '2') return 2;
        if (digit == '3') return 3;
        if (digit == '4') return 4;
        if (digit == '5') return 5;
        if (digit == '6') return 6;
        if (digit == '7') return 7;
        if (digit == '8') return 8;
        if (digit == '9') return 9;
        return 0;
}

std::vector<int> find_entry_nove(std::string entry){
    std::vector<int> result = {};
    std::string digit = "";
    int d = 0;
    for (char sim:entry){
        if (sim == ' ' or not isdigit(sim)){
            if (digit != ""){
                result.push_back(d);
                digit = "";
                d = 0;
            }
        }
        else if (isdigit(sim)){
            digit += sim;
            d = d * 10 + from_char(sim);
        }
    }
    if (digit != "") result.push_back(d);
    return result;
}

bool have_color(std::string board,std::string sim){
    if (sim == "w"){
        for (char el:board) if (el == 'w' or el == 'W') return true;
    }
    else{
        for (char el:board) if (el == 'b' or el == 'B') return true;
    }
    return false;
}


int main() {
    Position position;
    Searcher analizer;
    int H = 7;

    position.set_board(gen_start(),true);

    std::cout << " Добро пожайловать в игру\n\n\n>>";
    bool run = true;
    bool game = false;
    bool valid = false;
    std::string command = "";
    std::vector<std::vector<int>> moves;
    std::vector<int> way = {};

    while (run){
        std::cin >> command;
        std::cout << "\n\n";
        if (command == "exit") run = false;
        else if (command == "restart"){
            if (game){
                position.set_board(gen_start(),true);
                position.print();
                std::cout << "your move\n";
                std::cout << ">>";
            }
            else{
                std::cout << "You are not playing now\n>>";
            }
        }
        else if ("start" == command){
            if (game) std::cout << "You are playing now\n>>";
            else{
                game = true;
                position.set_board(gen_start(),true);
                position.print();
                std::cout << "\n>>";
            }
        }
        else if (command == "pass"){
            if (game){
                std::cout << "You are lost\n>>";
                game = false;
            }
            else{
                std::cout << "YOU are not playing now\n>>";
            }
        }
        else if (command == "show"){
            if (game){
                position.print();
                std::cout << "\n>>";
            }
            else{
                std::cout << "You are not playing now\n>>";
            }
        }
        else if (command == "help") print_help();
        else if (command.substr(0,4) == "move"){
            if (not game){
                std::cout << "You are not playing now\n>>";
            }
            else{
                std::getline(std::cin, command);
                way = find_entry_nove(command);
                valid = true;
                for (auto el:way) if (el < 0 or el > 7) valid = false;
                if (way.empty() or not valid) std::cout << "incorrect format of digits\n>>";
                else{
                    valid = false;
                    moves = position.all_moves(true);
                    for (std::vector<int> el:moves) if (el == way) valid = true;
                    if (not valid) std::cout << "this move not exist\n>>";
                    else{
                        position.move(way);
                        if (not have_color(position.board,"b")){
                            position.print();
                            std::cout << "You won\n>>";
                            game = false;
                        }
                        else{
                            way = analizer.find(position,H);
                            if (way.empty()){
                                std::cout << " have not move\nit is draw\n>>";
                                game = false;
                            }
                            else{
                                position.move(way);
                                position.print();
                                std::cout << "\nMy move is:\n";
                                for (int d:way) std::cout << d << ' ';
                                if (have_color(position.board,"w"))std::cout << "\n>>";
                                else{
                                    game = false;
                                    std::cout << "\nYou lose this game\n>>";
                                }
                            }
                        }
                    }
                }
            }
        }
        else if (command == "see"){
            if (not game){
                std::cout << "You are not playing now\n>>";
            }
            else{
                way = position.all_moves(true)[0];
                for (auto dig:way) std::cout << dig << ' ';
                std::cout << "\n>>";
            }
        }
        else{
            std::cout << "incorrect entry\n Write \"help\" for help\n>>";
        }


    }
  return 0;
}
