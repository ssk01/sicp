#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <assert.h>
//using namespace std;
using std::ostream;
using std::string;
using std::shared_ptr;
using std::vector;

class SchemeValue {
public:
	virtual void print(ostream& out) const = 0;
	virtual bool selfEvaluting() {
		return false;
	}
	
	virtual bool isDefinition() {
		return false;
	}
	virtual bool isVariable() {
		return false;
	}
	virtual shared_ptr<SchemeValue> var() {
		exit(0);
		return {};
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
ostream& operator<<(ostream& out, const SchemeValue& a) {
	a.print(out);
	return out;
}
class VoidValue : public SchemeValue {
	void print(ostream& out) const override {
		out << "#void";
	}
};
class IntValue : public SchemeValue {
public:
	void print(ostream& out) const override {
		out << value;
	}
	IntValue(string& input) : value(stoi(input)) {

	}
	bool selfEvaluting() override {
		return true;
	}
	int value;
};
class SymbolValue : public SchemeValue {
public:
	SymbolValue(string& input) : value(input) {}
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
	StringValue(string& input) : value(input.substr(1, input.size() - 2)) {}
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