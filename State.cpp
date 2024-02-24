#include "State.h"
#include "Block.h"

extern int blockcount;

//一个函数中生成多少个块
string state_make_random() {
    string res;

    //生成随机数量的块
    int tmp = rand() % 3 + 1;
    for (int i = 0; i < tmp; i++) {
        int t = rand() % 3;
        //在生成每个块前都将深度清空，保证每个块都可以最深为5
        blockcount = 0;
        res += block_make_random();
    }
    return res;
}

