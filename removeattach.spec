Name:           removeattach
Version:        1.1
Release:        1%{?dist}
Summary:        Removes attachments from MIME messages

Group:          Applications/System
License:        N/A
URL:            N/A
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  glib2-devel >= 2.12
BuildRequires:  gmime-devel >= 2.4
BuildRequires:  zlib-devel >= 1.2.1.1
BuildRequires:  pkgconfig
Requires:       glib2 >= 2.12
Requires:       gmime >= 2.4
Requires:       zlib >= 1.2.1.1

%description
removeattach provides a command line tool for stripping attachments from
MIME encoded e-mail messages.  The author originally used this tool to
remove attachments that Google gmail found to be illegal.


%prep
%setup -q


%build
gcc `pkg-config --cflags --libs glib-2.0` `pkg-config --cflags --libs gmime-2.4` removeattach.c -o removeattach


%install
rm -rf $RPM_BUILD_ROOT
install -m 755 -d %{buildroot}%{_bindir}/
install -m 755 removeattach %{buildroot}%{_bindir}/
#make install DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%doc README
%{_bindir}/*


%changelog
* Fri Jul 30 2010 Bill Hager <hager@e-techservices.com> - 1.1-1
- Now reading stdin myself instead of passing to gmime due to problems with procmail

* Fri Jul 30 2010 Bill Hager <hager@e-techservices.com> - 1.0-1
- Initial RPM release
