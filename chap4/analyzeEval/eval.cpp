#include "eval.h"
#include <algorithm>

using namespace std;
using analyzeFun = function<SchemeValuePtr(EnvPtr)>;
EnvPtr LambdaValue::newEnv(vector<shared_ptr<SchemeValue>> argsValue) {
	auto env = make_shared<Env>(this->parent);
	assert(args.size() == argsValue.size());
	for (int i = 0; i < args.size(); i++) {
		env->define(args[i], argsValue[i]);
	}
	return env;
}
shared_ptr<SchemeValue> makeIf(shared_ptr<SchemeValue> cond, shared_ptr<SchemeValue> falseBranch, shared_ptr<SchemeValue> trueBranch) {
	auto if_ = make_shared<ListValue>();
	if_->addValue(make_shared<SymbolValue>("if"));
	if_->addValue(cond);
	if_->addValue(falseBranch);
	if_->addValue(trueBranch);
	return if_;

}

shared_ptr<SchemeValue> _expandClauses(vector<shared_ptr<SchemeValue>> exp, int i) {
	if (i < exp.size()) {
		auto clause = exp[i];
		if (clause->isTagged("else")) {
			if (i != exp.size() - 1) {
				fck("after else should be empty");
			}
			return clause->causeExp();
		}
		else {
			return makeIf(clause->causeCond(), clause->causeExp(), _expandClauses(exp, i + 1));
		}
	}
	return Void();
}

shared_ptr<SchemeValue> expandClauses(vector<shared_ptr<SchemeValue>> exp) {
	return _expandClauses(exp, 0);
}
shared_ptr<SchemeValue> letExp(shared_ptr<SchemeValue> exp, EnvPtr env) {
	/*(let((x(*a a))
		(y(*b b)))
		(+x y)
		)*/
	auto body = exp->letBody();
	auto letSeq = exp->letArgs();
	vector<string> argVar;
	vector<shared_ptr<SchemeValue>> argVal;
	for_each(letSeq.begin(), letSeq.end(), [&](shared_ptr<SchemeValue> seq) {
		argVar.push_back(seq->letArgVar()->toString());
		argVal.push_back(eval(seq->letArgVal(), env));
	});
	auto lambda = make_shared<LambdaValue>(argVar, body, env);
	return apply(lambda, listOfValue(argVal, env));
}
shared_ptr<SchemeValue> apply(shared_ptr<SchemeValue> procedure, vector<shared_ptr<SchemeValue>> arguments) {
	if (procedure->isProcedure()) {
		if (procedure->isPrimitiveProcedure()) {
			return procedure->call(arguments);
		}
		else if (procedure->isLambda()) {
			auto env = procedure->newEnv(arguments);
			return evalSeq(procedure->body(), env);
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

shared_ptr<SchemeValue> evalSeq(shared_ptr<SchemeValue> exp, EnvPtr env) {
	if (exp->isPair()) {
		auto result = Void();
		for (auto v : exp->cdr(0)) {
			result = eval(v, env);
		}
		return result;
	}
	else {
		cout << exp;
		fck("type should be pair ");
	}
}

shared_ptr<SchemeValue>  eval(shared_ptr<SchemeValue> exp, EnvPtr env) {
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
	else if (exp->is("set!")) {
		 env->assign(exp->setVar(), exp->setVal()); 
	}
	else if (exp->isDefinition()) {
		//auto a = 1;
		auto var = exp->var();
		auto val = eval(exp->val(env), env);
		env->define(var, val);
	}
	else if (exp->isLambda()) {
		return exp;
	}
	else if (exp->isTagged("begin")) {
		return evalSeq(exp->beginBody(), env);
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
	else if (exp->isTagged("let")) {
		//makeLambda(/*)*/
		return letExp(exp, env);
	}
	else if (exp->application()) {
		return apply(eval(exp->operators(), env), listOfValue(exp->operands(), env));
	}
	else {
		exit(0);

	}

	//return {};
	//return make_shared<VoidValue>();
	return Void();


}
