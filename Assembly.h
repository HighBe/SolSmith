#pragma once
#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <string>
#include <vector>
#include "Variable.h"
using namespace std;

string inlineassembly();
string assemblyfor(int i);
string assemblyif(int i);
string assemblyfunc(int i);
string callassemblyfunc();

#endif // !ASSEMBLY_H
