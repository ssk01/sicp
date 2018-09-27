#include "env.h"
#include "scheme.h"
using namespace std;
EnvPtr initEnv() {
	auto init = make_shared<Env>();
	auto plus = make_shared<Procedure>([](vector<shared_ptr<SchemeValue>> argus) -> shared_ptr<SchemeValue> {
		int res = 0;
		for (auto i : argus) {
			if (i->isInt()) {
				res += i->intValue();
			}
			else {
				cout << "fuck";
				exit(1);
			}
		}
		return make_shared<IntValue>(res);
	}, true);
	init->define(make_shared<SymbolValue>("+"), plus);
	return init;
}