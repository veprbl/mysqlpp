Summary: MySQL++ is a C++ API for MySQL (and other SQL Databases soon.)
Name: mysql++
Version: 1.7.9
Release: 4.rh9
Copyright: LGPL
Group: Development/Databases
Source0: http://www.mysql.com/Downloads/mysql++/mysql++-1.7.9.tar.gz
Patch0: mysql++-versionfix.patch.gz
Patch1: http://www.mysql.com/Downloads/mysql++/mysql++-gcc-3.0.patch.gz
Patch2: http://www.mysql.com/Downloads/mysql++/mysql++-gcc-3.2.patch.gz
Patch3: http://www.mysql.com/Downloads/mysql++/mysql++-gcc-3.2.2.patch.gz
Patch4: mysql++-prefix.patch.gz
BuildRoot: /var/tmp/%{name}-buildroot

%description
The goal of this API is to make working with queries as easy as working with other STL Containers.

%prep
%setup -n mysql++-1.7.9
%patch0 -p1
%patch1 -p1
%patch2 -p1
%patch3 -p1
cp /usr/share/libtool/ltmain.sh .
aclocal
automake --add-missing
autoconf
./configure --with-prefix=/usr
%patch4 -p1

%build
make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/lib
mkdir -p $RPM_BUILD_ROOT/usr/include
%{__make} install DESTDIR=$RPM_BUILD_ROOT
cd $RPM_BUILD_ROOT/usr/lib
ln -sf libsqlplus.so.1.0.0 libsqlplus.so
ln -sf libsqlplus.so.1.0.0 libsqlplus.so.0
ln -sf libsqlplus.so.1.0.0 libsqlplus.so.1

%clean
rm -rf $RPM_BUILD_ROOT

%post   -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
%doc doc examples LGPL README

/usr/lib/libsqlplus.a
/usr/lib/libsqlplus.la
/usr/lib/libsqlplus.so.1.0.0
/usr/lib/libsqlplus.so
/usr/lib/libsqlplus.so.0
/usr/lib/libsqlplus.so.1
/usr/include/sqlplus

%changelog
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
