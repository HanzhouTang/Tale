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
#include"BooleanExpr.h"
#include"ReverseExpr.h"
#include"ExprLiteral.h"
#include"MapExpr.h"
#include"ExternalFunctionExpr.h"
#include"ExprLibrary.h"
#include"AndExpr.h"
#include"NotExpr.h"
#include"OrExpr.h"

struct ExprTest : testing::Test {
	std::shared_ptr<expr::NumberExpr> number_15;
	std::shared_ptr<expr::StringExpr> string_hello_world;
	std::shared_ptr<expr::VariableExpr> variable;
	void SetUp() override {
		number_15 = expr::NumberExpr::createNumberExpr(nullptr, 15);
		string_hello_world = expr::StringExpr::createStringExpr(nullptr, L"hello world");
		variable = expr::VariableExpr::createVariableExpr(nullptr, L"test");
	}

	void TearDown() override {
		number_15.reset();
		string_hello_world.reset();
	}
};
