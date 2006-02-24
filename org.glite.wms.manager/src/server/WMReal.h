// File: WMReal.h
// Author: Francesco Giacomini <Francesco.Giacomini@cnaf.infn.it>
// Copyright (c) 2002 EU DataGrid.
// For license conditions see http://www.eu-datagrid.org/license.html

// $Id$

#ifndef GLITE_WMS_MANAGER_SERVER_WMREAL_H
#define GLITE_WMS_MANAGER_SERVER_WMREAL_H

#include <boost/shared_ptr.hpp>

namespace classad {
class ClassAd;
}

namespace glite {

namespace wmsutils {
namespace jobid {
class JobId;
}}

namespace wms {
namespace manager {
namespace server {
	
class WMReal
{
  class Impl;
  boost::shared_ptr<Impl> m_impl;

public:
  WMReal();
  void submit(classad::ClassAd const* request_ad);
  void cancel(glite::wmsutils::jobid::JobId const& request_id);
};

}}}} // glite::wms::manager::server

#endif
