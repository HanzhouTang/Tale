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
		return createCallExpr(getRunTime(), std::dynamic_pointer_cast<FunctionExpr>(getFunction()->clone()), p);
	}

	std::shared_ptr<Expr> CallExpr::getValue() {
		auto param = getParameters();
		if (param.size() == 0 && getFunction()->getSignature().size() == 0) {
			return getFunction()->getValue();
		}
		else if (param.size() == getFunction()->getSignature().size()) {

			auto run = getFunction()->getRunTime();
			getFunction()->setRunTime(shared_from_this());
			std::vector<std::shared_ptr<Expr>> args;
			for (int i = 0; i < parameters.size(); i++) {
				auto x = parameters[i]->getValue();
				args.emplace_back(x);
			}
			//a little strange here
			/*why ?
			for (int i = 0; i < parameters.size(); i++) {
				//std::wcout << "param[" << i << "] " << param[i]->toString() << std::endl;
				auto x = parameters[i]->getValue();
				parameters[i] = x;
			}
			doesn't work?
			*/


			store(shared_from_this());
			auto ret = getFunction()->getValue(args);
			restore(shared_from_this());
			getFunction()->setRunTime(run);
			return ret;
		}
		else {
			quitWithError(__LINE__, __FILE__, L"call doesn't match function's signature");
			return NullExpr::createNullExpr();
		}
	}

	std::wstring CallExpr::toString()
	{
		std::wostringstream buf;
		buf << L"call( ";
		for (auto& x : parameters) {
			buf << x->toString() << L" , ";
		}
		buf << L")\n{";
		buf << getFunction()->toString() << L"}\n";
		return buf.str();
	}

	std::shared_ptr<CallExpr> CallExpr::createCallExpr(const std::shared_ptr<Expr>& runtime, const std::shared_ptr<FunctionExpr>& f,
		const std::vector<std::shared_ptr<Expr>>& param)
	{
		auto ret = std::make_shared<CallExpr>(runtime, f, param);
		for (auto& x : ret->getParameters()) {
			x->setRunTime(ret);
		}
		ret->getFunction()->setRunTime(ret);
		return ret;
	}
}