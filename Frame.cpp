#include <vector>
#include "Frame.h"
#include "Test.h"
#include "Function.h"
#include "Variable.h"
#include "Assign.h"

extern string testtestevent;   //只记录状态变量
extern string testtestemit;
extern vector<Variable> VariableList;//存放状态变量
extern vector<Variable> ArrayList; // 存放数组变量
extern vector<Variable> ArrayList_b; // 存放最原始的数组内容，因为测试脚本，在测每个函数时都会从最初始的状态开始
extern vector<Variable> ConstVariableList;

// 一个辅助测试的合约，只生成一个函数，没有状态变量，数组等内容
// 函数只有external一种结果，
string genOtherContract() {
    string res = "contract D{\n";
    int tmp = rand() % 2 + 1;
    for (int i = 0; i < tmp; ++i) res += genArray(ArrayList,i);
    res += genfunc("func_a", 1,0);
    res += "}\n";
    ArrayList.clear();
    return res;
}

string getFrame() {

    //每个合约开通固定的部分
    string res;
    res += "//SPDX-License-Identifier: GPL-3.0\n"
        "pragma solidity ^0.8.0;\n";
    int otherContract = 0; // 用来记录是否存在另一个合约
    if (rand() % 50 == 1) {
        res += genOtherContract();
        otherContract = 1;
    }
    res += "contract ContractName{\n\t";
    //生成随机数量个状态变量
    int count = rand() % 3 + 1;

    for (int i = 0; i < count; i++) {
        res += genVariable(VariableList, i, false);
    }
    count = rand() % 2 + 1;
    for (int i = 0; i < count; ++i) {
        res += genConstVariable(ConstVariableList, i);
    }

    // 生成随机个数组,所有数组都是动态一维数组
    int tmp = rand() % 2 + 1;
    for(int i = 0;i < tmp;++ i){
        res += genArray(ArrayList,i);
    }
    ArrayList_b = ArrayList;

    testtestevent.clear();
    testtestemit.clear();
    testtestevent += "event testtest(";
    testtestemit += "emit testtest(";

    for (vector<Variable>::iterator iter = VariableList.begin(); iter != VariableList.end(); iter++) {
        testtestevent += iter->Typename + " " + iter->Variablename;
        testtestemit += iter->Variablename;
        if (iter->Variablename != (VariableList.end() - 1)->Variablename) {
            testtestevent += ",";
            testtestemit += ",";
        }
    }

    testtestevent += ");\n";
    testtestemit += ");\n";
    res += testtestevent;
    //res += to_string(VariableList.size()) + "\n";
    testtestevent.clear();
    //生成3个函数
    count = 3;  //rand() % 10 + 1;
    for (int i = 0; i < count; i++) {
        ArrayList = ArrayList_b;
        res += genfunc("func_" + getName(i),0,otherContract);
    }

    res += "}";
    VariableList.clear();
    ArrayList.clear();
    ArrayList_b.clear();
    ConstVariableList.clear();
    return res;
}

// 以同样的方式生成另一个合约D，将D的所有函数都定义为external，并记录下来所有的函数名
// 只在func_c中才有几率以D的实例为返回值，只在func_b中才会调用func_c调用方式统一为func_c().函数名.selector

