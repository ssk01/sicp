#include <algorithm>
#include "amb_analyze.h"
#include "../base/env.h"
#include "../base/scheme.h"
#include "../base/parser.h"
using namespace std;
//using emptyFun = 
using failFun = function < SchemeValuePtr()>;
using succeedFun = function < SchemeValuePtr(SchemeValuePtr, failFun)>;
using amb_analyzeFun = function<SchemeValuePtr(EnvPtr,succeedFun, failFun)>;
amb_analyzeFun analyze1(SchemeValuePtr exp);



SchemeValuePtr amb_eval(SchemeValuePtr exp, EnvPtr e, succeedFun succeed, failFun fail) {
	auto fun = analyze1(exp);
	return fun( e, succeed, fail);
}


amb_analyzeFun amb_analyze(SchemeValuePtr exp) {
	auto ambChoices = exp->ambChoices();
	vector<amb_analyzeFun> choicesFun;
	transform(ambChoices.begin(), ambChoices.end(), back_inserter(choicesFun), analyze1);
	return [=](EnvPtr e, succeedFun succeed, failFun fail) {
		function<SchemeValuePtr(vector<amb_analyzeFun>)> try_next;
		try_next = [fail, succeed, e, &try_next](vector<amb_analyzeFun> choices)-> SchemeValuePtr {
			//[=](EnvPtr e, succeedFun succeed, failFun fail) {
			if (choices.empty()) {
				return fail();
			}
			else {
				auto fun = choices[0];
				vector<amb_analyzeFun> restChoices{ choices.begin()++, choices.end() };
				failFun fail1 = [&try_next, restChoices]() {
					return try_next(restChoices);
				};
				return fun(e, succeed, fail1);
			}
			//return Void();
		};
		return try_next(choicesFun);
	};
 }

//succeedFun amb_analyzeApply_success(vector<amb_analyzeFun>  argproc) {
//
//}
SchemeValuePtr	get_args(vector<amb_analyzeFun> argproc, EnvPtr env, succeedFun succeed, failFun fail) {
	if (argproc.empty()) {
		SchemeValuePtr args = make_shared<ListValue>();
		return succeed(args, fail);
	}
	else {
		//调用关系exn到爆炸，qq拼音是什么垃圾。
		auto arg = argproc[0];
		vector<amb_analyzeFun> restproc{ argproc.begin()++, argproc.end() };
		auto succeed1 = [=](SchemeValuePtr arg1, failFun fail1) {
			return get_args(restproc, env, [=](SchemeValuePtr arg2, failFun fail2) {
				//vector<SchemeValuePtr> args2
				arg2->addValue(arg1);
				//succeed()
				return succeed(arg2, fail2);
				
			}, fail1);
		};
		return arg(env, succeed1, fail);
	}
}
shared_ptr<SchemeValue> amb_analyzeApplyEval(shared_ptr<SchemeValue> procedure, vector<shared_ptr<SchemeValue>> arguments, succeedFun succeed, failFun fail) {
	if (procedure->isProcedure()) {
		if (procedure->isPrimitiveProcedure()) {
			return succeed(procedure->call(arguments),fail);
		}
		/*else if (procedure->isLambda()) {
			auto env = procedure->newEnv(arguments);
			return procedure->bodyFun()(env);
		}*/
	}
	fck("apply fuck");
}
amb_analyzeFun amb_analyzeApply(SchemeValuePtr exp) {
	auto funproc = analyze1(exp->operators());
	vector<amb_analyzeFun>  argproc;
	auto operands = exp->operands();
	transform(operands.begin(), operands.end(), back_inserter(argproc), analyze1);
	return [=](EnvPtr env, succeedFun succeed, failFun fail) {
		auto amb_analyzeApply_success = [=](SchemeValuePtr proc, failFun fail1) {
			return get_args(argproc, env, [=](SchemeValuePtr oper, failFun fail2) {
				return amb_analyzeApplyEval(proc, oper->cdr(0), succeed, fail2);
			}, fail1);
		};
		return funproc(env, amb_analyzeApply_success,fail);
	};
}
amb_analyzeFun analyze1(SchemeValuePtr exp) {
	if (exp->selfEvaluting()) {
		return [=](EnvPtr e, succeedFun succeed, failFun fail) {
			return succeed(exp, fail); 
		};
	}
	else if (exp->isVariable()) {
		return [=](EnvPtr env, succeedFun succeed, failFun fail) {
			return  succeed(env->lookup(exp), fail);
			//return succeed(exp, fail);
		};
		//return [=](EnvPtr env) {return env->lookup(exp); };
	}
	//else if (exp->isTagged("set!")) {
	//	return [=](EnvPtr env) { env->assign(exp->setVar(), exp->setVal()); return Void(); };
	//}
	//else if (exp->isTagged("define")) {
	//	auto val = amb_analyze(exp->val(nullptr));
	//	auto var = exp->var();

	//	return [=](EnvPtr env) {
	//		env->define(var, val(env));
	//		return Void();
	//	};
	//}
	//else if (exp->isTagged("begin")) {
	//	return amb_analyzeSeq(exp->beginBody());
	//}
	//else if (exp->isTagged("if")) {
	//	auto predict = amb_analyze(exp->predict());
	//	auto trueBranch = amb_analyze(exp->trueBranch());
	//	auto falseBranch = amb_analyze(exp->falseBranch());
	//	return [=](EnvPtr env) {
	//		if (predict(env)->T()) {
	//			return trueBranch(env);
	//		}
	//		else {
	//			return falseBranch(env);
	//		}
	//	};
	//}
	//else if (exp->isTagged("cond")) {
	//	return amb_analyze(expandClauses(exp->causes()));
	//}
	//else if (exp->isLambda()) {
	//	//cout << "amb_analyzelambd";
	//	return amb_analyzeLambda(exp);
	//}
	else if (exp->isTagged("amb")) {
		return amb_analyze(exp);
	}
	//else if (exp->isTagged("let")) {

	//	auto body = exp->letBody();
	//	auto letSeq = exp->letArgs();
	//	vector<string> argVar;
	//	vector<shared_ptr<SchemeValue>> argVal;
	//	for_each(letSeq.begin(), letSeq.end(), [&](shared_ptr<SchemeValue> seq) {
	//		argVar.push_back(seq->letArgVar()->toString());
	//		//argVal.push_back(eval(seq->letArgVal(), env));
	//	});
	//	auto lambda = make_shared<LambdaValue>(argVar, body, nullptr);
	//	auto list = make_shared<ListValue>();
	//	list->addValue(lambda);
	//	for (auto c : letSeq) {
	//		list->addValue(c->letArgVal());
	//	}
	//	return amb_analyze(list);
	//}
	else if (exp->application()) {
		return amb_analyzeApply(exp);
	}
}
void dirver_loop();
SchemeValuePtr internal_loop (failFun tryAgain) {
	string input;
	//getline(cin, input);
	input = "( +  1 2 )";
	cout << "input is: " << input << endl;
	if (input == "try") {
		return tryAgain();
	}
	else {
		cout << " starting new problem" << endl;
		auto initenv = initEnv();
		Parser a{ input };
		auto ast = a.parser();
		cout << *ast << endl;
		succeedFun succeed = [](SchemeValuePtr value, failFun fail) {
			cout << "result :" << value << endl;
			//return internal_loop(fail);
			return Void();

		};
		amb_eval(ast, initenv,  succeed,[]() {
			cout << "no more values" << endl;
			dirver_loop();

			return Void();
			}
		);
		return Void();
	}
}
void dirver_loop() {
	internal_loop([]() {
		cout << "no more problems" << endl;
		return Void();
	});
}