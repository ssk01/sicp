#pragma once
#include <memory>
#include "scheme.h"
#include "env.h"
#include <vector>
using std::shared_ptr;
using std::vector;
shared_ptr<SchemeValue>  eval(shared_ptr<SchemeValue>& exp, Env& env);
shared_ptr<SchemeValue> apply(shared_ptr<SchemeValue> procedure, vector<shared_ptr<SchemeValue>> arguments);
vector<shared_ptr<SchemeValue>>  listOfValue(vector<shared_ptr<SchemeValue>> arguments, Env& env);