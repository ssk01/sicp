#include "eval.h"
using namespace std;

shared_ptr<SchemeValue> apply(shared_ptr<SchemeValue> procedure, vector<shared_ptr<SchemeValue>> arguments) {
	if (procedure->isProcedure()) {
		if (procedure->isPrimitiveProcedure()) {
			return procedure->call(arguments);
		}
		else if (procedure->isLambda()) {
			auto env = procedure->newEnv(arguments);
			return eval(procedure->body(), env);
		}
	}
	fck("apply fuck");
}

vector<shared_ptr<SchemeValue>>  listOfValue(vector<shared_ptr<SchemeValue>> arguments, EnvPtr env) {
	vector<shared_ptr<SchemeValue>> result;
	for (auto arg : arguments) {
		result.emplace_back(eval(arg, env));
	}
	return result;
}
shared_ptr<SchemeValue>  eval(shared_ptr<SchemeValue>& exp, EnvPtr env) {
	//return {};
	if (exp->selfEvaluting()) {
		//cout << *exp << endl;
		return exp;
	}
	else if (exp->isVariable()) {
		auto result = env->lookup(exp);
		//cout << *result;
		return result;
	}
	else if (exp->isDefinition()) {
		//auto a = 1;
		auto var = exp->var();
		auto val = eval(exp->val(env), env);
		env->define(var, val);
		return make_shared<VoidValue>();
	}
	else if (exp->isLambda()) {
		return exp;
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
	else if (exp->isTagged("cond")) {
		return eval(expandClauses(exp->causes()), env);
	}
	else if (exp->application()) {
		return apply(eval(exp->operators(), env), listOfValue(exp->operands(), env));
	}
	else {
		exit(0);

	}

	//return {};
	return make_shared<VoidValue>();


}
