/// \file optionlist.h
/// \brief Declares connection option mechanisms used by Connection
///
/// These used to be part of the definition of class Connection, but
/// option setting is complex enough that it was worth extracting as
/// much of it out into a separate module as possible.

/***********************************************************************
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

#if !defined(MYSQLPP_OPTIONLIST_H)
#define MYSQLPP_OPTIONLIST_H

#include "common.h"

#include <deque>
#include <string>


namespace mysqlpp {
namespace option {

////////////////////////////////////////////////////////////////////////
// Enums

/// \brief Options you can set with Connection::set_option()
///
/// This list doesn't correspond directly to any one thing in the MySQL
/// C API.  It is a pastiche of the \c mysql_option and
/// \c enum_mysql_set_option C API enums, plus additional things that
/// MySQL++ treats as "options" that the C API handles separately.
enum Type 
{
	FIRST_OPTION = -1,       ///< INTERNAL USE ONLY

	connect_timeout = 0,     ///< change connect() timeout
	compress,                ///< enable data compression
	named_pipe,              ///< suggest use of named pipes
	init_command,            ///< give SQL executed on connect
	read_default_file,       ///< override use of my.cnf
	read_default_group,      ///< override one group in my.cnf
	set_charset_dir,         ///< give path to charset definition files
	set_charset_name,        ///< give name of default charset
	local_infile,            ///< enable LOAD LOCAL INFILE statement
	protocol,                ///< set type of protocol to use
	shared_memory_base_name, ///< set name of shmem segment for IPC
	read_timeout,            ///< set timeout for IPC data reads
	write_timeout,           ///< set timeout for IPC data writes
	use_result,              ///< unused from MySQL v4.1.1 on
	use_remote_connection,   ///< connect to remote server in preference to embedded one
	use_embedded_connection, ///< use embedded server in preference to remote server
	guess_connection,        ///< default, overriding use_remote_connection and use_embedded_connection
	set_client_ip,           ///< fake client IP address when connecting to embedded server
	secure_auth,             ///< enforce use of secure authentication, refusing connection if not available
	multi_results,           ///< enable multiple result sets in a reply
	multi_statements,        ///< enable multiple queries in a request
	report_data_truncation,  ///< set reporting of data truncation errors
	reconnect,               ///< enable automatic reconnection to server
	found_rows,              ///< return # of matched rows in results, not changed row count
	ignore_space,            ///< allow spaces after function names in queries
	interactive,             ///< mark program as interactive; affects connection timeout
	local_files,             ///< enable LOAD DATA LOCAL statement
	no_schema,               ///< disable db.tbl.col syntax in queries

	OPTION_COUNT             ///< INTERNAL USE ONLY (# of options supported)
};


/// \brief Legal types of option arguments
enum ArgType {
	type_none,      ///< option takes no argument
	type_string,    ///< option takes a string type argument
	type_integer,   ///< option takes an integral argument
	type_boolean    ///< option takes a Boolean argument
};


////////////////////////////////////////////////////////////////////////
// Structures

/// \brief Information about a particular connection option
///
/// This is the aggregate "value type" of the options mechanism.
/// Whenever you set an option, you are creating one of these objects
/// indirectly.  The options mechanism acts internally on lists of
/// these structures.
struct Info {
	Type option;
	ArgType arg_type;
	std::string str_arg;
	unsigned int int_arg;
	bool bool_arg;

	Info(Type o) :
	option(o),
	arg_type(type_none),
	int_arg(0),
	bool_arg(false)
	{
	}

	Info(Type o, const char* a) :
	option(o),
	arg_type(type_string),
	str_arg(a),
	int_arg(0),
	bool_arg(false)
	{
	}

	Info(Type o, unsigned int a) :
	option(o),
	arg_type(type_integer),
	int_arg(a),
	bool_arg(false)
	{
	}

	Info(Type o, bool a) :
	option(o),
	arg_type(type_boolean),
	int_arg(0),
	bool_arg(a)
	{
	}
};

/// \brief Types of option setting errors we can diagnose
enum Error {
	err_type,   ///< incorrect argument type for this option
	err_value,  ///< C API returned an error when setting this option
	err_conn,   ///< option can't be set after connection is up
};


////////////////////////////////////////////////////////////////////////
// Typedefs

/// \brief The data type of the list of connection options
typedef std::deque<Info> List;

/// \brief Primary iterator type into List
typedef List::const_iterator ListIt;


////////////////////////////////////////////////////////////////////////
// Function prototypes

/// \brief Returns a canned "bad option" error message
///
/// \param o option that was rejected
/// \param error reason option was rejected
///
/// \internal Used by DBDriver::set_option() when it doesn't know
/// how to process a given call.
MYSQLPP_EXPORT std::string error_message(Type o, Error error);

/// \brief Given an option value, return its proper argument type
MYSQLPP_EXPORT ArgType legal_arg_type(Type o);

} // end namespace option
} // end namespace mysqlpp

#endif // !defined(MYSQLPP_OPTIONLIST_H)
