# vim: set noexpandtab:

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(MYSQL++)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
FIND_PACKAGE(MySQL)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR} # for config.h
	${CMAKE_CURRENT_SOURCE_DIR}/lib
	${CMAKE_CURRENT_BINARY_DIR} # for querydef.h
	${MYSQL_INCLUDE_DIR})

SET(MYSQLPP_VERSION_MAJOR 3)
SET(MYSQLPP_VERSION_MINOR 2)
SET(MYSQLPP_VERSION_BUGFIX 1)
CONFIGURE_FILE(
	lib/mysql++.h.in
	${CMAKE_CURRENT_BINARY_DIR}/mysql++.h
	)

ADD_CUSTOM_COMMAND(
	OUTPUT querydef.h
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/querydef.pl
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)

ADD_CUSTOM_COMMAND(
	OUTPUT ssqls.h
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/ssqls.pl
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)

ADD_LIBRARY(
	mysql++
	SHARED
	lib/beemutex.cpp
	lib/cmdline.cpp
	lib/connection.cpp
	lib/cpool.cpp
	lib/datetime.cpp
	lib/dbdriver.cpp
	lib/field_names.cpp
	lib/field_types.cpp
	lib/manip.cpp
	lib/myset.cpp
	lib/mysql++.cpp
	lib/mystring.cpp
	lib/null.cpp
	lib/options.cpp
	lib/qparms.cpp
	lib/query.cpp
	lib/result.cpp
	lib/row.cpp
	lib/scopedconnection.cpp
	lib/sql_buffer.cpp
	lib/sqlstream.cpp
	lib/ssqls2.cpp
	lib/stadapter.cpp
	lib/tcp_connection.cpp
	lib/transaction.cpp
	lib/type_info.cpp
	lib/uds_connection.cpp
	lib/utility.cpp
	lib/vallist.cpp
	lib/wnp_connection.cpp
	querydef.h
	ssqls.h
	)

IF(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
ELSE()
	SET(MYSQL++_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR};${CMAKE_CURRENT_SOURCE_DIR}/lib;${MYSQL_INCLUDE_DIR}" PARENT_SCOPE)
	SET(MYSQL++_LIBRARIES "mysql++;${MYSQL_LIBRARY}" PARENT_SCOPE)
ENDIF()
