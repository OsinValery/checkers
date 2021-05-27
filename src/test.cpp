#include "catch.hpp"
#include "lib.hpp"
#include "position.hpp"
#include "searcher.hpp"
#include <string>
#include <vector>
#include <chrono>
#include <random>

std::string empty = "._._._.__._._._.._._._.__._._._.._._._.__._._._.._._._.__._._._.";

std::string gen_start(){
    return "w_w_w_w__w_w_w_ww_w_w_w__._._._.._._._.__b_b_b_bb_b_b_b__b_b_b_b";
}

int count_ways(std::vector<int> way, std::vector<std::vector<int>> found){
    int count = 0;
    for (auto el:found) if (el == way) count += 1;
    return count;
}

bool contains(std::vector<int> value, std::vector<std::vector<int>> list){
    for (auto el:list) if (el == value) return true;
    return false;
}

bool contains_all(std::vector<std::vector<int>> found, std::vector<std::vector<int>> result){
    // every value from result is in found ,1 sample
    for (std::vector<int> way : result)
        if (count_ways(way, found) != 1)
            return false;
    
    // нет лишних путей в result
    for (auto way: found){
        if (not contains(way, result))
            return false;
    }

    return true;
}


int len(std::vector<std::vector<int>> obj){
    int count = 0;
    for (auto el:obj) count+=1;
    return count;
}

/* 
comments about board
  .  - empty field
  _  - never can visit it
  W  - white queen
  w  - white simple figure
  B  - black queen
  b  - black simple

for move_of
  0  - white
  1  - black
*/

TEST_CASE("Something works") { REQUIRE(not_random() == not_random()); }

TEST_CASE("small methoods"){
    Position test;
    test.set_board(gen_start(),true);
    REQUIRE(test.board == gen_start());
    CHECK(test.move_of == 0);

    auto test2 = test.copy();
    CHECK(test.board == test2.board);
    CHECK(test.move_of == test2.move_of);
    
    test.move({0, 2, 1, 3});
    CHECK(test2.board != test.board);
    CHECK(test2.move_of != test.move_of);
}

TEST_CASE("Moving-simple-taking"){
    Position test;
    std::string board =   "._._._.__._._._.._._._.__._w_._.._._b_.__._._._.._._._.__._._._.";
    std::string board2 =  "w_._._.__b_._._.._._._.__._b_._.._._b_.__._._._.._._._.__._._._.";
    std::string board3 =  "._._._.__b_b_._.._._._.__b_._b_.._._._.__b_b_b_.w_._._.__._._._.";

    std::string result =  "._._._.__._._._.._._._.__._._._.._._._.__._._w_.._._._.__._._._.";
    std::string result2 = "._._._.__._._._.._._._.__._._._.._._._.__._._._.._._._w__._._._.";
    std::string result31 = "._._._.__._._._.w_._._.__b_._._.._._._.__._._._.._._._.__._._._.";
    std::string result32 = "._._._.__b_b_._.._._._.__b_._b_.._._._w__._._._.._._._.__._._._.";
    std::string result33 = "._._._.__b_b_._.._._._.__b_._b_.._w_._.__._b_b_.._._._.__._._._.";


    test.set_board(board,true);
    test.move({3, 3, 5, 5});
    REQUIRE(test.board == result);
    // цвет передан чёрным
    CHECK(test.move_of == 1);
    test.set_board(board2,true);
    test.move({0, 0, 2, 2, 4, 4, 6, 6});
    REQUIRE(test.board == result2);
    CHECK(test.move_of == 1);
    test.set_board(board3,true);
    test.move({0, 6, 2, 4, 4, 6, 6, 4, 4, 2, 2, 0, 0, 2});
    REQUIRE(test.board == result31);
    CHECK(test.move_of == 1);
    // not full way
    test.set_board(board3,true);
    test.move({0, 6, 2, 4, 4, 6, 6, 4});
    REQUIRE(test.board == result32);
    test.set_board(board3,true);
    test.move({0, 6, 2, 4});
    REQUIRE(test.board == result33);
}

TEST_CASE("Moving-simple-taking-2"){
    // for black
    Position test;
    
    std::string board =  "._._._.__._._._.._._._.__._._._.._._._.__._w_._.._b_._.__._._._.";
    std::string board2 = "._._._.__._._b_.._._b_w__w_._._.._._._.__._._._.._._._.__._._._.";
    std::string board3 = "._._._.__w_w_._.b_._._.__._._._.._._._.__._._._.._._._.__._._._.";
    std::string board4 = "._._._.__._._W_.._._._.__._w_._.._._._.__w_._._.b_._._.__._._._.";


    std::string result = "._._._.__._._._.._._._.__._._._.._._b_.__._._._.._._._.__._._._.";
    std::string result2 = "._._._.__._._._.._._b_.__w_._._b._._._.__._._._.._._._.__._._._.";
    std::string result3 = "._._._.__._._._.._._B_.__._._._.._._._.__._._._.._._._.__._._._.";
    std::string result4 = "._._._B__._._._.._._._.__._._._.._._._.__._._._.._._._.__._._._.";



    test.set_board(board,false);
    test.move({2, 6, 4, 4});
    REQUIRE(test.board == result);
    CHECK(test.move_of == 0);
    // рубить назад
    test.set_board(board2,false);
    test.move({5, 1, 7, 3});
    REQUIRE(test.board == result2);
    CHECK(test.move_of == 0);
    // transform to queen by the way
    test.set_board(board3,false);
    test.move({0, 2, 2, 0, 4, 2});
    REQUIRE(test.board == result3);

    // long way to queen
    test.set_board(board4,false);
    test.move({0, 6, 2, 4, 4, 2, 6, 0});
    REQUIRE(test.board == result4);

}

TEST_CASE("Moving-simple-not_tacking"){
    Position test;

    std::string board1 = "._._._.__w_._._.._._._.__._._._.._._._.__._._._.._._._.__b_._._.";

    std::string result1 = "._._._.__._._._.._w_._.__._._._.._._._.__._._._.._._._.__b_._._.";
    std::string result2 = "._._._.__._._._.._w_._.__._._._.._._._.__._._._.._b_._.__._._._.";

    test.set_board(board1,true);
    test.move({1, 1, 2, 2});
    REQUIRE(test.board == result1);
    CHECK(test.move_of == 1);
    test.move({1, 7, 2, 6});
    REQUIRE(test.board == result2);
    CHECK(test.move_of == 0);

    test.set_board(gen_start(), true);
    test.move({0,2, 1,3});
    std::string result =  "w_w_w_w__w_w_w_w._w_w_w__w_._._.._._._.__b_b_b_bb_b_b_b__b_b_b_b";
    REQUIRE(test.board == result);
    // create queen
    board1 = "._._._.__._._._b._._._.__._._._.._._._.__._._._.._._._.__._._._.";
    result1 = "._._._B__._._._.._._._.__._._._.._._._.__._._._.._._._.__._._._.";
    test.set_board(board1,false);
    test.move({7,1, 6, 0});
    CHECK(test.board == result1);
}

TEST_CASE("Short game"){
    unsigned short int color = 0;
    std::vector<std::vector<int>> moves = {
        {4,2, 3,3},
        {3,5, 4,4},
        {3,1, 4,2},
        {4,6, 3,5},
        {4,2, 5,3},
        {3,5, 2,4},
        {5,3, 3,5, 1,3}
    };
    Position game;
    game.set_board(gen_start(),true);

    for (auto move:moves){
        game.move(move);
        color = 1 - color;
        CHECK(game.move_of == color);
    }
    std::string result = "w_w_w_w__w_._w_ww_w_._w__w_w_._.._._._.__b_._b_bb_b_._b__b_b_b_b";
    REQUIRE(game.board == result);
}

TEST_CASE("Moving-queen"){
    Position test ;
    std::string board =  "._._._.__b_._._.._._._.__._._._.._._._.__._._._.._W_._.__._._._.";
    std::string result = "._._._.__b_._._.._._._.__._._W_.._._._.__._._._.._._._.__._._._.";

    test.set_board(board,true);
    test.move({2, 6, 5, 3});
    CHECK(test.board == result);
    CHECK(test.move_of == 1);

    board = "._._._.__._B_._.._._._.__._._._.._._._.__._._._.._._._.__._._._W";
    result = "._._B_.__._._._.._._._.__._._._.._._._.__._._._.._._._.__._._._W";
    
    test.set_board(board,false);
    test.move({3,1, 4,0});
    CHECK(test.board == result);
    CHECK(test.move_of == 0);

    test.move({7,7, 0,0});
    result = "W_._B_.__._._._.._._._.__._._._.._._._.__._._._.._._._.__._._._.";
    CHECK(result == test.board);
}

TEST_CASE("find all moves of simple figure"){
    Position test;

    test.set_board(gen_start(),true);
    std::vector<std::vector<int>> result = {{0,2, 1,3}};
    std::vector<std::vector<int>> found = test.fig_moves(0,2);
    CHECK(found == result);
    CHECK(test.board == gen_start());
    CHECK(test.move_of == 0);

    result = {{6,2, 5,3},{6, 2, 7, 3}};
    found = test.fig_moves(6,2);
    CHECK(contains_all(found, result));
    CHECK(test.board == gen_start());
    CHECK(test.move_of == 0);

    // test new function for tests
    result = { {6, 2, 7, 3},{6,2, 5,3}};
    CHECK(contains_all(found,result));
    found.push_back({1,1, 2,2});
    CHECK(not contains_all(found,result));
    found = {{6,2, 5,3}};
    CHECK(not contains_all(found,result));

    // if can't move
    result = {};
    CHECK(test.fig_moves(2,6) == result);
    CHECK(test.board == gen_start());

    std::string board =  "._._._.__._._._.._._._.__._b_._.._._._.__._._w_.._._._.__._._._.";
    std::vector<std::vector<int>> result1 = {{3,3, 2,2}, {3,3, 4,2}};
    std::vector<std::vector<int>> result2 = {{5,5,6,6}, {5,5, 4,6}};
    test.set_board(board,false);
    CHECK(contains_all(test.fig_moves(3,3), result1));
    CHECK(contains_all(test.fig_moves(5,5), result2));
}

TEST_CASE("all moves of simple figure with taking"){
    Position test;
    std::string board = "._._._.__._._._.._w_._.__b_._._.._._._.__b_._._.._._._.__._._._.";
    std::vector<std::vector<int>> result = {{2,2, 0,4}, {2,2,3,3}, {2,2, 0,4, 2,6}};
    test.set_board(board,true);
    std::vector<std::vector<int>> found = test.fig_moves(2,2);
    CHECK(contains_all(found, result));
    CHECK(test.move_of == 0);

    // with cycle
    board = "._._._.__b_b_._.._._._.__b_b_._.._._._.__b_._._.w_._._.__._._._.";
    test.set_board(board,true);

    result = {
        {0,6, 1,7},
        {0,6, 2,4},
        {0,6, 2,4, 0,2},
        {0,6, 2,4, 0,2, 2,0},
        {0,6, 2,4, 0,2, 2,0, 4,2},
        {0,6, 2,4, 0,2, 2,0, 4,2, 2,4},
        {0,6, 2,4, 4,2},
        {0,6, 2,4, 4,2, 2,0},
        {0,6, 2,4, 4,2, 2,0, 0,2},
        {0,6, 2,4, 4,2, 2,0, 0,2, 2,4}
    };

    found = test.fig_moves(0,6);
    CHECK(contains_all(found,result));

    // when free == false, figure can only take enemy
    board =  "._._._.__._._._.._._w_.__._._._.._._._.__._._._.._._._.__._._._.";
    result = {};
    test.set_board(board,true);
    CHECK(test.fig_moves(4,2,false) == result);

}

TEST_CASE("all moves of queen"){
    Position test;

    std::string board =  "._._._.__._._._.._._._.__._._B_.._._._.__._._._.._._._.__._._._.";
    std::vector<std::vector<int>> result = {
        {5,3, 6,4},        {5,3, 7,5},
        {5,3, 4,4},        {5,3, 3,5},
        {5,3, 2,6},        {5,3, 7,1},
        {5,3, 1,7},        {5,3, 4,2},
        {5,3, 3,1},        {5,3, 2,0},
        {5,3, 6,2}
    };

    test.set_board(board,true);
    auto found = test.fig_moves(5,3);
    CHECK(contains_all(found,result));
    CHECK(test.board == board);
    CHECK(test.move_of==0);

    board = "._._W_.__._._._.._._._.__._._._.._._._.__._._._.._._._.__._._._.";
    result = {
        {4,0, 3,1},        {4,0, 2,2},
        {4,0, 1,3},        {4,0, 0,4},
        {4,0, 5,1},        {4,0, 6,2},
        {4,0, 7,3}
    };
    test.set_board(board,true);
    found = test.fig_moves(4,0);
    CHECK(contains_all(found,result));
    CHECK(test.board == board);
    // can use the result in future
    test.move(found[4]);
}

TEST_CASE("all moves queen taking"){
    Position test;

    auto board = "._._._.__._._._.._._w_.__w_._B_.._._w_.__._._._.._._w_.__._._._.";
    std::vector<std::vector<int>> result = {
        {5,3, 6,2},
        {5,3, 7,1},
        {5,3, 6,4},
        {5,3, 7,5},
        {5,3, 3,1},
        {5,3, 2,0},
        {5,3, 3,1, 0,4},
        {5,3, 3,5},
        {5,3, 3,5, 0,2},
        {5,3, 3,5, 5,7},
        {5,3, 2,6},
        {5,3, 1,7}
    };
    test.set_board(board,false);
    auto found = test.fig_moves(5,3);
    CHECK(test.board == board);
    CHECK(contains_all(found,result));
}

TEST_CASE("Large queen moves"){
    Position test;
    auto board = "._._._.__w_._._.._w_w_w__._._B_.._w_w_w__._._._.._w_w_w__._._._.";
    test.set_board(board,false);
    auto start = std::chrono::steady_clock::now();
    auto found = test.fig_moves(5,3);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    double time = elapsed_seconds.count();
    CHECK(time < 0.002);
    // я вручную не смог все варианты найти, я тестирую время
    // там больше 40 вариантов
    CHECK(test.board == board);
}

TEST_CASE("Simple search"){
    Position test;
    Searcher analizer;

    auto board =  "._._._.__._._._.._._._.__._._._.._._w_.__._._._.._._._b__b_._._.";
    std::vector<int> result = {1, 7, 2, 6};
    
    test.set_board(board,false);
    auto found = analizer.find(test,3);
    CHECK(found == result);
    CHECK(test.board == board);
    CHECK(test.move_of == 1);

    // taking
    board = "._._._.__._._._.._._._.__._._._.._._w_.__._._b_.._._._.__._._._.";
    test.set_board(board,false);
    found = analizer.find(test,3);
    result = {5,5, 3,3};
    CHECK(result == found);
    CHECK(test.board == board);
    // queen have big weight
    // i test, that searcher prefer take and vin, not transforming to queen
    board = "._._._.__._b_._.._w_._.__._._._.._._._.__._._._.._._._.__._._._.";
    result = {3,1,1,3};
    test.set_board(board,false);
    found = analizer.find(test,3);
    CHECK(found == result);
}

TEST_CASE("Large search"){
    Position test;
    Searcher analizer;
    auto board = "._._._.__w_._._.._w_w_w__._._B_.._w_w_w__._._._.._w_w_w__._._._.";
    test.set_board(board,false);
    auto start = std::chrono::steady_clock::now();
    std::vector<int> found = analizer.find(test,3);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    double time = elapsed_seconds.count();
    CHECK(time<1);
    CHECK(test.board == board);
}

TEST_CASE("Searcher's things"){
    Position test;
    Searcher analizer;
    std::string board = "._._._.__w_._._.._w_w_w__._._B_.._w_w_w__._._._.._w_w_w__._._._.";
    test.set_board(board,true);
    CHECK(analizer.estimate_board(board,true) == 0);
    CHECK(test.board == board);
    board = "._._._.__._._._.._._._.__._._._.._._w_.__._._._.._._._b__b_._._.";
    CHECK(analizer.estimate_board(board,false) > 0);
}

TEST_CASE("all moves mixed figures with taking"){
    Position test;
    std::string board = "._._._.__w_._._.b_._._.__._._w_.._._._.__._._._.._._._.__._._._.";
    test.set_board(board,false);
    std::vector<std::vector<int>> result = {
        {0,2,2,0},
        {0,2,2,0,6,4},
        {0,2,2,0,7,5}
    };
    CHECK(contains_all(test.fig_moves(0,2), result));

    board = "._._._.__._._._.._._._.__._._._.._._._b__w_._._.._b_._.__._._._.";
    result = {
        {1,5,0,6},
        {1,5, 3,7},
        {1,5, 3,7, 7,3}
    };
    test.set_board(board,true);
    CHECK(contains_all(test.fig_moves(1,5), result));
}

TEST_CASE("all moves of queen with taking"){
    Position test;
    auto board =  "._._._.__._._._.._._._.__._._._.W_._._b__._._._.._b_._.__._._._.";
    test.set_board(board,true);
    std::vector<std::vector<int>> rezult = {
        {0,4,3,7,7,3},
        {0,4,3,7},
        {0,4,1,5},
        {0,4,1,3},
        {0,4,2,2},
        {0,4,3,1},
        {0,4,4,0}
    };
    CHECK( contains_all(test.all_moves(true),rezult));
    CHECK(test.board == board);
}


TEST_CASE("Short game with stupid player"){
    Position position;
    Searcher analizer;

    position.set_board(gen_start(),true);
    auto found = position.all_moves(true);

    for (int i=0;i<10;i++){
        found = position.all_moves(true);
        position.move(found[0]);
        position.move(analizer.find(position,5));

    }
    CHECK(analizer.estimate_board(position.board,false)>=0);

    // random move

    position.set_board(gen_start(),true);
    std::random_device rd;   
    std::mt19937 gen(rd());  
    std::uniform_int_distribution<> dist(1,100); // distribute results between 1 and 6 inclusive.
    int n = dist(gen);


    for (int i=0;i<10;i++){
        // choose random
        found = position.all_moves(true);
        int d = len(found);
        n = dist(gen);
        while (n>d-1) n = dist(gen);
        // my move
        position.move(found[n]);
        // bot move
        position.move(analizer.find(position,5));

    }
    CHECK(analizer.estimate_board(position.board,false)>=0);

}







