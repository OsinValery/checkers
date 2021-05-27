#include <string>
#include <vector>
#include "position.hpp"

/**
 * \brief класс реализует логику поиска лучшего хода за чёрных(для бота)
 */
class Searcher{
    public:
        /**
         * ценность фигур, дамки и обычной
         */
        int queen_weight = 10;
        /**
         * ценность фигур, дамки и обычной
         */
        int simple_weight = 1;

        /** 
         * \brief самая главная функция, она ищет лучший ход за чёрных
         * \param position позиция, которую оцениваем
         * \param h - высота дерева вариантов, которое строится во время поиска \n
         * должен быть нечётным числом >= 1 \n
         * будет произведён поиск на h/2 ходов вперёд, например 3 для h = 5
         */
        std::vector<int> find(Position position, int h);

        /**
         * \brief на листьях дерева позицию оценивает \n он 
         * умножает ценность фигуры на вес поля для всех фигур
         * \param board доска для оценки
         * \param color с точки зрения какого цвета оценивать доску \n true для белых
         */
        long int estimate_board(std::string board,bool color);

        /**
         * \brief строит дерево для данного хода
         * \param position  -текущая позиция в узле дерева
         * \param move - исследуемых ход - ребро дерева
         * \param h - высота узла до листа, при h = 1 возвращается estimate_board()
         */
        long int analize_move(Position position, std::vector<int> move, int h);
};

