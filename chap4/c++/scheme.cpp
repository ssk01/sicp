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