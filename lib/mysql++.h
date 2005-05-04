/// \file mysql++.h
/// \brief The main MySQL++ header file.  It simply #includes all of the
/// other header files (except for custom.h) in the proper order.
///
/// Most programs will have no reason to #include any of the other
/// MySQL++ headers directly, except for custom.h.

#include "platform.h"

#include "defs.h"

#include "coldata.h"
#include "compare.h"
#include "connection.h"
#include "const_string.h"
#include "convert.h"
#include "datetime.h"
#include "exceptions.h"
#include "field_names.h"
#include "field_types.h"
#include "fields.h"
#include "manip.h"
#include "myset.h"
#include "null.h"
#include "query.h"
#include "resiter.h"
#include "result.h"
#include "row.h"
#include "sql_query.h"
#include "sql_string.h"
#include "stream2string.h"
#include "tiny_int.h"
#include "type_info.h"
#include "vallist.h"

/**
	\mainpage MySQL++ Reference Manual

	\section getting_started Getting Started

	The best place to get started is the new user manual. It provides
	a guide to the example programs and more.


	\section classes Major Classes

	In MySQL++, the main user-facing classes are mysqlpp::Connection,
	mysqlpp::Query, mysqlpp::Result, and mysqlpp::Row.

	In addition, MySQL++ has a mechanism called Specialized SQL
	Structures (SSQLS), which allow you to create C++ structures
	that parallel the definition of the tables in your database
	schema. These let you manipulate the data in your database
	using native C++ data structures. Many programs using this
	feature never directly use SQL, because MySQL++ generates it all
	automatically. There is a chapter in the user manual on how to
	use this feature of the library. It isn't the only way to use
	MySQL++, but it sure makes some things a lot easier.


	\section files Major Files

	The only two header files your program ever needs to include
	are mysql++.h, and optionally custom.h. (The latter implements
	the SSQLS mechanism.) All of the other files are used within
	the library only.

	By the way, if, when installing this package, you didn't put
	the headers into their own subdirectory, you might consider
	reinstalling the package to remedy that. MySQL++ has a number
	of generically-named files (\link convert.h convert.h, \endlink
	\link fields.h fields.h, \endlink row.h...), so it's best to put
	them into a separate directory where they can't interfere with
	other code on your system. If you're on a Unixy platform, you do
	this by passing the \c --includedir option to the \c configure
	script. See the package's main README file for details.


	\section If You Have Questions...

	If you want to email someone to ask questions about this library,
	we greatly prefer that you send mail to the MySQL++ mailing list,
	which you can subscribe to here: http://lists.mysql.com/plusplus

	That mailing list is archived, so if you have questions, do a
	search to see if the question has been asked before.

	You may find people's individual email addresses in various
	files within the MySQL++ distribution. Please do not send mail
	to them unless you are sending something that is inherently
	personal. Questions that are about MySQL++ usage may well be
	ignored if you send them to our personal email accounts. Those of
	us still active in MySQL++ development monitor the mailing list,
	so you aren't getting any extra "coverage" by sending messages
	to those addresses in addition to the mailing list.
*/

