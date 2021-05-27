#include "position.hpp"
#include "searcher.hpp"
#include <iostream>


bool have_color(std::string board,char sim){
    if (sim == 'w'){
        for (char el:board) if (el == 'w' or el == 'W') return true;
    }
    else{
        for (char el:board) if (el == 'b' or el == 'B') return true;
    }
    return false;
}



std::vector<int> Searcher::find(Position position, int h){
    std::vector<int> result = {};
    // for black
    std::vector<std::vector<int>> possible = position.all_moves(false);
    if (possible.empty()) {return result;}
    auto best = possible[0];
    long int mark = -1900000000000;
    long int cur = 0;
    for (auto move:possible){
        cur= analize_move(position,move,h);
        if (cur > mark){
            mark = cur;
            best = move;
        }
    }
    std::cout << mark << '\n';
    return best;
}

long int Searcher::estimate_board(std::string board,bool color){
    // color = true - white
    int value = 0;
    // change weight
    int weight = 1;
    int d = 0;
    int white = 0;
    int black = 0;
    for (int y = 0; y < 8; y++)
        for (int x = y % 2;x<8; x+=2){
            d = x + 8*y;
            if (board[d] != '.'){
                if (board[d] == 'w'){
                    white += 1;
                    if (color) value += weight * simple_weight;
                    else value -= weight * simple_weight;
                }
                else if (board[d] == 'W'){
                    white += 1;
                    if (color) value += weight * queen_weight;
                    else value -= weight * queen_weight;
                }
                else if (board[d] == 'b'){
                    black += 1;
                    if (color) value -= weight * simple_weight;
                    else value += weight * simple_weight;
                }
                else{
                    black += 1;
                    if (color) value -= weight * queen_weight;
                    else value += weight * queen_weight;
                }
            } 
        }
    
    // случай победы 1 из игроков
    if (white == 0){
        if (color) {return -1000000;}
        else {return 1000000;}
    }
    if (black == 0){
        if (color) {return 1000000;}
        else {return -1000000;}
    }

    return value;
}

long int Searcher::analize_move(Position position, std::vector<int> move, int h){
    // h in 1 3 5 - it if for black
    position.move(move);
    if (h == 1){
        return estimate_board(position.board,false);
    }
    long int cur = 0;
    long int best = 10000000;
    int color = h % 2;
    if (color == 0){best = -10000000;}

    // возможно не нужно
    if (not have_color(position.board,'w')) return  10000000;
    if (not have_color(position.board,'b')) return -10000000;

    if (color == 1){
        // it was move of black, now it is for white
        std::vector<std::vector<int>> moves = position.all_moves(true);
        for (auto move1:moves){
            cur = analize_move(position,move1,h-1);
            if (cur < best){
                best = cur;
            }
        }
    }
    else{
        // for black
        std::vector<std::vector<int>> moves = position.all_moves(false);
        for (auto move1:moves){
            cur = analize_move(position,move1,h-1);
            if (cur > best){
                best = cur;
            }
        }
    }
    return best;
}




