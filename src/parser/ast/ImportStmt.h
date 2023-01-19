/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_REQUIRE_H
#define RONASCRIPT_REQUIRE_H

#include <string>
#include "NodeType.h"
#include "AstNode.h"
#include "Module.h"

class ImportStmt : public AstNode
{
 public:
	ImportStmt();
	~ImportStmt() override = default;
	std::string ToString(bool nl) override;
	[[nodiscard]] std::string GetFullSourceFile() const;

	std::string source_file;
	std::shared_ptr<Module> module = nullptr;

};

#endif //RONASCRIPT_REQUIRE_H
