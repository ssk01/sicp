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
	auto eq = listProc([](int a, int b) {return a == b; });
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
	init->define(make_shared<SymbolValue>("="), eq);
	auto cons = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {
		if (args.size() != 2) {
			fck("cons arg lens false");
		}
		auto list = make_shared<ListValue>();
		list->addValue(args[0]);
		//(cons 1 (list 2))  :ok
		//(cons 1 2): false todo
		for (auto v : args[1]->cdr[0]) {
			list->addValue(v);
		}
		return list;
	});
	auto append = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {
		if (args.size() != 2) {
			fck("cons arg lens false");
		}
		
		auto list = make_shared<ListValue>();
		for (auto v : args[0]->cdr[0]) {
			list->addValue(v);
		}
		for (auto v : args[1]->cdr[0]) {
			list->addValue(v);
		}
		return list;
	});
	auto car = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {
			//args
		return args[0]->
	});
	auto cdr = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {

	});
	auto cadr = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {

	});
	auto caddr = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {

	});
	return init;
}