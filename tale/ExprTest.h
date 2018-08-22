#pragma once
#include<gtest/gtest.h>
#include<gmock/gmock.h>
#include"NumberExpr.h"
#include"StringExpr.h"
#include"ClosureExpr.h"
#include"VariableExpr.h"
#include"AssignExpr.h"
#include"AddExpr.h"
using namespace testing;
struct ExprTest : testing::Test {
	std::shared_ptr<NumberExpr> number_15;
	std::shared_ptr<StringExpr> string_hello_world;
	std::shared_ptr<ClosureExpr> closure;
	std::shared_ptr<VariableExpr> variable;
	void SetUp() override {
		number_15 = NumberExpr::createNumberExpr(nullptr, 15);
		string_hello_world = StringExpr::createStringExpr(nullptr, L"hello world");
		closure = ClosureExpr::createClosureExpr(nullptr);
		variable = VariableExpr::createVariable(nullptr, L"test");
	}
	
	void TearDown() override {
		number_15.reset();
		string_hello_world.reset();
		closure.reset();
	}
};

TEST_F(ExprTest, BasicNumberExpr) {
	std::wstring str = number_15->toString();
	EXPECT_THAT(str, StartsWith(L"TYPE_NUMBER@"));
	EXPECT_THAT(str, EndsWith(L"15"));
	EXPECT_NE(str, number_15->clone()->toString());
}

TEST_F(ExprTest, BasicStringTest) {
	std::wstring str = string_hello_world->toString();
	EXPECT_THAT(str, StartsWith(L"TYPE_STRING@"));
	EXPECT_THAT(str, EndsWith(L"hello world"));
	EXPECT_NE(str, string_hello_world->clone()->toString());
}



TEST_F(ExprTest, BasicClosureTest) {
	closure->addExpression(number_15);
	closure->addExpression(string_hello_world);
	std::shared_ptr<ClosureExpr> x = std::dynamic_pointer_cast<ClosureExpr>(closure->clone());
	std::wstring str = closure->toString();
	EXPECT_NE(x->getIthSentence(0)->toString(), closure->getIthSentence(0)->toString());
	EXPECT_NE(x->getIthSentence(1)->toString(), closure->getIthSentence(1)->toString());
	auto temp = ClosureExpr::createClosureExpr(nullptr);
	temp->addExpression(closure);
}

TEST_F(ExprTest, BasicVariableTest) {
	auto number_15new = number_15->clone();
	auto closure_new = closure->clone();
	auto variable_new = variable->clone();
	variable_new->setRunTime(closure_new);
	std::dynamic_pointer_cast<ClosureExpr>(closure_new)->getContext()[L"test"] = number_15new;
	EXPECT_THAT(variable_new->getValue()->toString(), EndsWith(L"15"));
}

TEST_F(ExprTest, BasicAssignTest) {
	std::shared_ptr<AssignExpr> assign = AssignExpr::createAssignExpr(nullptr, variable, number_15);
	EXPECT_NE(variable, assign->getLeft());
	EXPECT_NE(number_15, assign->getRight());
	auto closure_new = std::dynamic_pointer_cast<ClosureExpr>(closure->clone());
	assign->setRunTime(closure_new);
	closure_new->addExpression(assign);
	closure_new->addExpression(variable);
	EXPECT_EQ(closure_new->getValue()->getType(), Expr::ExprType::TYPE_NUMBER);
}

TEST_F(ExprTest, BasicAddTest) {

	std::shared_ptr<AddExpr> addition = AddExpr::createAddExpr(nullptr, number_15, string_hello_world);
	auto result = addition->getValue();
	EXPECT_EQ(result->getType(), Expr::ExprType::TYPE_STRING);
	auto r = std::dynamic_pointer_cast<StringExpr>(result);
	EXPECT_EQ(r->getString(), L"15hello world");
}
TEST_F(ExprTest, ClosureTest) {
	std::shared_ptr<AssignExpr> assign = AssignExpr::createAssignExpr(nullptr, variable, number_15);
	auto closure_new = std::dynamic_pointer_cast<ClosureExpr>(closure->clone());
	closure_new->addExpression(assign);
	auto closure_inside = std::dynamic_pointer_cast<ClosureExpr>(closure->clone());
	std::shared_ptr<AssignExpr> add1 = AssignExpr::createAssignExpr(nullptr, variable, 
		AddExpr::createAddExpr(nullptr, variable, NumberExpr::createNumberExpr(nullptr, 1)));
	closure_inside->addExpression(add1);
	closure_new->addExpression(closure_inside);
	auto ret = closure_new->getValue();
	EXPECT_EQ(ret->getType(), Expr::ExprType::TYPE_NUMBER);
	EXPECT_EQ(std::dynamic_pointer_cast<NumberExpr>(ret)->getNumber(),16);
}