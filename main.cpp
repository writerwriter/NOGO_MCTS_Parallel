#include<iostream>
#include<set>
#include<map>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<time.h>
#include<string.h>
#include<sstream>
#include "board.h"
#include "MCTStree.h"
using namespace std;


string inttoGTPstring(int i)
{
	string s = "A1";
    s[0]+= i / 9;
    if(s[0]>'I')s[0]++;
    s[1]+= i % 9;
    return s;
}

int GTPstringtoint(string s){
    if(s[0] >= 'a' && s[0] <= 'z'){
        int x = s[0] - 'a';
        if(s[0] > 'i') x--;
        int y = s[1] - '1';
        return x * BOARDCUL + y;
    }
    else{
        int x = s[0] - 'A';
        if(s[0] > 'I') x--;
        int y =s[1] - '1';
        return x * BOARDCUL + y;
    }
}

MCTStree tree;

int main(int argc, char** argv){
    int i;
    board b;
    bool j;
    double t;
    int simulationcntbound;
    vector<float> policy;
    tree.clear();
    b.clear();
    srand(time(NULL));
    string s, c, p; //string, color, position
    if(argc == 2){
        istringstream(string(argv[1])) >> simulationcntbound;
    }

    while(cin>>s){
        if(s == "play" || s == "p"){
            cin >> c >> p;
            if(c[0] == 'b' || c[0] == 'B') j = 0;
            else j = 1;
            b.add(GTPstringtoint(p), j);
            cout<<"="<<endl<<endl;
        }
        else if(s[0] == 'e'){
            cout<<b.isempty()<<endl<<endl;
        }
        else if(s[0] == 'c'){
            b.clear();
            cout<<"="<<endl<<endl;
        }
        else if(s[0]=='g' || s == "reg_genmove"){
            bool j, f = false;
            int best_move, start_time, end_time;
            cin >> c;
            j = !b.just_play_color();
            for(i = 0; i < BOARDSSIZE; i++){
                if(b.check(i, j)){
                    f = true;
                    break;
                }
            }
            if(!f){
                cout<<"=resign"<<endl<<endl;
                continue;
            }
            tree.reset(b);
            start_time = clock();
            int simulationcount = 0;
            while(true){
                tree.MCTS();
                tree.MCTS();
                tree.MCTS();
                tree.MCTS();
                tree.MCTS();
                tree.MCTS();
                tree.MCTS();
                tree.MCTS();
                tree.MCTS();
                tree.MCTS();
                simulationcount += 10;
                end_time = clock();
                /*if(simulationcount % 10000 == 0){
                    tree.show_path();
                }*/
                if((end_time - start_time) / CLOCKS_PER_SEC >= 1){
                    break;
                }
            }
            int k = tree.root -> getbestmove();
            ucbnode* tmp = tree.root -> childptr;
            best_move = (tmp + k) -> place;
            policy = tree.root -> getPolicy();

            if(s != "reg_genmove"){
                b.add(best_move, !b.just_play_color());
            }
            cout<<"="<<inttoGTPstring(best_move)<<endl<<endl;
            tree.clear();
        }
        else if(s == "policy"){
            for(int i = 0; i < 9; i++){
                for(int j = 0; j < 9; j++){
                    cout << policy[i * 9 + j] << ' ';
                }
                cout << endl;
            }
        }
        /*else if(s == "value"){
            cout<<value<<endl;
        }*/
        else if(s == "protocol_version"){
            cout << "=2\n\n";
        }
        /*
        else if(s == "rev"){

        }*/
        else if(s == "name"){
            cout<<"=random_agent"<<endl<<endl;
        }
        else if(s == "time"){
            cin >> t;
            t *= 1000;
            t++;
            cout<<"="<<endl<<endl;
        }
        else if(s == "boardsize"){
            cin>>s;
            cout<<"="<<endl<<endl;
        }
        else if(s == "komi"){
            cin>>s;
            cout<<"="<<endl<<endl;
        }
        else if(s == "time_left"){
            cin>>s>>s>>s;
            cout<<"="<<endl<<endl;
        }
        else if(s == "showboard" || s == "sb"){
            b.showboard();
            cout<<endl;
        }
        else{
            cout<<"="<<endl<<endl;
        }
    }
}