#pragma once
#include"SimpleLexer.h"
#include "Expr.h"
#include"NullExpr.h"
#include<deque>
#include<unordered_map>
namespace parser {

	struct SimpleParser {
		parser::SimpleLexer lexer;
		//std::deque<SimpleLexer::LexerNode> lookahead;
		std::wstring content;
		struct MemoResult {
			std::shared_ptr<expr::Expr> result;
			SimpleLexer::Status newStatus;
			MemoResult(const std::shared_ptr<expr::Expr>& r, const SimpleLexer::Status& status) :
				result(r), newStatus(status) {}
			MemoResult() {
				result = expr::NullExpr::createNullExpr();
			}
		};
		typedef std::unordered_map<SimpleLexer::Status, MemoResult, SimpleLexer::StatusHasher> MomoMap;
		//===================record maps==========================
		MomoMap moresubstrMap;
		MomoMap substrMap;
		MomoMap strMap;
		MomoMap exprMap;

		//std::shared_ptr<expr::Expr> parse();
		SimpleParser(const std::wstring& content) : lexer(content) {}
		SimpleParser(std::wstring&& content) :lexer(std::move(content)) {}
		//SimpleLexer::LexerNode consume();
		//===============for math expression =====================
		std::shared_ptr<expr::Expr> expr();
		void term(std::deque<std::shared_ptr<expr::Expr>>&);
		void moreterms(std::deque<std::shared_ptr<expr::Expr>>&);
		void factor(std::deque<std::shared_ptr<expr::Expr>>&);
		void morefactors(std::deque<std::shared_ptr<expr::Expr>>&);
		//===============for string =============================
		std::shared_ptr<expr::Expr> str();
		std::shared_ptr<expr::Expr> substr();
		std::shared_ptr<expr::Expr> moresubstrs();
		//===============for element ============================

		//===============for assign =============================
		std::shared_ptr<expr::Expr> element();
		std::shared_ptr<expr::Expr> assign();
		std::shared_ptr<expr::EqualExpr> moresssign();


		void init();
		static void throwNotMatchError(const std::vector<SimpleLexer::Token>& expectedToken, SimpleLexer::Token realToken, int lineNumber = __LINE__);
		bool match(SimpleLexer::Token t);
	};
}