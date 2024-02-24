#include "Loop.h"
#include "Block.h"

extern int inLoop;

string loop_make_random() {
    string res;
    string varname;
    if (inLoop >= 3) return res;
    bool outDefination = false;
    
    switch (inLoop)
    {
    case 0:
        varname = "for_i";
        break;
    case 1:
        varname = "for_j";
        break;
    case 2:
        varname = "for_k";
        break;
    }
    inLoop++;
    switch (rand() % 2) {
    case 0:
        break;
    case 1:
        outDefination = true;
        res += "\n{\n";
        res += "uint256 " + varname + " = 0;\n";
        break;
    }
    // 加入循环不变量
    if (rand() % 3 == 1 && inLoop == 1) {
        res += "l_z = 1;\n";
    }
    if (outDefination) {
        res += "for(;" + varname + "<" + to_string(rand() % 5 + 1) + ";" + varname + "++)\n{\n";
    }
    else {
        switch (rand() % 2) {
        case 0:
            res += "for(uint256 " + varname + "=0;" + varname + "<" +
            to_string(rand() % 10 + 1) + ";" + varname + "++)\n{\n";
            break;
        case 1:
            res += "for(uint256 " + varname + "=0;;" + varname + "++)\n{\n";
            res += "if(" + varname + ">" + to_string(rand() % 5 + 1) + "){break;}\n";
            break;
        }
    }

    //中间调用block生成一系列块中的内容
    res += block_make_random();
    res += " \n }\n";
    if (outDefination) res += "}\n";
    inLoop--;
    return res;
}