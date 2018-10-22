#include "eval.h"
#include <algorithm>

using namespace std;
//using analyzeFun = function<SchemeValuePtr(EnvPtr)>;
EnvPtr LambdaValue::newEnv(vector<SchemeValuePtr> argsValue) {
	auto env = make_shared<Env>(this->parent);
	assert(args.size() == argsValue.size());
	for (int i = 0; i < args.size(); i++) {
		env->define(args[i], argsValue[i]);
	}
	return env;
}
SchemeValuePtr makeIf(SchemeValuePtr cond, SchemeValuePtr falseBranch, SchemeValuePtr trueBranch) {
	auto if_ = make_shared<ListValue>();
	if_->addValue(make_shared<SymbolValue>("if"));
	if_->addValue(cond);
	if_->addValue(falseBranch);
	if_->addValue(trueBranch);
	return if_;

}

SchemeValuePtr _expandClauses(vector<SchemeValuePtr> exp, int i) {
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

SchemeValuePtr expandClauses(vector<SchemeValuePtr> exp) {
	return _expandClauses(exp, 0);
}
SchemeValuePtr letExp(SchemeValuePtr exp, EnvPtr env) {
	/*(let((x(*a a))
		(y(*b b)))
		(+x y)
		)*/
	auto body = exp->letBody();
	auto letSeq = exp->letArgs();
	vector<string> argVar;
	vector<SchemeValuePtr> argVal;
	for_each(letSeq.begin(), letSeq.end(), [&](SchemeValuePtr seq) {
		argVar.push_back(seq->letArgVar()->toString());
		argVal.push_back(eval(seq->letArgVal(), env));
	});
	auto lambda = make_shared<LambdaValue>(argVar, body, env);
	return apply(lambda, listOfValue(argVal, env));
}
SchemeValuePtr apply(SchemeValuePtr procedure, vector<SchemeValuePtr> arguments) {
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

vector<SchemeValuePtr>  listOfValue(vector<SchemeValuePtr> arguments, EnvPtr env) {
	vector<SchemeValuePtr> result;
	for (auto arg : arguments) {
		result.emplace_back(eval(arg, env));
	}
	return result;
}



SchemeValuePtr evalSeq(SchemeValuePtr exp, EnvPtr env) {
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

SchemeValuePtr  eval(SchemeValuePtr exp, EnvPtr env) {
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
