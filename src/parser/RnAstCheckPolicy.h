/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_PARSER_RNASTCHECKPOLICY_H_
#define RONASCRIPT_PARSER_RNASTCHECKPOLICY_H_

#include "ast/AstNode.h"
#include <memory>
#include <vector>
#include <string>

class RnAstCheckPolicy
{
 public:
	[[nodiscard]] virtual bool CheckSubtree(std::shared_ptr<AstNode> subtree) const = 0;
	[[nodiscard]] virtual std::vector<std::string> GetErrorMessages() const = 0;
	virtual void SetErrorMessage(const std::string& message) const = 0;
	[[nodiscard]] virtual std::string GetPolicyName() const = 0;

 protected:
	[[nodiscard]] virtual std::string MakeErrorMessage() = 0;

 private:
	std::vector<std::string> _error_messages;
};

#endif //RONASCRIPT_PARSER_RNASTCHECKPOLICY_H_