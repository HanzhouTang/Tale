#pragma once
#include<gtest/gtest.h>
#include"SequenceExpr.h"
#include"VariableExpr.h"
#include"NumberExpr.h"
#include"AssignExpr.h"
#include"AddExpr.h"
struct EXPRTEST : testing::Test {
	std::shared_ptr<SequenceExpr> sequence;
	std::shared_ptr<VariableExpr> variable;
	std::shared_ptr<NumberExpr> number_19;
	std::shared_ptr<NumberExpr> number_39;
	std::shared_ptr<AssignExpr> assign;
	std::shared_ptr<AddExpr> add;
	std::shared_ptr<AddExpr> addVariable;
	void SetUp() override{
		sequence = make_shared<SequenceExpr>(nullptr);
		variable = std::make_shared<VariableExpr>(sequence, L"a");
		number_19 = std::make_shared<NumberExpr>(sequence, 19);
		number_39 = std::make_shared<NumberExpr>(sequence, 39);
		add = std::make_shared<AddExpr>(sequence, number_19, number_39);
		assign = std::make_shared<AssignExpr>(sequence, variable, add);
		addVariable = std::make_shared<AddExpr>(sequence, variable, number_19);
		sequence->addSentence(assign);
		sequence->addSentence(addVariable);
	}
	void TearDown() override{
		sequence.reset();
		variable.reset();
		number_19.reset();
		assign.reset();
		number_39.reset();
		add.reset();
	}
};

TEST_F(EXPRTEST, RunTimeIsDefined) {
	ASSERT_EQ(sequence, variable->getRunTime())<<L"runtime of variable a is sequence";
	ASSERT_EQ(sequence, assign->getRunTime());
	ASSERT_EQ(sequence->getType(), Expr::ExprType::sequence);
}


TEST_F(EXPRTEST, SetVariable) {

	ASSERT_EQ(58, std::dynamic_pointer_cast<NumberExpr>(assign->getValue())->getNumber());
	ASSERT_EQ(std::dynamic_pointer_cast<NumberExpr>(sequence->getVariable(variable))->getNumber(), 58);
}


TEST_F(EXPRTEST, AddTwoNumber) {
	std::shared_ptr<NumberExpr> number_58 = std::dynamic_pointer_cast<NumberExpr>(add->getValue());
	ASSERT_EQ(number_58->getNumber(), 58);
}

TEST_F(EXPRTEST, ToString) {
	ASSERT_EQ(sequence->toString(), L"sequence");
}

TEST_F(EXPRTEST, AddVariable) {
	assign->getValue();
	std::shared_ptr<NumberExpr> number = std::dynamic_pointer_cast<NumberExpr>(addVariable->getValue());
	ASSERT_EQ(number->getNumber(), 77);
}


TEST_F(EXPRTEST, Sequence) {
	std::shared_ptr<NumberExpr> number = std::dynamic_pointer_cast<NumberExpr>(sequence->getValue());
	ASSERT_EQ(number->getNumber(), 77);
}
