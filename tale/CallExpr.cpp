#include"CallExpr.h"
#include"FunctionExpr.h"
#include"NullExpr.h"
#include"Utility.h"
namespace expr {
	using namespace Utility;
	std::shared_ptr<Expr> CallExpr::clone() {
		std::vector<std::shared_ptr<Expr>> p;
		p.resize(parameters.size());
		std::transform(parameters.begin(), parameters.end(), p.begin(), [](const std::shared_ptr<Expr>& x) {return x->clone(); });
		return createCallExpr(getRunTime(), getCallable(), p);
	}

	std::shared_ptr<Expr> CallExpr::getValue() {
		auto param = getParameters();
		auto callObject = getCallable();
		if (param.size() == 0) {
			return callObject->getValue();
		}
		else {
			//auto run = callObject->getRunTime();
			//callObject->setRunTime(shared_from_this()); //? why 
			std::vector<std::shared_ptr<Expr>> args;
			for (int i = 0; i < parameters.size(); i++) {
				auto x = parameters[i]->getValue();
				args.emplace_back(x);
			}
			store(shared_from_this());
			auto ret = callObject->getValue(args);
			restore(shared_from_this());
			//callObject->setRunTime(run);
			return ret;
		}
	}

	std::wstring CallExpr::toString()
	{
		std::wostringstream buf;
		// when varibale doesn't exist, it also work 
		buf << callable->toString();
		buf << L" call( ";
		for (auto& x : parameters) {
			buf << x->toString() << L" , ";
		}
		buf << L")";
		return buf.str();
	}

	/*
	Here contain an infinite circle when doing recursion.
	The question is when directly do recursion, why no matter?

	*/
	std::shared_ptr<CallExpr> CallExpr::createCallExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<Expr>& f,
		const std::vector<std::shared_ptr<Expr>>& param)
	{
		using namespace std;
		auto ret = std::make_shared<CallExpr>(runtime, f, param);
		for (auto& x : ret->getParameters()) {
			x->setRunTime(ret);
		}
		ret->callable->setRunTime(ret);
		return ret;
	}
}