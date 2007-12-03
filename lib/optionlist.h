/// \file optionlist.h
/// \brief Declares connection option mechanisms used by Connection
///
/// These used to be part of the definition of class Connection, but
/// option setting is complex enough that it was worth extracting as
/// much of it out into a separate module as possible.

/***********************************************************************
 Copyright (c) 2007 by Educational Technology Resources, Inc.  Others
 may also hold copyrights on code in this file.  See the CREDITS
 file in the top directory of the distribution for details.

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

#if !defined(DOXYGEN_IGNORE)
class DBDriver;
#endif


////////////////////////////////////////////////////////////////////////
// Classes

/// \brief Define abstract interface for all *Option subclasses.
///
/// This is the base class for the mid-level interface classes that take
/// arguments, plus the direct base for options that take no arguments.
class Option
{
public:
	/// \brief Types of option setting errors we can diagnose
	enum Error {
		err_NONE,
		err_api_limit,
		err_api_reject,
		err_bad_arg,
		err_connected
	};
	
	virtual ~Option() { }					///< Destroy object
	virtual Error set(DBDriver* dbd) = 0;	///< Apply option
};


/// \brief Define abstract interface for all *Options that take an
/// argument.
template <typename T>
class DataOption : public Option
{
public:
	typedef T ArgType;						///< Alias for template param

protected:
	DataOption(const T& arg) : arg_(arg) { }///< Construct object
	T arg_;									///< The argument value
};

typedef DataOption<unsigned> IntegerOption;		///< Option w/ int argument
typedef DataOption<bool> BooleanOption;			///< Option w/ bool argument
typedef DataOption<std::string> StringOption;	///< Option w/ string argument


/// \brief Enable data compression on the connection
class CompressOption : public Option
{
#if !defined(DOXYGEN_IGNORE)
public:
	CompressOption() : Option() { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Change Connection::connect() default timeout
class ConnectTimeoutOption : public IntegerOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ConnectTimeoutOption(ArgType arg) : IntegerOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Make Query::affected_rows() return number of matched rows
///
/// Default is to return number of \b changed rows.
class FoundRowsOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	FoundRowsOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Allow C API to guess what kind of connection to use
///
/// This is the default.  The option exists to override
/// UseEmbeddedConnectionOption and UseEmbeddedConnectionOption.
class GuessConnectionOption : public Option
{
#if !defined(DOXYGEN_IGNORE)
public:
	GuessConnectionOption() : Option() { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Allow spaces after function names in queries
class IgnoreSpaceOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	IgnoreSpaceOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Give SQL executed on connect
class InitCommandOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	InitCommandOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Assert that this is an interactive program
///
/// Affects connection timeouts.
class InteractiveOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	InteractiveOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enable LOAD DATA LOCAL statement
class LocalFilesOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	LocalFilesOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enable LOAD LOCAL INFILE statement
class LocalInfileOption : public IntegerOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	LocalInfileOption(ArgType arg) : IntegerOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enable multiple result sets in a reply
class MultiResultsOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	MultiResultsOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enable multiple queries in a request to the server
class MultiStatementsOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	MultiStatementsOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Suggest use of named pipes
class NamedPipeOption : public Option
{
#if !defined(DOXYGEN_IGNORE)
public:
	NamedPipeOption() : Option() { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Disable db.tbl.col syntax in queries
class NoSchemaOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	NoSchemaOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Set type of protocol to use
class ProtocolOption : public IntegerOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ProtocolOption(ArgType arg) : IntegerOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Override use of my.cnf
class ReadDefaultFileOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ReadDefaultFileOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Override use of my.cnf
class ReadDefaultGroupOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ReadDefaultGroupOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Set timeout for IPC data reads
class ReadTimeoutOption : public IntegerOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ReadTimeoutOption(ArgType arg) : IntegerOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enable automatic reconnection to server
class ReconnectOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ReconnectOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Set reporting of data truncation errors
class ReportDataTruncationOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	ReportDataTruncationOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Enforce use of secure authentication, refusing connection if
/// not available
class SecureAuthOption : public BooleanOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	SecureAuthOption(ArgType arg) : BooleanOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Give path to charset definition files
class SetCharsetDirOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	SetCharsetDirOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Give name of default charset
class SetCharsetNameOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	SetCharsetNameOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Fake client IP address when connecting to embedded server
class SetClientIpOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	SetClientIpOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Set name of shmem segment for IPC
class SharedMemoryBaseNameOption : public StringOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	SharedMemoryBaseNameOption(ArgType arg) : StringOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Connect to embedded  server in preference to remote server
class UseEmbeddedConnectionOption : public Option
{
#if !defined(DOXYGEN_IGNORE)
public:
	UseEmbeddedConnectionOption() : Option() { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Connect to remote server in preference to embedded server
class UseRemoteConnectionOption : public Option
{
#if !defined(DOXYGEN_IGNORE)
public:
	UseRemoteConnectionOption() : Option() { }

private:
	Error set(DBDriver* dbd);
#endif
};


/// \brief Set timeout for IPC data reads
class WriteTimeoutOption : public IntegerOption
{
#if !defined(DOXYGEN_IGNORE)
public:
	WriteTimeoutOption(ArgType arg) : IntegerOption(arg) { }

private:
	Error set(DBDriver* dbd);
#endif
};


////////////////////////////////////////////////////////////////////////
// Typedefs

/// \brief The data type of the list of connection options
typedef std::deque<Option*> OptionList;

/// \brief Primary iterator type into List
typedef OptionList::const_iterator OptionListIt;

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_OPTIONLIST_H)
