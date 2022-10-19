#pragma once

#include <SFML/Graphics.hpp>
#include "macros.hpp"

class TetrisLoader
{
    sf::RenderWindow& tetris_window;

    static constexpr int M = 20;
    static constexpr int N = 10;

    int field[M][N] = {0};

    struct Point
    {
        int x,y;
    } a[4], b[4];

    int figures[7][4] =
    {
        1,3,5,7, // I
        2,4,5,7, // Z
        3,5,4,6, // S
        3,5,4,7, // T
        2,3,5,7, // L
        3,5,7,6, // J
        2,3,4,5, // O
    };

    bool check()
    {
        for (int i=0;i<4;i++)
	        if (a[i].x<0 || a[i].x>=N || a[i].y>=M) 
                return 0;
        else if (field[a[i].y][a[i].x]) 
            return 0;

        return 1;
    }
    public:
        TetrisLoader(sf::RenderWindow& win);
        void start();
};