Summary: C++ API for MySQL
Name: mysql++
Version: 1.7.11
Release: 2
Copyright: LGPL
Group: Development/Databases
Source: http://tangentsoft.net/mysql++/releases/mysql++-1.7.11.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot

%description
The goal of this API is to make working with queries as easy as working
with other STL Containers.

%prep
%setup -q
./configure --prefix=/usr --includedir=/usr/include/mysql++

%build
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/lib
mkdir -p $RPM_BUILD_ROOT/usr/include
%{__make} install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%post   -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
%doc doc/*.ps doc/man-text/*.txt LGPL README

/usr/include/mysql++
/usr/lib/libmysqlpp.a
/usr/lib/libmysqlpp.la
/usr/lib/libmysqlpp.so
/usr/lib/libmysqlpp.so.1.0.0

%changelog
* Tue Aug 18 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.10-2
- Removed examples from documentation.
- Limited documentation to just the generated files, not the sources.

* Tue Aug 17 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.10-1
- Updated for v1.7.10

* Wed Apr 16 2003 Tuan Hoang <tqhoang@bigfoot.com> 1.7.9-4
- Added gcc 3.2.2 patch.
- Packaged using Red Hat Linux 8.0 and 9.

* Thu Nov 14 2002 Tuan Hoang <tqhoang@bigfoot.com> 1.7.9-3
- Changed the version passed to libtool.

* Mon Oct 28 2002 Tuan Hoang <tqhoang@bigfoot.com> 1.7.9-2
- Updated the version numbering of the library to be 1.7.9.
- Packaged using Red Hat Linux 8.0.

* Thu Oct 17 2002 Philipp Berndt <philipp.berndt@gmx.net>
- packaged
