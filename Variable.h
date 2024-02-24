#pragma once
#ifndef VARABLE_H
#define VARABLE_H

#include <cstring>
#include <string>
#include <vector>

using namespace std;
class Variable
{
public:
    Variable() = default;
    string Typename;//存储类型名
    string Variablename;//变量名
    int num;//存储变量对应的值
    int a, b, c;//存放数组的三维，如果全为零，说明不是数组
    Variable(string tname, string vname) {
        Typename = tname;
        Variablename = vname;
    }
    Variable(string tname, string vname, int t) {
        Typename = tname;
        Variablename = vname;
        num = t;
        a = b = c = 0;
    }
    // 数组，l1 l2是二维数组的长度,如果是一维数组，则l2为零
    Variable(string tname,string vname,int l1,int l2){
        Typename = tname;
        Variablename = vname;
        a = l1;
        b = l2;
    }
};



string getName(int n);
string getUInt256();
string getInt256();
string getAdress();
string genVariable(vector<Variable>& VarList, int i, bool isLocal);
string genArray(vector<Variable>& VarList,int i);
string genConstVariable(vector<Variable>& VarList, int i);

#endif // !VARABLE_H
