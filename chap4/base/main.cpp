
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <assert.h>
#include "scheme.h"
#include "env.h"
#include "parser.h"
#include "eval.h"
#include "../analyzeEval/analyzeEval.h"
#include <numeric>
#include <algorithm>
using namespace std;


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
	cout << *(b) << endl;
	cout << "parser success" << endl;
	cout << ">...........................................<" << endl;
	cout << input << "\tresult ";
	cout << *eval(b, init) << endl;
	cout << ">...........................................<" << endl;


}
void testEval(const vector<string>& programs) {
	cout << "________________________________ test begin________________________________" << endl;
	auto init = initEnv();
	for (auto p : programs) {
		testEval(p, init);
	}
	cout << "________________________________ test end________________________________" << endl;

}
void testEval(initializer_list<string> input) {
	vector<string> programs{ input };
	testEval(programs);
}

 //todo support for - int;
void consTest() {
	string cons = R"(
	(define a (cons (cons 1 2) (cons 3 4)))
)";
	string cons_1 = R"(
	(define a1 (cons 1 (list 2 3)))
)";
	string list = R"( 
	(list 1 2)
)";
	string cons2 = R"(
	(car a)
)";
	string cons3 = R"(
	(cdr a)
)";
	string cons_2 = R"(
	(car a1)
)";
	string cons_3 = R"(
	(cdr a1)
)";
	string cons1 = R"(
	 a
)";	
	string cons_4 = R"(
	 a1
)";
	//testEval(vector<string>{ list});
	//testEval(vector<string>{ cons_1, cons_4, cons_2, cons_3, "(cadr a1)", "(caddr a1)",
		//"(cddr a1)", "(cdr (cdddr a1))"});
	testEval(vector<string>{ cons, cons1, cons2, cons3, "(car (car a))", "(cdr (car a))"});

}
void letTest() {
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
	
	testEval(vector<string>{ let, let1 });

}
void beginTest() {
	string beg1 = R"(
	(if 1 (begin (+3 3) 4)  5)
)";
	string fun = R"(
	(define (f a) (+ a 4) 44 )
)";
	//testEval({ fun, "(f 3)" });
	//testEval({ "(define(f a) (let((x 3))(+3 4)  (+x 4)) (+ x 77))", "(f 3)" });
	testEval({ beg1 });
}
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
void factTest() {
	string funFact = R"(
(define (fact n)
		(if (< n 1)
			1
			(* n (fact(- n 1)))))
	)";
	string funFactCall = R"((fact 10 ))";
	testEval({ funFact, funFactCall });
	/*vector<string> input(100, funFactCall);
	input.insert(input.begin(), funFact);
	testEval(input);*/
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
	//testEval({ "(list 1 2 ) " });
	testEval({ R"(    (list 1 2 3)      )" });
	//testEval(plus, init);
	/*testEval(func, init);
	testEval(funcCall, init);*/


	//(*n(fact(-n 1)))))
	//((> test 2) 3)
	
	
	//letTest();
	//consTest();
	//factTest();
	//beginTest();






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
	//analyzeLoop();

	cout << "hello world" << endl;
	int a;
	//cin >> a;
}

// fuck todo (list 'a 'b  'c)