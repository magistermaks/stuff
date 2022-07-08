
/*
 * MIT License
 *
 * Copyright (c) 2020, 2021 magistermaks
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

#ifndef VSTL_HPP_INCLUDED
#define VSTL_HPP_INCLUDED

#include <vector>
#include <functional>
#include <exception>
#include <string>
#include <chrono>
#include <iostream>

#define VSTL_MODE_LENIENT 1
#define VSTL_MODE_STRICT 2

#ifndef VSTL_TEST_COUNT
#	define VSTL_TEST_COUNT 1
#endif

#define ASSERT( condition, message ) if( !(condition) ) FAIL( message )
#define TEST( name, ... ) long __vstl_test__##name = vstl::Test( #name, __LINE__, [] () -> void __VA_ARGS__ ).add();
#define FAIL( what ) throw vstl::TestFail( what );
#define CHECK_ELSE( value, expected ) for( auto __vstl_a__ = value, __vstl_b__ = expected; __vstl_a__ != __vstl_b__; )
#define CHECK( value, expected ) CHECK_ELSE( value, expected ) FAIL( "Expected: " + std::to_string( __vstl_b__ ) + " got: " + std::to_string( __vstl_a__ ) )
#define REGISTER_EXCEPTION( id, name ) long __vstl_exception__##id = vstl::register_exception( [] (std::exception_ptr p) -> void { try{ if( p ) std::rethrow_exception(p); } catch( name& e ) { throw vstl::TestFail( e.what() ); } catch ( ... ) {} } )
#define BEGIN( mode ) int main() { return vstl::run( mode ); }
#define EXPECT_ERR( ... ) try{ __VA_ARGS__; FAIL( "Expected exception!" ); } catch (...) {}

namespace vstl {

	class TestFail: public std::exception {

		public:
			explicit TestFail ( const std::string& error );
			virtual const char* what() const throw();

		private:
			std::string error;
	};

	class Test {

		public:
			Test( std::string name, int line, std::function<void(void)> lambda );
			long add();
			bool run( int mode );

		private:
			const std::string name;
			const int line;
			const std::function<void(void)> lambda;

	};

	std::vector<Test> tests;
	std::vector<std::function<void(std::exception_ptr)>> exception_handles;
	int successful_count = 0;
	int failed_count = 0;

	int run( int mode );
	long register_exception( std::function<void(std::exception_ptr)> handle );

};

int vstl::run( int mode ) {
	auto start = std::chrono::steady_clock::now();

	for( Test& test : tests ) {
		if( !test.run( mode ) && mode == VSTL_MODE_STRICT ) {
			break;
		}
	}

	auto time = std::chrono::steady_clock::now() - start;

	std::cout << std::endl << "Executed " + std::to_string( vstl::failed_count + vstl::successful_count ) + " tests, " +
		std::to_string( vstl::failed_count ) + " failed, " +
		std::to_string( vstl::successful_count ) + " succeeded. (taken: " <<
		std::chrono::duration<double, std::milli>(time).count() << "ms)" << std::endl;

#ifndef VSTL_RETURN_0
	return vstl::failed_count;
#else
	return 0;
#endif

}

long vstl::register_exception( std::function<void(std::exception_ptr)> handle ) {
	exception_handles.push_back( handle );
	return exception_handles.size() - 1;
}


vstl::Test::Test( std::string _name, int _line, std::function<void(void)> _lambda ): name( _name ), line( _line ), lambda( _lambda ) {}

long vstl::Test::add() {
	vstl::tests.push_back( *this );
	return vstl::tests.size() - 1;
}

bool vstl::Test::run( int mode ) {
	try{
		for( int i = 0; i < VSTL_TEST_COUNT; i ++ ) this->lambda();
	} catch (vstl::TestFail &fail) {
		std::cerr << "Test '" + this->name + "' failed! Error message: " << fail.what() << std::endl;
		vstl::failed_count ++;
		return false;
	}catch( ... ) {
		std::exception_ptr p = std::current_exception();

		for( auto& handle : vstl::exception_handles ) {
			try{
				handle( p );
			}catch( vstl::TestFail &fail ) {
				std::cerr << "Test '" + this->name + "' failed! Error message: " << fail.what() << std::endl;
				vstl::failed_count ++;
				return false;
			}
		}

		try{
			std::rethrow_exception(p);
		}catch( std::exception& err ) {
			std::cerr << "Test '" + this->name + "' failed! Unregistered exception thrown! Error: " << err.what() << std::endl;
			vstl::failed_count ++;
			return false;
		}catch( ... ) {
			std::cerr << "Test '" + this->name + "' failed! Unregistered exception thrown! Error: unknown" << std::endl;
			vstl::failed_count ++;
			return false;
		}

	}

	std::cout << "Test '" + this->name + "' successful!" << std::endl;
	vstl::successful_count ++;
	return true;
}

vstl::TestFail::TestFail( const std::string& _error ): error( _error ) {}

const char* vstl::TestFail::what() const throw() {
	return this->error.c_str();
}

#endif // VSTL_HPP_INCLUDED

