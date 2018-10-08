
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <assert.h>
#include "scheme.h"
#include "env.h"
#include "parser.h"
#include "eval.h"
#include <numeric>
#include <algorithm>
using namespace std;

// 1
// "dfasf"
// (+ 1 2)
//(define c 3)
//(f 23 32)



//bool selfEvaluting(string& input) {
//	if (isdigit(input[0]) || input[0] == '"') {
//		return true;
//	}
//	return false;
//}

//
//(cond((self - evaluating ? exp) exp)
//((variable ? exp) (lookup - variable - value exp env))
//((quoted ? exp) (text - of - quotation exp))
//((assignment ? exp) (eval - assignment exp env))
//((definition ? exp) (eval - definition exp env))
//((if ? exp) (eval - if exp env))
//((lambda ? exp)
//(make - procedure(lambda - parameters exp)
//(lambda - body exp)
//env))
//((begin ? exp)
//(eval - sequence(begin - actions exp) env))
//((cond ? exp) (eval(cond->if exp) env))
//((application ? exp)
//(apply(eval(operator exp) env)
//(list - of - values(operands exp) env)))
//(else
//(error "Unknown expression type -- EVAL" exp))))




void testParser(string& input) {
	Parser a{ input };

	cout << *(a.parser()) << endl;
}
void testEval(string& input, EnvPtr init) {
	Parser a{ input };
	auto b = a.parser();
	/*cout << *(b) << endl;
	cout << "parser success" << endl;*/
	cout << "result " <<*eval(b, init) << endl;;

}
void testEval(vector<string>& programs) {
	auto init = initEnv();
	for (auto p : programs) {
		testEval(p, init);
	}

}

 //todo support for - int;
void condTest() {

	string cond = R"(
		(define (conds test) 
			(cond ((= test 1) 1)
	((> test 2) 3)

				(else 4)	
			)
		)

)";
	string cond_1 = R"(
	(conds 1)
)";
	string cond_2 = R"(
	(conds 2)
)";
	string cond_3 = R"(
	(conds 3)
)";
	string cond_4 = R"(
	(conds 4)
)";
	testEval(vector<string>{ cond, cond_1, cond_2, cond_3, cond_4 });
}
void runloop() {
	auto init = initEnv();
	string input = R"("a b c")";

	string input1 = R"(132)";
	string symbol = R"( a )";
	string list = R"( (1 2 3))";
	string list1 = R"( (+ 1 (f 2))";
	string define = R"((define a 411))";

	string defineLambda = R"((define (f a) (+ a 1)))";
	string look = R"(a)";
	string if_ = R"((if a 3 4))";
	string if_1 = R"((if a 3 ))";
	string if_2 = R"((if 0 3 ))";
	string plus = R"((+ 1 123))";
	string func = R"((define (f a) (+ a 11)))";
	string funcCall = R"((f 1))";
	//testEval(define, init);
	//testEval(plus, init);
	/*testEval(func, init);
	testEval(funcCall, init);*/
	string funFact = R"(
(define (fact n)
		(if (< n 1)
			1
			(* n (fact(- n 1)))))
	)";
	string funFactCall = R"((fact 5))";

	//(*n(fact(-n 1)))))
	//((> test 2) 3)

	//testEval(vector<string>{ funFact, funFactCall });
	//testEval(vector<string>{ plus });
	//testEval(vector<string>{ define, if_ });
	//testEval(vector<string>{ define, if_1, if_2 });
	//testEval(vector<string>{ define, "(< a 3)" });
	//testParser(input);
	//testParser(input1);
	//testParser(list);
	/*testEval(input);
	testEval(input1);*/
	//testParser(list);

	//testParser(list1);
	/*testParser(symbol);
	testEval(input);
	testEval(input1);*/
	/*testParser(list);
	testParser(list1);*/

	//cout << *(eval(input)) << endl;
	//cout << *(eval(input1)) << endl;
}

int main() {
	runloop();

	cout << "hello world" << endl;
	int a = 3;
	//map<int, shared_ptr<int>> a;
	//auto b =make_shared<int>(3);
	//cout << *a[1] << endl;
	//{
	//	//auto b = a[1];
	//	//aa(a[1]);
	//}
}