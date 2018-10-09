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
vector<string> listToString( vector<shared_ptr<SchemeValue>> &lists) {
	vector<string> result;
	for (auto &v : lists) {
		result.emplace_back(v->toString());
	}
	return result;
}
SchemeValuePtr cons(SchemeValuePtr first, SchemeValuePtr second) {
	return make_shared<PairValue>(first, second);
}
SchemeValuePtr car(SchemeValuePtr exp) {
	return exp->pairFirst();
}
SchemeValuePtr cdr(SchemeValuePtr exp) {
	return exp->pairSecond();
}
SchemeValuePtr makeList(vector<SchemeValuePtr> exp) {
	auto prev = Void();
	for (auto beg = exp.rbegin(); beg != exp.rend(); beg++) {
		prev = cons(*beg, prev);
	}
	return prev;
}
SchemeValuePtr append(SchemeValuePtr first, SchemeValuePtr second) {
	if (first->isVoid()) {
		return second;
	}
	return cons(car(first), append(cdr(first), second));
}