#include "Assembly.h"
#include "Function.h"
#include <iostream>

extern vector<Variable> AssemblyVarList; // 存放内联函数中的变量
extern vector<Variable> VariableList;//存放状态变量
extern vector<Variable> LocalVarList;//存放局部变量
extern vector<Variable> GlobalVarList;//存放全局变量
extern vector<sfunction> FuncReturnList;//存放函数名和返回值类型
extern string infunc;//记录当前在生成哪个函数中的内容，避免调用当前函数

vector<sfunction> AssemblyFunctionList;

string AssemblyAssign(int l);

// 内联汇编中的变量和函数中的局部变量可以直接互相赋值，但是内联汇编中的变量和状态变量不能直接赋值
// uint256 a = 12;a是状态变量，要赋值给内联函数中的变量a_a，需要先定义一个局部变量aValue.
// aValue := sload(a.slot),之后再用aValue进行操作  ，暂时不使用状态变量作为右值
// 如果要将a_a赋值给a，需要使用sstore(a.slot,a_a)
// 没找到给storage类型的数组赋值的方法，暂时放弃再内联汇编中使用数组
string inlineassembly() {
    /*
    let x := y
    let res := add(x,div(x,y))
    */

    AssemblyFunctionList.clear();
    AssemblyVarList.clear(); // 清空之前创建的变量
    
    string res = "assembly{\n";
    //随机定义数个变量
    int tmp = rand() % 1 + 1;
    
    for (int i = 0; i < tmp; ++i) {
        string varname = "aa" + getName(i);
        int varval = rand() % 10;
        res += "let " + varname + ":= " + to_string(varval) + "\n";
        AssemblyVarList.emplace_back(Variable("assemblyvar", varname, varval));
    }
    tmp = rand() % 3+1;
    for (int i = 0; i < tmp; ++i) res += assemblyfunc(i);

    res += AssemblyAssign(rand() % 2 + 1);
    tmp = rand() % 2 + 1;
    for (int i = 0; i < tmp; ++i) {
        switch (rand() % 3) {
        case 0:
            res += assemblyfor(0);
            break;
        case 1:
            res += assemblyif(0);
            break;
        }
    }
    res += "}\n";
    return res;
}
// 获取左值
string getLvalue(){
    string res = "";
    int ls = LocalVarList.size() + AssemblyVarList.size();//等号左边可以是状态变量和局部变量
    int tmp = rand() % ls;
    if(tmp < LocalVarList.size()){
        res += LocalVarList[tmp].Variablename;
    }else if(tmp < LocalVarList.size() + AssemblyVarList.size()){
        res += AssemblyVarList[tmp - LocalVarList.size()].Variablename;
    }
    return res;
}
// 获取右值
string getRvalue(){
    string res;
    int rs = LocalVarList.size() + AssemblyVarList.size();//等号左边可以是状态变量和局部变量
    int tmp = rand() % (rs + rs/2);
    if(tmp < LocalVarList.size()){
        res = LocalVarList[tmp].Variablename;
    }else if(tmp < LocalVarList.size() + AssemblyVarList.size()){
        res = AssemblyVarList[tmp - LocalVarList.size()].Variablename;
    }else{
        res = to_string(rand() % 100);
    }
    return res;
}
string AssemblyKeccak(){
    string res = "keccak256(";
    res += to_string(rand() % 10) + ",";
    res += to_string(rand() % 33) + ")";
    return res;
}
string mload(){
    string res = "mload("+ to_string(rand()%10) + ")";
    return res;
}
// 生成 加减乘除取余操作
string AssemblyOp(int deep){
    string res = "";
    if(deep >= 3) return to_string(rand() % 50 + 1);
    if(deep == 0) res += getLvalue() + " := ";
    switch (rand() % 5)
    {
    case 0:
        res += "add(";
        break;
    case 1: 
        res += "sub(";
        break;
    case 2:
        res += "mul(";
        break;
    case 3:
        res += "div("; // 如果除数为零，div结果为0，不需要额外处理
        break;
    case 4:
        res += "mod(";
        break;
    }
    switch (rand()%5)
    {
    case 0:
        res += getRvalue();
        break;
    case 1:
        res += mload();
        break;
    case 2:
        // res += AssemblyKeccak();
        res += to_string(rand() % 50);
        break;
    case 3:
        res += to_string(rand() % 50);
        break;
    case 4:
        res += AssemblyOp(deep+1);
        break;
    }
    res += ",";
    switch (rand() % 5)
    {
    case 0:
        res += getRvalue();
        break;
    case 1:
        res += mload();
        break;
    case 2:
        //res += AssemblyKeccak();
        res += to_string(rand() % 50);
        break;
    case 3:
        res += to_string(rand() % 50);
        break;
    case 4:
        res += AssemblyOp(deep+1);
        break;
    }
    res += ")";
    return res;
}
string AssemblyMstore(){
    string res = "";
    res += "mstore(" + to_string(rand()%10) + "," ;
    switch (rand() % 4)
    {
    case 0:
        res += to_string(rand() % 50);
        break;
    case 1:
        res += getRvalue();
        break;
    case 2:
    case 3:
        res += AssemblyOp(1);
        break;
    }
    res += ")";
    return res;
}
// 给状态变量赋值
string AssemblySstore() {
    string res = "sstore(";
    res += VariableList[rand() % VariableList.size()].Variablename + ",";
    res += AssemblyOp(1);
    res += ")";
    return res;
}
// 生成l条赋值语句
string AssemblyAssign(int l){
    string res = "";
    for(int m = 0;m < l;++ m){
        switch(rand() % 6){
        case 0:
        case 1:
        case 2:
            res += AssemblyOp(0) + "\n";
            break;
        case 3:
            res += AssemblyMstore() + "\n";
            break;
        case 4:
            res += getLvalue() + ":=" + mload() + "\n";
            break;
        case 5:
            if(rand() % 3 == 1) // 适当降低函数调用的次数
                res += getLvalue() + ":=" + callassemblyfunc() + "\n";
            break;
        case 6:
            res += AssemblySstore() + "\n";
            break;
        }
    }
    return res;
}

string AssemblyAssignNoFunc(int l) {
    string res = "";
    for (int m = 0; m < l; ++m) {
        switch (rand() % 6) {
        case 0:
        case 1:
        case 2:
            res += AssemblyOp(0) + "\n";
            break;
        case 3:
            res += AssemblyMstore() + "\n";
            break;
        case 4:
            res += getLvalue() + ":=" + mload() + "\n";
            break;
        case 5:
            res += AssemblySstore() + "\n";
            break;
        }
    }
    return res;
}

// i控制深度
// tmp表示有多少个内联汇编的变量 ，好像没什么用
string assemblyfor(int i) { 
    if (i >= 2)return "";
    string var = "assemblyfor_";
    var.push_back('i' + i);
    string res = "for {let " + var + " := 0} lt(" + var + "," + to_string(rand() % 10) + ") {" + var + " := add(" + var +",1)}\n{\n";
    // 添加其他代码
    int l = rand() % 2;
    res += AssemblyAssign(l);
    switch (rand() % 3) {
    case 0:
        res += assemblyfor(i + 1);
        break;
    case 1:
        res += assemblyif(i + 1);
        break;
    case 2:
        break;
    }
    l = rand() % 2;
    res += AssemblyAssign(l);
    res += "}\n";
    return res;
}
// i控制深度
string assemblyif(int i) {
    string res;
    if (i >= 2) return res;
    switch (rand() % 3)
    {
    case 0:
        res += "if eq(";
        break;
    case 1:
        res += "if lt(";
        break;
    case 2:
        res += "if gt(";
        break;
    }
    res += AssemblyVarList[rand() % AssemblyVarList.size()].Variablename + "," + AssemblyVarList[rand() % AssemblyVarList.size()].Variablename + ")\n{\n";
    // 添加其他代码
    int l = rand() % 2;  
    res += AssemblyAssign(l);
    switch (rand() % 3) {
    case 0:
        res += assemblyif(i + 1);
        break;
    case 1:
        res += assemblyfor(i + 1);
        break;
    case 2:
        break;
    }
    l = rand() % 2;
    res += AssemblyAssign(l);
    res += "}\n";
    return res;
}

string assemblyfunc(int i) {
    string assemblyfuncname = "assemblyfunc_" + getName(i);
    string res = "function " + assemblyfuncname + "(";
    int paranum = rand() % 3;
    switch (paranum) { //随机数量个参数
    case 0:
        res += "";
        break;
    case 1:
        res += "x";
        break;
    case 2:
        res += "x,y";
    }
    res += ") -> r { \n";
    int tmp = rand() % 3;
    for (int i = 0; i < tmp; ++i) {
        res += AssemblyAssignNoFunc(0);
    }
    switch (rand() % 3) {
    case 0:
        res += "return(0,0)\n";
        break;
    case 1:
        //res += "revert(0,0)\n";
        break;
    case 2:
        break;
    }
    res += "}\n";
    AssemblyFunctionList.emplace_back(sfunction(assemblyfuncname, paranum));
    return res;
}
// 调用内联汇编中的函数
string callassemblyfunc() {
    sfunction tmpF = AssemblyFunctionList[rand() % AssemblyFunctionList.size()];
    string res = tmpF.funcname + "(";
    for (int i = 0; i < tmpF.paranum; ++i) {
        if (rand()%5 != 1) {
            res += getRvalue(); // 正常的右值
        }
        else { 
            // 函数参数是另一个函数，作为参数的函数的参数不能再是函数
            sfunction tmptmpF = AssemblyFunctionList[rand() % AssemblyFunctionList.size()];
            res += tmptmpF.funcname + "(";
            for (int j = 0; j < tmptmpF.paranum; ++j) {
                res += getRvalue();
                if (j != tmptmpF.paranum - 1) res += ",";
            }
            res += ")";
        }
        if (i != tmpF.paranum - 1) res += ",";
    }
    res += ")\n";
    return res;
}