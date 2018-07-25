#pragma once
#include"d2dutil.h"
#include<memory>
#include<map>
#include<functional>
using namespace std;

/*
Basic idea
rescource->get("subpath")->get("subsubpath")->get("rescource")
*/


template<T>
abstract class RescourceManagerBase {
	shared_ptr<RescourceManagerBase<T>> get();
	wstring basePath;
	void setBasePath(wstring x) { basePath = x; }
	map<wstring, shared_ptr<RescourceManagerBase<T>>> memory;
	static function<T()> f_;
};
