/***********************************************************************
 optionlist.cpp - Implements the Type class hierarchy and related
 	things.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2007 by Educational Technology Resources, Inc.  Others may
 also hold copyrights on code in this file.  See the CREDITS file in
 the top directory of the distribution for details.

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

#define MYSQLPP_NOT_HEADER
#include "optionlist.h"

#include "exceptions.h"

#include <sstream>

namespace mysqlpp {
namespace option {

// Initialize table of legal option argument types.
static ArgType legal_opt_arg_types[OPTION_COUNT] =
{
	type_integer,	// connect_timeout
	type_none,		// compress
	type_none,		// named_pipe
	type_string,	// init_command
	type_string,	// read_default_file
	type_string,	// read_default_group
	type_string,	// set_charset_dir
	type_string,	// set_charset_name
	type_integer,	// local_infile
	type_integer,	// protocol
	type_string,	// shared_memory_base_name
	type_integer,	// read_timeout
	type_integer,	// write_timeout
	type_none,		// use_result
	type_none,		// use_remote_connection
	type_none,		// use_embedded_connection
	type_none,		// guess_connection
	type_string,	// set_client_ip
	type_boolean,	// secure_auth
	type_boolean,	// multi_results
	type_boolean,	// multi_statements
	type_boolean,	// report_data_truncation
	type_boolean,   // reconnect
	type_boolean,   // found_rows
	type_boolean,   // ignore_space
	type_boolean,   // interactive
	type_boolean,   // local_files
	type_boolean,   // no_schema
};




std::string
error_message(Type o, Error error)
{
	std::ostringstream os;

	switch (error) {
		case err_type: {
			// Type was set using wrong argument type
			ArgType type = legal_arg_type(o);
			os << "option " << o;
			if (type == type_none) {
				os << " does not take an argument";
			}
			else {
				os << " requires an argument of type " << type;
			}
			break;
		}

		case err_value:
			// C API rejected o, which probably indicates that
			// you passed a o that it doesn't understand.
			os << "option " << o << " not supported by database driver";
			break;

		case err_conn:
			os << "option " << o << " can only be set before "
					"connection is established";
			break;
	}

	return os.str();
}


ArgType
legal_arg_type(Type o) // Yes, there are known type o's in MySQL++.
{
	if ((o > FIRST_OPTION) && (o < OPTION_COUNT)) {
		return legal_opt_arg_types[o];
	}
	else {
		// Non-optional exception.  Something is wrong with the library
		// internals if this one is thrown.
		throw BadOption("bad value given to legal_arg_type()", o);
	}
}


} // end namespace option
} // end namespace mysqlpp
