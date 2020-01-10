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
#include<cstdint>
#include "board.h"
#include "MCTStree.h"
#include <algorithm>
#include <pthread.h>
using namespace std;
int number_of_cpu = 16;
pthread_mutex_t mutex;

string inttoGTPstring(int i)
{
	string s = "A1";
    s[0]+= i / 9;
    if(s[0]>='I')s[0]++;
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

void* root_pl(void* b){
    int start_time, end_time;
    MCTStree tree2;
    tree2.clear();
    tree2.reset(*(board*) b);
    start_time = clock();
    while(true){
        //cout << "func" << endl;
        tree2.MCTS();
        tree2.MCTS();
        tree2.MCTS();
        tree2.MCTS();
        tree2.MCTS();
        tree2.MCTS();
        tree2.MCTS();
        tree2.MCTS();
        tree2.MCTS();
        tree2.MCTS();
        //cout << "func2" << endl;
        //simulationcount += 10;
        end_time = clock();
        if((end_time - start_time) / CLOCKS_PER_SEC >= 1){
            break;
        }
    }
    //cout << "func3" << endl;
    int k = tree2.root -> getbestmove();
    //cout << "func4" << endl;
    ucbnode* tmp = tree2.root -> childptr;
    //cout << "func5" << endl;
    int best_move = (tmp + k) -> place;
    cout << "func6=====================" << best_move << endl;
    //tree2.clear();
    pthread_exit((void*)best_move);
    //return (void*)best_move;
}

int main(int argc, char** argv){
    int i;
    board* b = new board() ;
    bool j;
    double t;
    int simulationcntbound;
    void* ret ;
    vector<float> policy;
    tree.clear();
    cout<<"test1"<<endl;
    b->clear();
    srand(time(NULL));
    string s, c, p; //string, color, position
    if(argc == 2){
        istringstream(string(argv[1])) >> simulationcntbound;
    }
    
    cout<<"test2"<<endl;
    
    
    
    while(cin>>s){
        int* next_move = new int[81];
        for(int i=0;i<81;i++)next_move[i] = 0;
        //memset(next_move,0,sizeof(next_move));
        if(s == "play" || s == "p"){
            cin >> c >> p;
            if(c[0] == 'b' || c[0] == 'B') j = 0;
            else j = 1;
            b->add(GTPstringtoint(p), j);
            cout<<"="<<endl<<endl;
        }
        else if(s[0] == 'e'){
            cout<<b->isempty()<<endl<<endl;
        }
        else if(s[0] == 'c'){
            b->clear();
            cout<<"="<<endl<<endl;
        }
        else if(s[0]=='g' || s == "reg_genmove"){
            bool j, f = false;
            int best_move, start_time, end_time;
            cin >> c;
            j = !b->just_play_color();
            for(i = 0; i < BOARDSSIZE; i++){
                if(b->check(i, j)){
                    f = true;
                    break;
                }
            }
            if(!f){
                cout<<"=resign"<<endl<<endl;
                continue;
            }
            //tree.reset(*b);
            start_time = clock();
            int simulationcount = 0;
            cout << "test1" << endl;
            pthread_t threads[1];
            //pthread_mutex_init(&mutex, NULL);
            
            for(int i = 0; i < number_of_cpu ; i++){
                pthread_create (&threads[i], NULL, root_pl, (void*) b);
            }
            for(int i = 0;i < number_of_cpu; i++){
                
                pthread_join (threads[i], &ret);
                next_move[(intptr_t)ret]++;
            }
            //int next = max_element(next_move, next_move+81) - next_move;
            //cout << "Max element: " << max_element(next_move, next_move+81) - next_move << endl;
            
            //for (int i = 0; i < 81; i++) cout<<i<<"    "<<next_move[i]<<endl;
            //cout << distance(next_move, max_element(A,A+82));
            cout << "func8" << endl;
            best_move =  max_element(next_move, next_move+81) - next_move;
            
            
            //pthread_mutex_destroy(&mutex);
            cout << (int) best_move << endl;
            
            //int k = tree.root -> getbestmove();
            //ucbnode* tmp = tree.root -> childptr;
            //best_move = (tmp + k) -> place;
            //policy = tree.root -> getPolicy();
            cout << "func9" << endl;
            if(s != "reg_genmove"){
                b->add(best_move, !b->just_play_color());
                cout << "func10" << endl;
            }
            cout<<"="<<inttoGTPstring(best_move)<<endl<<endl;
            //tree.clear();
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
            b->showboard();
            cout<<endl;
        }
        else{
            cout<<"="<<endl<<endl;
        }
        //int next_move[82] = {};
         cout << "test10" << endl;
    }
}