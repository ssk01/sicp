
#include "test.h"
using namespace std;


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
