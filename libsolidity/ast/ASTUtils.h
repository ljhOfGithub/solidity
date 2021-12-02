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

#pragma once

namespace solidity::frontend
{

class VariableDeclaration;
class Declaration;
class Expression;

/// Find the topmost referenced constant variable declaration when the given variable
/// declaration value is an identifier. Works only for constant variable declarations.
/// Returns nullptr if an identifier in the chain is not referencing a constant variable declaration.
///找到最上面引用的常量变量声明时，给定的变量
///声明值是一个标识符。只适用于常量变量声明。
///如果链中的标识符没有引用常量变量声明，则返回nullptr。
VariableDeclaration const* rootConstVariableDeclaration(VariableDeclaration const& _varDecl);

/// Returns true if the constant variable declaration is recursive.
///如果常量变量声明是递归的，则返回true。
bool isConstantVariableRecursive(VariableDeclaration const& _varDecl);

}
