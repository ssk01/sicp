#pragma once
#include <string>
#include <memory>
#include "scheme.h"

//using std::cout;
//using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;

enum class Type {
	Symbol,
	List,
	Int,
	Str,
};
class Parser {
public:
	Parser(string& input) :input(input) {
	}
	shared_ptr<SchemeValue> getInt(int &i) {
		auto beg = i;
		while (input.size()> i && isdigit(input[i])) {
			i++;
		}
		return make_shared<IntValue>(input.substr(beg, i - beg));
	}
	shared_ptr<SchemeValue> getList(int &i) {
		assert(input[i] == '(');
		i++;
		//auto 
		//auto list = new ListValue();
		auto list = make_shared<ListValue>();
		//auto list = new ListValue();
		auto type = peek(i);
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
				//i++;

				//return (i);
			}
		}
		i++;
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
		return make_shared<StringValue>(input.substr(beg, i - beg + 1));
	}
	shared_ptr<SchemeValue> getSymbol(int &i) {
		//assert(input[i] == ';');
		auto beg = i;
		//i++;
		if (specialString.find(input[i]) != -1) {
			i++;
		}
		else {
			while (input.size() > i && (!isspace(input[i]) && (specialString.find(input[i]) == -1))) {
				i++;
			}
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
	static const string specialString;

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
