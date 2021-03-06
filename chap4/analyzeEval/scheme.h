#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <assert.h>
#include <functional>
#include "util.h"
//using namespace std;
using std::ostream;
using std::string;
using std::shared_ptr;
using std::vector;
using std::cout;
using std::make_shared;
using std::endl;
using std::function;
class VoidValue;
class SchemeValue;
class Env;
using SchemeValuePtr = shared_ptr<SchemeValue>;
using EnvPtr = shared_ptr<Env>;
using analyzeFun = function<SchemeValuePtr(EnvPtr)>;
//shared_ptr<SchemeValue> Void();
shared_ptr<SchemeValue> expandClauses(vector<shared_ptr<SchemeValue>> exp);
//class VoidValue;
//class SchemeValue;
shared_ptr<SchemeValue> Void();
vector<string> listToString(const vector<shared_ptr<SchemeValue>> &lists);
shared_ptr<SchemeValue> vectorToList(const vector<shared_ptr<SchemeValue>> &lists);


class SchemeValue {
public:
	virtual void print(ostream& out) const = 0;
	virtual void print(ostream& out, bool outside) const {}// const = 0;
	virtual bool selfEvaluting() {
		return false;
	}
	virtual bool T() {
		exit(0);
		return false;
	}
	virtual bool F() {
		return !T();
	}
	virtual void setEnv(EnvPtr env)  {
		fck("type error");
	}
	virtual analyzeFun bodyFun()  {
		fck("type error");
		return {};
	};
	virtual void bodyFun(analyzeFun call)  {
		fck("type error");
	}
	virtual	shared_ptr<SchemeValue> body(){
		fck("type error");
		return Void();
	}
	virtual bool isDefinition() {
		return false;
	}
	virtual bool isVoid()  {
		return false;
	}
	virtual SchemeValuePtr pairFirst() {
		fck("type error, should be pair");
		return Void();
	}
	virtual SchemeValuePtr pairSecond() {
		fck("type error, should be pair");
		return Void();
	}
	virtual bool isProcedure() {
		return false;
	}
	virtual bool isLambda() {
		return false;
	}
	
	virtual bool isPrimitiveProcedure() {
		return false;
	}
	bool application() {
		return isPair();
	}
	virtual bool isPair() {
		return false;
	}
	virtual int  intValue() {
		exit(0);
		return -1;
	}

	virtual bool isIf() {
		return false;
	}
	virtual bool isTagged(const string& tag){
		return false;
	}
	virtual bool isInt() {
		return false;
	}
	virtual bool isVariable() {
		return false;
	}
	virtual bool isPairList() {
		return false;
	}
	virtual shared_ptr<SchemeValue> var() {
		fck("type error");
		return Void();

	}
	virtual shared_ptr<SchemeValue> val(EnvPtr env) {
		fck("type error");
		return Void();

	}
	virtual EnvPtr newEnv(vector<shared_ptr<SchemeValue>> argsValue) {
		fck("type error");
		return nullptr;
	}
	virtual shared_ptr<SchemeValue> call(vector<shared_ptr<SchemeValue>>) {
		exit(0);
		return Void();
	}
	//virtual shared_ptr<SchemeValue>() {

	//}
	//virtual shared_ptr<SchemeValue> car() {
	//		exit(0);
	//		return Void();
	//}
	virtual shared_ptr<SchemeValue> car(int i) {
		exit(0);
		return Void();
	}
	virtual vector<shared_ptr<SchemeValue>> cdr(int i) {
		exit(0);
		return {};
	}
	/*virtual shared_ptr<SchemeValue> cadr() {
		exit(0);
		return Void();
	}
	virtual shared_ptr<SchemeValue> caddr() {
		exit(0);
		return Void();
	}
	virtual shared_ptr<SchemeValue> cadddr() {
		exit(0);
		return Void();
	}*/
	virtual shared_ptr<SchemeValue> operators() {
		return car(0);
	}
	virtual shared_ptr<SchemeValue> setVar() {
		return car(1);
	}
	virtual shared_ptr<SchemeValue> setVal() {
		return car(2);
	}
	virtual vector<shared_ptr<SchemeValue>> operands() {
		return cdr(1);
	}
	virtual shared_ptr<SchemeValue> causeExp() {
		return car(1);
	}
	virtual vector<shared_ptr<SchemeValue>> causes() {
		return cdr(1);
	}
	
	virtual vector<SchemeValuePtr> letBody() {
		return cdr(2);
	}
	virtual shared_ptr<SchemeValue> letArgVar() {
		return car(0);
	}
	virtual shared_ptr<SchemeValue> letArgVal() {
		return car(1);
	}
	virtual vector<shared_ptr<SchemeValue>> letArgs() {
		return car(1)->cdr(0);
	}
	virtual SchemeValuePtr  beginBody() {
		return vectorToList(cdr(1));
	}
	virtual shared_ptr<SchemeValue> causeCond() {
		return car(0);
	}	
	virtual shared_ptr<SchemeValue> predict() {
		return car(1);
	}
	virtual shared_ptr<SchemeValue> trueBranch() {
		return car(2);
	}
	virtual shared_ptr<SchemeValue> falseBranch() {
		return car(3);
	}
	virtual string toString() {
		exit(0);
		return{};
	}


	virtual ~SchemeValue() {};
//protected:
	virtual bool is(const string& word) {
		return false;
	}
	//virtual ostream& operator<<(ostream& out) = 0;

};

ostream& operator<<(ostream& out, const SchemeValue& a); 
class VoidValue : public SchemeValue {
	void print(ostream& out) const override {
		out << "#void";
	}
	bool isVoid() override {
		return true;
	}
	bool selfEvaluting() override {
		return true;
	}
};
class Procedure : public SchemeValue {
public:
	void print(ostream& out) const override {
		out << "#function";
	}
    bool isProcedure() override{
		return true;
	}
	bool isPrimitiveProcedure() override {
		return true;
	}
	shared_ptr<SchemeValue> call(vector<shared_ptr<SchemeValue>> args) {
		return value(args);
	}
	using func = std::function<shared_ptr<SchemeValue>(vector<shared_ptr<SchemeValue>>)>;
	Procedure(func input):value(input){}
	func value;
	//bool primitive;

};
class IntValue : public SchemeValue {
public:
	void print(ostream& out) const override {
		out << value;
	}
	bool T() override {
		return value > 0;
	}
	bool isInt() {
		return true;
	}
	int intValue() override {
		return value;
	}
	virtual bool isPair() {
		return false;
	}
	IntValue(const string& input) : value(stoi(input)) {
	}
	IntValue(int input) : value(input) {
	}
	bool selfEvaluting() override {
		return true;
	}
	int value;
};
class SymbolValue : public SchemeValue {
public:
	SymbolValue(const string& input) : value(input) {}
	string value;

	void print(ostream& out) const override {
		out << " symbol: ";
		out << value;
	}
	string toString() override {
		return value;
	}
	bool isVariable() override {
		return true;
	}
	bool is(const string& word) override {
		return value == word;
	}
};

class LambdaValue : public SchemeValue {
public:
	void print(ostream& out) const override {
		out << " LambdaValue: ";
	}

	LambdaValue(const vector<string>& args, vector<shared_ptr<SchemeValue>> body, EnvPtr env) : args(args), value(vectorToList(body)), parent(env) {}
	bool isLambda() override {
		return true;
	}
	bool isProcedure() override {
		return true;
	}
	shared_ptr<SchemeValue> body() override {
		return value;
	}
	EnvPtr newEnv(vector<shared_ptr<SchemeValue>> argsValue) override;
	EnvPtr parent;
	vector<string> args;
	shared_ptr<SchemeValue> value;
	analyzeFun bodyFun_;
	analyzeFun bodyFun() override {
		//bodyFun_ = call;
		return bodyFun_;
	};
	void bodyFun(analyzeFun call) override {
		 bodyFun_ = call;
	};
	void setEnv(EnvPtr env) override {
		assert(parent == nullptr);
		parent = env;
	}
};


class StringValue : public SchemeValue {
public:

	StringValue(const string& input) : value(input.substr(1, input.size() - 2)) {}
	string value;
	void print(ostream& out) const override {
		out << " str: ";
		out << value;
	}
	bool selfEvaluting() override {
		return true;
	}
};

class PairValue : public SchemeValue {
public:

	PairValue() {
	}
	//void print()
	SchemeValuePtr pairFirst() override {
		return first;
		//fck("type error");
		//return Void();
	}
	SchemeValuePtr pairSecond() override {
		//if (second->isVoid()) {
		//	return 
		//}
		return second;
		//fck("type error");
		//return Void();
	}
	void print(ostream& out, bool outside) const override {
		if (outside) {
			out << "( ";
		}
		first->print(out);
		out << " ";
		//if (!second->isPair()) {
		//	out << ". ";
		//}
		//out << ".a ";

		if (!second->isVoid()) {
			if (second->isPair()) {
				second->print(out, false);
			}
			else {
				out << ". ";
				second->print(out);
			}
		}
		if (outside) {

			out << ")";
		}
		
	}
	void print(ostream& out	) const override {
		/*out << "( ";
		first->print(out);
		if (!second->isPair()) {
			out << ", ";
		}
		if (!second->isVoid()) {
			second->print(out);
		}
		out << ")";*/
		print(out, true);
	}
	
	//void print(ostream& out) const override {
	//}
	PairValue(SchemeValuePtr first, SchemeValuePtr second): first(first), second(second) {
	}
	bool isPairList() override {
		//return true;
		if (second->isVoid()) {
			return true;
		}
		return second->isPairList();
	}
	bool isPair() override {
		return true;
	}
	shared_ptr<SchemeValue> first;
	shared_ptr<SchemeValue> second;
};
class ListValue : public SchemeValue {

public:

	ListValue() {
		/*if (input[0] == '(') {

		auto i = peek(input);
		}*/
	}
    shared_ptr<SchemeValue> car(int i) override {
		//exit(0);
		if (value.size() > i) {
			return value[i];
		}
		return Void();
	}
	vector<shared_ptr<SchemeValue>> cdr(int i) override {
		//exit(0);
		if (value.size() > i) {
			return {value.begin()+i, value.end()};
		}
		return {};
	}
	shared_ptr<SchemeValue> var() override {
		assert(value.size() >= 3);
		if (car(1)->isVariable()) {
			return car(1);
		}
		else if (car(1)->isPair()) {
			return car(1)->car(0);
		}
		fck("type error");
	}
	shared_ptr<SchemeValue> val(EnvPtr env) override {
		assert(value.size() >= 3);
		if (car(1)->isVariable()) {
			return car(2);
		}
		else if (car(1)->isPair()) {
			return make_shared<LambdaValue>(listToString(car(1)->cdr(1)), cdr(2), env);

		}
	}
	bool isDefinition() override {
		if (value.size() > 0 && value[0]->is("define")) {
			return true;
		}
		return false;
	}
	bool isPair() override {
		return true;
	}

	bool isIf() override {
		if (value.size() > 0 && value[0]->is("if")) {
			return true;
		}
		return false;
	}
	bool isTagged(const string& tag) override {
		if (value.size() > 0 && value[0]->is(tag)) {
			return true;
		}
		return false;
	}
	void print(ostream& out) const override {
		out << "(";
		auto end = value.size() - 1;
		for (auto i = 0; i < value.size(); i++) {
			value[i]->print(out);
			if (end != i) {
				out << ", ";
			}
		}
		out << ")";
	}
	void addValue(shared_ptr<SchemeValue> v) {
		value.push_back(v);
	}
	vector<shared_ptr<SchemeValue>> value;
};

SchemeValuePtr cons(SchemeValuePtr first, SchemeValuePtr second);
SchemeValuePtr append(SchemeValuePtr first, SchemeValuePtr second);
SchemeValuePtr car(SchemeValuePtr exp) ;
SchemeValuePtr cdr(SchemeValuePtr exp) ;
SchemeValuePtr makeList(vector<SchemeValuePtr> exp);
