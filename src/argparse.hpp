
/* License:
 *
 * MIT License
 *
 * Copyright (c) 2020 magistermaks
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
 * Very simple utility created for Sequensa,
 * might expand it some time in the future.
 * For documentation look at the ArgParse class,
 * its pretty simple.
 */

/* Usage:
 * use #define ARGPARSE_IMPLEMENT - Implement the library
 *
 * Example:
 * ArgParse argp( argc, argv );
 * bool a = argp.hasFlag( "-a" );
 */

#ifndef ARGPARSE_HPP_
#define ARGPARSE_HPP_

#include <vector>
#include <string>
#include <algorithm>

class ArgParse {

	public:
		ArgParse( int argc, char **argv );
		ArgParse( ArgParse&& argp );

		bool hasFlag( const char* name );
		bool isEmpty();
		const std::string& getName();
		const std::vector<std::string>& getFlags();
		const std::vector<std::string>& getValues();

	private:
		std::vector<std::string> flags;
		std::vector<std::string> values;
		std::string name;
		bool empty;

};

#ifdef ARGPARSE_IMPLEMENT

ArgParse::ArgParse( int argc, char **argv ) {

	if( argc > 0 ) {

		this->name = std::string( argv[0] );

		for( int i = 1; i < argc; i ++ ) {

			std::string arg( argv[i] );

			if( arg[0] == '-' ) {
				flags.push_back( arg );
			}else{
				values.push_back( arg );
			}

		}

	}

	this->empty = (argc <= 1);

}

ArgParse::ArgParse( ArgParse&& argp ) {
	this->flags = std::move( argp.flags );
	this->values = std::move( argp.values );
	this->name = std::move( argp.name );
	this->empty = std::move( argp.empty );
}

bool ArgParse::hasFlag( const char* name ) {
	return std::find(this->flags.begin(), this->flags.end(), std::string(name)) != this->flags.end();
}

bool ArgParse::isEmpty() {
	return this->empty;
}

const std::string& ArgParse::getName() {
	return this->name;
}

const std::vector<std::string>& ArgParse::getFlags() {
	return this->flags;
}

const std::vector<std::string>& ArgParse::getValues() {
	return this->values;
}

#undef ARGPARSE_IMPLEMENT
#endif

#endif /* ARGPARSE_HPP_ */
