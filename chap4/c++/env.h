#pragma once
#include <memory>
#include <map>
#include "scheme.h"
#include <iostream>
using std::shared_ptr;
using std::map;
class Env{
public:
	//enclosingEnvironment
	Env() = default;
	shared_ptr<Env> parent;
	map<string, shared_ptr<SchemeValue>> env;

	shared_ptr<SchemeValue> lookup(shared_ptr<SchemeValue> var_) {
		//return {};
		auto var = var_->toString();
		if (env.find(var) == env.end()) {
			std::cout << "not find 404";
			exit(1);
		}
		return env[var];
	}
	void define(shared_ptr<SchemeValue> var_, shared_ptr<SchemeValue> val) {
		auto var = var_->toString();
		env[var] = val;
	}
};