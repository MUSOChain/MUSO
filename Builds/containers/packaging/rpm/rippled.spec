%define MUSO_version %(echo $MUSO_RPM_VERSION)
%define rpm_release %(echo $RPM_RELEASE)
%define rpm_patch %(echo $RPM_PATCH)
%define _prefix /opt/MUSO
Name:           MUSO
# Dashes in Version extensions must be converted to underscores
Version:        %{MUSO_version}
Release:        %{rpm_release}%{?dist}%{rpm_patch}
Summary:        MUSO daemon

License:        MIT
URL:            http://MUSO.com/
Source0:        MUSO.tar.gz

BuildRequires:  cmake zlib-static ninja-build

%description
MUSO

%package devel
Summary: Files for development of applications using MUSOl core library
Group: Development/Libraries
Requires: openssl-static, zlib-static

%description devel
core library for development of standalone applications that sign transactions.

%prep
%setup -c -n MUSO

%build
cd MUSO
mkdir -p bld.release
cd bld.release
cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_BUILD_TYPE=Release -DCMAKE_UNITY_BUILD_BATCH_SIZE=10 -Dstatic=true -DCMAKE_VERBOSE_MAKEFILE=ON -Dvalidator_keys=ON
cmake --build . --parallel --target MUSO --target validator-keys -- -v

%pre
test -e /etc/pki/tls || { mkdir -p /etc/pki; ln -s /usr/lib/ssl /etc/pki/tls; }

%install
rm -rf $RPM_BUILD_ROOT
DESTDIR=$RPM_BUILD_ROOT cmake --build MUSO/bld.release --target install -- -v
rm -rf ${RPM_BUILD_ROOT}/%{_prefix}/lib64/cmake/date
install -d ${RPM_BUILD_ROOT}/etc/opt/MUSO
install -d ${RPM_BUILD_ROOT}/usr/local/bin
ln -s %{_prefix}/etc/MUSO.cfg ${RPM_BUILD_ROOT}/etc/opt/MUSO/MUSO.cfg
ln -s %{_prefix}/etc/validators.txt ${RPM_BUILD_ROOT}/etc/opt/MUSO/validators.txt
ln -s %{_prefix}/bin/MUSO ${RPM_BUILD_ROOT}/usr/local/bin/MUSO
install -D MUSO/bld.release/validator-keys/validator-keys ${RPM_BUILD_ROOT}%{_bindir}/validator-keys
install -D ./MUSO/Builds/containers/shared/MUSO.service ${RPM_BUILD_ROOT}/usr/lib/systemd/system/MUSO.service
install -D ./MUSO/Builds/containers/packaging/rpm/50-MUSO.preset ${RPM_BUILD_ROOT}/usr/lib/systemd/system-preset/50-MUSO.preset
install -D ./MUSO/Builds/containers/shared/update-MUSO.sh ${RPM_BUILD_ROOT}%{_bindir}/update-MUSO.sh
install -D ./MUSO/bin/getMUSOInfo ${RPM_BUILD_ROOT}%{_bindir}/getMUSOInfo
install -D ./MUSO/Builds/containers/shared/update-MUSO-cron ${RPM_BUILD_ROOT}%{_prefix}/etc/update-MUSO-cron
install -D ./MUSO/Builds/containers/shared/MUSO-logrotate ${RPM_BUILD_ROOT}/etc/logrotate.d/MUSO
install -d $RPM_BUILD_ROOT/var/log/MUSO
install -d $RPM_BUILD_ROOT/var/lib/MUSO

%post
USER_NAME=MUSO
GROUP_NAME=MUSO

getent passwd $USER_NAME &>/dev/null || useradd $USER_NAME
getent group $GROUP_NAME &>/dev/null || groupadd $GROUP_NAME

chown -R $USER_NAME:$GROUP_NAME /var/log/MUSO/
chown -R $USER_NAME:$GROUP_NAME /var/lib/MUSO/
chown -R $USER_NAME:$GROUP_NAME %{_prefix}/

chmod 755 /var/log/MUSO/
chmod 755 /var/lib/MUSO/

chmod 644 %{_prefix}/etc/update-MUSO-cron
chmod 644 /etc/logrotate.d/MUSO
chown -R root:$GROUP_NAME %{_prefix}/etc/update-MUSO-cron

%files
%doc MUSO/README.md MUSO/LICENSE
%{_bindir}/MUSO
/usr/local/bin/MUSO
%{_bindir}/update-MUSO.sh
%{_bindir}/getMUSOInfo
%{_prefix}/etc/update-MUSO-cron
%{_bindir}/validator-keys
%config(noreplace) %{_prefix}/etc/MUSO.cfg
%config(noreplace) /etc/opt/MUSO/MUSO.cfg
%config(noreplace) %{_prefix}/etc/validators.txt
%config(noreplace) /etc/opt/MUSO/validators.txt
%config(noreplace) /etc/logrotate.d/MUSO
%config(noreplace) /usr/lib/systemd/system/MUSO.service
%config(noreplace) /usr/lib/systemd/system-preset/50-MUSO.preset
%dir /var/log/MUSO/
%dir /var/lib/MUSO/

%files devel
%{_prefix}/include
%{_prefix}/lib/*.a
%{_prefix}/lib/cmake/MUSO

%changelog
* Wed Aug 28 2019 Mike Ellery <mellery451@gmail.com>
- Switch to subproject build for validator-keys

* Wed May 15 2019 Mike Ellery <mellery451@gmail.com>
- Make validator-keys use local MUSO build for core lib

* Wed Aug 01 2018 Mike Ellery <mellery451@gmail.com>
- add devel package for signing library

* Thu Jun 02 2016 Brandon Wilson <bwilson@MUSO.com>
- Install validators.txt

