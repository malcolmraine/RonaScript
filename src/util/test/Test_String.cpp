/*****************************************************************************
* File: RnVirtualMachine.cpp
* Description:
* Author: Malcolm Hall
* Date: 6/19/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*****************************************************************************/

#include <catch2/catch_test_macros.hpp>
#include "../String.h"

TEST_CASE("String::Join") {
    REQUIRE(String::Join({"1", "2", "3"}) == "123");
    REQUIRE(String::Join({"1", "2", "3"}, ", ") == "1, 2, 3");
    REQUIRE(String::Join({"", "", ""}, ", ") == ", , ");
    REQUIRE(String::Join({"", "", ""}, "") == "");
}

TEST_CASE("String::Split") {
    REQUIRE(String::Split("1-2-3-4", "-") ==
            std::vector<std::string>{"1", "2", "3", "4"});
    REQUIRE(String::Split("1 2 3 4", " ") ==
            std::vector<std::string>{"1", "2", "3", "4"});
    REQUIRE(String::Split("", "") == std::vector<std::string>{});
    REQUIRE(String::Split("12345", "") ==
            std::vector<std::string>{"1", "2", "3", "4", "5"});
    REQUIRE(String::Split("", "--") == std::vector<std::string>{});
}

TEST_CASE("String::Before") {
    REQUIRE(String::Before("FirstSecond", "Second") == "First");
    REQUIRE(String::Before("FirstSecond", "") == "");
    REQUIRE(String::Before("FirstSecond", "sdfs") == "FirstSecond");
    REQUIRE(String::Before("", "") == "");
    REQUIRE(String::Before("fififif", "i") == "f");
}

TEST_CASE("String::After") {}

TEST_CASE("String::Trim") {}

TEST_CASE("String::Replace") {}

TEST_CASE("String::Contains") {
    REQUIRE(String::Contains("The quick brown fox", "fox"));
    REQUIRE(String::Contains("The quick brown fox", "The"));
    REQUIRE(String::Contains("The quick brown fox", " "));
    REQUIRE(String::Contains("12345", "34"));
    REQUIRE(String::Contains("", ""));
    REQUIRE(String::Contains("The quick brown fox", ""));
    REQUIRE(String::Contains("      ", ""));
    REQUIRE_FALSE(String::Contains("The quick brown fox", "white"));
}

TEST_CASE("String::IsWhiteSpace") {
    REQUIRE(String::IsWhiteSpace(' '));
    REQUIRE(String::IsWhiteSpace('\n'));
    REQUIRE(String::IsWhiteSpace('\t'));
    REQUIRE(String::IsWhiteSpace('\r'));
    REQUIRE(String::IsWhiteSpace("\t\n\r "));
    REQUIRE(String::IsWhiteSpace("      "));
    REQUIRE(String::IsWhiteSpace(" "));
    REQUIRE(String::IsWhiteSpace("\0"));
    REQUIRE_FALSE(String::IsWhiteSpace("The quick brown fox"));
    REQUIRE_FALSE(String::IsWhiteSpace("0000"));
}

TEST_CASE("String::Condense") {
    REQUIRE(String::Condense("1 2 3 4") == "1 2 3 4");
    REQUIRE(String::Condense("1 2  3 4") == "1 2 3 4");
    REQUIRE(String::Condense("    ") == " ");
    REQUIRE(String::Condense("\n\n\n\t") == "\n\t");
}