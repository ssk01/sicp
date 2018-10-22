#include "scheme.h"
#include "env.h"
#include "parser.h"
#include "eval.h"
#include <string>
#include <vector>
#include <iostream>
#include <memory> 
#include <algorithm>
#include "analyzeEval.h"
using namespace std;

using analyzeFun = function<SchemeValuePtr(EnvPtr)>;


analyzeFun analyzeSeq(SchemeValuePtr exp) {
	if (exp->isPair()) {
		vector<analyzeFun> analyzes;
		/*for (auto v : ) {
			analyzes.emplace_back(analyze(v));
		}*/
		auto seq = exp->cdr(0);
		transform(seq.begin(), seq.end(), back_inserter(analyzes), analyze);
		return [=](EnvPtr env) {
			auto result = Void();
			for (auto v : analyzes) {
				result = v(env);
			}
			return result;
		};
	}
	else {
		cout << exp;
		fck("type should be pair ");
	}
}
analyzeFun analyzeLambda(SchemeValuePtr exp) {
	exp->bodyFun(analyzeSeq(exp->body()));
	return [=](EnvPtr env) {
		exp->setEnv(env);
		return exp;
	};
}

SchemeValuePtr analyzeApplyEval(SchemeValuePtr procedure, vector<SchemeValuePtr> arguments) {
	if (procedure->isProcedure()) {
		if (procedure->isPrimitiveProcedure()) {
			return procedure->call(arguments);
		}
		else if (procedure->isLambda()) {
			auto env = procedure->newEnv(arguments);
			return procedure->bodyFun()(env);
		}
	}
	fck("apply fuck");
}

analyzeFun analyzeApply(SchemeValuePtr exp) {
	auto operators = analyze(exp->operators());
	vector<analyzeFun> argsfun;
	for (auto arg : exp->operands()) {
		argsfun.emplace_back(analyze(arg));
	}
	return [=](EnvPtr env) {
		auto fun = operators(env);
		vector<SchemeValuePtr> args;
		for (auto arg : argsfun) {
			args.emplace_back(arg(env));
		}
		return analyzeApplyEval(fun, args);
	};
}
SchemeValuePtr analyzeEval(SchemeValuePtr exp, EnvPtr env) {
	auto fun = analyze(exp);
	return fun(env);
}
analyzeFun analyze(SchemeValuePtr exp) {
	if (exp->selfEvaluting()) {
		return [=](EnvPtr) {return exp; };
	}
	else if (exp->isVariable()) {
		return [=](EnvPtr env) {return env->lookup(exp); };
	}
	else if (exp->isTagged("set!")) {
		return [=](EnvPtr env) { env->assign(exp->setVar(), exp->setVal()); return Void(); };
	}
	else if (exp->isTagged("define")) {
		auto val = analyze(exp->val(nullptr));
		auto var = exp->var();

		return [=](EnvPtr env) {
			env->define(var, val(env));
			return Void();
		};
	}
	else if (exp->isTagged("begin")) {
		return analyzeSeq(exp->beginBody());
	}
	else if (exp->isTagged("if")) {
		auto predict = analyze(exp->predict());
		auto trueBranch = analyze(exp->trueBranch());
		auto falseBranch = analyze(exp->falseBranch());
		return [=](EnvPtr env) {
			if (predict(env)->T()) {
				return trueBranch(env);
			}
			else {
				return falseBranch(env);
			}
		};
	}
	else if (exp->isTagged("cond")) {
		return analyze(expandClauses(exp->causes()));
	}
	else if (exp->isLambda()) {
		//cout << "analyzelambd";
		return analyzeLambda(exp);
	}
	else if (exp->isTagged("let")) {
	
		auto body = exp->letBody();
		auto letSeq = exp->letArgs();
		vector<string> argVar;
		vector<SchemeValuePtr> argVal;
		for_each(letSeq.begin(), letSeq.end(), [&](SchemeValuePtr seq) {
			argVar.push_back(seq->letArgVar()->toString());
			//argVal.push_back(eval(seq->letArgVal(), env));
		});
		auto lambda = make_shared<LambdaValue>(argVar, body, nullptr);
		auto list = make_shared<ListValue>();
		list->addValue(lambda);
		for (auto c : letSeq) {
			list->addValue(c->letArgVal());
		}
		return analyze(list);
	}
	else if (exp->application()) {
		return analyzeApply(exp);
	}
}







void testAnalyzeEval(string& input, EnvPtr init) {
	Parser a{ input };
	auto b = a.parser();
	cout << *(b) << endl;
	cout << "parser success" << endl;
	cout << ">...........................................<" << endl;
	cout << input << "\tresult ";
	cout << *analyzeEval(b, init) << endl;
	cout << ">...........................................<" << endl;


}
void testAnalyzeEval(const vector<string>& programs) {
	cout << "________________________________ test begin________________________________" << endl;
	auto init = initEnv();
	for (auto p : programs) {
		testAnalyzeEval(p, init);
	}
	cout << "________________________________ test end________________________________" << endl;

}
void testAnalyzeEval(initializer_list<string> input) {
	vector<string> programs{ input };
	testAnalyzeEval(programs);
}

void analyzeLoop() {
	/*testAnalyzeEval({ "(define a 411)", "(if a 3 4)" });
	string defineLambda = R"((define (f a) (+ a 1)))";
	string funcCall = R"((f 1))";
	testAnalyzeEval({ defineLambda, funcCall });*/
//	string funFact = R"(
//(define (fact n)
//		(if (< n 1)
//			1
//			(* n (fact(- n 1)))))
//	)";
//	string funFactCall = R"((fact 4 ))";
//	testAnalyzeEval({ funFact, funFactCall });

//	string beg1 = R"(
//	(if 1 (begin (+3 3) 4)  5)
//)";
//	string fun = R"(
//	(define (f a) (+ a 4) 44 )
//)";
//	testAnalyzeEval({ beg1 });

	string let = R"(
	(define (squaresum a b)
			(let ((x (* a a))
				   (y (* b b)))
				(+ x y)
			)
	)	
	
)";
	string let1 = R"(
	(squaresum 3 4)
)";
	testAnalyzeEval({ let, let1 });

}