#pragma once
#include<gtest/gtest.h>
#include<gmock/gmock.h>
#include"NumberExpr.h"
#include"StringExpr.h"
#include"ClosureExpr.h"
#include"VariableExpr.h"
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

//assign need test
