#include <vector>
#include "Function.h"
#include "Block.h"
#include "State.h"
#include "Test.h"
#include "Variable.h"

extern vector<sfunction> FuncList;//存放函数
extern vector<sfunction> FuncReturnList;//存放函数名和返回值类型
extern string infunc;//记录当前在生成哪个函数中的内容，避免调用当前函数
extern string testtestevent;   //只记录状态变量
extern string testtestemit;
extern vector<Variable> VariableList;//存放状态变量
extern vector<Variable> LocalVarList;//存放局部变量
extern vector<Variable> ArrayList; // 存放数组变量
extern vector<Variable> ArrayList_b; // 最初的数组内容
extern int blockcount;//终止调用block



// funcName 表示需要生成的函数的名字
// keyword 传入函数的可见性，如果为0则随机选择，如果为1则固定为external
string genfunc(string funcName,int keyword,int otherContract) {
    string res;
    string sfuncname = funcName;
    int selector = otherContract;
    if(otherContract == 1) selector = rand() % 5000;                                     //////之后再调
    infunc = funcName;
    int svisibility;
    string sreturnPara;
    //修改状态：1修改状态变量2触发事件3创建其他合约4出发selfdestruct
    //5通过调用发送以太币6调用非view或pure函数7触发低级调用8使用包含某些操作码的内联汇编

    //读取状态：1从状态变量中读取2访问address(this).balance和address.balance
    //3访问block tx msg的任何成员(除了msg.sig msg.data)
    //4调用非pure函数5使用包含某些操作码的内联汇编
    //enum funcstate
    //{
    //    eView,//不修改状态 
    //    ePure//不读取和修改状态
    //};
    enum visibility
    {
        ePublic,
        ePrivate,
        eExternal,//只能从合约外部调用（如果要从智能合约内部调用它，必须使用this）
        eInternal
    };
    res = "function " + funcName + "() ";
    int tmp = keyword;//rand() % 4;
    switch (tmp)
    {
    case 0:
        res += "public ";
        svisibility = 0;
        break;
    case 1:
        res += "external ";
        svisibility = 1;
        break;
    case 2:
        res += "private ";
        svisibility = 2;
        break;
    case 3:
        res += "internal ";
        svisibility = 3;
        break;


    }

    //添加modifiy，最多3个，
    /*tmp = rand() % 3;
    for (int i = 0; i <= tmp; i++) {
        res += "mod" + to_string(i);
    }*/


    //返回的变量放到局部变量list中，同时将函数名和返回变量的变量类型组合成一个新的变量放到状态变量list中
    //returns(uint256 b_a)  其中b_a可以在函数中不参与任何过程。
    int hasreturn = rand() % 2;
    if (selector == 1 && funcName == "func_c") {
        res += "returns (D) ";
    }else if (hasreturn == 1) {//有返回值
        res += "returns(";
        int t = rand() % 2;//将变量的类型暂时限定在uint和int两种
        switch (t) {
        case 0:
            res += "uint256";
            sreturnPara = "uint256";
            LocalVarList.emplace_back(Variable("uint256", "b_a", 0));
            FuncReturnList.emplace_back(sfunction(funcName, svisibility, "b_a"));
            break;
        case 1:
            res += "int256";
            sreturnPara = "int256";
            LocalVarList.emplace_back(Variable("int256", "b_a", 1));
            FuncReturnList.emplace_back(sfunction(funcName, svisibility, "b_a"));
            break;
        case 2:
            res += "bool";
            sreturnPara = "bool";
            LocalVarList.emplace_back(Variable("bool", "b_a", 2));
            FuncReturnList.emplace_back(sfunction(funcName, svisibility, "b_a"));
            break;
        case 3:
            res += "address";
            sreturnPara = "address";
            LocalVarList.emplace_back(Variable("address", "b_a", 3));
            FuncReturnList.emplace_back(sfunction(funcName, svisibility, "b_a"));
            break;
        case 4:
            res += "bytes32";
            sreturnPara = "bytes32";
            LocalVarList.emplace_back(Variable("bytes32", "b_a", 4));
            FuncReturnList.emplace_back(sfunction(funcName, svisibility, "b_a"));
            break;
        }
        res += " b_a)";
    }

    res += "\n{\n";

    // 加入unchecked，在0.8版本之前，如果出现溢出，则不做任何处理，0.8版本后，如果发现溢出，会花费部分gas将已经进行的运算恢复
    // 使用unchecked会使编译器行为与0.8版本前一致
    res += "unchecked{\n";

    // 加入一个l_z变量用来在循环中插入循环不变量，方便修改
    
    res += "uint256 l_z = 1;\n";

    if (selector == 1 && funcName == "func_b") {
        res += "func_c().func_a.selector;\n";
    }
    
    

    //生成局部变量 上限4个
    tmp = rand() % 2+1;
    //先清空一下上个函数生成的局部变量
    LocalVarList.clear();
    for (int i = 0; i < tmp; i++) {
        res += genVariable(LocalVarList, i, true);
    }

    //确定在函数体中语句的数量
    //用state来生成函数体
    res += state_make_random();
    res += testtestemit;
    if (hasreturn == 1) res += "\nreturn b_a;";
    if (selector == 1 && funcName == "func_c") {
        res += "return new D();\n";
    }
    res += "\n}\n"; // unchecked的右括号
    res += "\n}\n"; // 函数的右括号

    blockcount = 0;

    //加入函数列表
    FuncList.emplace_back(sfunction(sfuncname, svisibility, sreturnPara));
    return res;
}