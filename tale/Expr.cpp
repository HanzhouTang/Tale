#include"Expr.h"
#pragma once
#include<iostream>
#include<memory>
#include<vector>

const std::vector<std::wstring> Expr::TypeList = {
	L"TYPE_NULL", L"TYPE_STRING", L"TYPE_NUMBER", L"TYPE_VARIABLE",L"TYPE_BOOLEAN", L"TYPE_SEQUENCE", L"TYPE_FUNCTION", L"TYPE_BINARYOPERATION"
};