#pragma once
#include <memory>
#include <map>
#include "scheme.h"
#include <iostream>
using std::shared_ptr;
using std::map;
class Env;
using EnvPtr = shared_ptr<Env>;
EnvPtr initEnv();
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
			std::cout << "lookup not find 404:" << var_->toString()<<" fck";
			exit(1);
		}
		return env[var];
	}
	void define(shared_ptr<SchemeValue> var_, shared_ptr<SchemeValue> val) {
		auto var = var_->toString();
		env[var] = val;
	}
	void assign(shared_ptr<SchemeValue> var_, shared_ptr<SchemeValue> val) {
		auto var = var_->toString();
		if (env.find(var) == env.end()) {
			if (parent.get() != nullptr) {
				 parent->assign(var_, val);
			}
			std::cout << "assign not find 404:" << var_->toString() << " fck";
			exit(1);
		}
		env[var] = val;
		//return env[var];
	}
	void define(const string& var, shared_ptr<SchemeValue> val) {
		//auto var = var_->toString();
		env[var] = val;
	}
};

//是否要看见实现才能使用 shared_ptr