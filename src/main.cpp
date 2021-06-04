#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>
#include "position.hpp"
#include "searcher.hpp"

#include "raylib.h"

// w - simple white
// W - queen of white
// b - black
// . - empty
// _ - never step
std::string gen_start(){
    return "w_w_w_w__w_w_w_ww_w_w_w__._._._.._._._.__b_b_b_bb_b_b_b__b_b_b_b";
}
std::string gen_empty(){
    return "._._._.__._._._.._._._.__._._._.._._._.__._._._.._._._.__._._._.";
}

unsigned int length(std::vector<int> const& obj){
    unsigned int res = 0;
    for (auto const& el:obj) res += 1;
    return res;
}


bool have_color(std::string &board,std::string sim){
    if (sim == "w"){
        for (char el:board) if (el == 'w' or el == 'W') return true;
    }
    else{
        for (char el:board) if (el == 'b' or el == 'B') return true;
    }
    return false;
}

bool contains(std::vector<int> const& move,std::vector<std::vector<int>> const& moves){
    for (const std::vector<int> &way:moves) if (way == move) return true;
    return false;
}

bool have_addition(std::vector<int> const& move,std::vector<std::vector<int>> const& moves){
    for (std::vector<int> const& way:moves){
        if (length(way) > length(move)){
            bool yes = true;
            unsigned int len = length(move);
            for (unsigned int i = 0;i<len;i+=1)
                if (way[i] != move[i] ) {yes = false;}
            if (yes) return true;
        }
    }
    return false;
}

std::vector<int> find_real_coords(int x, int y, int board_pos[], int field_size){
    if (x < board_pos[0] or x > board_pos[0]+8*field_size) return {-1,-1};
    if (y > board_pos[1] or y < board_pos[1] - 8*field_size) return {-1,-1};
    x -= board_pos[0];
    y -= (board_pos[1] - 8 * field_size);
    return {x / field_size, 7- y / field_size};
}




int main() {
    // for graphics
    const int screenWidth = 800;
    const int screenHeight = 500;
    int board_line_width = 4;
    int board_pos[] = {100,screenHeight - 50};
    int field_size = 40;    

    // for player's move choosing logic
    bool have_taking_variant = false;
    bool can_move = true;
    std::vector<int> planned_way = {};
    std::vector<int> choose_pos = {-1,-1};

    // for logic of game
    Position position;
    Searcher analizer;
    position.set_board(gen_empty(),true);
    std::vector<std::vector<int>> moves;
    std::vector<int> way = {};

    // SETTINGS FOR GAME STATE !!!
    int H = 5;    
    bool with_bot = false;
    bool color_move = true;      // color = true -> move of white
    bool playing = false;

    // other
    std::vector<int> choose_null = {-1,-1};


    InitWindow(screenWidth, screenHeight, "Шашки");
    SetTargetFPS(60);    
    Rectangle BtnRectExit = {500,150,150,50 };  
    Rectangle BtnRectLeave = {500,220,150,50};  
    Rectangle BtnRectPlayer = {500, 290, 150, 50};
    Rectangle BtnRectBot = {500,360, 150, 50};    

    while (!WindowShouldClose())    
    {
        // Update
        // -------------------------------------------------------------------------------

        auto click = GetMousePosition();

        if (IsMouseButtonPressed(0)) {
            int x = click.x;
            int y = click.y;

            // check buttons
            // taking button
            if (CheckCollisionPointRec(click, BtnRectExit)){
                if (have_taking_variant){
                    if (contains(planned_way,position.all_moves(color_move))){
                        have_taking_variant = false;
                        position.move(planned_way);
                        planned_way = {};
                        if (with_bot){
                            // bot do move
                            can_move = false;
                            way = analizer.find(position,H);
                            position.move(way);
                            way = {};
                            choose_pos = choose_null;
                            can_move = true;
                        }
                        else{
                            // clear move date for other player
                            color_move = not color_move;
                            choose_pos = choose_null;
                        }
                    }
                }
            }
            else if (CheckCollisionPointRec(click, BtnRectLeave)){ // leave game
                planned_way = {};
                choose_pos = choose_null;
                playing = false;
                have_taking_variant = false;
                color_move = true;
                can_move = true;
            }
            else if (CheckCollisionPointRec(click, BtnRectBot)){ // start with bot
                if (not playing){
                    with_bot = true;
                    color_move = true;
                    position.set_board(gen_start(),true);
                    playing = true;
                    choose_pos = choose_null;
                    planned_way = {};
                }
            }
            else if (CheckCollisionPointRec(click, BtnRectPlayer)){ // start with player
                if (not playing){
                    with_bot = false;
                    color_move = true;
                    position.set_board(gen_start(),true);
                    playing = true;
                    choose_pos = choose_null;
                    planned_way = {};
                }
            }
            else if (playing){   // check board
                if (can_move)
                    if (choose_pos == choose_null){
                        choose_pos = find_real_coords(x,y,board_pos,field_size);
                        for (int dig:choose_pos) planned_way.push_back(dig);
                        // impossible field for move
                        if ( (choose_pos[0] + choose_pos[1]) % 2 == 1 ){
                            choose_pos = choose_null;
                            planned_way = {};
                        }
                    }
                    else{
                        choose_pos = find_real_coords(x,y,board_pos,field_size);
                        for (int dig:choose_pos) planned_way.push_back(dig);
                        if (contains(planned_way,position.all_moves(color_move))){
                            // this way exist!
                            // have player plan to teke more figures?
                            if (not have_addition(planned_way, position.all_moves(color_move) )){
                                // can't
                                have_taking_variant = false;
                                position.move(planned_way);
                                planned_way = {};
                                if (with_bot){
                                    // bot do move
                                    can_move = false;
                                    way = analizer.find(position,H);
                                    if (!way.empty()){
                                        position.move(way);
                                        way = {};
                                        choose_pos = choose_null;
                                        can_move = true;
                                    }
                                    else{
                                        playing = false;

                                    }
                                }
                                else{
                                    // clear move date for other player
                                    color_move = not color_move;
                                    choose_pos = choose_null;
                                }
                            }
                            else{
                                // can append
                                have_taking_variant = true;
                            }
                        }
                        else{
                            // error! move is not exist!
                            have_taking_variant = false;
                            choose_pos = choose_null;
                            planned_way = {};

                        }
                    }
            }

        } // work mouseclick

        if (not have_color(position.board,"w")){
            playing = false;
            choose_pos = choose_null;
            planned_way = {};
        }
        else if (not have_color(position.board,"b")){
            playing = false;
            planned_way = {};
            choose_pos = choose_null;
        }


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(YELLOW);

        // draw title
        if (playing)
            if (with_bot){
                if (can_move) DrawText("Bot Think", 190, 0, 50, GREEN);
                else DrawText("With bot", 190, 0, 50, GREEN);
            }
            else if (color_move){
                DrawText("White's move", 190, 0, 50, GREEN);
            }
            else DrawText("Black's move", 190, 0, 50, GREEN);
        else{
            if (with_bot){
                if (not have_color(position.board,"w"))DrawText("You lose", 190, 0, 50, GREEN);
                else DrawText("You win", 190, 0, 50, GREEN);
            }
            else if (not have_color(position.board,"w") or color_move){
                DrawText("White lose", 190, 0, 50, GREEN);
            }
            else DrawText("Black lose", 190, 0, 50, GREEN);
        }

        // draw planned move
        if (not planned_way.empty()){
            int len = length(planned_way);
            int pos = 0;

            while (pos != len){
                int x = planned_way[pos];
                int y = planned_way[1+pos];
                pos += 2;
                std::vector<int> angle = {
                    board_pos[0]+ x * field_size,
                    board_pos[1]- (y + 1) * field_size
                };
                DrawRectangle(angle[0],angle[1],field_size,field_size,ORANGE);
            }
        }

        // draw selected figure
        if (choose_pos != choose_null){
            std::vector<int> angle = {
                board_pos[0]+ choose_pos[0] * field_size,
                board_pos[1]- (choose_pos[1] + 1) * field_size
                };
            DrawRectangle(angle[0],angle[1],field_size,field_size,PINK);
        }

        // draw board
        for (int i = 0;i<9;i+=1){
            DrawRectangle(
                board_pos[0] ,
                board_pos[1] - (8-i)*field_size ,
                8 * field_size,
                board_line_width,
                BLUE
            );
            DrawRectangle(
                board_pos[0]+i*field_size , 
                board_pos[1] - 8* field_size ,
                board_line_width, 
                8*field_size,
                BLUE
            );
        }

        // draw figures
        std::string line = "";
        for (int i = 0;i<8;i+=1){
            line = position.board.substr(8*i,8);
            int n = 0;
            int d = 5;
            for (char sim:line){
                Vector2 pos = {
                    board_pos[0] + (n+0.5)*field_size+d - board_line_width/2,
                    board_pos[1] - (i + 0.5) * field_size + d - board_line_width / 2
                };
                if (sim == 'w'){DrawCircleV(pos,field_size/2 - 2*d,  LIGHTGRAY) ;}
                if (sim == 'b'){DrawCircleV(pos,field_size/2 - 2*d,  BLACK) ;}
                if (sim == 'W'){DrawCircleV(pos,field_size/2 - 2*d,  GREEN) ;}
                if (sim == 'B'){DrawCircleV(pos,field_size/2 - 2*d,  RED) ;}
                n+=1;
            }
        }
        
        
        // draw interface
        DrawRectangleRec(BtnRectExit,GREEN);
        DrawText("Move",BtnRectExit.x+10, BtnRectExit.y+5,20, RED);
        DrawRectangleRec(BtnRectLeave,GREEN);
        DrawText("Leave",BtnRectLeave.x+10, BtnRectLeave.y+5,20, RED);
        DrawRectangleRec(BtnRectPlayer,GREEN);
        DrawText("With Player",BtnRectPlayer.x+10, BtnRectPlayer.y+5,20, RED);
        DrawRectangleRec(BtnRectBot,GREEN);
        DrawText("With bot",BtnRectBot.x+10, BtnRectBot.y+5,20, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context

    
  return 0;
}
