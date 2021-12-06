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
 * Exceptions in Yul.
 */

#pragma once

#include <libsolutil/Exceptions.h>
#include <libsolutil/Assertions.h>

#include <libyul/YulString.h>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/facilities/overload.hpp>

namespace solidity::yul
{

struct YulException: virtual util::Exception {};
struct OptimizerException: virtual YulException {};
struct CodegenException: virtual YulException {};
struct YulAssertion: virtual YulException {};

struct StackTooDeepError: virtual YulException
{
	StackTooDeepError(YulString _variable, int _depth, std::string const& _message):
		variable(_variable), depth(_depth)
	{
		*this << util::errinfo_comment(_message);
	}
	StackTooDeepError(YulString _functionName, YulString _variable, int _depth, std::string const& _message):
		functionName(_functionName), variable(_variable), depth(_depth)
	{
		*this << util::errinfo_comment(_message);
	}
	YulString functionName;
	YulString variable;
	int depth;
};

/// Assertion that throws an YulAssertion containing the given description if it is not met.
///如果不满足，抛出包含给定描述的YulAssertion的断言。
#if !BOOST_PP_VARIADICS_MSVC
#define yulAssert(...) BOOST_PP_OVERLOAD(yulAssert_,__VA_ARGS__)(__VA_ARGS__)
#else
#define yulAssert(...) BOOST_PP_CAT(BOOST_PP_OVERLOAD(yulAssert_,__VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY())
#endif
//可变参数宏BOOST_PP_OVERLOAD https://blog.csdn.net/weixin_30267697/article/details/98058493
//BOOST_PP_CAT宏主要用来连接两个标识符。此宏被其它地方用到。https://blog.csdn.net/freemannnn/article/details/24524919
#define yulAssert_1(CONDITION) \
	yulAssert_2(CONDITION, "")

#define yulAssert_2(CONDITION, DESCRIPTION) \
	assertThrowWithDefaultDescription( \
		CONDITION, \
		::solidity::yul::YulAssertion, \
		DESCRIPTION, \
		"Yul assertion failed" \
	)

}
