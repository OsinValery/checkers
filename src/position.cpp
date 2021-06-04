#include <vector>
#include <string>
#include <cctype>
#include <iostream>
#include "position.hpp"

bool equal_color(char c1, char c2){
    return tolower(c1) == tolower(c2);
}

// условие ? true : false
int abs(int value) {return value<0 ? -value : value;}



void Position::set_board(std::string new_board, bool new_move_of){
    this->board = new_board;
    this->move_of = new_move_of ? 0 : 1;
}

void Position::move(std::vector<int> const& moves){
    // from a,b   to   x,y
    int a,b,x,y;
    a = moves[0];
    b = moves[1];
    unsigned short pos = 2;  
    unsigned short length = moves.size();
    char color = tolower(board[a+8*b]);
    while (pos != length){ 
        x = moves[pos];
        y = moves[pos+1];
        pos += 2;

        // in first move_of == 0
        if ((y == 7 and color == 'w' )or(y == 0 and color == 'b' )) {
            // create queen
            std::string sim = "W";
            if (color == 'b') sim = "B";
            board.replace(a+8*b,1,sim);
        }
        
        std::swap(board[a + 8 * b],board[x + 8 * y]);
        if (abs(a-x)!= 1 ){
            while (a != x){
                if (board[a + 8*b] != '.') {board.replace(a+8*b,1,".");}
                if (a > x) {a -= 1;} else {a += 1;};
                if (b > y) {b -= 1;} else {b += 1;};
            }
            
        }
        a = x; b = y;
    }
    move_of = move_of == 0 ? 1:0;
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
        for (char const& field:line) if (field == '_')result += "###"; else result += "   ";
        result += "|   |\n  | ";
        result += std::to_string(7-x);
        result += " |";
        for (char const& field:line){
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
        for (char const& field:line) if (field == '_')result += "###"; else result += "   ";
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
    new_position.set_board(board,move_of == 0);
    return new_position;
}

// просто бежит по всем полям, если есть фигура нужного цвета, вызывает fig_moves
std::vector<std::vector<int>> Position::all_moves(bool color){
    std::vector<std::vector<int>> temp_moves;
    for (short int y = 0; y < 8; y++){
        for (short int x = y % 2; x < 8; x += 2){
            char figure = tolower(board[x+8*y]);
            if (figure != '.' ){
                if (figure == 'w' and color ){
                    auto res = fig_moves(x,y,true);
                    for (const auto &el:res)
                        temp_moves.push_back(el);
                }
                else if (figure == 'b' and !color){
                    auto res = fig_moves(x,y,true);
                    for (const auto &el:res)
                        temp_moves.push_back(el);
                }
            }
        }
    }
    return temp_moves;
}

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
    int d = x + 8 * y;
    std::vector<std::vector<int>> temp_moves;
    bool is_queen = not islower(board[d]);
    char color = tolower(board[d]);
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
            if (board[d+9] == '.'){
                if (free and color == 'w'){
                    temp_moves.push_back({x,y, x+1,y+1});
                }
            }
            else if (x<6 and y<6 and !equal_color(color,board[d+9]) ){
                if (board[d+18] == '.'){
                    temp_moves.push_back({x,y,x+2,y+2});
                    Position temp_pos = copy();
                    temp_pos.move({x,y,x+2,y+2});
                    std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x+2,y+2,false,3);

                    if (color == 'w' and y == 5){
                        for (std::vector<int> &way:new_moves){
                            way[0] = x;
                            way[1] = y;
                            temp_moves.push_back(way);
                        }
                    }
                    else{
                        for (const std::vector<int> &way:new_moves){
                            std::vector<int> new_way{x,y};
                            for (int dig:way) {new_way.push_back(dig);}
                            temp_moves.push_back(new_way);
                        }
                    }
                }
            }
        }
        // down - right
        if (x != 7 and y > 0){
            if (board[d-7] == '.') {
                if (free and color != 'w'){
                    temp_moves.push_back({x,y,x+1,y-1});
                }
            }
            else if (x<6 and y>1 and !equal_color(color, board[d-7]) ){
                if (board[d-14] == '.'){
                    temp_moves.push_back({x,y,x+2,y-2});
                    Position temp_pos = copy();
                    temp_pos.move({x,y,x+2,y-2});
                    std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x+2,y-2,false,4);

                    if (color == 'b' and y == 2){
                        for (std::vector<int> &way:new_moves){
                            way[0] = x;
                            way[1] = y;
                            temp_moves.push_back(way);
                        }
                    }
                    else{
                        for (const std::vector<int> &way:new_moves){
                            std::vector<int> new_way{x,y};
                            for (int dig:way) {new_way.push_back(dig);}
                            temp_moves.push_back(new_way);
                        }
                    }
                }
            }
        }
        // down - left
        if (x != 0 and y != 0){
            if (board[d-9] == '.') {
                if (free and color != 'w'){
                    temp_moves.push_back({x,y,x-1,y-1});
                }
            }
            else if (x > 1 and y>1 and !equal_color(color,board[d-9]) ){
                if (board[d-18] == '.'){
                    temp_moves.push_back({x,y,x-2,y-2});
                    Position temp_pos = copy();
                    temp_pos.move({x,y,x-2,y-2});
                    std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x-2,y-2,false,1);

                    if (y == 2 and color == 'b'){
                        for (std::vector<int> &way:new_moves){
                            way[0] = x;
                            way[1] = y;
                            temp_moves.push_back(way);
                        }
                    }
                    else{
                        for (const std::vector<int> &way:new_moves){
                            std::vector<int> new_way{x,y};
                            for (int dig:way) {new_way.push_back(dig);}
                            temp_moves.push_back(new_way);
                        }
                    }
                }
            }
        }
        // up - left
        if (x != 0 and y != 7){
            if (board[d+7] == '.') {
                if (free and color == 'w'){
                    temp_moves.push_back({x,y, x-1,y+1});
                }
            }
            else if (x>1 and y < 6 and !equal_color(color,board[d+7]) ){
                if (board[d+14] == '.'){
                    temp_moves.push_back({x,y, x-2,y+2});
                    Position temp_pos = copy();
                    temp_pos.move({x,y,x-2,y+2});
                    std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x-2,y+2,false,2);

                    if (color == 'w' and y == 5){
                        for (std::vector<int> &way:new_moves){
                            // when simple transforms to queen, fig_moves returns 
                            // doubled first move
                            // it is normal for queen
                            way[0] = x;
                            way[1] = y;
                            temp_moves.push_back(way);
                        }
                    }
                    else{
                        for (const std::vector<int> &way:new_moves){
                            std::vector<int> new_way{x,y};
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
            while (!end){
                // check right and left lines
                bool must = true;
                int way_x = x;
                int way_y = y;
                while (must){
                    int x8y = way_x + 8 * way_y;
                    if (way_x == 0 or way_y == 7){must = false;}
                    else if (equal_color(color, board[x8y+7]) ){must = false;}
                    else if (board[x8y+7] != '.'){
                        // враг на подветке найден
                        if (way_x<2 or way_y>5 or board[x8y+18] != '.' ){must = false;}
                        else{
                            must = false;
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x-2,way_y+2});
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x-2,way_y+2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x-2,way_y+2,false,2);
                            for (auto &way:new_moves){
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
                    int x8y = way_x + 8 * way_y;
                    if (way_x == 7 or way_y == 0){must = false;}
                    else if (equal_color(color,board[x8y-7]) ){must=false;}
                    else if (board[x8y-7] != '.' ){
                        // враг найден на поддереве
                        must = false;
                        if (way_x>5 or way_y<2 or board[x8y-14] != '.' ){}
                        else{
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x+2,way_y-2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x+2,way_y-2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x+2,way_y-2,false,4);
                            for (auto &way:new_moves){
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
                        for (auto &way:new_moves){
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
                int d = x + 8 * y;
                if (x==7 or y == 7){must=false;}
                else if (equal_color(color,board[d+9]) ){must=false;}
                else if (board[d+9] != '.' ) {
                    must = false;
                    // враг на линии
                    if (x>5 or y>5 or board[d+18] != '.' ){}
                    else{
                        // take
                        temp_moves.push_back({cur_x,cur_y,x+2,y+2});
                        auto temp_pos = copy();
                        temp_pos.move({cur_x,cur_y,x+2,y+2});
                        std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x+2,y+2,false,3);
                        for (auto &way:new_moves){
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
            while (!end){
                // check right and left lines of main way
                int way_x = x;
                int way_y = y;
                bool must = true;
                // check up - right and down - left lines
                while (must){
                    int x8y = way_x + 8 * way_y;
                    if (way_x == 7 or way_y == 7){must = false;}
                    else if (equal_color(color, board[x8y+9])){must=false;}
                    else if (board[x8y+9] != '.'){
                        // enemy found
                        must = false;
                        if (way_x > 5 or way_y > 5 or board[x8y+18] != '.'){}
                        else{
                            // try take it
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x+2,way_y+2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x+2,way_y+2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x+2,way_y+2,false,3);
                            for (auto &way:new_moves){
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
                    int x8y = way_y * 8 + way_x;
                    if (way_x == 0 or way_y == 0){must=false;}
                    else if (equal_color(color, board[x8y-9])){must=false;}
                    else if (board[x8y-9] != '.'){
                        must = false;
                        if (way_x < 2 or way_y < 2 or board[x8y-18] != '.'){}
                        else{
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x-2,way_y-2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x-2,way_y-2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x-2,way_y-2,false,1);
                            for (auto &way:new_moves){
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
                        for (auto &way:new_moves){
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
                int d = x + 8 * y;
                if (x==7 or y == 0){must=false;}
                else if (equal_color(color, board[d-7]) ){must=false;}
                else if (board[d-7] != '.'){
                    must = false;
                    if (x > 5 or y < 2 or board[d-14] != '.'){}
                    else{
                        temp_moves.push_back({cur_x,cur_y,x+2,y-2});
                        auto temp_pos = copy();
                        temp_pos.move({cur_x,cur_y,x+2,y-2});
                        std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x+2,y-2,false,4);
                        for (auto &way:new_moves){
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
                    int x8y = way_y * 8 + way_x;
                    if (way_x == 7 or way_y == 0){must=false;}
                    else if (equal_color(color, board[x8y-7])){must=false;}
                    else if (board[x8y-7] == '.'){
                        way_x += 1;
                        way_y -= 1;
                    }
                    else{
                        // enemy found
                        must = false;
                        if (way_x > 5 or way_y < 2 or board[x8y-14] != '.'){}
                        else {
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x+2,way_y-2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x+2,way_y-2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x+2,way_y-2,false,4);
                            for (auto &way:new_moves){
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
                    int x8y = 8 * way_y + way_x;
                    if (way_x == 0 or way_y == 7){must=false;}
                    else if (equal_color(color, board[x8y+7])){must=false;}
                    else if (board[x8y+7] == '.'){
                        way_y += 1;
                        way_x -= 1;
                    }
                    else{
                        // enemy found
                        must = false;
                        if (way_x < 2 or way_y > 5 or board[x8y+14] != '.'){}
                        else{
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x-2,way_y+2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x-2,way_y+2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x-2,way_y+2,false,2);
                            for (auto &way:new_moves){
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
                        for (auto &way:new_moves){
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
                int d = x + 8 * y;
                if (x==0 or y == 0){must=false;}
                else if (equal_color(color,board[d-9])){must=false;}
                else if (board[d-9] == '.'){
                    y -= 1;
                    x -= 1;
                    temp_moves.push_back({cur_x,cur_y,x,y});
                }
                else{
                    must = false;
                    if (x < 2 or y<2 or board[d-18] != '.'){}
                    else{
                        temp_moves.push_back({cur_x,cur_y,x-2,y-2});
                        auto temp_pos = copy();
                        temp_pos.move({cur_x,cur_y,x-2,y-2});
                        std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(x-2,y-2,false,1);
                        for (auto &way:new_moves){
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
                    int x8y = way_y * 8 + way_x;
                    if (way_x==7 or way_y == 7){must=false;}
                    else if (equal_color(color, board[x8y+9])){must=false;}
                    else if (board[x8y+9] == '.'){
                        way_y += 1;
                        way_x += 1;
                    }
                    else{
                        // enemy found
                        must = false;
                        if (way_x>5 or way_y > 5 or board[x8y+18] != '.'){}
                        else {
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x+2,way_y+2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x+2,way_y+2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x+2,way_y+2,false,3);
                            for (auto &way:new_moves){
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
                    int x8y = way_x + way_y * 8;
                    if (way_x == 0 or way_y == 0){must = false;}
                    else if (equal_color(color, board[x8y-9])){must = false;}
                    else if (board[x8y-9] == '.'){
                        way_y -= 1;
                        way_x -= 1;
                    }
                    else{
                        // enemy found
                        must = false;
                        if (way_x < 2 or way_y < 2 or board[x8y-18] != '.'){}
                        else {
                            temp_moves.push_back({cur_x,cur_y,x,y,way_x-2,way_y-2});
                            auto temp_pos = copy();
                            temp_pos.move({cur_x,cur_y,x,y,way_x-2,way_y-2});
                            std::vector<std::vector<int>> new_moves = temp_pos.fig_moves(way_x-2,way_y-2,false,1);
                            for (auto &way:new_moves){
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
                        for (auto &way:new_moves){
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
                        for (auto &way:new_moves){
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




