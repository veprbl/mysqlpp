/***********************************************************************
 cmdline.h - Declares the interface to the MySQL++'s command line
	parsing logic, used by the examples and the utility programs.
	Not intended for use by third parties!	If it breaks, you
	get to keep all the pieces.

 Copyright (c) 2007-2009 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the
 CREDITS.txt file in the top directory of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#if !defined(MYSQLPP_CMDLINE_H) && !defined(DOXYGEN_IGNORE)
#define MYSQLPP_CMDLINE_H

#include "common.h"

#include <string>
#include <vector>

#include <assert.h>

namespace mysqlpp {
	// Simple class for parsing a command line and holding the results.
	// Just contains common functionality and data structures; see
	// overrides below for instantiable classes.
	class MYSQLPP_EXPORT CommandLineBase
	{
	public:
		//// Public types
		typedef std::vector<std::string> ArgumentList;
		typedef ArgumentList::const_iterator ArgumentListIt;

		//// Public interface
		// Get reference to list of command line arguments past the
		// last flag and its possible argument.
		const ArgumentList& extra_args() const 
				{ return extra_args_; }

		// Return truthy value if command line was parsed successfully
		operator void*() const
		{
			return successful_ ? const_cast<bool*>(&successful_) : 0;
		}

	protected:
		//// Subclass interface
		// Hidden ctor and dtor to prevent instantiation
		CommandLineBase(int argc, char* const argv[], const char* opts) :
		argc_(argc),
		argv_(argv),
		opts_(opts),
		successful_(false)
		{
			assert(argc > 0 && argv && opts);
		}
		virtual ~CommandLineBase() { }

		// Save non-option arguments to extra_args_ list, and mark the
		// object as "successful".  Subclass ctor should call this after
		// parse_next() loop gets EOF.
		void finish_parse();

		// Wrapper for getopt()
		int parse_next() const;

		// Accessors for getopt() globals, so subclasses can ignore
		// getopt()'s interface entirely.
		const char* option_argument() const;
		int option_index() const;

		// Get the program's executable name
		const char* program_name() const { return argv_[0]; }

	private:
		//// Internal data
		int argc_;
		char* const* argv_;
		const char* opts_;
		bool successful_;
		ArgumentList extra_args_;
	};

	// Stuff related to MySQL++ examples specifically
	namespace examples {
		// Name of examples' DB
		extern const char* db_name;

		// Command line parsing mechanism for ../examples/*.cpp
		class MYSQLPP_EXPORT CommandLine : public CommandLineBase
		{
		public:
			//// Public interface
			// Ctor
			CommandLine(int argc, char* const argv[],
					const char* user = 0, const char* pass = 0,
					const char* usage_extra = 0);

			// Show a mesage explaining the program's proper usage
			void print_usage(const char* extra) const;

			// Accessors
			bool dtest_mode() const { return dtest_mode_; }
			const char* pass() const { return pass_; }
			int run_mode() const { return run_mode_; }
			const char* server() const { return server_; }
			const char* user() const { return user_; }

		private:
			//// Internal data
			bool dtest_mode_;
			int run_mode_;
			const char* server_;
			const char* user_;
			const char* pass_;
		};
	} // end namespace mysqlpp::examples
} // end namespace mysqlpp

#endif // !defined(MYSQLPP_CMDLINE_H)

