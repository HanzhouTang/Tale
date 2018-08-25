#pragma once
#include"Expr.h"
struct MapExpr : Expr {
	typedef std::wstring KeyType;
	std::unordered_map<KeyType, std::shared_ptr<Expr>> map;
	MapExpr(const std::shared_ptr<Expr>& runtime) :
		Expr(runtime) {
		setType(TYPE_MAP);
	}
	static std::shared_ptr<MapExpr> createMapExpr(const std::shared_ptr<Expr>& runtime) {
		return std::make_shared<MapExpr>(runtime);
	}
	static std::shared_ptr<MapExpr> createMapExpr() {
		return createMapExpr(nullptr);
	}
	virtual std::shared_ptr<Expr> clone() override {
		auto ret = createMapExpr(getRunTime());
		for (auto & x : map) {
			ret->map[x.first] = x.second->clone();
		}
		return ret;
	}
	void add(const KeyType& key, const std::shared_ptr<Expr>& value) {
		map[key] = value;
	}
	std::shared_ptr<Expr> get(const KeyType& key);
	virtual std::shared_ptr<Expr> getValue(const std::vector<std::shared_ptr<Expr>>& params) override;
	virtual std::wstring toString() override;
};