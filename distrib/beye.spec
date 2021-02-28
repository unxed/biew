%define name	beye
%define version	6.1.0
%define versrc	610
%define release	1

%define prefix	/usr
%define bindir	%{prefix}/bin
%define datadir	%{prefix}/share
%define mandir	%{datadir}/man

Name:		%{name}
Version:	%{version}
Release:	%{release}
Prefix:		%{prefix}
Summary:	console hex viewer/editor and disassembler
Copyright:	GPL
Group:		Development/Other
Packager:	Nickols_k <nickols_k@mail.ru>
URL:		http://beye.sourceforge.net

Source:		%{name}-%{versrc}-src.tar.bz2

%description
BEYE (Binary EYE) is a free, portable, advanced file viewer with
built-in editor for binary, hexadecimal and disassembler modes.

It contains a highlight i86/i386/Athlon64/Java/AVR/ARM-XScale/PPC disassembler,
full preview of MZ, NE, PE, LE, LX, DOS.SYS, NLM, ELF, a.out, arch,
coff32, PharLap, rdoff executable formats, a code guider, and lot of
other features, making it invaluable for examining binary code.

Linux, Unix, QNX, BeOS, DOS, Win32, OS/2 versions are available.

%prep
%setup -q

%build
./configure --prefix=%{prefix}
make
make install

%install
rm -rf $RPM_BUILD_ROOT
install -d ${RPM_BUILD_ROOT}{%{bindir},%{datadir}/%{name},%{mandir}/man1}

install -m 755 beye $RPM_BUILD_ROOT%{bindir}/%{name}
strip -R .note -R .comment $RPM_BUILD_ROOT%{bindir}/%{name}

cp -a bin_rc/{xlt,skn,*.hlp} $RPM_BUILD_ROOT%{datadir}/%{name}
install doc/beye.1 $RPM_BUILD_ROOT%{mandir}/man1

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc doc/beye_en.txt doc/beye_ru.txt doc/unix.txt doc/release.txt

%{bindir}/%{name}
%{datadir}/%{name}/*
%{mandir}/man?/%{name}.1*

%changelog
* Sun Jan 6 2002 konst <konst@linuxassembly.org> 5.3.2-1
- build from the original source archive
* Fri Jan 4 2002 konst <konst@linuxassembly.org> 5.3.1-2
- explicitly define TARGET_CPU, TARGET_OS, USE_MOUSE, PREFIX
* Wed Aug 15 2001 konst <konst@linuxassembly.org> 5.3.0-1
- moved skins to skn folder
