#ifndef UCBNODE_H
#define UCBNODE_H
#include "board.h"
#include <math.h>
#include <pthread.h> //*
#define basenum 0
#define ravenum 20

const double UCB_WEIGHT = 0.25;
class ucbnode
{
public:
    char place;
    bool color;
    double count, mean;
    ucbnode *childptr;
    double lnc;
    int childnum;
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; //*
    ucbnode()
    {
    }
    ~ucbnode()
    {
        if (childptr != NULL)
        {
            delete[] childptr;
            childptr = NULL;
        }
    }
    void setlnc()
    {
        lnc = log(count);
    }
    void initucbnode(int i, bool j)
    {
        place = i;
        color = j;
        mean = 0.5;
        count = basenum;
        childnum = 0;
        childptr = NULL;
        lnc = 1;
    }
    void update(double result)
    {
        pthread_mutex_lock(&lock);
        if ((color == BLACK))
        {
            mean = (mean * count + result) / (count + 1);
        }
        else
        {
            mean = (mean * count + 1 - result) / (count + 1);
        }
        count += 1;
        setlnc();
        pthread_mutex_unlock(&lock);
    }

    void expansion(board &b)
    {
        pthread_mutex_lock(&lock);
        int k = 0;
        bool j = !b.just_play_color();
        childnum = 0;
        for (int i = 0; i < BOARDSSIZE; i++)
        {

            if (b.check(i, j))
            {
                childnum++;
            }
        }
        if (childnum == 0)
        {
            pthread_mutex_unlock(&lock);
            return;
        }
        childptr = new ucbnode[childnum];
        for (int i = 0; i < BOARDSSIZE; i++)
        {
            if (b.check(i, j))
            {
                childptr[k].initucbnode(i, j);
                k++;
            }
        }
        pthread_mutex_unlock(&lock);
    }
    //return the best node position within children
    int getbestmove()
    {
        int res = -1;
        double ans = -100, tmp;
        for (int i = 0; i < childnum; i++)
        {
            tmp = childptr[i].count;
            if (tmp > ans)
            {
                ans = tmp;
                res = i;
            }
        }
        return res;
    }
    //返�?mcts?�斷該盤?��?點落子�?好�?比�?
    vector<float> getPolicy()
    {
        vector<float> ret(BOARDSSIZE, 0);
        float sum = 0;
        for (int i = 0; i < childnum; i++)
        {
            sum += childptr[i].count;
        }
        for (int i = 0; i < childnum; i++)
        {
            ret[childptr[i].place] = childptr[i].count / sum;
        }
        return ret;
    }
};
#endif