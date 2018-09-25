
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <assert.h>
#include "scheme.h"
#include "env.h"
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






enum class Type {
	Symbol,
	List,
	Int,
	Str,
};
class Parser {
public:
	Parser(string& input):input(input) {
	}
	shared_ptr<SchemeValue> getInt(int &i) {
		auto beg = i;
		while (input.size()> i && isdigit(input[i])) {
			i++;
		}
		return make_shared<IntValue>(input.substr(beg, i- beg));
	}
	shared_ptr<SchemeValue> getList(int &i) {
		assert(input[i] == '(');
		i++;
		//auto 
		//auto list = new ListValue();
		auto list = make_shared<ListValue>();
		//auto list = new ListValue();
		while (input[i] != ')') {
			auto type = peek(i);
			if (type == Type::Int) {
				list->addValue(getInt(i));
			}
			else if (type == Type::Str) {
				list->addValue(getStr(i));

				 //getStr(i);
			}
			else if (type == Type::Symbol) {
				list->addValue(getSymbol(i));

				//return getSymbol(i);
			}
			else if (type == Type::List) {
				list->addValue(getList(i));

				//return (i);
			}
		}
		//return  (shared_ptr<SchemeValue>)(SchemeValue*)list;
		return list;
	}
	shared_ptr<SchemeValue> getStr(int &i) {
		assert(input[i] == '"');
		auto beg = i;
		i++;
		while (input.size()> i && input[i] != '"') {
			i++;
		}
		return make_shared<StringValue>(input.substr(beg, i - beg+1));
	}
	shared_ptr<SchemeValue> getSymbol(int &i) {
		//assert(input[i] == ';');
		auto beg = i;
		//i++;
		while (input.size()> i && (!isspace(input[i]))) {
			i++;
		}
		return make_shared<SymbolValue>(input.substr(beg, i - beg));
	}
	shared_ptr<SchemeValue> parser() {
		int i = 0;
		auto type = peek(i);
		if (type == Type::Int) {
			return getInt(i);
		}
		else if (type == Type::Str) {
			return getStr(i);
		}
		else if (type == Type::Symbol) {
			return getSymbol(i);
		}
		else if (type == Type::List) {
			return getList(i);
		}
	}
	Type peek(int &idx) {
		auto i = idx;
		while (input.size()> i && isspace(input[i])) {
			i++;
		}
		if (i == input.size()) {
			cout << "error " << input << endl;
			exit(1);
		}
		idx = i;
		if (isdigit(input[i])) {
			return Type::Int;
		}
		else if (input[i] == '(') {
			return Type::List;
		}
		else if (input[i] == '"') {
			return Type::Str;
		}
		/*else if (input[i] == ')') {
			return Type::listEN
		}*/
		return Type::Symbol;

		/*
		auto j = i;
		while (input.size()> i && isspace(input[i])) {
		i++;
		}*/
	}
	string input;
};
//pair<Type, string> peek(const string& input) {
//	auto i = 0;
//	while (input.size()> i && isspace(input[i])) {
//		i++;
//	}
//	if (i == input.size()) {
//		cout << "error " << input << endl;
//		exit(1);
//	}
//	auto j = i;
//	while (input.size()> i && isspace(input[i])) {
//		i++;
//	}
//	string result = input.substr(j, i - j);
//	if (isdigit[0]) {
//		return 
//	}
//}
shared_ptr<SchemeValue>  eval(shared_ptr<SchemeValue>& exp, Env& env) {
	//return {};
	if (exp->selfEvaluting()) {
		//cout << *exp << endl;
		return exp;
	}
	else 
	if (exp->isVariable()) {
		auto result =  env.lookup(exp);
		//cout << *result;
		return result;
	}
	else if (exp->isDefinition()) {
		//auto a = 1;
		env.define(exp->var(), eval(exp->val(), env));
		return make_shared< VoidValue>();
	}
		//return {};
	return make_shared< VoidValue>();


}
void testParser(string& input) {
	Parser a{ input };

	cout << *(a.parser()) << endl;
}
void testEval(string& input, Env& init) {
	Parser a{ input };
	auto b = a.parser();
	cout << "result " <<*eval(b, init) << endl;;

}
void runloop() {
	string input = R"("a b c")";

	string input1 = R"(132)";
	string symbol = R"( a )";
	string list = R"( (1 2 3))";
	string list1 = R"( (+ 1 (f 2))";
	string define = R"((define a 411))";
	string look = R"(a)";
	auto init = Env();
	testEval(define, init);
	testEval(look, init);

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