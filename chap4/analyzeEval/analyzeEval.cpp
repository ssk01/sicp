#include "scheme.h"
#include "env.h"
#include "parser.h"
#include "eval.h"
#include <string>
#include <vector>
#include <iostream>
#include <memory> 
using namespace std;
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
	string funFact = R"(
(define (fact n)
		(if (< n 1)
			1
			(* n (fact(- n 1)))))
	)";
	string funFactCall = R"((fact 4 ))";
	testAnalyzeEval({ funFact, funFactCall });

}