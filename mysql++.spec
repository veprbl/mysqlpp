Summary: C++ API for MySQL
Name: mysql++
Version: 1.7.20
Release: 1
Copyright: LGPL
Group: Development/Databases
Source: http://tangentsoft.net/mysql++/releases/mysql++-%{version}.tar.gz
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
/usr/lib/libmysqlpp.so.2.0.0

%changelog
* Wed Nov 03 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.20-1
- Updated for 1.7.20

* Mon Oct 25 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.19-1
- Updated for 1.7.19

* Fri Oct 1 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.18-1
- Updated for 1.7.18

* Thu Sep 16 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.17-1
- Updated for 1.7.17

* Thu Sep 13 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.16-1
- Updated for 1.7.16

* Thu Sep 2 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.15-1
- Updated for 1.7.15

* Thu Aug 26 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.14-1
- Updated for 1.7.14

* Mon Aug 23 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.13-1
- Updated for 1.7.13

* Thu Aug 19 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.12-1
- Updated for 1.7.12

* Wed Aug 18 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.11-1
- Updated for 1.7.11
- Removed examples from documentation.
- Limited documentation to just the generated files, not the sources.

* Tue Aug 17 2004 Warren Young <mysqlpp@etr-usa.com> 1.7.10-1
- Updated for 1.7.10

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
