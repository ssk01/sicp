
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <assert.h>
#include "scheme.h"
#include "env.h"
#include "parser.h"
#include "eval.h"
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
void testEval(string& input, Env& init) {
	Parser a{ input };
	auto b = a.parser();
	cout << "result " <<*eval(b, init) << endl;;

}
// todo support for - int;

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
	string if_ = R"((if 0 3 4))";
	string plus = R"((+ 1 123))";
	//testEval(define, init);
	//testEval(look, init);
	testEval(plus, init);

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
	int a = 3;
	//map<int, shared_ptr<int>> a;
	//auto b =make_shared<int>(3);
	//cout << *a[1] << endl;
	//{
	//	//auto b = a[1];
	//	//aa(a[1]);
	//}
}