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
	auto _cons = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {
		if (args.size() != 2) {
			fck("cons arg lens false");
		}
		//auto list = make_shared<ListValue>();
		//list->addValue(args[0]);
		////(cons 1 (list 2))  :ok
		////(cons 1 2): false todo
		//for (auto v : args[1]->cdr[0]) {
		//	list->addValue(v);
		//}
		return cons(args[0], args[1]);
	});
	auto _append = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {
		if (args.size() != 2) {
			fck("append arg lens false");
		}
		if (args[0]->isPairList() && args[1]->isPairList()) {
			append(args[0], args[1]);
		}
		fck("append type error");
	});
	//auto null = Vo

	auto list = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {
		//args
		if (args.size()  == 0) {
			fck("list arg lens false");
		}
		return makeList(args);
		//return car(args[0]);
	});
	auto _car = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {
			//args
		if (args.size() != 1) {
			fck("cars arg lens false");
		}
		return car(args[0]);
	});
	auto _cdr = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {
		if (args.size() != 1) {
			fck("cons arg lens false");
		}
		return cdr(args[0]);
	});
	auto _cadr = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {

	});
	auto _caddr = make_shared<Procedure>(
		[](vector<shared_ptr<SchemeValue>> args)->shared_ptr<SchemeValue> {

	});
	init->define("car", _car);
	init->define("cdr", _cdr);
	init->define("cadr", _cadr);
	init->define("caddr", _caddr);
	init->define("append", _append);
	init->define("list", list);
	init->define("null", Void());
	init->define("nil", Void());
	return init;
}