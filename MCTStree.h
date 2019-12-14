#ifndef MCTS_H
#define MCTS_H
#include "ucbnode.h"

class MCTStree{
    public:
        ucbnode* root;
        int selectlist[BOARDSSIZE]; //同分的node
        int selectsize; //同分的child有多少
        vector<ucbnode*> path;
        board rboard; //root board
        double rave_num[2][BOARDSSIZE];
        double rave_win_num[2][BOARDSSIZE];


        MCTStree(){}
        ~MCTStree(){}

        void clear(){
            if(root != NULL) delete root;
        }
        
        double getscore(ucbnode* nodeptr, int child){
            ucbnode* tmp = (nodeptr->childptr) + child;
            double &count = tmp -> count;
            double &rave_count = tmp -> ravecount;
            double temp = tmp -> ravemean * rave_count + tmp -> mean * count + sqrt(nodeptr -> lnc * count) * UCB_WEIGHT;
            return temp / (count + rave_count);
        }

        ucbnode* getbestchild(ucbnode* nodeptr){
            if(nodeptr -> childnum == 0) return NULL;
            double ans, tmp;
            ans = tmp = getscore(nodeptr, 0);
            selectlist[0] = 0;
            selectsize = 1;
            for(int i = 1; i < (nodeptr->childnum); i++){
                tmp = getscore(nodeptr, i);
                double diff = tmp - ans; //+-0.0001視為相等 高於0.0001表示新的node更好
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
            int best = selectlist[rand() % selectsize]; //隨機選一個value相等的node當作best node
            return nodeptr -> childptr + best;
        }

        void select(board &b){
            ucbnode* nodeptr = root;
            b.bpsize=0;
	        b.wpsize=0;
            path.clear();
            path.push_back(nodeptr);
            while(nodeptr -> childptr != NULL && nodeptr -> childnum != 0){
                nodeptr = getbestchild(nodeptr);
                path.push_back(nodeptr);
                if(nodeptr -> color == BLACK){
                    b.addbp(nodeptr -> place);
                }
                else{
                    b.addwp(nodeptr -> place);
                }
                b.add(nodeptr -> place, nodeptr -> color);
            }
        }

        void update(double result, board& b){
            for(int i = 0; i < path.size(); i++){
                path[i] -> update(result);
                if(path[i] -> color == 0){
                    for(int j = 0; j < b.wpsize; j++){
                        int k = (path[i] -> child[b.wpath[j]]);
                        //printf("%d ", path[i] -> childptr == NULL);
                        if(k != -1) ((path[i] -> childptr) + k) -> updaterave(result);
                    }
                }
                else{
                    for(int j = 0; j < b.bpsize; j++){
                        int k = (path[i] -> child[b.bpath[j]]);
                        if(k != -1) ((path[i] -> childptr) + k) -> updaterave(result);
                    }
                }
            }
        }

        void MCTS(){
            board b;
            double result;
            b = rboard;
            select(b); //get to the best leaf node
            ucbnode &last = (*(path.back()));
            ucbnode *nodeptr;
            if(last.childnum == 0 && last.count > basenum){
                last.expansion(b, rave_num, rave_win_num);
                if(last.childnum != 0){
                    nodeptr = getbestchild(&last);
                    path.push_back(nodeptr);
                    if(nodeptr -> color == BLACK){
                        b.addbp(nodeptr -> place);
                    }
                    else{
                        b.addwp(nodeptr -> place);
                    }
                    b.add(nodeptr -> place, nodeptr -> color);
                }
            }
            if(b.check_is_end()){
                result = b.just_play_color() == BLACK ? 1 : 0;
            }
            else{
                result = b.simulate();
            }
            //b.showboard();
            update(result, b);
        }
        void reset(board &b){
            rboard = b;
            root = new ucbnode;
            root -> color = rboard.just_play_color();
            root -> place = 81;
            root -> count = basenum;
            root -> lnc = 1;
            memset(root -> child, -1, sizeof(root -> child));
            root -> expansion(b, rave_num, rave_win_num);
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