/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0
/**
 * Analysis part of inline assembly.
 */

#pragma once

#include <liblangutil/Exceptions.h>
#include <liblangutil/EVMVersion.h>

#include <libyul/ASTForward.h>
#include <libyul/Dialect.h>
#include <libyul/Scope.h>

#include <libyul/backends/evm/AbstractAssembly.h>
#include <libyul/backends/evm/EVMDialect.h>

#include <functional>
#include <list>
#include <memory>
#include <optional>
#include <utility>

namespace solidity::langutil
{
class ErrorReporter;
struct SourceLocation;
}

namespace solidity::yul
{

struct AsmAnalysisInfo;

/**
 * Performs the full analysis stage, calls the ScopeFiller internally, then resolves
 * references and performs other checks.
 * If all these checks pass, code generation should not throw errors.
 *执行完整的分析阶段，在内部调用ScopeFiller，然后解析引用并执行其他检查。
如果所有这些检查都通过了，代码生成应该不会抛出错误。
 */
class AsmAnalyzer
{
public:
	explicit AsmAnalyzer(
		AsmAnalysisInfo& _analysisInfo,
		langutil::ErrorReporter& _errorReporter,
		Dialect const& _dialect,
		ExternalIdentifierAccess::Resolver _resolver = ExternalIdentifierAccess::Resolver(),
		std::set<YulString> _dataNames = {}
	):
		m_resolver(std::move(_resolver)),
		m_info(_analysisInfo),
		m_errorReporter(_errorReporter),
		m_dialect(_dialect),
		m_dataNames(std::move(_dataNames))
	{
		if (EVMDialect const* evmDialect = dynamic_cast<EVMDialect const*>(&m_dialect))
			m_evmVersion = evmDialect->evmVersion();
	}

	bool analyze(Block const& _block);

	/// Performs analysis on the outermost code of the given object and returns the analysis info.
	/// Asserts on failure.///对给定对象的最外层代码进行分析并返回分析信息。
	static AsmAnalysisInfo analyzeStrictAssertCorrect(Dialect const& _dialect, Object const& _object);

	std::vector<YulString> operator()(Literal const& _literal);
	std::vector<YulString> operator()(Identifier const&);
	void operator()(ExpressionStatement const&);
	void operator()(Assignment const& _assignment);
	void operator()(VariableDeclaration const& _variableDeclaration);
	void operator()(FunctionDefinition const& _functionDefinition);
	std::vector<YulString> operator()(FunctionCall const& _functionCall);
	void operator()(If const& _if);
	void operator()(Switch const& _switch);
	void operator()(ForLoop const& _forLoop);
	void operator()(Break const&) { }
	void operator()(Continue const&) { }
	void operator()(Leave const&) { }
	void operator()(Block const& _block);

private:
	/// Visits the expression, expects that it evaluates to exactly one value and
	/// returns the type. Reports errors on errors and returns the default type.访问表达式，期望它恰好计算出一个值并/返回类型。报告关于错误的错误，并返回默认类型。
	YulString expectExpression(Expression const& _expr);
	YulString expectUnlimitedStringLiteral(Literal const& _literal);
	/// Vists the expression and expects it to return a single boolean value.访问该表达式并期望它返回一个布尔值。
	/// Reports an error otherwise.
	void expectBoolExpression(Expression const& _expr);

	/// Verifies that a variable to be assigned to exists, can be assigned to
	/// and has the same type as the value.///验证要赋值的变量是否存在，是否可以赋值并具有与该值相同的类型。
	void checkAssignment(Identifier const& _variable, YulString _valueType);

	Scope& scope(Block const* _block);
	void expectValidIdentifier(YulString _identifier, langutil::SourceLocation const& _location);
	void expectValidType(YulString _type, langutil::SourceLocation const& _location);
	void expectType(YulString _expectedType, YulString _givenType, langutil::SourceLocation const& _location);

	bool validateInstructions(evmasm::Instruction _instr, langutil::SourceLocation const& _location);
	bool validateInstructions(std::string const& _instrIdentifier, langutil::SourceLocation const& _location);
	bool validateInstructions(FunctionCall const& _functionCall);

	yul::ExternalIdentifierAccess::Resolver m_resolver;
	Scope* m_currentScope = nullptr;
	/// Variables that are active at the current point in assembly (as opposed to
	/// "part of the scope but not yet declared")
	//在程序集中当前点处于活动状态的变量(与“范围的一部分但尚未声明”相反)
	std::set<Scope::Variable const*> m_activeVariables;
	AsmAnalysisInfo& m_info;
	langutil::ErrorReporter& m_errorReporter;
	langutil::EVMVersion m_evmVersion;
	Dialect const& m_dialect;
	/// Names of data objects to be referenced by builtin functions with literal arguments.///内置函数引用的数据对象的名称。
	std::set<YulString> m_dataNames;
	ForLoop const* m_currentForLoop = nullptr;
};

}
