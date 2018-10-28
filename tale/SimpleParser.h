#pragma once
#include"SimpleLexer.h"
#include "Expr.h"
#include"NullExpr.h"
#include"MapExpr.h"
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
		typedef std::unordered_map<SimpleLexer::Status, MemoResult, SimpleLexer::StatusHasher> MemoMap;
		//===================record maps==========================
		MemoMap moresubstrMap;
		MemoMap substrMap;
		MemoMap strMap;
		MemoMap exprMap;
		MemoMap assignMap;
		MemoMap mapMap;
		MemoMap stateMap;
		MemoMap closureMap;
		MemoMap funcMap;
		MemoMap callableMap;
		//std::shared_ptr<expr::Expr> parse();
		SimpleParser(const std::wstring& content) : lexer(content) {}
		SimpleParser(std::wstring&& content) :lexer(std::move(content)) {}
		//SimpleLexer::LexerNode consume();
		//===============for math expression =====================
		std::shared_ptr<expr::Expr> expr();
		void term(std::deque<std::shared_ptr<expr::Expr>>&);
		void moreterms(std::deque<std::shared_ptr<expr::Expr>>&);
		void factor(std::deque<std::shared_ptr<expr::Expr>>&);
		std::shared_ptr<expr::Expr>  factorHelper();
		void morefactors(std::deque<std::shared_ptr<expr::Expr>>&);
		//===============for string =============================
		std::shared_ptr<expr::Expr> str();
		std::shared_ptr<expr::Expr> substr();
		std::shared_ptr<expr::Expr> moresubstrs();
		//===============for element ============================
		std::shared_ptr<expr::Expr> element();
		std::deque<std::shared_ptr<expr::Expr>> elementlists();
		std::deque<std::shared_ptr<expr::Expr>> moreelements();
		//===============for assign =============================
		std::shared_ptr<expr::Expr> assign();
		//===============for map ================================
		typedef std::tuple<expr::MapExpr::KeyType, std::shared_ptr<expr::Expr>> Pair;
		std::shared_ptr<expr::Expr> map();
		Pair pair();
		std::deque<Pair> pairs();
		std::deque<Pair>  morepairs();
		//==============for statement ==========================
		std::shared_ptr<expr::Expr> state();
		std::deque<std::shared_ptr<expr::Expr>> states();
		//==============for closure ============================
		std::shared_ptr<expr::Expr> closure();
		//==============for functions ==========================
		std::shared_ptr<expr::Expr> func();
		std::wstring funcName();
		std::deque<std::wstring> funcParameters();
		std::deque<std::wstring> moreFuncParameters();
		//==============for callable ===========================
		std::shared_ptr<expr::Expr> callable();
		std::shared_ptr<expr::Expr> callobject();
		std::deque<std::deque<std::shared_ptr<expr::Expr>>> paramlists();
		std::deque<std::deque<std::shared_ptr<expr::Expr>>> moreparamlists();
		std::tuple<bool,std::deque<std::shared_ptr<expr::Expr>>> paramlist();
		
		void init();
		static void throwNotMatchError(const std::vector<SimpleLexer::Token>& expectedToken, SimpleLexer::Token realToken, int lineNumber = __LINE__);
		bool match(SimpleLexer::Token t);
	};
}