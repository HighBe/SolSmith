#include <vector>

#include <string>
#include <cstring>
#include <unordered_map>
#include "Function.h"
#include "Assign.h"
#include "Variable.h"

using namespace std;

extern vector<Variable> VariableList;//存放状态变量
extern vector<Variable> LocalVarList;//存放局部变量
extern vector<Variable> ArrayList; // 存放数组变量
extern vector<Variable> ArrayList_b; // 最初的数组内容
extern vector<Variable> ConstVariableList; // 存放const类型变量
extern vector<sfunction> FuncReturnList;//存放函数名和返回值类型
extern string infunc;//记录当前在生成哪个函数中的内容，避免调用当前函数
extern int inLoop;
unordered_map<string,int> hashvar;

string AddSub(string varname);

// 表示左值是哪个变量，判断是否需要进行类型转换
string getRvalue(Variable tmpV) {
    int rs = VariableList.size() + LocalVarList.size() + ArrayList.size() + ConstVariableList.size();//等号右边可以是状态变量和局部变量
    
    //确定等号右边。确定有几个变量，分别是什么,运算符是什么，右值可以是字面量    需要判断是否越界
//右边如果是多个变量的运算，在经过多次赋值后，有概率导致overflow，故只简单的将一个变量的值赋给另一个变量
    int count = rand() % 2;//最多2个最少1个
    bool flag = tmpV.Typename == "uint256";
    string tmpstring;//记录等号右边的表达式，如果最后右边表达式的结果越界，清空该字符串，重新生成表达式，直到成功，将字符串连接到res后
    Variable tmpR;
    string tmpRname, tmpRAftername; // 
    auto numtypecast = [tmpV](int n)->string {return tmpV.Typename + "(" + to_string(n) + ")"; };
    int t = rand() % 3 + 1;
    char preop = ' ';
    for (int i = 0; i < t; ++i) {
        int tmp = rand() % rs;
        string varname;
        if (flag) tmpstring += "uint256(";//如果等号左边是uint256类型，则在变量前加一个显式类型转换
        else tmpstring += "int256(";      //如果等号右边是int256类型，则在变量前加一个显示类型转换
        if (tmp < VariableList.size()) { //状态变量
            tmpR = VariableList[tmp];
            tmpRname = tmpR.Variablename;
            tmpRAftername = tmpRname;
            if (hashvar.count(tmpRname) == 0) { // 之前没有使用过该变量
                if (rand() % 4 == 1) { //有自增自减操作
                    hashvar[tmpRname] = 2;
                    tmpRAftername = AddSub(tmpRname); // 将自增自减操作与变量名合为一体
                }
                else {
                    hashvar[tmpRname] = 1; // 不做改变
                }
            }
            else if (hashvar[tmpRname] == 2) {
                tmpRAftername = to_string(rand() % 50);
            }
            if (preop == '/') tmpstring += tmpRname + "==0?1:" + tmpRAftername;
            else tmpstring += tmpRAftername;
        }
        else if (tmp < VariableList.size() + LocalVarList.size()) {
            tmpR = LocalVarList[tmp - VariableList.size()];//局部变量
            tmpRname = tmpR.Variablename;
            tmpRAftername = tmpRname;
            if (hashvar.count(tmpRname) == 0) { // 之前没有使用过该变量
                if (rand() % 4 == 1) { //有自增自减操作
                    hashvar[tmpRname] = 2;
                    tmpRAftername = AddSub(tmpRname); // 将自增自减操作与变量名合为一体
                }
                else {
                    hashvar[tmpRname] = 1; // 不做改变
                }
            }
            else if (hashvar[tmpRname] == 2) {
                tmpRAftername = to_string(rand() % 50);
            }
            if (preop == '/')  tmpstring += tmpRname + "==0?1:" + tmpRAftername;
            else tmpstring += tmpRAftername;
        }
        else if(tmp < VariableList.size() + LocalVarList.size() + ArrayList.size()){ // 数组变量
            tmpR = ArrayList[tmp - VariableList.size() - LocalVarList.size()];
            tmpRname = tmpR.Variablename + "[" + to_string(rand() % tmpR.a) + "]";
            tmpRAftername = tmpRname;
            if (hashvar.count(tmpRname) == 0) { // 之前没有使用过该变量
                if (rand() % 4 >= 1) { //有自增自减操作
                    hashvar[tmpRname] = 2;
                    tmpRAftername = AddSub(tmpRname); // 将自增自减操作与变量名合为一体
                }
                else {
                    hashvar[tmpRname] = 1; // 不做改变
                }
            }
            else if (hashvar[tmpRname] == 2) {
                tmpRAftername = to_string(rand() % 50);
            }
            int tmpp = rand() % tmpR.a;
            if (preop == '/') tmpstring += tmpRname  + "==0?1:" + tmpRAftername;
            else tmpstring += tmpRAftername;
        }
        else {
            tmpR = ConstVariableList[tmp - VariableList.size() - LocalVarList.size() - ArrayList.size()];
            if (preop == '/') tmpstring += tmpR.Variablename + "==0?1:" + tmpR.Variablename;
            else tmpstring += tmpR.Variablename;
        }
        tmpstring += ")";
        
        switch (rand() % 2)
        {
        case 0:
            // 加入常量
            switch (rand() % 4)
            {
            case 0:
                tmpstring += " + " + numtypecast(rand() % 200);
                break;
            case 1:
                if (flag) tmpstring += " + " + numtypecast(rand() % 100 + 200);
                tmpstring += " - " + numtypecast(rand() % 200);
                break;
            case 2:
                tmpstring += " / " + numtypecast(rand() % 5 + 1);
                tmpstring += " * " + numtypecast(rand() % 5);
                break;
            case 3:
                tmpstring += " / " + numtypecast(rand() % 20 + 1);
                break;
            }
            break;
        case 1:
            // 不额外加常量
            break;
        }
        if (i != t - 1) {
            tmp = rand() % 4;
            if (flag) tmp = 0;
            switch (tmp)
            {
            case 0:
                tmpstring += " + ";
                preop = '+';
                break;
            case 1:
                tmpstring += " - ";
                preop = '-';
                break;
            case 2:
                tmpstring += " / ";
                preop = '/';
                break;
            case 3: // 乘法前有一个除法，减少越界可能性
                tmpstring += "/" + to_string(rand() % 200 + 1);
                tmpstring += " * ";
                preop = '*';
                break;
            }
        }
    }
    hashvar.clear();
    return tmpstring;
}

string assign_make_random() {
    string res;
    long long numres = 0;//得到等号右边表达时的值，判断是否越界
    long long numtmp = 0;//记录运算的中间结果
    int a = 0;//记录运算符
    res += "\n";
    int ls = VariableList.size() + LocalVarList.size() + ArrayList.size();//等号左边可以是状态变量和局部变量
    int rs = ls; // 全局变量和函数返回值会出问题
    // int rs = ls + GlobalVarList.size() + FuncReturnList.size();//右边可以是状态变量、局部变量、函数返回值、全局变量
    int pushpop = rand() % 10;
    int tmparray = rand() % ArrayList.size();
    if (pushpop == 1) {
        Variable tmpv = ArrayList[tmparray];
        if(rand() % 2 == 1)res += tmpv.Variablename + ".push(" + getRvalue(tmpv) + ");\n";
        else res += tmpv.Variablename + ".push();\n";
    }
    else if(pushpop == 0 && ArrayList[tmparray].a > 1 && inLoop == 0) {
        Variable tmpv = ArrayList[tmparray];
        if(rand() % 2 == 1) res += tmpv.Variablename + ".push(" + getRvalue(tmpv) + ");\n";
        else res += tmpv.Variablename + ".push();\n";
        res += ArrayList[tmparray].Variablename + ".pop();\n";
        
    }
    else {
        //确定左边的变量。
        int tmp = rand() % ls;
        Variable tmpV;
        if (tmp < VariableList.size()) {
            tmpV = VariableList[tmp];
            res += VariableList[tmp].Variablename;
        }
        else if(tmp < VariableList.size() + LocalVarList.size()){
            tmpV = LocalVarList[tmp - VariableList.size()];
            res += LocalVarList[tmp - VariableList.size()].Variablename;
        }else { // 数组变量
            tmpV = ArrayList[tmp - VariableList.size() - LocalVarList.size()];
            res += tmpV.Variablename + "[" + to_string(rand() % tmpV.a) + "]";

        }
        //确定等于号
        tmp = rand() % 4;// *= 容易越界  暂时只用a=b+c;形式
        switch (tmp)
        {
        case 0:
            res += " = ";
            break;
        case 1:
            res += " += ";
            break;
        case 2:
            res += " -= ";
            break;
        case 3:
            res += " = ";
            break;
        case 4:
            res += " *= ";
            break;
        }
        res += getRvalue(tmpV);
        res += ";\n";
    }
    return res;
}

string getRvalue_onlyvariable(Variable tmpV) {
    int rs = VariableList.size() + ArrayList.size() + ConstVariableList.size();//等号右边可以是状态变量和局部变量
    
    char preop = ' ';
    //确定等号右边。确定有几个变量，分别是什么,运算符是什么，右值可以是字面量    需要判断是否越界
//右边如果是多个变量的运算，在经过多次赋值后，有概率导致overflow，故只简单的将一个变量的值赋给另一个变量
    int count = rand() % 2;//最多2个最少1个
    bool flag = tmpV.Typename == "uint256";
    string tmpstring;//记录等号右边的表达式，如果最后右边表达式的结果越界，清空该字符串，重新生成表达式，直到成功，将字符串连接到res后
    Variable tmpR;
    string tmpRname, tmpRAftername; // tmpRname记录变量的名字，tmpRAftername记录可能经过自增自减操作的名字
    bool firstconst = true;
    int t = rand() % 3 + 1;
    auto numtypecast = [tmpV](int n)->string {return tmpV.Typename + "(" + to_string(n) + ")"; };
    for (int i = 0; i < t; ++i) {
        int tmp = rand() % rs;
        string varname;
        if (flag) tmpstring += "uint256(";//如果等号左边是uint256类型，则在变量前加一个显式类型转换
        else tmpstring += "int256(";      //如果等号右边是int256类型，则在变量前加一个显示类型转换
        if (tmp < VariableList.size()) { //状态变量
            tmpR = VariableList[tmp];
            tmpRname = tmpR.Variablename;
            tmpRAftername = tmpR.Variablename;
            if (hashvar.count(tmpRname) == 0) { // 之前没有使用过该变量
                if (rand() % 4 == 1) { //有自增自减操作
                    hashvar[tmpRname] = 2;
                    tmpRAftername = AddSub(tmpRname); // 将自增自减操作与变量名合为一体
                }
                else {
                    hashvar[tmpRname] = 1; // 不做改变
                }
            }
            else if (hashvar[tmpRname] == 2) {
                tmpRAftername = to_string(rand() % 50);
            }
            if (preop == '/') tmpstring += tmpRname + "==0?1:" + tmpRAftername;
            else tmpstring += tmpRAftername;
        }
        else if(tmp < VariableList.size() + ArrayList.size() ){ // 数组变量
            tmpR = ArrayList[tmp - VariableList.size()];
            tmpRname = tmpR.Variablename + "[" + to_string(rand() % tmpR.a) + "]";
            tmpRAftername = tmpRname;
            if (hashvar.count(tmpRname) == 0) { // 之前没有使用过该变量
                if (rand() % 4 == 1) { //有自增自减操作
                    hashvar[tmpRname] = 2;
                    tmpRAftername = AddSub(tmpRname); // 将自增自减操作与变量名合为一体
                }
                else {
                    hashvar[tmpRname] = 1; // 不做改变
                }
            }
            else if (hashvar[tmpRname] == 2) {
                tmpRAftername = to_string(rand() % 50);
            }
            if(preop == '/') tmpstring += tmpRname  + "==0?1:" + tmpRAftername;
            else tmpstring += tmpRAftername;
        }
        else {
            tmpR = ConstVariableList[tmp - VariableList.size() - ArrayList.size()];
            if (preop == '/') tmpstring += tmpR.Variablename + "==0?1:" + tmpR.Variablename;
            else tmpstring += tmpR.Variablename;
        }
        tmpstring += ")";
        switch (rand() % 2)
        {
        case 0:
            // 加入常量
            switch (rand() % 4)
            {
            case 0:
                tmpstring += " + " + numtypecast(rand() % 200);
                break;
            case 1:
                if (flag) tmpstring += " + " + numtypecast(rand() % 100 + 200);
                tmpstring += " - " + numtypecast(rand() % 200);
                break;
            case 2:
                tmpstring += " / " + numtypecast(rand() % 5 + 1);
                tmpstring += " * " + numtypecast(rand() % 5);
                break;
            case 3:
                tmpstring += " / " + numtypecast(rand() % 20 + 1);
                break;
            }
            break;
        case 1:
            // 不额外加常量
            break;
        }
        if (i != t - 1) {
            tmp = rand() % 4;
            if (flag) tmp = 0;
            switch (tmp)
            {
            case 0:
                tmpstring += " + ";
                preop = '+';
                break;
            case 1:
                tmpstring += " - ";
                preop = '-';
                break;
            case 2:
                tmpstring += " / ";
                preop = '/';
                break;
            case 3: // 乘法前有一个除法，减少越界可能性
                tmpstring += "/" + to_string(rand() % 200 + 1);
                tmpstring += " * ";
                preop = '*';
                break;
            }
        }
        
    }
    hashvar.clear();
    return tmpstring;
}

string assign_make_random_onlyVariable() {
    string res = "";
    int ls = VariableList.size() + ArrayList.size();
    int rs = ls; // 全局变量和函数返回值会出问题
    int pushpop = rand() % 5;
    int tmparray = rand() % ArrayList.size();
    if (pushpop == 1) {
        Variable tmpv = ArrayList[tmparray];
        if (rand() % 2 == 1) {
            res += tmpv.Variablename + ".push(" + getRvalue_onlyvariable(tmpv) + ");\n";
        }
        else {
            res += tmpv.Variablename + ".push();\n";
        }
        
    }
    else if (pushpop == 0 && ArrayList[tmparray].a > 1 && inLoop == 0) {
        Variable tmpv = ArrayList[tmparray];
        if (rand() % 2 == 1) {
            res += tmpv.Variablename + ".push(" + getRvalue_onlyvariable(tmpv) + ");\n";
        }
        else {
            res += tmpv.Variablename + ".push();\n";
        }
        res += ArrayList[tmparray].Variablename + ".pop();\n";
    }
    else {
        //确定左边的变量。
        int tmp = rand() % ls;
        Variable tmpV;
        if (tmp < VariableList.size()) {
            tmpV = VariableList[tmp];
            res += VariableList[tmp].Variablename;
        }
        else { // 数组变量
            tmpV = ArrayList[tmp - VariableList.size()];
            res += tmpV.Variablename + "[" + to_string(rand() % tmpV.a) + "]";

        }
        //确定等于号
        tmp = rand() % 4;// *= 容易越界  暂时只用a=b+c;形式
        switch (tmp)
        {
        case 0:
            res += " = ";
            break;
        case 1:
            res += " += ";
            break;
        case 2:
            res += " -= ";
            break;
        case 3:
            res += " = ";
            break;
        case 4:
            res += " *= ";
            break;
        }
        res += getRvalue_onlyvariable(tmpV);
        res += ";\n";
    }

    return res;
}

string AddSub(string varname) {
    string res = "";
    int tmp = rand() % 4;
    switch (tmp) {
    case 0:
        res += varname + "++";
        break;
    case 1:
        res += varname + "--";
        break;
    case 2:
        res += "++" + varname;
        break;
    case 3:
        res += "--" + varname;
        break;
    }
    return res;
}
