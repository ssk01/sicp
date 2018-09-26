#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <assert.h>
#include <functional>
//using namespace std;
using std::ostream;
using std::string;
using std::shared_ptr;
using std::vector;
using std::cout;
using std::endl;
class VoidValue;
class SchemeValue;
shared_ptr<SchemeValue> Void();
class SchemeValue {
public:
	virtual void print(ostream& out) const = 0;
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
	virtual bool isDefinition() {
		return false;
	}
	virtual bool isProcedure() {
		return false;
	}
	virtual bool isPrimitive() {
		return false;
	}
	virtual bool application() {
		return false;
	}
	virtual int  intValue() {
		exit(0);
		return -1;
	}

	virtual bool isIf() {
		return false;
	}
	virtual bool isInt() {
		return false;
	}
	virtual bool isVariable() {
		return false;
	}
	virtual shared_ptr<SchemeValue> var() {
		exit(0);
		return Void();
	}
	virtual shared_ptr<SchemeValue> call(vector<shared_ptr<SchemeValue>>) {
		exit(0);
		return Void();
	}
	//virtual shared_ptr<SchemeValue>() {

	//}
	virtual shared_ptr<SchemeValue> car() {
		exit(0);
		return Void();
	}
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
	virtual vector<shared_ptr<SchemeValue>> operands() {
		return cdr(1);
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
	virtual shared_ptr<SchemeValue> val() {
		exit(0);
		return{};
	}
	virtual bool is(const string& word) {
		return false;
	}
	virtual ~SchemeValue() {};


	//virtual ostream& operator<<(ostream& out) = 0;

};
ostream& operator<<(ostream& out, const SchemeValue& a); 
class VoidValue : public SchemeValue {
	void print(ostream& out) const override {
		out << "#void";
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
	bool isPrimitive() override {
		return true;
	}
	shared_ptr<SchemeValue> call(vector<shared_ptr<SchemeValue>> args) {
		return value(args);
	}
	using func = std::function<shared_ptr<SchemeValue>(vector<shared_ptr<SchemeValue>>)>;
	Procedure(func input, bool primitived=false):value(input), primitive(primitived){}
	func value;
	bool primitive;

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
	shared_ptr<SchemeValue> var() {
		assert(value.size() == 3);
		assert(value[1]->isVariable());
		return value[1];
	}
	shared_ptr<SchemeValue> val() {
		assert(value.size() == 3);
		return value[2];
	}
	bool isDefinition() override {
		if (value.size() > 0 && value[0]->is("define")) {
			return true;
		}
		return false;
	}
	bool application() override {
		return true;
	}

	bool isIf() override {
		if (value.size() > 0 && value[0]->is("if")) {
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

