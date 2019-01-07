#pragma once
#include"Expr.h"
namespace expr {
	struct FunctionExpr;
	struct MapExpr : Expr {
		typedef std::wstring KeyType;
		std::unordered_map<KeyType, std::shared_ptr<Expr>> map;
		std::shared_ptr<FunctionExpr> getter;
		std::shared_ptr<FunctionExpr> setter;
		std::shared_ptr<FunctionExpr> getSetter() { return setter; }
		std::shared_ptr<FunctionExpr> getGetter() { return getter; }

		MapExpr(const std::shared_ptr<Expr>& runtime, const std::unordered_map<KeyType, std::shared_ptr<Expr>>& m) :
			Expr(runtime),map(m) {
			setType(TYPE_MAP);
		}
		static std::shared_ptr<MapExpr> createMapExpr(const std::shared_ptr<Expr>& runtime, 
			const std::unordered_map<KeyType, std::shared_ptr<Expr>>& map);

		static std::shared_ptr<MapExpr> createMapExpr() {
			return createMapExpr(nullptr, std::unordered_map<KeyType, std::shared_ptr<Expr>>());
		}

		static std::shared_ptr<MapExpr> createMapExpr(const std::shared_ptr<MapExpr>& map) {
			auto m = map->map;
			std::unordered_map<KeyType, std::shared_ptr<Expr>> m1;
			for (auto& x : m) {
				m1[x.first] = x.second->clone();
			}
			return createMapExpr(nullptr, m1);
		}
		static std::shared_ptr<MapExpr> createMapExpr(const std::shared_ptr<Expr>& runtime) {
			return createMapExpr(runtime, std::unordered_map<KeyType, std::shared_ptr<Expr>>());
		}
		
		static std::shared_ptr<MapExpr> createMapExpr(
			const std::unordered_map<KeyType, std::shared_ptr<Expr>>& map) {
			return createMapExpr(nullptr, map);
		}

		virtual std::shared_ptr<Expr> clone() override {
			auto ret = createMapExpr(getRunTime(),map);
			for (auto & x : ret->map) {
				ret->map[x.first] = x.second->clone();
			}
			return ret;
		}
		std::shared_ptr<Expr> add(const KeyType& key, const std::shared_ptr<Expr>& value) {
			map[key] = value;
			return map[key];
		}
		std::shared_ptr<Expr> get(const KeyType& key);
		virtual std::shared_ptr<Expr> getValue(const std::vector<std::shared_ptr<Expr>>& params) override;
		virtual std::wstring toString() override;
		virtual std::wstring repr() override;
	};
}