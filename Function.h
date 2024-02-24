#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <cstring>

using namespace std;

class sfunction
{
public:
    string funcname;//函数名
    int visbility;//函数可见性 0 public 1 private 2 external 3 internal 
    //string funcstate;//函数状态
    string returnPara;//函数返回值都有哪些,暂定最多一个返回值
    int paranum = 0;

    sfunction(const string& funcname, int visbility, const string& returnPara)
        : funcname(funcname), visbility(visbility), returnPara(returnPara)
    {
    }
    sfunction(const string& funcname,int paranum) : funcname(funcname), paranum(paranum) {}

    sfunction() = default;

    bool operator==(const sfunction& other) const
    {
        return funcname == other.funcname && visbility == other.visbility && returnPara == other.returnPara;
    }
};

string genfunc(string funcName,int keyword,int otherContract);

#endif // !FUNCTION_H
