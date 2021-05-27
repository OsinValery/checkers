#include <vector>
#include <string>
#include <cctype>
#include <iostream>
#include "position.hpp"

bool equal_color(char c1, char c2){
    return tolower(c1) == tolower(c2);
}

int abs(int value){if (value < 0) return -value; else return value;}




void Position::set_board(std::string new_board, bool new_move_of){
    this->board = new_board;
    if (new_move_of){ this->move_of = 0;} else {this->move_of = 1;};
}

void Position::move(std::vector<int> moves){
    // from a,b   to   x,y
    int a,b,x,y;
    a = moves[0];
    b = moves[1];
    unsigned long pos = 1;
    unsigned long length = moves.size();
    char color = tolower(board[a+8*b]);
    while (pos + 1 < length){
        x = moves[pos+1];
        y = moves[pos+2];
        pos += 2;

        // in first move_of == 0
        if ((color == 'w' and y == 7)or(color == 'b' and y == 0)) {
            // create queen
            std::string sim = "W";
            if (color == 'b') sim = "B";
            board.replace(a+8*b,1,sim);
        }
        
        int c,d;
        std::swap(board[a + 8 * b],board[x + 8 * y]);
        if (abs(a-x)!= 1 ){
            c = a;
            d = b;
            while (c != x){
                if (board[c + 8*d] != '.') {board.replace(c+8*d,1,".") ;}
                if (c > x) {c -= 1;} else {c += 1;};
                if (d > y) {d -= 1;} else {d += 1;};
            }
            
        }
        a = x; b = y;
    }

    if (move_of == 0) move_of = 1; else move_of = 0;

}

void Position::print(){
    unsigned short int i = 64;
    std::string result = "\n\n  .___.________________________.___.\n";
    result += "  |   .                        .   |\n";
    result += "  |   . 0  1  2  3  4  5  6  7 .   |\n";
    result += "  |....------------------------....|\n";
    std::string line = "";
    for (short int x = 0;x<8; x++){
        line = board.substr(i-8,8);
        result += "  |   |";
        for (char field:line) if (field == '_')result += "###"; else result += "   ";
        result += "|   |\n  | ";
        result += std::to_string(7-x);
        result += " |";
        for (char field:line){
            if (field == '_') result += "###";
            else if (field == '.') result += "   ";
            else{
                result += " ";
                if (field == 'b') result += "b";
                if (field == 'B') result += "B";
                if (field == 'w') result += "w";
                if (field == 'W') result += "W";
                result += " ";
            }
        }
        result += "| ";
        result += std::to_string(7-x);
        result += " |\n  |   |";
        for (char field:line) if (field == '_')result += "###"; else result += "   ";
        result += "|   |\n";
        i -= 8;
    }
    result += "  |....________________________....|";
    result += "\n  |   . 0  1  2  3  4  5  6  7 .   |\n";
    result += "  .___.________________________.___.\n\n";
    std::cout << result;
}

Position Position::copy(){
    Position new_position;
    new_position.set_board(board,true);
    new_position.move_of = move_of;
    return new_position;
}

// просто бежит по всем полям, если есть фигура нужного цвета, вызывает fig_moves
std::vector<std::vector<int>> Position::all_moves(bool color){
    std::vector<std::vector<int>> temp_moves;
    for (short int y = 0; y < 8; y++){
        for (short int x = y % 2; x < 8; x += 2){
            if (board[x+8*y] != '.' ){
                char figure = board[x+8*y];
                if (tolower(figure) == 'w' && color ){
                    auto res = fig_moves(x,y,true);
                    for (auto el:res)
                        temp_moves.push_back(el);
                }
                else if (tolower(figure) == 'b' and not color){
                    auto res = fig_moves(x,y,true);
                    for (auto el:res)
                        temp_moves.push_back(el);
                }
            }
        }
    }
    return temp_moves;
}

// строит дерево позиций и применяет minimax
std::vector<std::vector<int>> Position::fig_moves(int x, int y, bool free , int line){
    // free - can figure do all the moves or it must only take other 
    // true - can
    // line - parametr for queen, it is line of movement by the long moves
    // откуда пришёл на это поле
    // 0 - default, not important or it is not queen
    // 1 - up-right
    // 2 - down-right
    // 3 - down-left
    // 4 - up-left
    std::vector<std::vector<int>> temp_moves;
    bool is_queen = not islower(board[x+8*y]);
    char color = tolower(board[x+8*y]);
    // если не дамка - проверяет воля вокруг, если свободно - все простые ходы тоже
    // если возможно взятие, рубит и заходит в реккурсию и ставит free = false
    // для дамки
    // свободно - бежит во все стороны, записывая все поля
    // встретив врага, заходит в реккурсию и становится несвободным
    // несвободен - двигается по линии, задаваемой line и проверяет все перпендикулярные направления, 
    // при случае заходит в реккурсию
    // что происходит внутри цикла - 
    // проверяет _|_ направления и пытается двинуться дальше по линии
    // 4 условия по каждому направлению для случая свободы и несвободы
    // после этой длинной функции ничего нет
    if (not is_queen){
        // up - right
        if (x != 7 and y != 7){
            if (board[x+8*y+9] == '.'){
                if (free and color == 'w'){
                    temp_moves.push_back({x,y, x+1,y+1});
                }
            }
            else if (x<6 and y<6 and not equal_color(color,board[x+9+8*y]) ){
                if (board[x+18+8*y] == '.'){
                    temp_moves.push_back({x,y,x+2,y+2});
                    Position temp_pos = copy();
                    temp_pos.move({x,y,x+2,y+2});
                    std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x+2,y+2,false,3);
                    for (std::vector<int> way:new_moves){
                        if (color == 'w' and y == 5){
                            way[0] = x;
                            way[1] = y;
                            temp_moves.push_back(way);
                        }
                        else{
                            std::vector<int> new_way = {x,y};
                            for (int dig:way) {new_way.push_back(dig);}
                            temp_moves.push_back(new_way);
                        }
                    }
                }
            }
        }
        // down - right
        if (x != 7 and y > 0){
            if (board[x-7+8*y] == '.') {
                if (free and color != 'w'){
                    temp_moves.push_back({x,y,x+1,y-1});
                }
            }
            else if (x<6 and y>1 and not equal_color(color, board[x-7+8*y]) ){
                if (board[x-14+8*y] == '.'){
                    temp_moves.push_back({x,y,x+2,y-2});
                    Position temp_pos = copy();
                    temp_pos.move({x,y,x+2,y-2});
                    std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x+2,y-2,false,4);
                    for (std::vector<int> way:new_moves){
                        if (color == 'b' and y == 2){
                            way[0] = x;
                            way[1] = y;
                            temp_moves.push_back(way);
                        }
                        else{
                            std::vector<int> new_way = {x,y};
                            for (int dig:way) {new_way.push_back(dig);}
                            temp_moves.push_back(new_way);
                        }
                    }
                }
            }
        }
        // down - left
        if (x != 0 and y != 0){
            if (board[x-9+8*y] == '.') {
                if (free and color != 'w'){
                    temp_moves.push_back({x,y,x-1,y-1});
                }
            }
            else if (x > 1 and y>1 and not equal_color(color,board[x-9+8*y]) ){
                if (board[x-2+8*(y-2)] == '.'){
                    temp_moves.push_back({x,y,x-2,y-2});
                    Position temp_pos = copy();
                    temp_pos.move({x,y,x-2,y-2});
                    std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x-2,y-2,false,1);
                    for (std::vector<int> way:new_moves){
                        if (y == 2 and color == 'b'){
                            way[0] = x;
                            way[1] = y;
                            temp_moves.push_back(way);
                        }
                        else{
                            std::vector<int> new_way = {x,y};
                            for (int dig:way) {new_way.push_back(dig);}
                            temp_moves.push_back(new_way);
                        }
                    }
                }
            }
        }
        // up - left
        if (x != 0 and y != 7){
            if (board[x+7+8*y] == '.') {
                if (free and color == 'w'){
                    temp_moves.push_back({x,y, x-1,y+1});
                }
            }
            else if (x>1 and y < 6 and not equal_color(color,board[x+7+8*y]) ){
                if (board[x+14+8*y] == '.'){
                    temp_moves.push_back({x,y, x-2,y+2});
                    Position temp_pos = copy();
                    temp_pos.move({x,y,x-2,y+2});
                    std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x-2,y+2,false,2);
                    for (std::vector<int> way:new_moves){
                        if (color == 'w' and y == 5){
                            // when simple transforms to queen, fig_moves returns 
                            // doubled first move
                            // it is normal for queen
                            way[0] = x;
                            way[1] = y;
                            temp_moves.push_back(way);
                        }
                        else{
                            std::vector<int> new_way = {x,y};
                            for (int dig:way) {new_way.push_back(dig);}
                            temp_moves.push_back(new_way);
                        }
                    }
                }
            }
        }
    }
    else{
        const short int cur_x = x;
        const short int cur_y = y;
        // first line
        if (not free and line == 3){
            // up - right
            bool end = false;
            while (not end){
                // check right and left lines
                bool must = true;
                int way_x = x;
                int way_y = y;
                while (must){
                    if (way_x == 0 or way_y == 7){must = false;}
                    else if (equal_color(color, board[way_x+7+8*way_y]) ){must = false;}
                    else if (board[way_x+7+8*way_y] != '.'){
                        // враг на подветке найден
                        if (way_x<2 or way_y>5 or board[way_x+18+8*way_y] != '.' ){must = false;}
                        else{
                            must = false;
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x-2,way_y+2});
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x-2,way_y+2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x-2,way_y+2,false,2);
                            for (auto way:new_moves){
                                way[0] = x;
                                way[1] = y;
                                std::vector<int> new_way = {cur_x,cur_y};
                                for (int dig:way) new_way.push_back(dig);
                                temp_moves.push_back(new_way);
                            }
                        }
                    }
                    else{
                        way_x -= 1;
                        way_y += 1;
                    }
                }
                must = true;
                way_y = y;
                way_x = x;
                while (must){
                    if (way_x == 7 or way_y == 0){must = false;}
                    else if (equal_color(color,board[way_x-7+8*way_y]) ){must=false;}
                    else if (board[way_x+1+8*(way_y-1)] != '.' ){
                        // враг найден на поддереве
                        must = false;
                        if (way_x>5 or way_y<2 or board[way_x-14+8*way_y] != '.' ){}
                        else{
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x+2,way_y-2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x+2,way_y-2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x+2,way_y-2,false,4);
                            for (auto way:new_moves){
                                way[0] = x;
                                way[1] = y;
                                std::vector<int> new_way = {cur_x,cur_y};
                                for (int dig:way) new_way.push_back(dig);
                                temp_moves.push_back(new_way);
                            }
                        }
                    }
                    else{
                        way_x += 1;
                        way_y -= 1;
                        }
                }
                // after try move by line
                if (x == 7 or y == 7){end = true;}
                else if (equal_color(color,board[x+9+8*y])){
                    end = true;
                }
                else if (board[x+9+8*y] == '.'){
                    x += 1;
                    y += 1;
                    temp_moves.push_back({cur_x,cur_y,x,y});
                }
                else{
                    // enemy
                    end = true;
                    if (x>5 or y>5 or board[x+18+8*y] != '.'){}
                    else{
                        // try beat it
                        auto temp_pos = copy();
                        temp_moves.push_back({cur_x,cur_y,x+2,y+2});
                        temp_pos.move({cur_x,cur_y,x+2,y+2});
                        std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x+2,y+2,false,3);
                        for (auto way:new_moves){
                            way[0] = cur_x;
                            way[1] = cur_y;
                            temp_moves.push_back(way);
                        }
                    }
                }
            }
            x = cur_y;
            y = cur_y;
        }
        else if (free){
            bool must = true;
            while(must){
                if (x==7 or y == 7){must=false;}
                else if (equal_color(color,board[x+9+8*y]) ){must=false;}
                else if (board[x+9+8*y] != '.' ) {
                    must = false;
                    // враг на линии
                    if (x>5 or y>5 or board[x+18+8*y] != '.' ){}
                    else{
                        // take
                        temp_moves.push_back({cur_x,cur_y,x+2,y+2});
                        auto temp_pos = copy();
                        temp_pos.move({cur_x,cur_y,x+2,y+2});
                        std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x+2,y+2,false,3);
                        for (auto way:new_moves){
                            way[0] = cur_x;
                            way[1] = cur_y;
                            temp_moves.push_back(way);
                        }
                    }
                }
                else{
                    x += 1;
                    y += 1;
                    temp_moves.push_back({cur_x,cur_y,x,y});
                }
            }

            x = cur_x;
            y = cur_y;
        }

        // next line == 4
        if (not free and line == 4){
            // down - right
            bool end = false;
            while (not end){
                // check right and left lines of main way
                int way_x = x;
                int way_y = y;
                bool must = true;
                // check up - right and down - left lines
                while (must){
                    if (way_x == 7 or way_y == 7){must = false;}
                    else if (equal_color(color, board[way_x+9+8*way_y]) ){must=false;}
                    else if (board[way_x+9+8*way_y] != '.'){
                        // enemy found
                        must = false;
                        if (way_x > 5 or way_y > 5 or board[way_x+18+8*way_y] != '.'){}
                        else{
                            // try take it
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x+2,way_y+2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x+2,way_y+2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x+2,way_y+2,false,3);
                            for (auto way:new_moves){
                                way[0] = x;
                                way[1] = y;
                                std::vector<int> new_way = {cur_x,cur_y};
                                for (int dig:way) new_way.push_back(dig);
                                temp_moves.push_back(new_way);
                            }
                        }

                    }
                    else{
                        way_x += 1;
                        way_y += 1;
                    }
                }
                way_y = y;
                way_x = x;
                must = true;
                while (must){
                    if (way_x == 0 or way_y == 0){must=false;}
                    else if (equal_color(color, board[way_x-9+8*way_y])){must=false;}
                    else if (board[way_x-9+8*way_y] != '.'){
                        must = false;
                        if (way_x < 2 or way_y < 2 or board[way_x-18+8*way_y] != '.'){}
                        else{
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x-2,way_y-2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x-2,way_y-2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x-2,way_y-2,false,1);
                            for (auto way:new_moves){
                                way[0] = x;
                                way[1] = y;
                                std::vector<int> new_way = {cur_x,cur_y};
                                for (int dig:way) new_way.push_back(dig);
                                temp_moves.push_back(new_way);
                            }
                        }
                    }
                    else{
                        way_y -= 1;
                        way_x -= 1;
                    }
                }
                // try move by the main way
                if (x == 7 or y == 0){end = true;}
                else if (equal_color(color,board[x-7+8*y]) ){end = true;}
                else if (board[x-7+8*y] == '.' ){
                    x += 1;
                    y -= 1;
                    temp_moves.push_back({cur_x,cur_y,x,y});
                }
                else{
                    // enemy
                    end = true;
                    if (x > 5 or y < 2 or board[x-14+8*y] != '.'){}
                    else{
                        // take it
                        temp_moves.push_back({cur_x,cur_y,x+2,y-2});
                        auto temp_pos = copy();
                        temp_pos.move({cur_x,cur_y,x+2,y-2});
                        std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x+2,y-2,false,4);
                        for (auto way:new_moves){
                            way[0] = cur_x;
                            way[1] = cur_y;
                            temp_moves.push_back(way);
                        }
                    }
                }
            }
            x = cur_x;
            y = cur_y;
        }
        else if (free){
            bool must = true;
            while (must){
                if (x==7 or y == 0){
                    must=false;
                    }
                else if (equal_color(color, board[x-7+8*y]) ){must=false;}
                else if (board[x-7+8*y] != '.'){
                    must = false;
                    if (x > 5 or y < 2 or board[x-14+8*y] != '.'){}
                    else{
                        temp_moves.push_back({cur_x,cur_y,x+2,y-2});
                        auto temp_pos = copy();
                        temp_pos.move({cur_x,cur_y,x+2,y-2});
                        std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x+2,y-2,false,4);
                        for (auto way:new_moves){
                            way[0] = cur_x;
                            way[1] = cur_y;
                            temp_moves.push_back(way);
                        }
                    }
                }
                else{
                    x += 1;
                    y -= 1;
                    temp_moves.push_back({cur_x,cur_y,x,y});
                }
            }
            x = cur_x;
            y = cur_y;
        }

        //next line == 1
        if (not free and line == 1){
            bool end = false;
            while (not end){
                int way_x = x;
                int way_y = y;
                bool must = true;
                // check down - right line
                while (must){
                    if (way_x == 7 or way_y == 0){must=false;}
                    else if (equal_color(color, board[way_x-7+8*way_y])){must=false;}
                    else if (board[way_x-7+8*way_y] == '.'){
                        way_x += 1;
                        way_y -= 1;
                    }
                    else{
                        // enemy found
                        must = false;
                        if (way_x > 5 or way_y < 2 or board[way_x-14+8*way_y] != '.'){}
                        else {
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x+2,way_y-2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x+2,way_y-2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x+2,way_y-2,false,4);
                            for (auto way:new_moves){
                                way[0] = x;
                                way[1] = y;
                                std::vector<int> new_way = {cur_x,cur_y};
                                for (int dig:way) new_way.push_back(dig);
                                temp_moves.push_back(new_way);
                            }
                        }
                    }
                }
                way_y = y;
                way_x = x;
                must = true;
                while (must){
                    if (way_x == 0 or way_y == 7){must=false;}
                    else if (equal_color(color, board[way_x+7+8*way_y])){must=false;}
                    else if (board[way_x+7+8*way_y] == '.'){
                        way_y += 1;
                        way_x -= 1;
                    }
                    else{
                        // enemy found
                        must = false;
                        if (way_x < 2 or way_y > 5 or board[way_x+14+8*way_y] != '.'){}
                        else{
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x-2,way_y+2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x-2,way_y+2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x-2,way_y+2,false,2);
                            for (auto way:new_moves){
                                way[0] = x;
                                way[1] = y;
                                std::vector<int> new_way = {cur_x,cur_y};
                                for (int dig:way) new_way.push_back(dig);
                                temp_moves.push_back(new_way);
                            }
                        }
                    }
                }
                // try move by the main way
                if (x == 0 or y == 0){end=true;}
                else if (equal_color(board[x-9+8*y],color) ){end=true;}
                else if (board[x-9+8*y] == '.'){
                    x -= 1;
                    y -= 1;
                    temp_moves.push_back({cur_x,cur_y,x,y});
                }
                else{
                    // enemy
                    end = true;
                    if (x < 2 or y < 2 or board[x-18+8*y] != '.'){}
                    else{
                        temp_moves.push_back({cur_x,cur_y,x-2,y-2});
                        auto temp_pos = copy();
                        temp_pos.move({cur_x,cur_y,x-2,y-2});
                        std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x-2,y-2,false,1);
                        for (auto way:new_moves){
                            way[0] = cur_x;
                            way[1] = cur_y;
                            temp_moves.push_back(way);
                        }
                    }
                }
            }
            x = cur_x;
            y = cur_y;
        }
        else if (free){
            // if free
            bool must = true;
            while (must){
                if (x==0 or y == 0){must=false;}
                else if (equal_color(color,board[x-9+8*y])){must=false;}
                else if (board[x-9+8*y] == '.'){
                    y -= 1;
                    x -= 1;
                    temp_moves.push_back({cur_x,cur_y,x,y});
                }
                else{
                    must = false;
                    if (x < 2 or y<2 or board[x-18+8*y] != '.'){}
                    else{
                        temp_moves.push_back({cur_x,cur_y,x-2,y-2});
                        auto temp_pos = copy();
                        temp_pos.move({cur_x,cur_y,x-2,y-2});
                        std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x-2,y-2,false,1);
                        for (auto way:new_moves){
                            way[0] = cur_x;
                            way[1] = cur_y;
                            temp_moves.push_back(way);
                        }
                    }
                }
            }
            x = cur_x;
            y = cur_y;
        }

        // line 2
        if (not free and line == 2){
            bool end = false;
            while (not end){
                int way_y = y;
                int way_x = x;
                bool must = true;
                // up - right line
                while(must){
                    if (way_x==7 or way_y == 7){must=false;}
                    else if (equal_color(color, board[way_x+9+8*way_y])){must=false;}
                    else if (board[way_x+9+8*way_y] == '.'){
                        way_y += 1;
                        way_x += 1;
                    }
                    else{
                        // enemy found
                        must = false;
                        if (way_x>5 or way_y > 5 or board[way_x+18+8*way_y] != '.'){}
                        else {
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x+2,way_y+2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x+2,way_y+2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x+2,way_y+2,false,3);
                            for (auto way:new_moves){
                                way[0] = x;
                                way[1] = y;
                                std::vector<int> new_way = {cur_x,cur_y};
                                for (int dig:way) new_way.push_back(dig);
                                temp_moves.push_back(new_way);
                            }
                        }
                    }
                }
                way_x = x;
                way_y = y;
                must = true;
                while (must){
                    if (way_x == 0 or way_y == 0){must = false;}
                    else if (equal_color(color, board[way_x-9+8*way_y])){must = false;}
                    else if (board[way_x-9+8*way_y] == '.'){
                        way_y -= 1;
                        way_x -= 1;
                    }
                    else{
                        // enemy found
                        must = false;
                        if (way_x < 2 or way_y < 2 or board[way_x-18+8*way_y] != '.'){}
                        else {
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x-2,way_y-2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x-2,way_y-2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x-2,way_y-2,false,1);
                            for (auto way:new_moves){
                                way[0] = x;
                                way[1] = y;
                                std::vector<int> new_way = {cur_x,cur_y};
                                for (int dig:way) new_way.push_back(dig);
                                temp_moves.push_back(new_way);
                            }
                        }
                    }
                }
                // try move by the main line
                if (x == 0 or y == 7){end = true;}
                else if (equal_color(color, board[x+7+8*y])){end=true;}
                else if (board[x+7+8*y] == '.'){
                    x -= 1;
                    y += 1;
                    temp_moves.push_back({cur_x,cur_y,x,y});
                }
                else{
                    // enemy found
                    end = true;
                    if (x < 2 or y > 5 or board[x+14+8*y] != '.'){}
                    else{
                        temp_moves.push_back({cur_x,cur_y,x-2,y+2});
                        auto temp_pos = copy();
                        temp_pos.move({cur_x,cur_y,x-2,y+2});
                        std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x-2,y+2,false,2);
                        for (auto way:new_moves){
                            way[0] = cur_x;
                            way[1] = cur_y;
                            temp_moves.push_back(way);
                        }
                    }
                }
            }
        } 
        else if (free){
            // free
            bool must = true;
            while (must){
                if (x == 0 or y == 7){must=false;}
                else if (equal_color(color, board[x+7+8*y])){must=false;}
                else if (board[x+7+8*y] == '.'){
                    x -= 1;
                    y += 1;
                    temp_moves.push_back({cur_x,cur_y,x,y});
                }
                else{
                    // enemy found
                    must = false;
                    if (x < 2 or y > 5 or board[x+14+8*y] != '.'){}
                    else{
                        temp_moves.push_back({cur_x,cur_y,x-2,y+2});
                        auto temp_pos = copy();
                        temp_pos.move({cur_x,cur_y,x-2,y+2});
                        std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x-2,y+2,false,2);
                        for (auto way:new_moves){
                            way[0] = cur_x;
                            way[1] = cur_y;
                            temp_moves.push_back(way);
                        }
                    }
                }
            }
        }

    }
    return temp_moves;
}




