
/*
 * MIT License
 *
 * Copyright (c) 2020 - 2023 magistermaks
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * VSTL - Very Simple Test Library
 */

#pragma once

#ifndef VSTL_TEST_COUNT
#define VSTL_TEST_COUNT 1
#endif

#include <vector>
#include <functional>
#include <exception>
#include <chrono>
#include <iostream>

#define VSTL_VERSION "3.0"

// internal macros, don't use :gun:
#define VSTL_UNEQUAL(va, vb) for(auto __vstl_a__ = (va), __vstl_b__ = (decltype(__vstl_a__)) (vb); __vstl_a__ != __vstl_b__;)
#define VSTL_ENTRYPOINT int main()
#define VSTL_BLC ;
#define VSTL_JOIN(prefix, suffix) prefix##suffix
#define VSTL_CAT(prefix, suffix) VSTL_JOIN(prefix, suffix)
#define VSTL_UNIQUE(prefix) VSTL_CAT(prefix, __LINE__)
#define VSTL_STR_VALUE(value) #value
#define VSTL_TO_STR(macro) VSTL_STR_VALUE(macro)
#define VSTL_LINE "on line " VSTL_TO_STR(__LINE__)
#define VSTL_EXCEPT "Expected exception " VSTL_LINE "!"
#define VSTL_RETHROW catch (vstl::TestFail& fail) { throw fail; }

/// used to define a test of the given [name]: TEST(example_test) { /* the test */ }
#define TEST(name)          VSTL_BLC  vstl::Test VSTL_UNIQUE(__vstl_test__) = #name+[] ()

/// used as a starting point for the VSTL, place anywhere in the test file, preferebly at the end: BEGIN(VSTL_MODE_LENIENT)
#define BEGIN(mode)         VSTL_BLC  VSTL_ENTRYPOINT { return vstl::run(std::cout, mode); }

/// used to defined error handlers (converters), place anywhere in the test file. use like this: HANDLER { CATCH_PTR (my_error_class& err) { FAIL(err.str())  } }
#define HANDLER             VSTL_BLC  vstl::Handler VSTL_UNIQUE(__vstl_handler__) = "handler"+[] (std::exception_ptr ptr)

/// helper used in defining error handlers
#define CATCH_PTR                     try { if(ptr) std::rethrow_exception(ptr); } catch

/// failes the test with the given [reason] when called
#define FAIL(reason)                  vstl::fail((reason));

/// asserts the [condition] is true, otherwise failes the test with the custom [reason]
#define ASSERT_MSG(condition, reason) if(!(condition)) FAIL(reason)

/// asserts the [condition] is true, otherwise failes the test
#define ASSERT(condition)             ASSERT_MSG(condition, "Expected " #condition " to be true, but it was not, " VSTL_LINE "!")

/// checks if the [va] equals [vb], otherwise failes the test
#define CHECK(va, vb)                 VSTL_UNEQUAL(va, vb) FAIL("Expected " #va " to be equal " #vb ", but it was not, " VSTL_LINE "!")

/// checks if the given block [...] throws an exception, otherwise failes the test
#define EXPECT_ANY(...)               try { __VA_ARGS__; FAIL(VSTL_EXCEPT); } VSTL_RETHROW catch (...) {}

/// checks if the given block [...] throws an exception of the given [type], otherwise failes the test
#define EXPECT(type, ...)             try{ __VA_ARGS__; FAIL(VSTL_EXCEPT); } VSTL_RETHROW catch (type& t) {} catch (...) { FAIL("Expected exception of type " #type ", " VSTL_LINE "!"); }

enum TestMode : short {

	/// will skip failed tests
	VSTL_MODE_LENIENT,

	/// will stop as soon any any test failes
	VSTL_MODE_STRICT

};

// internal namespace, don't use :gun:
namespace vstl {

	struct Test;
	struct Handler;

	std::vector<Test> tests;
	std::vector<Handler> handlers;
	size_t failed = 0, successful = 0;

	/// add new test
	void add_test(const Test& test) {
		tests.push_back(test);
	}

	/// add new error handler
	void add_handler(const Handler& handler) {
		handlers.push_back(handler);
	}

	struct TestFail final: public std::runtime_error {

		explicit TestFail(const std::string& error)
		: runtime_error(error) {}

	};

	struct Handler final {

		using Func = std::function<void(std::exception_ptr)>;

		const Func func;

		Handler(const Func& func)
		: func(func) {
			vstl::add_handler(*this);
		}

		void call(std::exception_ptr ptr) const {
			func(ptr);
		}

	};

	struct Test final {

		using Func = std::function<void(void)>;

		const char* name;
		const Func func;

		Test(const char* name, const Func& func)
		: name(name), func(func) {
			vstl::add_test(*this);
		}

		void call(const size_t count) const {
			for (size_t i = 0; i < count; i ++) {
				func();
			}
		}

		bool run(std::ostream& out) const throw() {
			try {
				call(VSTL_TEST_COUNT);

			} catch (vstl::TestFail& fail) {
				out << "Test '" << this->name << "' failed! Error: " << fail.what() << std::endl;
				vstl::failed ++;
				return false;

			} catch (...) {
				std::exception_ptr ptr = std::current_exception();

				// try to convert the error using the defined error handlers
				for (const Handler& handler : vstl::handlers) {
					try {
						handler.call(ptr);
					} catch(vstl::TestFail& fail) {
						out << "Test '" << this->name << "' failed! Error: " << fail.what() << std::endl;
						vstl::failed ++;
						return false;
					} catch (...) {
						// ignore
					}
				}

				// everything has failed us, just try to print some reason
				try {
					std::rethrow_exception(ptr);
				} catch (std::exception& err) {
					out << "Test '" << this->name << "' failed! Unregistered exception thrown! Error: " << err.what() << std::endl;
				} catch (const char* err) {
					out << "Test '" << this->name << "' failed! Unregistered exception thrown! Error: " << err << std::endl;
				} catch (int err) {
					out << "Test '" << this->name << "' failed! Unregistered exception thrown! Error: " << err << std::endl;
				} catch (...) {
					out << "Test '" << this->name << "' failed! Unregistered exception thrown! Error: unknown" << std::endl;
				}

				vstl::failed ++;
				return false;
			}

			out << "Test '" << this->name << "' successful!" << std::endl;
			vstl::successful ++;
			return true;
		}

	};

	void summary(std::ostream& out, const auto& time) {
		size_t executed = vstl::failed + vstl::successful;
		double millis = std::chrono::duration<double, std::milli>(time).count();

		out << std::endl << "Executed " << executed << " ";
		out << (executed == 1 ? "test" : "tests") << ", ";
		out << vstl::failed << " failed, ";
		out << vstl::successful << " succeeded.";
		out << " (time: " << millis << "ms)";
		out << std::endl;
	}

	int run(std::ostream& out, TestMode mode) {
		const auto start = std::chrono::steady_clock::now();

		for (const Test& test : tests) {
			if (!test.run(out) && mode == VSTL_MODE_STRICT) {
				break;
			}
		}

		summary(out, std::chrono::steady_clock::now() - start);

		#ifdef VSTL_RETURN_ZERO
		return 0;
		#endif

		#ifdef VSTL_RETURN_BOOL
		return vstl::failed != 0 ? 1 : 0;
		#endif

		return vstl::failed;
	}

	template<typename S>
	void fail(const S& message) {
		throw TestFail {message};
	}

}

vstl::Test operator +(const char* name, const vstl::Test::Func& tester) {
    return vstl::Test {name, tester};
}

vstl::Handler operator +(const char* name, const vstl::Handler::Func& handler) {
    return vstl::Handler {handler};
}