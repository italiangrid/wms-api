/*
	Copyright (c) Members of the EGEE Collaboration. 2004.
	See http://public.eu-egee.org/partners/ for details on the copyright holders.
	For license conditions see the license file or http://www.eu-egee.org/license.html
*/
//
// File: wmpgsoapfaultmanipulator.cpp
// Author: Giuseppe Avellino <giuseppe.avellino@datamat.it>
//

// Boost
#include <boost/lexical_cast.hpp>

// gSOAP
#include "soapH.h"

#include "utilities/wmpexception_codes.h"

//Logger
#include "glite/wms/common/logger/edglog.h"
#include "glite/wms/common/logger/logger_utils.h"
#include "utilities/logging.h"

namespace logger       = glite::wms::common::logger;
namespace wmputilities = glite::wms::wmproxy::utilities; //Exception codes


// To remove
vector<string> *
convertStackVector(vector<string> stack)
{
	vector<string> *returnVector = new vector<string>(0);
	if (&stack) {
		string element;
		for (unsigned int i = 0; i < stack.size(); i++) {
			element = &(stack[i][0]);
			returnVector->push_back(element);
		}
	}
	return returnVector;
}

int
getServiceFaultType(int code)
{
	switch (code) {
		case wmputilities::WMS_AUTHENTICATION_ERROR: // AuthenticationFault
			return SOAP_TYPE_ns1__AuthenticationFaultType;
			break;

		case wmputilities::WMS_NOT_AUTHORIZED_USER: // AuthorizationFault
		case wmputilities::WMS_PROXY_ERROR:
		case wmputilities::WMS_DELEGATION_ERROR:
			return SOAP_TYPE_ns1__AuthorizationFaultType;
			break;

		case wmputilities::WMS_JDL_PARSING: // InvalidArgumentFault
		case wmputilities::WMS_INVALID_ARGUMENT:
			return SOAP_TYPE_ns1__InvalidArgumentFaultType;
			break;

		case wmputilities::WMS_NOT_ENOUGH_QUOTA: // GetQuotaManagementFault
		case wmputilities::WMS_NOT_ENOUGH_SPACE: // ?
			return SOAP_TYPE_ns1__GetQuotaManagementFaultType;
			break;

		case wmputilities::WMS_NO_SUITABLE_RESOURCE: // NoSuitableResourcesFault
		case wmputilities::WMS_MATCHMAKING: // ?
			return SOAP_TYPE_ns1__NoSuitableResourcesFaultType;
			break;

		case wmputilities::WMS_JOB_NOT_FOUND: // JobUnknownFault
			return SOAP_TYPE_ns1__JobUnknownFaultType;
			break;

		case wmputilities::WMS_JOB_NOT_DONE: // OperationNotAllowedFault
		case wmputilities::WMS_OPERATION_NOT_ALLOWED: // Generated inside wmproxy code
			return SOAP_TYPE_ns1__OperationNotAllowedFaultType;
			break;

		case wmputilities::WMS_FATAL:
		case wmputilities::WMS_SANDBOX_IO:
		case wmputilities::WMS_WRONG_COMMAND:
		case wmputilities::WMS_JOB_SIZE:
		case wmputilities::WMS_IS_FAILURE:
		case wmputilities::WMS_MULTI_ATTRIBUTE_FAILURE:
		case wmputilities::WMS_LOGGING_ERROR:
			return SOAP_TYPE_ns1__GenericFaultType;
			break;

		default:
			return SOAP_TYPE_ns1__GenericFaultType;
			break;

		// WMS_CONNECTION_ERROR -> soap server
	}
}

void *
initializeStackPointer(int code)
{
	void *sp = NULL;
	switch(getServiceFaultType(code)) {
		case SOAP_TYPE_ns1__AuthenticationFaultType:
			sp = new ns1__AuthenticationFaultType;
			break;
		case SOAP_TYPE_ns1__AuthorizationFaultType:
			sp = new ns1__AuthorizationFaultType;
			break;
		case SOAP_TYPE_ns1__InvalidArgumentFaultType:
			sp = new ns1__InvalidArgumentFaultType;
			break;
		case SOAP_TYPE_ns1__GetQuotaManagementFaultType:
			sp = new ns1__GetQuotaManagementFaultType;
			break;
		case SOAP_TYPE_ns1__NoSuitableResourcesFaultType:
			sp = new ns1__NoSuitableResourcesFaultType;
			break;
		case SOAP_TYPE_ns1__GenericFaultType:
			sp = new ns1__GenericFaultType;
			break;
		case SOAP_TYPE_ns1__JobUnknownFaultType:
			sp = new ns1__JobUnknownFaultType;
			break;
		case SOAP_TYPE_ns1__OperationNotAllowedFaultType:
			sp = new ns1__OperationNotAllowedFaultType;
			break;
		default:
			sp = new ns1__GenericFaultType;
			break;
	}
	return sp;
}

void
setFaultDetails(struct soap *soap, int type, void *sp)
{
	if (soap->version == 2) {
		soap->fault->SOAP_ENV__Detail = (struct SOAP_ENV__Detail*)
			soap_malloc(soap, sizeof(struct SOAP_ENV__Detail));
		soap->fault->SOAP_ENV__Detail->__type = type; // stack type
		soap->fault->SOAP_ENV__Detail->fault = sp; // point to stack
		soap->fault->SOAP_ENV__Detail->__any = NULL; // no other XML data
	} else {
		soap->fault->detail = (struct SOAP_ENV__Detail*)
			soap_malloc(soap, sizeof(struct SOAP_ENV__Detail));
		soap->fault->detail->__type = type; // stack type
		soap->fault->detail->fault = sp; // point to stack
		soap->fault->detail->__any = NULL; // no other XML data
	}
}

void
setSOAPFault(struct soap *soap, int code, const string &method_name, time_t time_stamp,
	int error_code, const string &description, vector<string> stack)
{
	// Logging fault
	edglog(debug)
		<<"------------------------------- Fault Description "
			"--------------------------------"
		<<endl;
	edglog(debug)<<"Method: "<<method_name<<endl;
	edglog(debug)<<"Code: "<<boost::lexical_cast<std::string>(code)<<endl;
	edglog(debug)<<"Description: "<<description<<endl;
	edglog(debug)<<"Stack: "<<endl;
	for (unsigned int i = 0; i < stack.size(); i++) {
		edglog(debug)<<stack[i]<<endl;
	}
	edglog(debug)
			<<"----------------------------------------"
				"------------------------------------------"
			<<endl;
	
	// Generating a fault
	ns1__BaseFaultType *sp = (ns1__BaseFaultType*)initializeStackPointer(code);

	// Filling fault fields
	sp->methodName = method_name;
	sp->Timestamp = time_stamp;
	sp->ErrorCode = new string(boost::lexical_cast<std::string>(error_code));
	sp->Description = new string(description);
	sp->FaultCause = convertStackVector(stack);
	
	// Sending fault
	soap_receiver_fault(soap, "Stack dump", NULL);
	setFaultDetails(soap, getServiceFaultType(code), sp);
}

void
setSOAPFault(struct soap *soap, int code, const string &method_name, time_t time_stamp,
	int error_code, const string &description)
{
	setSOAPFault(soap, code, method_name, time_stamp, error_code, description,
		*(new vector<string>));
}
