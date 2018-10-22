#pragma once
void analyzeLoop();
analyzeFun analyze(SchemeValuePtr exp);
SchemeValuePtr analyzeEval(SchemeValuePtr exp, EnvPtr env);