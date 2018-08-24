#pragma once
#include<gtest/gtest.h>
#include<gmock/gmock.h>
#include"NumberExpr.h"
#include"StringExpr.h"
#include"ClosureExpr.h"
#include"VariableExpr.h"
#include"AssignExpr.h"
#include"AddExpr.h"
#include"ReturnExpr.h"
#include"FunctionExpr.h"
#include"GreaterThanExpr.h"
#include"ConditionExpr.h"
#include"EqualExpr.h"
#include"NullExpr.h"
#include"CallExpr.h"

using namespace testing;

struct ExprTest : testing::Test {
	std::shared_ptr<NumberExpr> number_15;
	std::shared_ptr<StringExpr> string_hello_world;
	std::shared_ptr<VariableExpr> variable;
	void SetUp() override {
		number_15 = NumberExpr::createNumberExpr(nullptr, 15);
		string_hello_world = StringExpr::createStringExpr(nullptr, L"hello world");
		variable = VariableExpr::createVariableExpr(nullptr, L"test");
	}

	void TearDown() override {
		number_15.reset();
		string_hello_world.reset();
	}
};


//TEST_F(ExprTest, BasicNumberExpr) {
//	std::wstring str = number_15->toString();
//	EXPECT_THAT(str, StartsWith(L"TYPE_NUMBER@"));
//	EXPECT_THAT(str, EndsWith(L"15"));
//}
//
//TEST_F(ExprTest, BasicStringTest) {
//	std::wstring str = string_hello_world->toString();
//	EXPECT_THAT(str, StartsWith(L"TYPE_STRING@"));
//	EXPECT_THAT(str, EndsWith(L"hello world"));
//}
//
//
//
//
//TEST_F(ExprTest, BasicVariableTest) {
//	auto number_15new = number_15;
//	auto closure = ClosureExpr::createClosureExpr();
//	auto variable_new = variable;
//	variable_new->setRunTime(closure);
//	closure->getContext()[L"test"] = number_15new;
//	EXPECT_THAT(variable_new->getValue()->toString(), EndsWith(L"15"));
//}
//
//TEST_F(ExprTest, BasicAssignTest) {
//	std::shared_ptr<AssignExpr> assign = AssignExpr::createAssignExpr(nullptr, variable, number_15);
//	auto closure_new = ClosureExpr::createClosureExpr();
//	assign->setRunTime(closure_new);
//	closure_new->addExpression(assign);
//	closure_new->addExpression(variable);
//	EXPECT_EQ(closure_new->getValue()->getType(), Expr::ExprType::TYPE_NUMBER);
//	EXPECT_EQ(15,std::dynamic_pointer_cast<NumberExpr>(closure_new->getValue())->getNumber());
//}
//
//TEST_F(ExprTest, BasicAddTest) {
//
//	std::shared_ptr<AddExpr> addition = AddExpr::createAddExpr(nullptr, number_15, string_hello_world);
//	auto result = addition->getValue();
//	EXPECT_EQ(result->getType(), Expr::ExprType::TYPE_STRING);
//	auto r = std::dynamic_pointer_cast<StringExpr>(result);
//	EXPECT_EQ(r->getString(), L"15hello world");
//}
//TEST_F(ExprTest, ClosureTest) {
//	std::shared_ptr<AssignExpr> assign = AssignExpr::createAssignExpr(nullptr, variable, number_15);
//	auto closure_new = ClosureExpr::createClosureExpr();
//	closure_new->addExpression(assign);
//	auto closure_inside = ClosureExpr::createClosureExpr();
//	std::shared_ptr<AssignExpr> add1 = AssignExpr::createAssignExpr(nullptr, variable,
//		AddExpr::createAddExpr(nullptr, variable, NumberExpr::createNumberExpr(nullptr, 1)));
//	closure_inside->addExpression(add1);
//	closure_new->addExpression(closure_inside);
//	auto ret = closure_new->getValue();
//	EXPECT_EQ(ret->getType(), Expr::ExprType::TYPE_NUMBER);
//	EXPECT_EQ(std::dynamic_pointer_cast<NumberExpr>(ret)->getNumber(), 16);
//}
//
//
//TEST_F(ExprTest, ReturnTest) {
//	std::shared_ptr<AssignExpr> assign = AssignExpr::createAssignExpr(nullptr, variable, number_15);
//	auto closure_new = ClosureExpr::createClosureExpr();
//	closure_new->addExpression(assign);
//	auto retExpr = ReturnExpr::createReturnExpr(nullptr, variable);
//	closure_new->addExpression(retExpr);
//	auto closure_inside = ClosureExpr::createClosureExpr();
//	std::shared_ptr<AssignExpr> add1 = AssignExpr::createAssignExpr(nullptr, variable,
//		AddExpr::createAddExpr(nullptr, variable, NumberExpr::createNumberExpr(nullptr, 1)));
//	closure_inside->addExpression(add1);
//	closure_new->addExpression(closure_inside);
//	auto ret = closure_new->getValue();
//	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, ret->getType());
//	EXPECT_EQ(std::dynamic_pointer_cast<NumberExpr>(ret)->getNumber(), 15);
//}
//
//
///*
//f(test){
//test = test +1
//}
//expected:
//f(1)==2
//f(2)==3
//*/
//TEST_F(ExprTest, BasicFunctionTest) {
//	std::shared_ptr<FunctionExpr> function = FunctionExpr::createFunctionExpr(nullptr);
//	std::shared_ptr<AssignExpr> add1 = AssignExpr::createAssignExpr(nullptr, variable,
//		AddExpr::createAddExpr(nullptr, variable, NumberExpr::createNumberExpr(nullptr, 1)));
//	auto closure_new = ClosureExpr::createClosureExpr();
//	closure_new->addExpression(add1);
//	function->setClosure(closure_new);
//	function->setSignature({ L"test" });
//	auto ret = function->getValue({ NumberExpr::createNumberExpr(nullptr,1) });
//	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, ret->getType());
//	EXPECT_EQ(2, std::dynamic_pointer_cast<NumberExpr>(ret)->getNumber());
//
//	auto ret_new = function->getValue({ NumberExpr::createNumberExpr(nullptr,2) });
//	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, ret_new->getType());
//	EXPECT_EQ(3, std::dynamic_pointer_cast<NumberExpr>(ret_new)->getNumber());
//}
////
////
//
//
//
//
//
//
///**
//	function(){
//		a = 0;
//		function(){
//			a=a+1;
//		}()
//		a}
//*/
//TEST_F(ExprTest, InnerFunctionTest) {
//	std::shared_ptr<FunctionExpr> function = FunctionExpr::createFunctionExpr(nullptr);
//	auto variable_a = VariableExpr::createVariableExpr(nullptr, L"a");
//	std::shared_ptr<AssignExpr> assign = AssignExpr::createAssignExpr(nullptr, variable_a, NumberExpr::createNumberExpr(nullptr, 0));
//	auto closure_new = ClosureExpr::createClosureExpr();
//	std::shared_ptr<FunctionExpr> functionInside = FunctionExpr::createFunctionExpr(nullptr);
//	auto closureInside = ClosureExpr::createClosureExpr();
//	std::shared_ptr<AssignExpr> add1 = AssignExpr::createAssignExpr(nullptr, variable_a,
//		AddExpr::createAddExpr(nullptr, variable_a, NumberExpr::createNumberExpr(nullptr, 1)));
//	closureInside->addExpression(add1);
//	functionInside->setClosure(closureInside);
//	closure_new->addExpression(assign);
//	closure_new->addExpression(functionInside);
//	closure_new->addExpression(variable_a);
//	function->setClosure(closure_new);
//	auto ret = function->getValue();
//	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, ret->getType());
//	EXPECT_EQ(1, std::dynamic_pointer_cast<NumberExpr>(ret)->getNumber());
//}
//
//
//
///**
//funtion f(){
//a=0;
//function f1(){
//a=a+1;
//}();
//return f1;
//}
//
//expected:
//
//result = f();
//result() ==1
//result() ==2
//...
//
//*/
////
//TEST_F(ExprTest, littleAdvanceClosureTest) {
//	std::shared_ptr<FunctionExpr> function = FunctionExpr::createFunctionExpr(nullptr);
//	auto variable_a = VariableExpr::createVariableExpr(nullptr, L"a");
//	std::shared_ptr<AssignExpr> assign = AssignExpr::createAssignExpr(nullptr, variable_a, NumberExpr::createNumberExpr(nullptr, 0));
//
//	std::shared_ptr<AssignExpr> add1 = AssignExpr::createAssignExpr(nullptr, variable_a,
//		AddExpr::createAddExpr(nullptr, variable_a, NumberExpr::createNumberExpr(nullptr, 1)));
//	auto closure_new = ClosureExpr::createClosureExpr();
//	closure_new->addExpression(add1);
//	std::shared_ptr<FunctionExpr> functionInside = FunctionExpr::createFunctionExpr(nullptr);
//	functionInside->setClosure(closure_new);
//
//	auto closure_outter = ClosureExpr::createClosureExpr(nullptr);
//	closure_outter->addExpression(assign);
//	closure_outter->addExpression(functionInside);
//	closure_outter->addExpression(ReturnExpr::createReturnExpr(nullptr, functionInside));
//	function->setClosure(closure_outter);
//	auto ret = function->getValue();
//	EXPECT_EQ(Expr::ExprType::TYPE_FUNCTION, ret->getType());
//	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, ret->getValue()->getType());
//	EXPECT_EQ(3, std::dynamic_pointer_cast<NumberExpr>(ret->getValue())->getNumber());
//	EXPECT_EQ(4, std::dynamic_pointer_cast<NumberExpr>(ret->getValue())->getNumber());
//	EXPECT_EQ(5, std::dynamic_pointer_cast<NumberExpr>(ret->getValue())->getNumber());
//}
////From 3, because functionInside has been exectued.
//
//
///*
//
//if(a>b){ return a}
//else {return b}
//
//*/
//
//TEST_F(ExprTest, BasicConditionTest) {
//	auto a = NumberExpr::createNumberExpr(1);
//	auto b = NumberExpr::createNumberExpr(2);
//	auto condition = GreaterThanExpr::createGreaterThanExpr(a, b);
//	auto ifstatement = ConditionExpr::createConditionExpr(condition, ReturnExpr::createReturnExpr(a),
//		ReturnExpr::createReturnExpr(b));
//	auto ret = ifstatement->getValue();
//	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, ret->getType());
//	EXPECT_EQ(2, std::dynamic_pointer_cast<NumberExpr>(ret)->getNumber());
//}
//
//
///*
//if(a==b){
//return "hello world"
//}
//else{
//return "not equal"}
//*/
//TEST_F(ExprTest, BasicConditionTest1) {
//	auto a = NumberExpr::createNumberExpr(1);
//	auto b = NumberExpr::createNumberExpr(2);
//	auto condition = EqualExpr::createEqualExpr(a, b);
//	auto ifstatement = ConditionExpr::createConditionExpr(condition, ReturnExpr::createReturnExpr(StringExpr::createStringExpr(L"hello world")),
//		ReturnExpr::createReturnExpr(StringExpr::createStringExpr(L"not equal")));
//	auto ret = ifstatement->getValue();
//	EXPECT_EQ(Expr::ExprType::TYPE_STRING, ret->getType());
//	EXPECT_EQ(L"not equal", std::dynamic_pointer_cast<StringExpr>(ret)->getString());
//}
//
//
///*
//f(x){
//if(x==10){
//return 10}
//}
//*/
//TEST_F(ExprTest, BasicConditionFunctionTest) {
//	auto function = FunctionExpr::createFunctionExpr();
//	auto closure_new = ClosureExpr::createClosureExpr();
//	function->setSignature({ L"x" });
//	auto variable = VariableExpr::createVariableExpr(L"x");
//	auto con = ConditionExpr::createConditionExpr(EqualExpr::createEqualExpr(variable,
//		NumberExpr::createNumberExpr(10)), 
//		ReturnExpr::createReturnExpr(NumberExpr::createNumberExpr(10)), ReturnExpr::createReturnExpr(NullExpr::createNullExpr()));
//	closure_new->addExpression(con);
//	function->setClosure(closure_new);
//	auto ret = function->getValue({ NumberExpr::createNumberExpr(10) });
//	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, ret->getType());
//	EXPECT_EQ(10, std::dynamic_pointer_cast<NumberExpr>(ret)->getNumber());
//}
//
///*
//f(x){
//if(x==10) return 10
//else return x+1
//}
//f1(x){
//f(x)
//}
//*/
//TEST_F(ExprTest, FunctionTest1) {
//	auto function = FunctionExpr::createFunctionExpr();
//	auto closure_new = ClosureExpr::createClosureExpr();
//	function->setSignature({ L"x" });
//	auto variable = VariableExpr::createVariableExpr(L"x");
//	auto con = ConditionExpr::createConditionExpr(EqualExpr::createEqualExpr(variable,
//		NumberExpr::createNumberExpr(10)),
//		ReturnExpr::createReturnExpr(NumberExpr::createNumberExpr(10)), 
//		ReturnExpr::createReturnExpr(
//			AddExpr::createAddExpr(NumberExpr::createNumberExpr(1), variable)
//		));
//	closure_new->addExpression(con);
//	function->setClosure(closure_new);
//	auto function1 = FunctionExpr::createFunctionExpr();
//	auto closure1 = ClosureExpr::createClosureExpr();
//	closure1->addExpression(CallExpr::createCallExpr(function, { NumberExpr::createNumberExpr(1) }));
//	function1->setClosure(closure1);
//	function1->setSignature({ L"x" });
//
//	auto ret = function1->getValue({ NumberExpr::createNumberExpr(9) });
//	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, ret->getType());
//	EXPECT_EQ(2, std::dynamic_pointer_cast<NumberExpr>(ret)->getNumber());
//}

/*
f(x){
if(x==10){return 10;}
return x+f(x+1);}

expected:
f(10) == 10
f(9) == 19
f(0) == 55
*/
TEST_F(ExprTest, RecurisionTest) {
	auto function = FunctionExpr::createFunctionExpr();
	auto closure_new = ClosureExpr::createClosureExpr();
	function->setSignature({ L"x" });
	auto variable = VariableExpr::createVariableExpr(L"x");
	auto call = CallExpr::createCallExpr(function, { AddExpr::createAddExpr(variable,NumberExpr::createNumberExpr(1)) });
	auto con = ConditionExpr::createConditionExpr(EqualExpr::createEqualExpr(variable,
		NumberExpr::createNumberExpr(10)),
		ReturnExpr::createReturnExpr(NumberExpr::createNumberExpr(10)), 
		ReturnExpr::createReturnExpr(
			AddExpr::createAddExpr(call, variable)
		));
	closure_new->addExpression(con);
	function->setClosure(closure_new);
	EXPECT_EQ(call, function->getRunTime());
	EXPECT_EQ(function, closure_new->getRunTime());
	auto ret = function->getValue({ NumberExpr::createNumberExpr(9) });
	EXPECT_EQ(Expr::ExprType::TYPE_NUMBER, ret->getType());
	EXPECT_EQ(19, std::dynamic_pointer_cast<NumberExpr>(ret)->getNumber());
	
}




/*function->getValue({ AddExpr::createAddExpr(variable,NumberExpr::createNumberExpr(1)) })*/



