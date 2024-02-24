#pragma once
#ifndef	GLOBALVARIABLE_H
#define GOLBALVARIABLE_H

#include <vector>

#include "Variable.h"
#include "Function.h"

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
vector<Variable> GlobalVarList = { };//存放全局变量

#endif