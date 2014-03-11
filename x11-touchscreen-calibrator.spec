Name:           x11-touchscreen-calibrator
Version:        0.1
Release:        1%{?dist}
Summary:        X Window System's Touchscreen Calibrator
Group:          Applications/System
License:        GPLv3+
URL:            http://fourdollars.github.io/x11-touchscreen-calibrator/
#https://github.com/fourdollars/%{name}/archive/%{version}.tar.gz
Source0:        %{name}-%{version}.tar.gz
BuildRequires:  libXi-devel
BuildRequires:  libX11-devel
BuildRequires:  libXrandr-devel
Requires:       xorg-x11-xinit

%description
X11 Touchscreen Calibrator runs as a daemon in the background.
It will detect the touchscreen automatically and adjust the corresponding
Coordinate Transformation Matrix of Touchscreen xinput when the resolution
is changed.

It should also support rotation, reflection and different scaling modes of
display output.

%prep
%setup -q

%build
autoreconf -if
%configure
make %{?_smp_mflags}


%install
%make_install xsessiondir=%{_sysconfdir}/X11/xinit/xinitrc.d/


%files
%doc LICENSE README.md
%{_sysconfdir}/X11/xinit/xinitrc.d/75x11-touchscreen-calibrator
%{_bindir}/%{name}
%{_mandir}/man1/%{name}.1.*


%changelog
* Tue Mar 11 2014 Wei-Lun Chao <bluebat@member.fsf.org> - 0.1-1
- Initial package
