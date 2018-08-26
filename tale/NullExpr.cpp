#pragma once
#include"NullExpr.h"
namespace expr {
	const std::shared_ptr<NullExpr> NullExpr::NONE = std::make_shared<NullExpr>();
}
