#pragma once
#include<gtest/gtest.h>
#include<gmock/gmock.h>
#include"ExternalFunctionExpr.h"
#include"ExprLibrary.h"
struct SimpleParserTest : testing::Test {
	std::shared_ptr<expr::ExternalFunctionExpr> print;
	std::shared_ptr<expr::ExternalFunctionExpr> set;
	std::shared_ptr<expr::ExternalFunctionExpr> get;
	void SetUp() override {
		print = expr::ExternalFunctionExpr::createExternalFunctionExpr(expr::printExpr);
		set = expr::ExternalFunctionExpr::createExternalFunctionExpr(expr::set);
		get = expr::ExternalFunctionExpr::createExternalFunctionExpr(expr::get);
	}
};
