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

	Currently, there is no tutorial or other new user documentation.
	(There is an old manual in the doc/old directory, but it's
	badly outdated.)

	Until we can make some good user documentation, the best way
	to get started is to read through the example programs. Start
	with the simple1 program, and progress from there. The custom*
	programs are of particular interest. See the examples/README
	file for further details.


	\section classes Major Classes

	In MySQL++, the main user-facing classes are mysqlpp::Connection,
	mysqlpp::Query, mysqlpp::Result, and mysqlpp::Row.

	In addition, MySQL++ has a mechanism called Specialized SQL
	Structures (SSQLS), which allow you to create C++ structures
	that parallel the definition of the tables in your database
	schema. These let you manipulate the data in your database
	using native C++ data structures. Many programs using this
	feature never directly use SQL, because MySQL++ generates it
	all automatically. See the custom.* programs in the examples
	directory to see how to use this feature. It is not necessary to
	use MySQL++ this way, but it sure makes some things a lot easier.


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


	\section history A Brief History of MySQL++

	MySQL++ was created in 1998 by Kevin Atkinson. It started out
	MySQL-specific, but there were early efforts to try and make
	it database-independent, and call it SQL++. This is where the
	old library name "sqlplus" came from. This is also why the old
	versions prefixed some class names with "Mysql" but not others:
	the others were supposed to be the database-independent parts.

	Then in 1999, Sinisa Milivojevic unofficially took over maintenance
	of the library, releasing versions 1.0 and 1.1. (All of Kevin's
	releases were pre-1.0 point releases.) Kevin gave over maintenance
	to Sinisa officially with 1.2, and Sinisa went on to maintain
	the library through 1.7.9, released in mid-2001. Since Sinisa
	is an employee of <a href="http://mysql.com/">MySQL AB</a>, it
	seems to be during this time that the dream of multiple-database
	compatibility died.

	With version 1.7.9, MySQL++ went into a period of stasis, lasting
	over three years. During this time, Sinisa ran the MySQL++ mailing
	list and supported its users, but made no new releases. There were
	many patches submitted during this period, some of which were
	ignored, others which were just put on the MySQL++ web site for
	people to try. A lot of these patches were mutually-incompatible,
	and not all of them gave a fully-functional copy of MySQL++. Most
	of them centered on GCC compatibility, because GCC 3 was created
	around this same time. The continual tightening of the rules
	starting with GCC 3.0 exposed a lot of places in MySQL++ that
	used deprecated and non-conforming facilities.

	In early August of 2004, the current maintainer (Warren Young)
	got fed up with this situation and took over. He released 1.7.10
	later that month.

	This Doxygen-based reference manual replaces section 5 from the
	old v1.7.9 manual. The remaining sections of that manual are in
	the new user manual, in ../doc/userman.


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

