#include "ScriptExpression.h"
#include "ScriptToken.h"


enum class ScriptExpression::State
{
	None,
	BinaryOperator,
	UnaryOperator,
	VariableDeclaration,
	VariableEvaluation
};

std::shared_ptr<BaseScriptVariable> ScriptExpression::Evaluate()
{
	ScriptExpression::State state = ScriptExpression::State::None;

	for(size_t i = 0; i < _tokens.size(); ++i)
	{
		const ScriptToken& token = _tokens[i];

		switch(state)
		{
		case ScriptExpression::State::None:
			//Check next token type
			break;
		case ScriptExpression::State::BinaryOperator:
			//Check next token type
			break;
		case ScriptExpression::State::UnaryOperator:
			//Check next token type
			break;
		case ScriptExpression::State::VariableDeclaration:
			//Check next token type
			break;
		case ScriptExpression::State::VariableEvaluation:
			//if is return, break, continue
			//	destroy world
			break;
		}
	}

	return std::shared_ptr<BaseScriptVariable>();
}

ScriptExpression::ScriptExpression(const std::vector<ScriptToken>& tokens) : _tokens(tokens)
{

}

ScriptExpression::~ScriptExpression()
{
}
