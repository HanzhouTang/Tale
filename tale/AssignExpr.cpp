#include"AssignExpr.h"
#include"SequenceExpr.h"
 std::shared_ptr<Expr> AssignExpr::getValue(){
	 auto run = getRunTime();
	 auto l = left;
	 auto r = right->getValue();
	 if (run->getType() == sequence) {
		 std::shared_ptr<SequenceExpr> runtime = std::dynamic_pointer_cast<SequenceExpr>(run);
		 if(l->getType()==variable){
			 std::shared_ptr<VariableExpr> variable = std::dynamic_pointer_cast<VariableExpr>(l);
			 runtime->setVariable(variable, r->getValue());
			 return runtime->getVariable(variable);
		 }
		 else {
			 Utility::quitWithError(__LINE__, __FILE__, L"only variable can be assigned");
			 return std::make_shared<Expr>(shared_from_this());
		 }
	 }
	 else {
		 Utility::quitWithError(__LINE__, __FILE__, L"variable can only be assigned inside a sequence");
		 return std::make_shared<Expr>(shared_from_this());
	 }
}