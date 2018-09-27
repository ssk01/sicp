#include "scheme.h"
using namespace std;
shared_ptr<SchemeValue> Void() {
	static shared_ptr<SchemeValue> void_ = make_shared<VoidValue>();
	return void_;
}
ostream& operator<<(ostream& out, const SchemeValue& a) {
	a.print(out);
	return out;
}
vector<string> listToString(const vecotr<shared_ptr<SchemeValue>> &lists) {
	vector<string> result;
	for (auto &v : lists){
		result.emplace_back(v->toString());
	}
	return result;
}