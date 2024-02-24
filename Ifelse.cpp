#include "Ifelse.h"
#include "Loop.h"
#include "Variable.h"
#include "Function.h"
#include "Block.h"

extern vector<Variable> VariableList;//存放状态变量
extern vector<Variable> LocalVarList;//存放局部变量
extern vector<Variable> GlobalVarList;//存放全局变量
extern vector<Variable> ArrayList; // 存放数组变量
extern vector<sfunction> FuncReturnList;//存放函数名和返回值类型
extern int inLoop;

string returnInt(string int1, string int2) {
    //输入两个int类型的值，得到int类型的值
    //可进行的运算有：  +  -  *  /  |  &  ^
    string  res;
    res += "(" + int1;
    switch (rand() % 7) {
    case 0:
        res += "+";
        break;
    case 1:
        res += "-";
        break;
    case 2:
        res += "*";
        break;
    case 3:
        res += "/";
        break;
    case 4:
        res += "|";
        break;
    case 5:
        res += "&";
        break;
    case 6:
        res += "^";
        break;
    }
    res += int2 + ")";
    return res;
}
string returnBool(string int1, string int2) {
    //输入两个字符串表示两个Int类型的数
    //两个int类型数要得到bool类型结果，可以由几种方法
    //  <  >  ==  !=  
    string  res;
    res += "(" + int1;
    switch (rand() % 4) {
    case 0:
        res += "<";
        break;
    case 1:
        res += ">";
        break;
    case 2:
        res += "==";
        break;
    case 3:
        res += "!=";
        break;
    }
    res += int2 + ")";
    return res;
}
string boolreturnBool(string bool1, string bool2) {
    //输入两个字符串表示bool类型的变量
    //将两个bool类型操作 || &&  == != 
    string  res;
    res += "(" + bool1;
    switch (rand() % 4) {
    case 0:
        res += "||";
        break;
    case 1:
        res += "&&";
        break;
    case 2:
        res += "==";
        break;
    case 3:
        res += "!=";
        break;
    }
    res += bool2 + ")";
    return res;
}


string ifelse_make_random() {
    string res;
    res += "\nif(";

    //确定有几个变量或常量,保底一个
    int vl = VariableList.size() + LocalVarList.size() + ArrayList.size();
    // 判断语句的深度
    int tmp = rand() % 3 + 1;
    //得到几组bool类型变量
    string val1[4], val2[4], boolval[4];
    for (int i = 0; i < tmp; i++) {
        int t = rand() % vl;
        val1[i] = "int256(";
        if (t < VariableList.size()) { //状态变量
            val1[i] += VariableList[t].Variablename;
        }
        else if (t < VariableList.size() + LocalVarList.size()) { // 局部变量 
            val1[i] += LocalVarList[t - VariableList.size()].Variablename;
        }
        else{  // 数组
            Variable tmpv = ArrayList[t - VariableList.size() - LocalVarList.size()];
            val1[i] += tmpv.Variablename + "[" + to_string(rand() % tmpv.a) + "]";
        }
        val1[i] += ")";

        t = rand() % vl;
        val2[i] = "int256(";
        if (t < VariableList.size()) { //状态变量
            val2[i] += VariableList[t].Variablename;
        }
        else if (t < VariableList.size() + LocalVarList.size()) { // 局部变量 
            val2[i] += LocalVarList[t - VariableList.size()].Variablename;
        }
        else {  //  数组
            Variable tmpv = ArrayList[t - VariableList.size() - LocalVarList.size()];
            val2[i] += tmpv.Variablename + "[" + to_string(rand() % tmpv.a) + "]";
        }
        val2[i] += ")";
        boolval[i] = returnBool(val1[i], val2[i]);
    }
    for (int i = 1; i < tmp; ++i) {
        boolval[0] = boolreturnBool(boolval[0], boolval[i]);
    }
    res += boolval[0] + ")\n{\n";

    //插入其他语句
    res += block_make_random();

    if (inLoop != 0) {
        tmp = rand() % 3;
        switch (tmp) {
        case 0://break;
            res += "\nbreak;\n";
            break;
        case 1://continue;
            res += "\ncontinue;\n";
            break;
        case 2://
            break;
        }
    }

    res += "}\n";
    return res;
}