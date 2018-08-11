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
	shared_ptr<T> get(wstring name);
	map<wstring, shared_ptr<T>> memory;
	static function<T()> f_;
};
//I need support import different kind of resource and manage them by a single interface.
// I am not sure how to manage them.
//Need do some resource
