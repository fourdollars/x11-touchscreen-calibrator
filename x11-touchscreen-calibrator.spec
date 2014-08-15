Name:           x11-touchscreen-calibrator
Version:        0.2
Release:        1%{?dist}
Summary:        X Window System's Touchscreen Calibrator
Group:          Applications/System
License:        GPLv3+
URL:            http://fourdollars.github.io/x11-touchscreen-calibrator/
Source0:        https://github.com/fourdollars/%{name}/releases/download/%{version}/%{name}-%{version}.tar.xz
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
%doc COPYING README AUTHORS ChangeLog NEWS
%{_sysconfdir}/X11/xinit/xinitrc.d/75x11-touchscreen-calibrator
%{_bindir}/%{name}
%{_mandir}/man1/%{name}.1.*


%changelog
* Fri Aug 15 2014 Wei-Lun Chao <bluebat@member.fsf.org> - 0.2-1
- Update to 0.2

* Tue Mar 11 2014 Wei-Lun Chao <bluebat@member.fsf.org> - 0.1-1
- Initial package
