include $(top_srcdir)/common.am

EXTRA_DIST = custom.pl *.bpf *.bpr *.bpg

lib_LTLIBRARIES=libmysqlpp.la

# Interface version, bug fix revision, and 'age' -- number of previous
# interfaces supported.  Bump the last only if explicit
# backwards-compatibility is added -- like two different versions of a
# single function exist.  'age' must be less than the current interface
# version.
libmysqlpp_la_LDFLAGS=-version-info 1:0:0

libmysqlpp_la_SOURCES = coldata.cc connection.cc datetime.cc field_names.cc \
	field_types.cc manip.cc query.cc result.cc  set.cc sql_query.cc \
	type_info.cc vallist.cc string_util.cc

include_HEADERS = \
	coldata.h \
	compare.h \
	connection.h \
	const_string.h \
	convert.h \
	custom.hh \
	custom-macros.hh \
	defs.hh \
	define_short.hh \
	datetime.h \
	exceptions.hh \
	field_names.h \
	field_types.h \
	fields1.hh fields2.hh \
	manip1.hh manip2.hh manip3.hh \
	mysql++.h mysql++.hh \
	null1.hh null2.hh null3.hh \
	platform.h \
	query.h \
	resiter1.hh \
	result1.hh result2.hh result3.hh \
	row1.hh row2.hh row3.hh \
	set1.hh set2.hh set3.hh \
	sqlplus.hh \
	sql_query0.hh sql_query1.hh sql_query2.hh sql_query3.hh \
	sql_string1.hh sql_string2.hh sql_string3.hh \
	stream2string1.hh stream2string2.hh stream2string3.hh \
	tiny_int1.hh tiny_int2.hh tiny_int3.hh \
	type_info1.hh type_info2.hh type_info3.hh \
	string_util.hh \
	vallist1.hh vallist2.hh vallist3.hh

all-local: custom.hh custom-macros.hh

custom.hh custom-macros.hh: custom.pl
	perl custom.pl

distclean-local:
	rm -f custom.hh custom-macros.hh
