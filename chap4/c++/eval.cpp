#include "eval.h"
using namespace std;

shared_ptr<SchemeValue> apply(shared_ptr<SchemeValue> procedure, vector<shared_ptr<SchemeValue>> arguments) {
	if (procedure->isProcedure()) {
		if (procedure->isPrimitive()) {
			return procedure->call(arguments);
		}
	}
}

vector<shared_ptr<SchemeValue>>  listOfValue(vector<shared_ptr<SchemeValue>> arguments, Env& env) {
	vector<shared_ptr<SchemeValue>> result;
	for (auto arg : arguments) {
		result.emplace_back(eval(arg, env));
	}
	return result;
}
shared_ptr<SchemeValue>  eval(shared_ptr<SchemeValue>& exp, Env& env) {
	//return {};
	if (exp->selfEvaluting()) {
		//cout << *exp << endl;
		return exp;
	}
	else if (exp->isVariable()) {
		auto result = env.lookup(exp);
		//cout << *result;
		return result;
	}
	else if (exp->isDefinition()) {
		//auto a = 1;
		env.define(exp->var(), eval(exp->val(), env));
		return make_shared< VoidValue>();
	}
	else if (exp->isIf()) {
		auto predict = eval(exp->predict(), env);
		if (predict->T()) {
			return eval(exp->trueBranch(), env);
		}
		auto falseBranch = exp->falseBranch();
		if (falseBranch != Void()) {
			return eval(falseBranch, env);
		}

	}
	else if (exp->application()) {
		return apply(eval(exp->operators(), env), listOfValue(exp->operands(), env));
	}
	else {
		exit(0);

	}

	//return {};
	return make_shared< VoidValue>();


}
