#include "board.h"

const boardcn board::nb;

board::board(){}

board::board(bitboard bb, bitboard ww){
    Initboard(bb, ww);
}

void board::Initboard(bitboard bb, bitboard ww){
    bitb[0] = bb;
    bitb[1] = ww;
    setData();
}

inline bool board::get(int i, bool j){
    return bitb[j].get(i);
}

void board::setData(){
    int i, j, k;
    memset(liberty, 0, sizeof(liberty));
    for(i = 0; i < BOARDSSIZE; i++){
        parent[i] = i;
    }
    for(i = 0; i < BOARDCUL; i++){
        for(j = 1; j < BOARDCUL; j++){
            k = i * BOARDCUL + j;
            if(bitb[0].get(k) && bitb[0].get(k - 1)){
                parent[k] = parent[k - 1];
            }
            else if(bitb[1].get(k) && bitb[1].get(k - 1)){
                parent[k] = parent[k - 1];
            }
        }
    }
    for(j = 0; j < BOARDCUL; j++){
        for(i = 1; i < BOARDCUL; i++){
            k = i * BOARDCUL + j;
            if(bitb[0].get(k) && bitb[0].get(k - BOARDCUL)){
                unite(k, k - BOARDCUL);
            }
            else if(bitb[1].get(k) && bitb[1].get(k - BOARDCUL)){
                unite(k, k - BOARDCUL);
            }
        }
    }
    getAllLiberty();
}

void board::getAllLiberty(){
    int i, j, t, u, r, d, l;
    memset(liberty, 0, sizeof(liberty));
    memset(countliberty, 0, sizeof(countliberty));
    for(i = 0; i < BOARDCUL; i++){
        for(j = 0; j < BOARDCUL; j++){
            u = r = d = l = -1;
            t = i * BOARDCUL + j;
            if(!bitb[0].get(t) && !bitb[1].get(t)){ //?¥ç„¡å­?ä»?¡¨?‰å¯?½æ??ºæ°£
                if(i != 0){
                    if(bitb[0].get(t - BOARDCUL) || bitb[1].get(t - BOARDCUL)){
                        u = findParent(t - BOARDCUL);
                        liberty[u].add(t);
                    }
                }
                if(i != (BOARDCUL - 1)){
                    if(bitb[0].get(t + BOARDCUL) || bitb[1].get(t + BOARDCUL)){
                        d = findParent(t + BOARDCUL);
                        if(d != u) liberty[d].add(t);
                    }
                }
                if(j != 0){
                    if(bitb[0].get(t - 1) || bitb[1].get(t - 1)){
                        r = findParent(t - 1);
                        if(r != u && r != d) liberty[r].add(t);
                    }
                }
                if(j != (BOARDCUL - 1)){
                    if(bitb[0].get(t + 1) || bitb[1].get(t + 1)){
                        l = findParent(t + 1);
                        if(l != u && l != d && l != r) liberty[l].add(t);
                    }
                }
            }
        }
    }
    for(i = 0; i < BOARDSSIZE; i++){
        if(bitb[0].get(i) || bitb[1].get(i)){
            if(parent[i] == i) countliberty[i] = liberty[i].count();
        }
    }
}

int board::findParent(int i){  //?¾å?å¡Šç›¸????€å·¦ä?é»?    
    char &j = parent[i];
    if(j == parent[j]) return j;
    return j = findParent(j);
}

void board::unite(int x, int y){   //?Šå…©?€å¡Šç›¸??µ·ä¾??´æ”¹?€å¡Šå·¦ä¸Šé?
    char i = findParent(x), j = findParent(y);
    if(i < j) parent[j] = i;
    else parent[i] = j;
}

void board::add(int i, bool j){    //place a stone
    int k, l, lp;
    bitboard tmp;
    ban[0].add(i);
    ban[1].add(i);
    bitb[j].add(i);
    for(k = 0; k < nb.nbsize[i]; k++){
        l = nb.nb[i][k];
        if(bitb[j].get(l)){
            lp = findParent(l);
            tmp |= liberty[lp];
            liberty[lp].clear();
            countliberty[lp] = 0;
            unite(i, lp);
        }
        else if(bitb[!j].get(l)){
            lp = findParent(l);
            liberty[lp].minus(i);
            countliberty[lp] = liberty[lp].count();
            if(countliberty[lp] == 1){
                ban[j] |= liberty[lp];
            }
        }
        else{
            tmp.add(l);
        }
    }
    tmp.minus(i);
    k = findParent(i);
    liberty[k] = tmp;
    countliberty[k] = liberty[k].count();
    if(countliberty[k] == 1){
        ban[!j] |= liberty[k];
    }
}

bool board::just_play_color(){     //...as the function name mean
    int bc = bitb[0].count();
    int wc = bitb[1].count();
    if(bc == wc) return 1;
    return 0;
}

bool board::check(int i, bool j){  //check if pos i can place a stone
    int l, k;
    if(ban[j].get(i)) return false;
    for(k = 0; k < nb.nbsize[i]; k++){
        l = nb.nb[i][k];
        if(bitb[j].get(l)){
            if(countliberty[findParent(l)] != 1) return true;
        }
        else if (!bitb[!j].get(l)){
            return true;
        }
    }
    ban[j].add(i);
    return false;
}

void board::clear(){    
    int i;
    for(i=0; i<BOARDSSIZE; i++){
        parent[i]=i;
    }
    bitb[0].clear();
    bitb[1].clear();
    ban[0].clear();
    ban[1].clear();
    memset(liberty, 0, sizeof(liberty));
    memset(countliberty, 0, sizeof(countliberty));
}

void board::showboard(){
    int i;
    for(i = 0; i < BOARDCUL * BOARDCUL; i++){
        if(bitb[0].get(i)){
            cout<<'@';
        }
        else if(bitb[1].get(i)){
            cout<<'O';
        }
        else{
            cout<<'.';
        }
        if(i % BOARDCUL == BOARDCUL - 1){
            cout<<endl;
        }
    }
}

void board::showParent(){
    for(int i = 0; i < BOARDCUL * BOARDCUL; i++){
        printf("%3d ", (int)parent[i]);
        if(i % BOARDCUL == BOARDCUL - 1){
            cout <<endl;
        }
    }
}

bool board::isempty(){
    return (bitb[0].isempty()) && (bitb[1].isempty());
}

bool board::check_is_end(){
    bool j = !just_play_color();
    for(int i = 0; i < BOARDSSIZE; i++){
        if(check(i, j)){
            return false;
        }
    }
    return true;
}
void board::getlegalmove(int blegal[BOARDSSIZE], int wlegal[BOARDSSIZE], int& blegalsize, int& wlegalsize){
    for(int i = 0; i < BOARDSSIZE; i++){
        if(!bitb[0].get(i) && !bitb[1].get(i)){
            if(check(i, 0)){
                blegal[blegalsize] = i;
                blegalsize++;
            }
            if(check(i, 1)){
                wlegal[wlegalsize] = i;
                wlegalsize++;
            }
        }
    }
}

int board::simulate(){
    int blegalsize = 0;
    int wlegalsize = 0;
    int blegal[BOARDSSIZE], wlegal[BOARDSSIZE];
    memset(blegal, 0, sizeof(blegal));
    memset(wlegal, 0, sizeof(wlegal));
    getlegalmove(blegal, wlegal, blegalsize, wlegalsize);
    bool j = !just_play_color();
    while(true){
        FLAG:
        if(j == BLACK){
            while(blegalsize > 0){
                int i = rand() % blegalsize;
                int k = blegal[i];
                blegal[i] = blegal[blegalsize - 1];
                blegalsize--;
                if(check(k, j)){
                    add(k, j);
                    j = !j;
                    goto FLAG;
                }
            }
            return 0;
        }
        else if(j == WHITE){
            while(wlegalsize > 0){
                int i = rand() % wlegalsize;
                int k = wlegal[i];
                wlegal[i] = wlegal[wlegalsize - 1];
                wlegalsize--;
                if(check(k, j)){
                    add(k, j);
                    j = !j;
                    goto FLAG;
                }
            }
            return 1;
        }
    }
}