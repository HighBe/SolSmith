#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <string>
#include <cstring>
//#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
// #include "frame.h"
#include <shlwapi.h>


//#include "Ifelse.h"
//#include "Variable.h"
#include "Function.h"
//#include "Array.h"
//#include "Assembly.h"
//#include "Assign.h"
//#include "Block.h"
//#include "Loop.h"
//#include "State.h"

#include "Test.h"
#include "Variable.h"
#include "Frame.h"
using namespace std;


vector<Variable> AssemblyVarList; // 存放内联函数中的变量
int blockcount = 0;//终止调用block

vector<sfunction> FuncList;//存放函数
vector<sfunction> FuncReturnList;//存放函数名和返回值类型
string infunc;//记录当前在生成哪个函数中的内容，避免调用当前函数
int inLoop = 0;//用来记录在第几循环中，最多三重
string testtestevent;   //只记录状态变量
string testtestemit;
vector<Variable> VariableList;//存放状态变量
vector<Variable> LocalVarList;//存放局部变量
vector<Variable> GlobalVarList;//存放全局变量
vector<Variable> ArrayList; // 存放数组变量
vector<Variable> ArrayList_b; // 最初的数组内容
vector<Variable> ConstVariableList;

int main() {
    srand(time(0));
    ofstream fout;

    //vector<long long > a;
    //for (int i = 0; i < 10; i++) {
    //    a.emplace_back(i);
    //}
    //cout << a.size() << endl;

    for (int i = 0; i < 10000; i++) {
        string res = getFrame();
        //存放生成的测试用例   在linux下生成存放的目录
        //string path = "/home/liu/CODE/ConsoleApplication1/testsuite/test" + to_string(i);
        //int status = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        //fout.open("/home/liu/CODE/ConsoleApplication1/testsuite/test" + to_string(i) + "/ContractName.sol");
        cout << "********" << i << "********" << endl;
        string testpath = "C:\\Users\\A\\Desktop\\paper\\testsuite\\";
        string contractpath = testpath + "test" + to_string(i);
        CreateDirectory(contractpath.c_str(), NULL);
        fout.open(contractpath + "/ContractName.sol");
        if (!fout.is_open()) cout << "could not open" << endl;
        fout << res;
        fout.close();
        //生成测试脚本
        //res = generateScript();
        ////fout.open("/home/liu/CODE/ConsoleApplication1/testsuite/test" + to_string(i) + "/func_" + getName(j) + ".js");
        //fout.open(testpath + "test" + to_string(i) + "\\test.js");
        //if (!fout.is_open()) cout << "could not open" << endl;
        //fout << res;
        //fout.close();
    }
    return 0;
}
//大数运算，如果越界就重新生成当前语句，直到不再越界
//如果结果没有越界返回true，否则返回false
//n1和n2都是处理后的的数据，不带0x的十六进制数，
// bool bignum(string n1,string n2,char opt){
//     string res;//保留运算结果，不带0x的十六进制数
//     string num1 = n1,num2 = n2;
//     bool flag;
//     int f = 0;//记录进位
//     //大数加法
//     if(opt == '+'){
//         for(int i = sizeof(num1)-1;i >= 0;i --){
//             int temp1,temp2,temp3;
//             if(num1[i] >= 'a') temp1 = 10 + num1[i] - 'a';
//             else temp1 = num1[i] - '0';
//             if(num2[i] >= 'a') temp2 = 10 + num2[i] - 'a';
//             else temp2 = num2[i] - '0';
//             temp3 = temp1 + temp2 + flag;
//             if(temp3 >= 16){
//                 temp3 -= 16;
//                 f = 1;
//             }
//             if(temp3 >= 10) res += 'a' + (temp3 - 10);
//             else res += temp3 + '0';
//         }
//     }else if(opt == '-'){
//         //先找出较大的数，保证不会出现不够减的问题
//         for(int i = 0;i < sizeof(n1);++ i){
//             if(n1[i] == 0 && n2[i] == 0) continue;
//             else if(n1[i] == 0 && n2[i] != 0){
//                 num1 = n2;
//                 num2 = n1;
//             }else if(n1[i] != 0 && n2[i] == 0){
//                 num1 = n1;
//                 num2 = n2;
//             }else{
//                 if(n1[i] > n2[i]){
//                     num1 = n1;
//                     num2 = n2;
//                 }else if(n1[i] < n2[i]){
//                     num1 = n2;
//                     num2 = n1;
//                 }else continue;
//             }
//         }
//         for(int i = sizeof(num1)-1;i >= 0;i --){
//             int temp1,temp2,temp3;
//             if(num1[i] >= 'a') temp1 = 10 + num1[i] - 'a';
//             else temp1 = num1[i] - '0';
//             if(num2[i] >= 'a') temp2 = 10 + num2[i] - 'a';
//             else temp2 = num2[i] - '0';
//             //够减，直接出结果
//             if(temp1 >= temp2 + f) {
//                 temp3 = temp1 - temp2 - f;
//                 f = 0;//如果之前f有借位，重置
//             }else {
//                 //不够减，往前借位
//                 temp3 = temp1 + 16 - temp2 - f;
//                 f = 1;
//             }
//             if(temp3 >= 10) res += 'a' + (temp3 - 10);
//             else res += temp3 + '0';
//         }
//         //如果两个数的位置倒换了，要取负号
//         if(n1 == num2) res = '-' + res;
//     }else{
//     }
//     return flag;
// }

// bool dealintopt(string num1,string num2,char opt){
//     int f1,f2;
//     f1 = f2 = 0;
//     if(num1[0] == '-') f1 = 1;
//     if(num2[0] == '-') f2 = 1;
//     if(f1 == 0 && f2 == 0){
//         string n1 = num1.substr(2);
//         string n2 = num2.substr(2);
//         return bignum(n1,n2,opt);
//     }else if(f1 == 1 && f2 == 1){
//         string n1 = num1.substr(3);
//         string n2 = num2.substr(3);
//         return bignum(n1,n2,opt);
//     }else if(f1 == 0 && f2 == 1){
//         string n1 = num1.substr(2);
//         string n2 = num2.substr(3);
//         if(opt == '-') return bignum(n1,n2,'+');
//         if(opt == '+') return bignum(n1,n2,'-'); 
//     }else if(f1 == 1 && f2 == 0){
//         string n1 = num1.substr(3);
//         string n2 = num2.substr(2);
//         if(opt == '-') return bignum(n1,n2,'+');
//         if(opt == '+') return bignum(n2,n1,'-');
//     }
// }


