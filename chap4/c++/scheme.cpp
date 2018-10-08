#include "scheme.h"
#include "env.h"
#include <assert.h>
using namespace std;
shared_ptr<SchemeValue> Void() {
	static shared_ptr<SchemeValue> void_ = make_shared<VoidValue>();
	return void_;
}
ostream& operator<<(ostream& out, const SchemeValue& a) {
	a.print(out);
	return out;
}
vector<string> listToString(const vector<shared_ptr<SchemeValue>> &lists) {
	vector<string> result;
	for (auto &v : lists) {
		result.emplace_back(v->toString());
	}
	return result;
}
EnvPtr LambdaValue::newEnv(vector<shared_ptr<SchemeValue>> argsValue)  {
	auto env = make_shared<Env>(this->parent);
	assert(args.size() == argsValue.size());
	for (int i = 0; i < args.size(); i++) {
		env->define(args[i], argsValue[i]);
	}
	return env;
}
shared_ptr<SchemeValue> makeIf(shared_ptr<SchemeValue> cond, shared_ptr<SchemeValue> falseBranch, shared_ptr<SchemeValue> trueBranch) {
	auto if_ = make_shared<ListValue>();
	if_->addValue(make_shared<SymbolValue>("if"));
	if_->addValue(cond);
	if_->addValue(falseBranch);
	if_->addValue(trueBranch);
	return if_;

}

shared_ptr<SchemeValue> _expandClauses(vector<shared_ptr<SchemeValue>> exp, int i) {
	if (i < exp.size()) {
		auto clause = exp[i];
		if (clause->isTagged("else")) {
			if (i != exp.size()-1) {
				fck("after else should be empty");
			}
			return clause->causeExp();
		}
		else {
			return makeIf(clause->causeCond(), clause->causeExp(), _expandClauses(exp, i + 1));
		}
	}
	return Void();
}

shared_ptr<SchemeValue> expandClauses(vector<shared_ptr<SchemeValue>> exp) {
	return _expandClauses(exp, 0);
}
