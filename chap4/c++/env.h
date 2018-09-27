#pragma once
#include <memory>
#include <map>
#include "scheme.h"
#include <iostream>
using std::shared_ptr;
using std::map;
class Env {
public:
	//enclosingEnvironment
	Env() = default;
	Env(shared_ptr<Env> parent_) :parent(parent_) {}
	shared_ptr<Env> parent;
	map<string, shared_ptr<SchemeValue>> env;

	shared_ptr<SchemeValue> lookup(shared_ptr<SchemeValue> var_) {
		//return {};
		auto var = var_->toString();
		if (env.find(var) == env.end()) {
			if (parent.get() != nullptr) {
				return parent->lookup(var_);
			}
			std::cout << "not find 404" << var_->toString();
			exit(1);
		}
		return env[var];
	}
	void define(shared_ptr<SchemeValue> var_, shared_ptr<SchemeValue> val) {
		auto var = var_->toString();
		env[var] = val;
	}
	void define(const string& var, shared_ptr<SchemeValue> val) {
		//auto var = var_->toString();
		env[var] = val;
	}
};
EnvPtr initEnv();
using EnvPtr = shared_ptr<Env>;
//是否要看见实现才能使用 shared_ptr