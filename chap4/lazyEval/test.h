#pragma once
#include <string>
#include "env.h"
#include "parser.h"
#include "eval.h"
using std::string;
void testParser(string& input);
void testEval(string& input, EnvPtr init);
void testEval(const vector<string>& programs);
void testEval(std::initializer_list<string> input);