#ifndef BOARD_H
#define BOARD_H
#include "bitboard.h"
#include "boardcn.h"
#include<iostream>
#include<map>
#include<string.h>
#include<vector>
#include<stdio.h>
#include<string>
#include<stdlib.h>
#include<math.h>

#define BLACK 0
#define WHITE 1

using namespace std;

class board{
    public:
        bitboard ban[2];    //speed up check process
        bitboard bitb[2];

        const static boardcn nb;

        char parent[BOARDCUL * BOARDCUL];
        bitboard liberty[BOARDCUL * BOARDCUL];
        char countliberty[BOARDSSIZE];

        board();
        board(bitboard bb, bitboard ww);

        void Initboard(bitboard bb, bitboard ww);
        inline bool get(int i, bool j);
        void setData();
        void getAllLiberty();
        int findParent(int i);
        void unite(int x, int y);
        void add(int i, bool j);
        bool just_play_color();
        bool check(int i, bool j);
        void clear();
        void showboard();
        void showParent();
        bool isempty();
        bool check_is_end();
        void getlegalmove(int blegal[BOARDSSIZE], int wlegal[BOARDSSIZE], int& blegalsize, int& wlegalsize);
        int simulate();
};
#endif