#include "lazy.h"
using namespace std;
SchemeValuePtr lazyEval(SchemeValuePtr exp, EnvPtr env);
SchemeValuePtr force(SchemeValuePtr exp);
SchemeValuePtr actualVal(SchemeValuePtr exp, EnvPtr env) {
	//thunk may be high level, while eval not treat thunk;
	return force(lazyEval(exp, env));
}
SchemeValuePtr lazyEvalSeq(SchemeValuePtr exp, EnvPtr env) {
	if (exp->isPair()) {
		auto result = Void();
		for (auto v : exp->cdr(0)) {
			result = lazyEval(v, env);
		}
		return result;
	}
	else {
		cout << exp;
		fck("type should be pair ");
	}
}
SchemeValuePtr force(SchemeValuePtr exp) {
	if (exp->isThunk()){
		if (exp->isEvaled()) {
			return exp->result();
		}
		else {
			exp->isEvaled() = true;
			return actualVal(exp->value(), exp->env());
		}
	}
	else {
		return exp;
	}
}
SchemeValuePtr delay(SchemeValuePtr exp, EnvPtr env) {
	return make_shared<Thunk>(exp, env);
}


vector<SchemeValuePtr>  lazyListOfValue(vector<SchemeValuePtr> arguments, EnvPtr env) {
	vector<SchemeValuePtr> result;
	for (auto arg : arguments) {
		result.emplace_back(delay(arg, env));
	}
	return result;
}
SchemeValuePtr lazyApply(SchemeValuePtr procedure, vector<SchemeValuePtr> arguments, EnvPtr env) {
	if (procedure->isProcedure()) {
		if (procedure->isPrimitiveProcedure()) {
			vector<SchemeValuePtr> args;
			for (auto a : arguments) {
				args.push_back(actualVal(a, env));
			}
			return procedure->call(args);
		}
		else if (procedure->isLambda()) {
			vector<SchemeValuePtr> args;
			for (auto a : arguments) {
				args.push_back(delay(a, env));
			}
			auto env = procedure->newEnv(args);
			return lazyEvalSeq(procedure->body(), env);
		}
	}
	fck("apply fuck");
}
SchemeValuePtr lazyEval(SchemeValuePtr exp, EnvPtr env) {
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
		auto val = lazyEval(exp->val(env), env);
		env->define(var, val);
	}
	else if (exp->isLambda()) {
		return exp;
	}
	else if (exp->isTagged("begin")) {
		return lazyEvalSeq(exp->beginBody(), env);
	}
	else if (exp->isIf()) {
		auto predict = actualVal(exp->predict(), env);
		if (predict->T()) {
			return lazyEval(exp->trueBranch(), env);
		}
		auto falseBranch = exp->falseBranch();
		if (falseBranch != Void()) {
			return lazyEval(falseBranch, env);
		}
	}
	else if (exp->isTagged("cond")) {
		return lazyEval(expandClauses(exp->causes()), env);
	}
	else if (exp->isTagged("let")) {
		//makeLambda(/*)*/
		return letExp(exp, env);
	}
	else if (exp->application()) {
		return lazyApply(actualVal(exp->operators(), env), exp->operands(), env);
	}
	else {
		exit(0);

	}

	//return {};
	//return make_shared<VoidValue>();
	return Void();
}

void testLazyEval(string& input, EnvPtr init) {
	Parser a{ input };
	auto b = a.parser();
	cout << *(b) << endl;
	cout << "parser success" << endl;
	cout << ">...........................................<" << endl;
	cout << input << "\tresult ";
	cout << *lazyEval(b, init) << endl;
	cout << ">...........................................<" << endl;
}
void testLazyEval(const vector<string>& programs) {
	cout << "________________________________ test begin________________________________" << endl;
	auto init = initEnv();
	for (auto p : programs) {
		testLazyEval(p, init);
	}
	cout << "________________________________ test end________________________________" << endl;

}
void testLazyEval(initializer_list<string> input) {
	vector<string> programs{ input };
	testLazyEval(programs);
}
void testDelay() {
	string a1 = R"(
	(define(try a b)
(if (= a 0) 1 b))
)";
//; = > ok

	string a2 = R"(
(try 0 (/ 1 0))
)";
//; = > 10
	testLazyEval({ a1,a2 });
}