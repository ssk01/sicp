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