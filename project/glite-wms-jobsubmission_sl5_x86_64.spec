Summary: Condor-G connector for the Workload Management System
Name: glite-wms-jobsubmission
Version:
Release:
License: Apache Software License
Vendor: EMI
Packager: CREAM group <cream-support@lists.infn.it>
URL: http://glite.cern.ch/
Group: Applications/Internet
BuildArch: x86_64
#Requires: glite-wms-common
#Requires: glite-jobid-api-c
#Requires: classads
#Requires: glite-jdl-api-cpp
#Requires: glite-lb-client
Requires: glite-wms-configuration
#Requires: condor
#Requires: glite-wms-purger
#Requires: boost
Requires(post): chkconfig
Requires(preun): chkconfig
Requires(preun): initscripts
BuildRequires: chrpath
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
AutoReqProv: yes
Source: %{name}-%{version}-%{release}.tar.gz

%description
Condor-G connector for the Workload Management System

%prep
 

%setup -c

%build
%{!?extbuilddir:%define extbuilddir "--"}
if test "x%{extbuilddir}" == "x--" ; then
  ./configure --prefix=%{buildroot}/usr --sysconfdir=%{buildroot}/etc --disable-static PVER=%{version}
  make
fi


%install
rm -rf %{buildroot}
mkdir -p %{buildroot}
%{!?extbuilddir:%define extbuilddir "--"}
if test "x%{extbuilddir}" == "x--" ; then
  make install
else
  cp -R %{extbuilddir}/* %{buildroot}
fi
sed 's|^prefix=.*|prefix=/usr|g' %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller-adapter.pc > %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller-adapter.pc.new
mv %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller-adapter.pc.new %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller-adapter.pc
sed 's|^prefix=.*|prefix=/usr|g' %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller.pc > %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller.pc.new
mv %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller.pc.new %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller.pc
sed 's|^prefix=.*|prefix=/usr|g' %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller-wrapper.pc > %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller-wrapper.pc.new
mv %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller-wrapper.pc.new %{buildroot}/usr/lib64/pkgconfig/wms-jss-controller-wrapper.pc
sed 's|^prefix=.*|prefix=/usr|g' %{buildroot}/usr/lib64/pkgconfig/wms-jss-logmonitor.pc > %{buildroot}/usr/lib64/pkgconfig/wms-jss-logmonitor.pc.new
mv %{buildroot}/usr/lib64/pkgconfig/wms-jss-logmonitor.pc.new %{buildroot}/usr/lib64/pkgconfig/wms-jss-logmonitor.pc
sed 's|^prefix=.*|prefix=/usr|g' %{buildroot}/usr/lib64/pkgconfig/wms-jss-common.pc > %{buildroot}/usr/lib64/pkgconfig/wms-jss-common.pc.new
mv %{buildroot}/usr/lib64/pkgconfig/wms-jss-common.pc.new %{buildroot}/usr/lib64/pkgconfig/wms-jss-common.pc
rm %{buildroot}/usr/lib64/*.la
chrpath --delete %{buildroot}/usr/lib64/libglite_wms_jss_*.so.0.0.0


%clean
 
%post
/sbin/chkconfig --add glite-wms-lm
/sbin/chkconfig --add glite-wms-jc

%preun
if [ $1 -eq 0 ] ; then
    /sbin/service  glite-wms-lmstop >/dev/null 2>&1
    /sbin/chkconfig --del glite-wms-lm
    /sbin/service glite-wms-jc stop >/dev/null 2>&1
    /sbin/chkconfig --del glite-wms-jc
fi


%files
%defattr(-,root,root)
%dir /etc/rc.d/init.d/
/etc/rc.d/init.d/glite-wms-lm
/etc/rc.d/init.d/glite-wms-jc
/usr/bin/glite-wms-log_monitor
/usr/bin/glite-wms-job_controller
/usr/libexec/glite-wms-lm-job_status
/usr/libexec/glite-wms-clean-lm-recycle.sh

%changelog


%package lib
Summary: Condor-G connector for the WMS (libraries)
Group: System Environment/Libraries

%description lib
Condor-G connector for the WMS (libraries)

%post lib -p /sbin/ldconfig

%postun lib -p /sbin/ldconfig

%files lib
%defattr(-,root,root)
/usr/lib64/libglite_wms_jss_*.so.0.0.0
/usr/lib64/libglite_wms_jss_*.so.0
%dir /usr/share/doc/glite-wms-jobsubmission-%{version}/
%doc /usr/share/doc/glite-wms-jobsubmission-%{version}/LICENSE


%package devel
Summary: Condor-G connector for the WMS (Development files)
Group: System Environment/Libraries

%description devel
Condor-G connector for the WMS (Development files)

%files devel
%defattr(-,root,root)
/usr/lib64/pkgconfig/wms-jss-controller-adapter.pc
/usr/lib64/pkgconfig/wms-jss-controller.pc
/usr/lib64/pkgconfig/wms-jss-controller-wrapper.pc
/usr/lib64/pkgconfig/wms-jss-logmonitor.pc
/usr/lib64/pkgconfig/wms-jss-common.pc
/usr/lib64/libglite_wms_jss_*.so
%dir /usr/include/glite/
%dir /usr/include/glite/wms/
%dir /usr/include/glite/wms/jobsubmission/
/usr/include/glite/wms/jobsubmission/SubmitAdapter.h

