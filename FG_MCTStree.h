#ifndef MCTS_H
#define MCTS_H
#include "ucbnode.h"
#include <pthread.h>//*
#include <stdlib.h>
class MCTStree{
    public:
        pthread_mutex_t lock;//*
        ucbnode* root;
        int selectlist[BOARDSSIZE]; //?Œå??„node
        int selectsize; //?Œå??„child?‰å?å°?        
        //vector<ucbnode*> path;
        board rboard; //root board


        MCTStree(){}
        ~MCTStree(){}

        void clear(){
            if(root != NULL) delete root;
        }
        
        double getscore(ucbnode* nodeptr, int child){
            ucbnode* tmp = (nodeptr->childptr) + child;
            double &count = tmp -> count;
            return tmp->mean + UCB_WEIGHT * sqrt(nodeptr->lnc / (count + 1));
        }

        ucbnode* getbestchild(ucbnode* nodeptr){
            if(nodeptr -> childnum == 0) return NULL;
            double ans, tmp;
            ans = tmp = getscore(nodeptr, 0);
            selectlist[0] = 0;
            selectsize = 1;
            for(int i = 1; i < (nodeptr->childnum); i++){
                tmp = getscore(nodeptr, i);
                double diff = tmp - ans; //+-0.0001è¦–ç‚º?¸ç? é«˜æ–¼0.0001è¡¨ç¤º?°ç?node?´å¥½
                if(diff > -0.0001){
                    if(diff > 0.0001){
                        selectlist[0] = i;
                        selectsize = 1;
                        ans = tmp;
                    }
                    else{
                        selectlist[selectsize] = i;
                        selectsize++;
                    }
                }
            }
            int best = selectlist[rand() % selectsize]; //?¨æ??¸ä??‹value?¸ç??„node?¶ä?best node
            return nodeptr -> childptr + best;
        }

        void select(board &b,vector<ucbnode*> &path){
            ucbnode* nodeptr = root;
            path.clear();
            path.push_back(nodeptr);
            while(nodeptr -> childptr != NULL && nodeptr -> childnum != 0){
                nodeptr = getbestchild(nodeptr);
                path.push_back(nodeptr);
                b.add(nodeptr -> place, nodeptr -> color);
            }
        }

        void update(double result, board& b,vector<ucbnode*> &path){
            for(int i = 0; i < path.size(); i++){
                path[i] -> update(result);
            }
        }
        void MCTS(){
            unsigned long long  number_of_cpu=16;
            pthread_t* threads = (pthread_t*)malloc(number_of_cpu * sizeof(pthread_t));//*
            pthread_mutex_init(&lock, NULL);
            for (unsigned long long i = 0; i < number_of_cpu; i++) {
            		if (pthread_create(&threads[i], NULL, &MCTStree::send_wrapper, this)) {
            			  printf("Error: pthread_create()\n");
            			  exit(1);
      		      }              
	          }
            for (unsigned long long i = 0; i < number_of_cpu; i++) {
		            pthread_join(threads[i], NULL);
            }
            free(threads);
            //pthread_mutex_destroy(&lock);
        }
        static void* send_wrapper(void* object)
        {
            reinterpret_cast<MCTStree*>(object)->MCTSS();
            printf("1\n");
            return 0;
        }
        void MCTSS(){
            vector<ucbnode*> path;
            pthread_t   tid;
 
            tid = pthread_self();
            printf("%lu tid %lu (0x%lx)\n", (unsigned long)tid, (unsigned long)tid);
            printf("2\n");
            board b;
            double result;
            b = rboard;
            pthread_mutex_lock(&lock);
            printf("3\n");
            select(b,path); //get to the best leaf node
            pthread_mutex_unlock(&lock);
            ucbnode &last = (*(path.back()));
            ucbnode *nodeptr;
            pthread_mutex_lock(&lock);
            if(last.childnum == 0 && last.count > basenum){
                
                last.expansion(b);
                
                if(last.childnum != 0){
                    nodeptr = getbestchild(&last);
                    path.push_back(nodeptr);
                    b.add(nodeptr -> place, nodeptr -> color);
                }
            }
            pthread_mutex_unlock(&lock);
            if(b.check_is_end()){
                result = b.just_play_color() == BLACK ? 1 : 0;
            }
            else{
                result = b.simulate();
            }
            pthread_mutex_lock(&lock);
            update(result, b,path);
            printf("end\n");
            pthread_mutex_unlock(&lock);
        }
        void reset(board &b){
            rboard = b;
            root = new ucbnode;
            root -> color = rboard.just_play_color();
            root -> place = 81;
            root -> count = basenum;
            root -> lnc = 1;
            root -> expansion(b);
        }
        void show_path(){
            ucbnode* nodeptr = root;
            int i = 0, k;
            while(nodeptr -> childptr != NULL && i < 10){
                k = nodeptr -> getbestmove();
                nodeptr = nodeptr-> childptr + k;
                i++;
                if(nodeptr != NULL){
                    if(nodeptr -> color == BLACK){
                        cout << "B(";
                    }
                    else cout <<"W(";
                    cout << inttostring(nodeptr -> place) << ") ";
                }
            }
            cout << endl;
        }
        string inttoGTPstring(int i)
        {
            string s = "A1";
            s[0]+= i / 9;
            if(s[0]>='I')s[0]++;
            s[1]+= i % 9;
            return s;
        }
        string inttostring(int i)
        {
            return inttoGTPstring(i);
        }
};

#endif