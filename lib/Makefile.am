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
	custom.h custom-macros.h \
	defs.hh \
	define_short.hh \
	datetime.h \
	exceptions.hh \
	field_names.h \
	field_types.h \
	fields1.hh fields2.hh \
	manip.h \
	mysql++.h mysql++.hh \
	null.h \
	platform.h \
	query.h \
	resiter.h \
	result1.hh result2.hh result3.hh \
	row1.hh row2.hh row3.hh \
	set1.hh set2.hh set3.hh \
	sqlplus.hh \
	sql_query1.hh sql_query2.hh sql_query3.hh \
	sql_string.h \
	stream2string1.hh stream2string2.hh stream2string3.hh \
	tiny_int.h \
	type_info1.hh type_info2.hh type_info3.hh \
	string_util.hh \
	vallist1.hh vallist2.hh vallist3.hh

all-local: custom.h custom-macros.h

custom.h custom-macros.h: custom.pl
	./custom.pl

distclean-local:
	rm -f custom.h custom-macros.h
