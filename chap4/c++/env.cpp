#include <numeric>
#include <algorithm>
#include "env.h"
#include "scheme.h"
using namespace std;
EnvPtr initEnv() {
	auto init = make_shared<Env>();
	auto listProc = [=](function<int(int, int)> f) ->shared_ptr<SchemeValue> {
		return make_shared<Procedure>([=](vector<shared_ptr<SchemeValue>> args) -> shared_ptr<SchemeValue> {
			int res = args[0]->intValue();
			for_each(args.begin() + 1, args.end(), [=,&res](shared_ptr<SchemeValue> a) {
				res = f(res, a->intValue());
			});
			return make_shared<IntValue>(res);
		});
	};
	auto plus = listProc([](int a, int b) {return a + b; });
	auto sub = listProc([](int a, int b) {return a - b; });
	auto mul = listProc([](int a, int b) {return a * b; });
	auto div = listProc([](int a, int b) {return a / b; });
		/*make_shared<Procedure>([](vector<shared_ptr<SchemeValue>> args) -> shared_ptr<SchemeValue> {
		int res = 0;
		for (auto i : args) {
			res += i->intValue();
		}
		return make_shared<IntValue>(res);
	});*/

	//auto sub = make_shared<Procedure>([](vector<shared_ptr<SchemeValue>> args) -> shared_ptr<SchemeValue> {
	//	int res = args[0]->intValue();
	//	for (auto iter = args.begin()+1; iter!=args.end(); iter++){
	//	//for (auto i : args) {
	//		res -= (*iter)->intValue();
	//	}
	//	return make_shared<IntValue>(res);
	//});

	auto little = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args) -> shared_ptr<SchemeValue> {
			return make_shared<IntValue>(args[0]->intValue() < args[1]->intValue());
		}
	);
	auto great = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args) -> shared_ptr<SchemeValue> {
		return make_shared<IntValue>(args[0]->intValue() > args[1]->intValue());
	}
	);
	init->define(make_shared<SymbolValue>("+"), plus);
	init->define(make_shared<SymbolValue>("-"), sub);
	init->define(make_shared<SymbolValue>("*"), mul);
	init->define(make_shared<SymbolValue>("<"), little);
	init->define(make_shared<SymbolValue>(">"), great);
	return init;
}