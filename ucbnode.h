#ifndef UCBNODE_H
#define UCBNODE_H
#include "board.h"
#include <math.h>

#define basenum 0
#define ravenum 20

const double UCB_WEIGHT = 0.25;
class ucbnode {
    public:
        char place;
        bool color;
        double count, mean;
        ucbnode* childptr;
        double lnc;
        int childnum;
        //double ravecount;
        //double ravemean;
        //char child[BOARDSSIZE + 1]; //find children at specific position

        ucbnode(){}
        ~ucbnode(){
            if(childptr != NULL){
                delete [] childptr;
                childptr = NULL;
            }
        }
        void setlnc(){
            lnc = log(count);
        }
        void initucbnode(int i, bool j){
            place = i;
            color = j;
            mean = 0.5;
            count = basenum;
            childnum = 0;
            childptr = NULL;
            lnc = 1;
            //ravecount = rc;
            //ravemean = rm;
            //memset(child, -1, sizeof(child));
        }
        void update(double result, int thread_num){
            if(color == BLACK){
                mean = (mean * count + result) / (count + thread_num);
            }
            else{
                mean = (mean * count + thread_num - result) / (count + thread_num);
            }
            count += thread_num;
            setlnc();
        }
        /*void updaterave(double result, int thread_num){
            if(color == BLACK){
                ravemean = (ravemean * ravecount + result) / (ravecount + thread_num);
            }
            else{
                ravemean = (ravemean * ravecount + thread_num - result) / (ravecount + thread_num);
            }
            ravecount+= thread_num;
        }*/

        void expansion(board &b){
            int k = 0;
            bool j = !b.just_play_color();
            childnum = 0;
            for(int i = 0; i < BOARDSSIZE; i++){
                if(b.check(i, j)){
                    childnum++;
                }
            }
            if(childnum == 0){
                return;
            }
            childptr = new ucbnode [childnum];
            //double rmean, rcount;
            for(int i = 0; i < BOARDSSIZE; i++){
                if(b.check(i, j)){
                    //child[i] = k;
                    //rmean = 0.5;
                    //rcount = ravenum;
                    childptr[k].initucbnode(i, j);
                    k++;
                }
            }
        }
        //return the best node position within children
        int getbestmove(){
            int res = -1;
            double ans = -100, tmp;
            for(int i = 0; i < childnum; i++){
                tmp = childptr[i].count;
                if(tmp > ans){
                    ans = tmp;
                    res = i;
                }
            }
            return res;
        }
        //返回mcts判斷該盤面各點落子的好壞比例
        vector<float> getPolicy(){
            vector<float> ret(BOARDSSIZE, 0);
            float sum = 0;
            for(int i = 0; i < childnum; i++){
                sum += childptr[i].count;
            }
            for(int i = 0; i < childnum; i++){
                ret[childptr[i].place] = childptr[i].count / sum;
            }
            return ret;
        }

        

};
#endif