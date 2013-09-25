Summary:	PPD and filter files for Index Braille's Printers, supplied by Index Braille.
Name:		indexbraille-cups-driver
Version:	20130925
Release:	1lsb3.2
License:	TBD
Group:		Applications/System
URL:		http://www.indexbraille.com

%define drivername indexbrailledriver
%define driverstr Index Braille Driver

%define supplierstr Index Braille
#define supplier indexbraille

%define extraversion %nil

%define filternames indexbraille.sh

BuildRequires:	lsb-build-cc, lsb-build-c++, lsb-appchk
BuildRequires:	perl, gzip
Requires:	lsb >= 3.2
BuildRoot:	%_tmppath/%name-%version-%release-root
BuildArch:	noarch

%install_into_opt

%description
TODO: Meaningful description.
...


%build
# Nothing to build (ppds and shellscripts).

%install

%adjust_ppds

%pre

%create_opt_dirs

%post

%set_ppd_links
%set_cups_links
%update_ppds_fast

%restart_cups

%preun
%not_on_rpm_update

:
%end_not_on_rpm_update

%postun
%not_on_rpm_update

%remove_opt_paths
%remove_ppd_links
%remove_cups_links

%restart_cups

%end_not_on_rpm_update

%clean
rm -rf %{buildroot}

%changelog
* Wed Sep 25 2013 IndexBrailleDriver <support@indexbraille.com> 20130925-1lsb3.2
- Initial version of Linux driver for Index Braille printers.