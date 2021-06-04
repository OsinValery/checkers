#include "position.hpp"
#include "searcher.hpp"
#include <iostream>


bool have_color(std::string const& board,char sim){
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
    for (const auto &move : possible){
        cur= analize_move(position,move,h,mark);
        if (cur > mark){
            mark = cur;
            best = move;
        }
    }
    return best;
}

long int Searcher::estimate_board(std::string board,bool color){
    // color = true - white
    int value = 0;
    int d = 0;
    int white = 0;
    int black = 0;
    for (int y = 0; y < 8; y++)
        for (int x = y % 2;x<8; x+=2){
            d = x + 8*y;
            if (board[d] != '.'){
                if (board[d] == 'w'){
                    white += 1;
                    if (color) value += weights[7-y][7-x] * simple_weight;
                    else value -= weights[7-y][7-x] * simple_weight;
                }
                else if (board[d] == 'W'){
                    white += 1;
                    if (color) value += weight * queen_weight;
                    else value -= weight * queen_weight;
                }
                else if (board[d] == 'b'){
                    black += 1;
                    if (color) value -= weights[y][x] * simple_weight;
                    else value += weights[y][x] * simple_weight;
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

// строит дерево позиций и применяет minimax
// оптимизировано при помощи альфа-бетта отсечений
/*
    зная значение best, полученное из соседней ветки поддерева в узле выше
    можно не высчитывать всё поддерево для текущего узла
    на этом узле белый высчитывает ход с минимальной оценкой(по минимаксу так)
    на узле выше чёрный выбирает с наибольшей оценкой по минимаксу, для поддерева 1
    (соседа для текущего узла) уже подсчитано значение alfa, и далее вызван текущий узел для рассчёта
    минимизатор на текущем узле уже получил значение beta, подсчитывая свои дочерние поддеревья.
    он может и дальше продолжать абсолютоно бесполезную работу, ища n< beta, так как любой n< alfa
    поэтомо родительский узел(который ищет максимум) n никогда не выберет
    это было alfa отсечение
    бетта отсечение - дать текущеме узлу лучшее значение дедушки, и заинициализировать 
    текущее значение best этим значением, и у parent узле возможно вернётся оно-же, и далее не пойдёт 
    вычисление, так как минимизатор дошёл до числа, равного best у parent для parent текущего
    тут это не ускоряет сильно
    альфа - бета отсечения не считают бесполезные ветки дерева вычислений
*/
long int Searcher::analize_move(Position position, std::vector<int> const& move, 
                int h, long int alfa){
    // h in 1 3 5 - it is for black
    position.move(move);
    if (h == 0){
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
        if (moves.empty()) return 0;
        for (const auto &move1:moves){
            cur = analize_move(position,move1,h-1,best);
            if (cur < best){
                best = cur;
                if (best <= alfa) return best;
            }
        }
    }
    else{
        // for black
        std::vector<std::vector<int>> moves = position.all_moves(false);
        if (moves.empty()) return 0;
        for (const auto &move1:moves){
            cur = analize_move(position,move1,h-1,best);
            if (cur > best){
                best = cur;
                if (best>=alfa) return best;
            }
        }
    }
    return best;
}




