
#include "options_utils.h"
#include <stdio.h>
// i/o streams
#include <iostream>
#include <fstream>

namespace glite {
namespace wms{
namespace client {
namespace utilities {

using namespace std;
/*
 * Help messages
*/
const char* Options::HELP_COPYRIGHT = "Copyright (C) 2003 by DATAMAT SpA";
const char* Options::HELP_EMAIL = "fabrizio.pacini@datamat.it";
const char* Options::HELP_UI = "WMS User Interface" ;
const char* Options::HELP_VERSION = "version  1.0.2" ;

/*
*	LONG OPTION STRINGS
*/
const char* Options::LONG_ALL 		= "all";
const char* Options::LONG_CHKPT	= "chkpt";
const char* Options::LONG_CONFIGVO	= "config-vo";
const char* Options::LONG_DEBUG	= "debug";
const char* Options::LONG_DIR	= "dir";
const char* Options::LONG_FROM		= "from";
const char* Options::LONG_HELP 		= "help";
const char* Options::LONG_LMRS		= "lmrs";
const char* Options::LONG_LOGFILE	= "logfile";
const char* Options::LONG_NOGUI		= "nogui";
const char* Options::LONG_NOINT	= "noint";
const char* Options::LONG_NOLISTEN	= "nolisten";
const char* Options::LONG_NOMSG	= "nomsg";
const char* Options::LONG_RANK 	= "rank";
const char* Options::LONG_TO		= "to";
const char* Options::LONG_USERTAG	= "user-tag";
const char* Options::LONG_VERSION	= "version";
const char* Options::LONG_VO		= "vo";

/*
*	LONG OPTION STRINGS & SHORT CHARs
*/
// ouput
const char* Options::LONG_OUTPUT	= "output";
const char Options::SHORT_OUTPUT	= 'o' ;
// input
const char* Options::LONG_INPUT	= "input" ;
const char Options::SHORT_INPUT	= 'i' ;
// config
const char*Options::LONG_CONFIG	= "config";
const char Options::SHORT_CONFIG	= 'c' ;
// resource
const char* Options::LONG_RESOURCE = "resource";
const char Options::SHORT_RESOURCE = 'r' ;
// validity
const char* Options::LONG_VALID	= "valid" ;
const char Options::SHORT_VALID	= 'v' ;
// verbosity
const char* Options::LONG_VERBOSE 	= "verbose-level";
const char Options::SHORT_VERBOSE = 'l';
// status
const char* Options::LONG_STATUS = "status";
const char Options::SHORT_STATUS = 's' ;
// exclude
const char* Options::LONG_EXCLUDE = "exclude";
const char Options::SHORT_EXCLUDE = 'e';
// port
const char* Options::LONG_PORT		= "port";
const char Options::SHORT_PORT 	= 'p';

// argument chars for short option definition string
// (semicolon or white space)
const char Options::short_required_arg = ':' ;
const char Options::short_no_arg = ' ' ;


/*
*	Long options for the job-submit
*/
const struct option Options::submitLongOpts[] = {
	{	Options::LONG_CONFIGVO,         	required_argument,		0,		Options::CONFIGVO	},
	{	Options::LONG_VO,             		required_argument,		0,		Options::VO	},
	{	Options::LONG_LOGFILE,             	required_argument,		0,		Options::LOGFILE},
	{	Options::LONG_CHKPT,              	required_argument,		0,		Options::CHKPT},
	{	Options::LONG_LMRS,              	required_argument,		0,		Options::LMRS},
	{	Options::LONG_TO,              		required_argument,		0,		Options::TO},
	{	Options::LONG_OUTPUT,             	required_argument,		0,		Options::SHORT_OUTPUT},
	{ 	Options::LONG_INPUT,              	required_argument,		0,		Options::SHORT_INPUT},
	{	Options::LONG_CONFIG,              	required_argument,		0,		Options::SHORT_CONFIG},
	{	Options::LONG_RESOURCE,         	required_argument,		0,		Options::SHORT_RESOURCE},
	{	Options::LONG_VALID,              	required_argument,		0,		Options::SHORT_VALID},
	{	Options::LONG_NOMSG,		no_argument,			0,		Options::NOMSG	},
	{	Options::LONG_NOGUI,			no_argument,			0,		Options::NOGUI	},
	{	Options::LONG_NOLISTEN,		no_argument,			0,		Options::NOLISTEN	},
	{	Options::LONG_NOINT,			no_argument,			0,		Options::NOINT	},
	{	Options::LONG_VERSION,		no_argument,			0,		Options::VERSION	},
	{	Options::LONG_HELP,			no_argument,			0,		Options::HELP	},
	{0, 0, 0, 0}
};
/*
*	Long options for the job-status
*/
const struct option Options::statusLongOpts[] = {
	{	Options::LONG_VERSION,		no_argument,			0,		Options::VERSION	},
	{	Options::LONG_HELP,			no_argument,			0,		Options::HELP	},
	{	Options::LONG_ALL,			no_argument,			0,		Options::ALL	},
	{ 	Options::LONG_INPUT,              	required_argument,		0,		Options::SHORT_INPUT},
	{ 	Options::LONG_VERBOSE,             required_argument,		0,		Options::SHORT_VERBOSE},
	{	Options::LONG_FROM,              	required_argument,		0,		Options::FROM},
	{	Options::LONG_TO,              		required_argument,		0,		Options::TO},
	{	Options::LONG_CONFIG,              	required_argument,		0,		Options::SHORT_CONFIG},
	{	Options::LONG_CONFIGVO,         	required_argument,		0,		Options::CONFIGVO	},
	{	Options::LONG_USERTAG,         	required_argument,		0,		Options::USERTAG	},
	{	Options::LONG_STATUS,         	required_argument,		0,		Options::SHORT_STATUS},
	{	Options::LONG_EXCLUDE,         	required_argument,		0,		Options::SHORT_EXCLUDE},
	{	Options::LONG_VO,             		required_argument,		0,		Options::VO	},
	{	Options::LONG_OUTPUT,             	required_argument,		0,		Options::SHORT_OUTPUT},
	{	Options::LONG_NOINT,			no_argument,			0,		Options::NOINT	},
	{	Options::LONG_DEBUG,			no_argument,			0,		Options::DBG	},
	{	Options::LONG_LOGFILE,             	required_argument,		0,		Options::LOGFILE},
	{0, 0, 0, 0}
};

/*
*	Long options for the job-logging-info
*/
const struct option Options::loginfoLongOpts[] = {
	{	Options::LONG_VERSION,		no_argument,			0,		Options::VERSION	},
	{	Options::LONG_HELP,			no_argument,			0,		Options::HELP	},
	{ 	Options::LONG_VERBOSE,              required_argument,	0,		Options::SHORT_VERBOSE},
	{	Options::LONG_CONFIG,              	required_argument,		0,		Options::SHORT_CONFIG},
	{	Options::LONG_OUTPUT,             	required_argument,		0,		Options::SHORT_OUTPUT},
	{	Options::LONG_NOINT,			no_argument,			0,		Options::NOINT	},
	{	Options::LONG_DEBUG,			no_argument,			0,		Options::DBG	},
	{	Options::LONG_LOGFILE,             	required_argument,		0,		Options::LOGFILE},
	{0, 0, 0, 0}
};

/*
*	Long options for the job-cancel
*/
const struct option Options::cancelLongOpts[] = {
	{	Options::LONG_VERSION,		no_argument,			0,		Options::VERSION	},
	{	Options::LONG_HELP,			no_argument,			0,		Options::HELP	},
	{	Options::LONG_ALL,			no_argument,			0,		Options::ALL	},
	{ 	Options::LONG_INPUT,              	required_argument,		0,		Options::SHORT_INPUT},
	{	Options::LONG_CONFIG,              	required_argument,		0,		Options::SHORT_CONFIG},
	{	Options::LONG_CONFIGVO,         	required_argument,		0,		Options::CONFIGVO	},
	{	Options::LONG_VO,             		required_argument,		0,		Options::VO	},
	{	Options::LONG_OUTPUT,             	required_argument,		0,		Options::SHORT_OUTPUT},
	{	Options::LONG_NOINT,			no_argument,			0,		Options::NOINT	},
	{	Options::LONG_DEBUG,			no_argument,			0,		Options::DBG	},
	{	Options::LONG_LOGFILE,             	required_argument,		0,		Options::LOGFILE},
	{0, 0, 0, 0}
};


/*
*	Long options for the job-list-match
*/
const struct option Options::lsmatchLongOpts[] = {
	{	Options::LONG_VERSION,		no_argument,			0,		Options::VERSION	},
	{	Options::LONG_HELP,			no_argument,			0,		Options::HELP	},
	{ 	Options::LONG_RANK,              	no_argument,			0,		Options::RANK},
	{	Options::LONG_CONFIG,              	required_argument,		0,		Options::SHORT_CONFIG},
	{	Options::LONG_CONFIGVO,         	required_argument,		0,		Options::CONFIGVO	},
	{	Options::LONG_VO,             		required_argument,		0,		Options::VO	},
	{	Options::LONG_OUTPUT,             	required_argument,		0,		Options::SHORT_OUTPUT},
	{	Options::LONG_NOINT,			no_argument,			0,		Options::NOINT	},
	{ 	Options::LONG_DEBUG,              	required_argument,		0,		Options::DBG},
	{	Options::LONG_LOGFILE,             	required_argument,		0,		Options::LOGFILE},
	{0, 0, 0, 0}
};

/*
*	Long options for the job-output
*/
const struct option Options::outputLongOpts[] = {
	{	Options::LONG_VERSION,		no_argument,			0,		Options::VERSION	},
	{	Options::LONG_HELP,			no_argument,			0,		Options::HELP	},
	{ 	Options::LONG_INPUT,              	required_argument,		0,		Options::SHORT_INPUT},
	{ 	Options::LONG_DIR, 	             	required_argument,		0,		Options::DIR},
	{	Options::LONG_CONFIG,              	required_argument,		0,		Options::SHORT_CONFIG},
	{	Options::LONG_NOINT,			no_argument,			0,		Options::NOINT	},
	{ 	Options::LONG_DEBUG,              	required_argument,		0,		Options::DBG},
	{	Options::LONG_LOGFILE,             	required_argument,		0,		Options::LOGFILE},
	{0, 0, 0, 0}
};

/*
*	Long options for the job-attach
*/
const struct option Options::attachLongOpts[] = {
	{	Options::LONG_VERSION,		no_argument,			0,		Options::VERSION	},
	{	Options::LONG_HELP,			no_argument,			0,		Options::HELP	},
	{	Options::LONG_PORT,              	required_argument,		0,		Options::SHORT_PORT},
	{	Options::LONG_NOLISTEN,			no_argument,		0,		Options::NOLISTEN	},
	{	Options::LONG_NOGUI,			no_argument,		0,			Options::NOGUI},
	{	Options::LONG_CONFIG,              	required_argument,		0,		Options::SHORT_CONFIG},
	{ 	Options::LONG_INPUT,              	required_argument,		0,		Options::SHORT_INPUT},
	{	Options::LONG_NOINT,			no_argument,			0,		Options::NOINT	},
	{ 	Options::LONG_DEBUG,              	required_argument,		0,		Options::DBG},
	{	Options::LONG_LOGFILE,             	required_argument,		0,		Options::LOGFILE},
	{0, 0, 0, 0}
};

/*
*	short usage constants
*/
const string Options::USG_ALL = "--" + string(LONG_ALL) ;

const string Options::USG_CHKPT = "--" + string(LONG_CHKPT )	 + "\t<file_path>" ;

const string Options::USG_CONFIG = "--" + string(LONG_CONFIG ) +  ", -" + SHORT_CONFIG  + "\t<file_path>"	;

const string Options::USG_CONFIGVO  = "--" + string(LONG_CONFIGVO) + "\t<file_path>" ;

const string Options::USG_DEBUG  = "--" + string(LONG_DEBUG );

const string Options::USG_DIR  = "--" + string(LONG_DIR )+ "\t<directory_path>"	;

const string Options::USG_EXCLUDE  = "--" + string(LONG_EXCLUDE )+ ", -" + SHORT_EXCLUDE + "\t<status_value>";

const string Options::USG_FROM  = "--" + string(LONG_FROM )+ "\t\t[MM:DD:]hh:mm[:[CC]YY]";

const string Options::USG_HELP = "--" + string(LONG_HELP) ;

const string Options::USG_INPUT = "--" + string(LONG_INPUT )  + ", -" + SHORT_INPUT  + "\t<file_path>";

const string Options::USG_LMRS = "--" + string(LONG_LMRS ) + "\t<lmrs_type>" 	;

const string Options::USG_LOGFILE = "--" + string(LONG_LOGFILE )+ "\t<file_path>" ;

const string Options::USG_NOGUI = "--" + string(LONG_NOGUI);

const string Options::USG_NOINT = "--" + string(LONG_NOINT) ;

const string Options::USG_NOLISTEN  = "--" + string(LONG_NOLISTEN);

const string Options::USG_NOMSG	 = "--" + string(LONG_NOMSG);

const string Options::USG_OUTPUT = "--" + string(LONG_OUTPUT) + ", -" + SHORT_OUTPUT + "\t<file_path>";

const string Options::USG_PORT  = "--" + string(LONG_PORT )+ ", -" + SHORT_PORT + "\t<port_num>";

const string Options::USG_RANK = "--" + string(LONG_RANK ) ;

const string Options::USG_RESOURCE = "--" + string(LONG_RESOURCE ) + ", -" + SHORT_RESOURCE + "\t<ce_id>";

const string Options::USG_STATUS = "--" + string(LONG_STATUS ) + ", -" + SHORT_STATUS + "\t<status_value>";

const string Options::USG_TO = "--" + string(LONG_TO)+ "\t\t[MM:DD:]hh:mm[:[CC]YY]";

const string Options::USG_USERTAG = "--" + string(LONG_USERTAG ) + "\t<tag name>=<tag value>";

const string Options::USG_VALID = "--" + string(LONG_VALID ) ;

const string Options::USG_VERBOSE  = "--" + string(LONG_VERBOSE ) +  ", -" + SHORT_VERBOSE + "\t[0|1|2|3]";

const string Options::USG_VERSION = "--" + string(LONG_VERSION );

const string Options::USG_VO	 = "--" + string(LONG_VO ) + "\t\t<vo_name>";

/*
*	prints the help usage message for the job-submit
*	@param exename the name of the executable
*	@param long_usage if the value is true it prints the long help msg
*/
void Options::submit_usage(const char* &exename, const bool &long_usg){
	cerr << HELP_UI << " " << HELP_VERSION << "\n" ;
	cerr << HELP_COPYRIGHT << "\n\n" ;
	cerr << "Usage: " << exename <<   " [options]  <jdl_file>\n\n";
	cerr << "Options:\n" ;
	cerr << "\t" << USG_HELP << "\n";
	cerr << "\t" << USG_VERSION << "\n\n";
	cerr << "\t" << USG_VO << "\n";
	cerr << "\t" << USG_INPUT << "\n";
	cerr << "\t" << USG_RESOURCE << "\n";
	cerr << "\t" << USG_NOLISTEN << "\n";
	cerr << "\t" << USG_NOGUI << "\n";
	cerr << "\t" << USG_NOMSG << "\n";
	cerr << "\t" << USG_LMRS << "\n";
	cerr << "\t" << USG_TO << "\n";
	cerr << "\t" << USG_VALID << "\n";
	cerr << "\t" << USG_CONFIG << "\n";
	cerr << "\t" << USG_CONFIGVO << "\n";
	cerr << "\t" << USG_OUTPUT << "\n";
	cerr << "\t" << USG_CHKPT << "\n";
	cerr << "\t" << USG_NOINT << "\n";
	cerr << "\t" << USG_DEBUG << "\n";
	cerr << "\t" << USG_LOGFILE << "\n\n";
	cerr << "Please report any bug at:\n" ;
	cerr << "\t" << HELP_EMAIL << "\n";
	if (long_usg){
		cerr  << exename << " full help\n\n" ;
	}
};
/*
*	prints the help usage message for the job-status
*	@param exename the name of the executable
*	@param long_usage if the value is true it prints the long help msg
*/
void Options::status_usage(const char* &exename, const bool &long_usg){
	cerr << HELP_UI << " " << HELP_VERSION << "\n" ;
	cerr << HELP_COPYRIGHT << "\n\n" ;
	cerr << "Usage: " << exename <<   " [options]   <job Id(s)>\n\n";
	cerr << "Options:\n" ;
	cerr << "\t" << USG_HELP << "\n";
	cerr << "\t" << USG_VERSION << "\n\n";
	cerr << "\t" << USG_ALL << "\n";
	cerr << "\t" << USG_VERBOSE << "\n";
	cerr << "\t" << USG_FROM << "\n";
	cerr << "\t" << USG_TO << "\n";
	cerr << "\t" << USG_CONFIG << "\n";
	cerr << "\t" << USG_CONFIGVO << "\n";
	cerr << "\t" << USG_USERTAG<< "\n";
	cerr << "\t" << USG_STATUS << "\n";
	cerr << "\t" << USG_EXCLUDE << "\n";
	cerr << "\t" << USG_VO << "\n";
	cerr << "\t" << USG_OUTPUT << "\n";
	cerr << "\t" << USG_NOINT << "\n";
	cerr << "\t" << USG_DEBUG << "\n";
	cerr << "\t" << USG_LOGFILE << "\n\n";
	cerr << "Please report any bug at:\n" ;
	cerr << "\t" << HELP_EMAIL << "\n";
	if (long_usg){
		cerr  << exename << " full help\n\n" ;
	}
};

/*
*	prints the help usage message for the job-logging-info
*	@param exename the name of the executable
*	@param long_usage if the value is true it prints the long help msg
*/
void Options::loginfo_usage(const char* &exename, const bool &long_usg){
	cerr << HELP_UI << " " << HELP_VERSION << "\n" ;
	cerr << HELP_COPYRIGHT << "\n\n" ;
	cerr << "Usage: " << exename <<   " [options]   <job Id(s)>\n\n";
	cerr << "Options:\n" ;
	cerr << "\t" << USG_HELP << "\n";
	cerr << "\t" << USG_VERSION << "\n\n";
	cerr << "\t" << USG_VERBOSE << "\n";
	cerr << "\t" << USG_OUTPUT << "\n";
	cerr << "\t" << USG_NOINT << "\n";
	cerr << "\t" << USG_DEBUG << "\n";
	cerr << "\t" << USG_LOGFILE << "\n\n";
	cerr << "Please report any bug at:\n" ;
	cerr << "\t" << HELP_EMAIL << "\n";
	if (long_usg){
		cerr  << exename << " full help\n\n" ;
	}
};

/*
*	prints the help usage message for the job-cancel
*	@param exename the name of the executable
*	@param long_usage if the value is true it prints the long help msg
*/
void Options::cancel_usage(const char* &exename, const bool &long_usg){
	cerr << HELP_UI << " " << HELP_VERSION << "\n" ;
	cerr << HELP_COPYRIGHT << "\n\n" ;
	cerr << "Usage: " << exename <<   " [options]   <job Id(s)>\n\n";
	cerr << "Options:\n" ;
	cerr << "\t" << USG_HELP << "\n";
	cerr << "\t" << USG_VERSION << "\n\n";
	cerr << "\t" << USG_ALL << "\n";
	cerr << "\t" << USG_INPUT << "\n";
	cerr << "\t" << USG_CONFIG << "\n";
	cerr << "\t" << USG_CONFIGVO << "\n";
	cerr << "\t" << USG_VO << "\n";
	cerr << "\t" << USG_OUTPUT << "\n";
	cerr << "\t" << USG_NOINT << "\n";
	cerr << "\t" << USG_DEBUG << "\n";
	cerr << "\t" << USG_LOGFILE << "\n\n";
	cerr << "Please report any bug at:\n" ;
	cerr << "\t" << HELP_EMAIL << "\n";
	if (long_usg){
		cerr  << exename << " full help\n\n" ;
	}
};

/*
*	prints the help usage message for the job-list-match
*	@param exename the name of the executable
*	@param long_usage if the value is true it prints the long help msg
*/
void Options::lsmatch_usage(const char* &exename, const bool &long_usg){
	cerr << HELP_UI << " " << HELP_VERSION << "\n" ;
	cerr << HELP_COPYRIGHT << "\n\n" ;
	cerr << "Usage: " << exename <<   " [options]   <jdl file>\n\n";
	cerr << "Options:\n" ;
	cerr << "\t" << USG_HELP << "\n";
	cerr << "\t" << USG_VERSION << "\n\n";
	cerr << "\t" << USG_RANK << "\n";
	cerr << "\t" << USG_CONFIG << "\n";
	cerr << "\t" << USG_CONFIGVO << "\n";
	cerr << "\t" << USG_VO << "\n";
	cerr << "\t" << USG_OUTPUT << "\n";
	cerr << "\t" << USG_NOINT << "\n";
	cerr << "\t" << USG_DEBUG << "\n";
	cerr << "\t" << USG_LOGFILE << "\n\n";
	cerr << "Please report any bug at:\n" ;
	cerr << "\t" << HELP_EMAIL << "\n";
	if (long_usg){
		cerr  << exename << " full help\n\n" ;
	}
};

/*
*	prints the help usage message for the job-output
*	@param exename the name of the executable
*	@param long_usage if the value is true it prints the long help msg
*/
void Options::output_usage(const char* &exename, const bool &long_usg){
	cerr << HELP_UI << " " << HELP_VERSION << "\n" ;
	cerr << HELP_COPYRIGHT << "\n\n" ;
	cerr << "Usage: " << exename <<   " [options]   <job Id(s)>\n\n";
	cerr << "Options:\n" ;
	cerr << "\t" << USG_HELP << "\n";
	cerr << "\t" << USG_VERSION << "\n\n";
	cerr << "\t" << USG_INPUT << "\n";
	cerr << "\t" << USG_DIR << "\n";
	cerr << "\t" << USG_CONFIG << "\n";
	cerr << "\t" << USG_NOINT << "\n";
	cerr << "\t" << USG_DEBUG << "\n";
	cerr << "\t" << USG_LOGFILE << "\n\n";
	cerr << "Please report any bug at:\n" ;
	cerr << "\t" << HELP_EMAIL << "\n";
	if (long_usg){
		cerr  << exename << " full help\n\n" ;
	}
};

/*
*	prints the help usage message for the job-attach
*	@param exename the name of the executable
*	@param long_usage if the value is true it prints the long help msg
*/
void Options::attach_usage(const char* &exename, const bool &long_usg){
	cerr << HELP_UI << " " << HELP_VERSION << "\n" ;
	cerr << HELP_COPYRIGHT << "\n\n" ;
	cerr << "Usage: " << exename <<   " [options]   <job Id>\n\n";
	cerr << "Options:\n" ;
	cerr << "\t" << USG_HELP << "\n";
	cerr << "\t" << USG_VERSION << "\n\n";
	cerr << "\t" << USG_PORT << "\n\n";
	cerr << "\t" << USG_NOLISTEN << "\n";
	cerr << "\t" << USG_NOGUI << "\n";
	cerr << "\t" << USG_CONFIG << "\n";
	cerr << "\t" << USG_INPUT << "\n";
	cerr << "\t" << USG_NOINT << "\n";
	cerr << "\t" << USG_DEBUG << "\n";
	cerr << "\t" << USG_LOGFILE << "\n\n";
	cerr << "Please report any bug at:\n" ;
	cerr << "\t" << HELP_EMAIL << "\n";
	if (long_usg){
		cerr  << exename << " full help\n\n" ;
	}
};

/*
*	constructor
*	@param command command to be handled
*/
Options::Options (const WMPCommands &command){
	jdlFile = NULL ;
	// init of the string attributes
	chkpt = NULL;
	config = NULL;
	configvo = NULL;
	dir = NULL;
	exclude = NULL;
	from = NULL;
	input = NULL;
	lmrs = NULL;
	logfile = NULL;
	output = NULL;
	resource = NULL ;
	status = NULL;
	to = NULL;
	vo = NULL;
	// init of the boolean attributes
	all  = false ;
	debug  = false ;
	help = false  ;
	nogui  = false ;
	noint  = false ;
	nolisten = false  ;
	nomsg = false  ;
	noversion  = false ;
	rank = false  ;
	version  = false ;
	// init of the numerical attributes
	valid = NULL ;
	port = NULL ;
	verbosity = NULL ;
	// definitions of short and long options
	switch (command){
		case (JOBSUBMIT) :{
			// short options
			asprintf (&shortOpts,
				"%c%c%c%c%c%c%c%c%c%c",
				Options::SHORT_OUTPUT, 	short_required_arg,
				Options::SHORT_INPUT,		short_required_arg,
				Options::SHORT_CONFIG,	short_required_arg,
				Options::SHORT_RESOURCE,	short_required_arg,
				Options::SHORT_VALID,	short_required_arg);
			// long options
			longOpts = submitLongOpts ;
			break ;
		} ;
		case(JOBSTATUS):
		{
			// short options
			asprintf (&shortOpts,
				"%c%c%c%c%c%c" ,
				Options::SHORT_OUTPUT, 	short_required_arg,
				Options::SHORT_INPUT,		short_required_arg,
				Options::SHORT_CONFIG,	short_required_arg);
			// long options
			longOpts = statusLongOpts ;
			break;
		} ;
		case(JOBLOGINFO) :{
			// short options
			asprintf (&shortOpts,
				"%c%c%c%c%c%c%c%c" ,
				Options::SHORT_OUTPUT, 	short_required_arg,
				Options::SHORT_INPUT,		short_required_arg,
				Options::SHORT_CONFIG,	short_required_arg,
				Options::SHORT_VERBOSE,	short_required_arg);
			// long options
			longOpts = loginfoLongOpts ;
			break;
		} ;
		case(JOBCANCEL) :
		{
			// short options
			asprintf (&shortOpts,
				"%c%c%c%c%c%c" ,
				Options::SHORT_OUTPUT, 	short_required_arg,
				Options::SHORT_INPUT,		short_required_arg,
				Options::SHORT_CONFIG,	short_required_arg);
			// long options
			longOpts = cancelLongOpts ;
			break;
		} ;
		case(JOBMATCH) :{
			// short options
			asprintf (&shortOpts,
				"%c%c%c%c" ,
				Options::SHORT_OUTPUT, 	short_required_arg,
				Options::SHORT_CONFIG,	short_required_arg);
			// long options
			longOpts = cancelLongOpts ;
			break;
		} ;
		case(JOBOUTPUT) :{
			// short options
			asprintf (&shortOpts,
				"%c%c%c%c" ,
				Options::SHORT_INPUT,		short_required_arg,
				Options::SHORT_CONFIG,	short_required_arg);
			// long options
			longOpts = outputLongOpts ;
			break;
		} ;
		case(JOBATTACH) :{
			// short options
			asprintf (&shortOpts,
				"%c%c%c%c%c%c" ,
				Options::SHORT_PORT,	 	short_required_arg,
				Options::SHORT_INPUT,		short_required_arg,
				Options::SHORT_CONFIG,	short_required_arg);
			// long options
			longOpts = attachLongOpts ;
			break;
		} ;
		default : {
			throw "constructor> unknown command" ;
		} ;
	};
	// command type attribute
	cmdType = command ;
};

/*
*	gets the value of the option string-attribute
*	@param attribute name of the attribute
*/
string* Options::getStringAttribute (const OptsAttributes &attribute){
	string *value = NULL ;
	switch (attribute){
		case(CONFIGVO) : {
			if (configvo){
				value = new string (*configvo) ;
			}
			break ;
		}
		case(VO) : {
			if (vo){
				value = new string (*vo) ;
			}
			break ;
		}
		case(DIR) : {
			if (dir){
				value = new string (*dir) ;
			}
			break ;
		}
		case(LOGFILE) : {
			if (logfile){
				value = new string (*logfile) ;
			}
			break ;
		}
		case(CHKPT) : {
			if (chkpt){
				value = new string (*chkpt) ;
			}
			break ;
		}
		case(LMRS) : {
			if (lmrs){
				value = new string (*lmrs) ;
			}
			break ;
		}
		case(TO) : {
			if (to){
				value = new string (*to) ;
			}
			break ;
		}
		case(OUTPUT) : {
			if (output){
				value = new string (*output) ;
			}
			break ;
		}
		case(INPUT) : {
			if (input){
				value = new string (*input) ;
			}
			break ;
		}
		case(CONFIG) : {
			if (config){
				value = new string (*config) ;
			}
			break ;
		}
		case(RESOURCE) : {
			if (resource){
				value = new string (*resource) ;
			}
			break ;
		}
		default : {

			break ;
		}
	};
	return value ;
};

/*
*	gets the value of the option string-attribute
*	@param attribute name of the attribute
*/
long* Options::getLongAttribute (const OptsAttributes &attribute){
	long *value = NULL ;
	switch (attribute){
		case(VALID) : {
			if (valid){
				value = (long*)malloc(sizeof(long));
				*value = *valid ;
			}
			break ;
		}
		default : {
			break ;
		}
	};
	return value ;
};

/*
*	gets the value of the option int-attribute
*	@param attribute name of the attribute
*/
int* Options::getIntAttribute (const OptsAttributes &attribute){
	int *value = NULL ;
	switch (attribute){
		case(PORT) : {
			if (port){
				value = (int*)malloc(sizeof(int));
				*value = *port ;
			}
			break ;
		}
		case(VERBOSE) : {
			if (verbosity){
				value = (int*)malloc(sizeof(int));
				*value = *verbosity ;
			}
			break ;
		}
		default : {
			break ;
		}
	};
	return value ;
};
/*
*	gets the value of the option string-attribute
*	@param attribute name of the attribute
*/
bool Options::getBoolAttribute (const OptsAttributes &attribute){
	bool value = false ;
	switch (attribute){
		case(HELP) : {
			value = help  ;
			break ;
		}
		case(VERSION) : {
			value = version ;
			break ;
		}
		case(NOVERSION) : {
			value = noversion  ;
			break ;
		}
		case(NOMSG) : {
			value = nomsg ;
			break ;
		}
		case(NOGUI) : {
			value = nogui ;
			break ;
		}
		case(NOLISTEN) : {
			value = nolisten ;
			break ;
		}
		default : {
			break ;
		}
	};
	return value ;
};

/*
*	gets the value of the option list of strings-attribute
*	@param attribute name of the attribute
*/
const vector<string> Options::getListAttribute (const Options::OptsAttributes &attribute){
	vector<string> *vect ;
	switch (attribute){
		case(USERTAG) : {
			vect = &usertag ;
		}
		default : {
			cout << "getListAttribute> no attribute !\n";
			throw exception();
			break ;
		}
	};
	return (*vect);
};
/*
*	gets the list of job identifiers
*	@return a vector with the list of jobid's
*/
const vector<string> Options::getJobIds () {
	return jobIds;
};
/*
*	gets the path to the JDL file
*	@return the filepath string
*/
string Options::getPath2Jdl () {
	if (!jdlFile) {
		cerr << "ERROR (getPath2Jdl )> no path to JDL has been set\n";
		throw exception();
	}
	return *jdlFile;
};
/*
*	sets the value of the option attribute
*	@param in_opt code assigned to the option
*	(the last parameter in the long option struct assigned to each option)
*	@param command line options
*/
void Options::setAttribute (const int &in_opt, const char **argv) {
cout << "setAttribute > start in_opt=" << in_opt << "\n";
	switch (in_opt){
		case ( Options::SHORT_OUTPUT ) : {
cout << "1\n";			if (output){
				// DUPLICATE  OPT !!!
				throw "output - DUPLICATE  OPT !!!" ;
			} else {
				output = new string (optarg);
			}
			break ;
		};
		case ( Options::SHORT_INPUT ) : {
cout << "2\n";				if (input){
				// DUPLICATE  OPT !!!
				throw "input - DUPLICATE  OPT !!!" ;
			} else {
				input = new string (optarg);
			}
			break ;
		};
		case ( Options::SHORT_CONFIG ) : {
			if (config){
				// DUPLICATE  OPT !!!
				throw "input - DUPLICATE  OPT !!!" ;
			} else {
				config = new string (optarg);
			}
			break ;
		};
		case ( Options::SHORT_RESOURCE ) : {
			if (resource){
				// DUPLICATE  OPT !!!
				throw "resource - DUPLICATE  OPT !!!" ;
			} else {
				resource = new string (optarg);
			}
			break ;
		};
		case ( Options::SHORT_VALID ) : {
			if (valid){
				// DUPLICATE  OPT !!!
				throw "valid - DUPLICATE  OPT !!!" ;
			} else {
				valid = (long*) malloc (sizeof(long));
				*valid = atol (optarg);
			}
			break ;
		};
		case ( Options::SHORT_VERBOSE ) : {
			if (verbosity){
				// DUPLICATE  OPT !!!
				throw "vebose - DUPLICATE  OPT !!!" ;
			}else {
				verbosity = (int*) malloc (sizeof(int));
				*verbosity = atoi (optarg);
			}
			break ;
		};
		case ( Options::SHORT_STATUS ) : {
			if (status){
				// DUPLICATE  OPT !!!
				throw "status - DUPLICATE  OPT !!!" ;
			} else {
				status = new string (optarg);
			}
			break ;
		};
		case ( Options::SHORT_EXCLUDE ) : {
			if (exclude){
				// DUPLICATE  OPT !!!
				throw "exclude - DUPLICATE  OPT !!!" ;
			} else {
				exclude = new string (optarg);
			}
			break ;
		};
		case ( Options::ALL ) : {
			all = true ;
			break ;
		};
		case ( Options::CHKPT ) : {
			if (chkpt){
				// DUPLICATE  OPT !!!
				throw "chkpt - DUPLICATE  OPT !!!" ;
			} else {
				chkpt = new string (optarg);
			}
			break ;
		};
		case ( Options::CONFIGVO ) : {
			if (configvo){
				// DUPLICATE  OPT !!!
				throw "configvo- DUPLICATE  OPT !!!" ;
			} else {
				configvo = new string (optarg);
			}
			break ;
		};
		case ( Options::DBG ) : {
			debug = true ;
		};
		case ( Options::DIR ) : {
			if (dir){
				// DUPLICATE  OPT !!!
				throw "dir - DUPLICATE  OPT !!!" ;
			} else {
				dir = new string (optarg);
			}
			break ;
		};
		case ( Options::FROM ) : {
			if (from){
				// DUPLICATE  OPT !!!
				throw "from- DUPLICATE  OPT !!!" ;
			} else {
				from = new string (optarg);
			}
			break ;
		};
		case ( Options::HELP ) : {
	cout << "setAttribute > help \n";
			help = true ;
			break;
		};
		case ( Options::LMRS ) : {
			if (lmrs){
				// DUPLICATE  OPT !!!
				throw "lmrs- DUPLICATE  OPT !!!" ;
			} else {
				lmrs = new string (optarg);
			}
			break ;
		};
		case ( Options::LOGFILE ) : {
			if (logfile){
				// DUPLICATE  OPT !!!
				throw "logfile- DUPLICATE  OPT !!!" ;
			} else {
				logfile = new string (optarg);
			}
			break ;
		};
		case ( Options::NOGUI ) : {
			nogui = true ;
			break ;
		};
		case ( Options::NOINT ) : {
			noint = true ;
			break ;
		};
		case ( Options::NOLISTEN ) : {
			nolisten = true ;
			break ;
		};
		case ( Options::NOMSG ) : {
			nomsg = true ;
			break ;
		};
		case ( Options::SHORT_PORT ) : {
			if (port){
				// DUPLICATE  OPT !!!
				throw "port - DUPLICATE  OPT !!!" ;
			}else {
				port= (int*) malloc (sizeof(int));
				*port = atoi (optarg);
			}
			break ;
		};
		case ( Options::RANK ) : {
			rank = true ;
			break ;
		};
		case ( Options::TO ) : {
			if (to){
				// DUPLICATE  OPT !!!
				throw "to - DUPLICATE  OPT !!!" ;
			} else {
				to = new string (optarg);
			}
			break ;
		};
		case ( Options::USERTAG ) : {
			usertag.push_back(optarg);
			break ;
		};
		case ( Options::VERSION ) : {
			version = true ;
			break ;
		};
		case ( Options::VO ) : {
			if (vo){
				// DUPLICATE  OPT !!!
				throw "vo - DUPLICATE  OPT !!!" ;
			}else {
				vo = new string (optarg);
			}
			break ;
		};
		default : {
			throw "setAttribute > no option ! " ;
			break ;
		};
	};
};

/*
*	reads the input options for submission
*	@param argv option string
*/
void Options::readOptions(const int &argc, const char **argv){
        int next_opt = 0;
	int *indexptr  = (int*) malloc (sizeof(int));
	/*
	cout << "---->argc=" << argc << "\n";
	cout << "---->optopt=" << optopt << "\n" ;
	cout << "---->optind=" << optind << "\n";
	cout << "---->argv=" << argv[optind]<< "\n";
	*/
	do {
		// option parsing
		next_opt = getopt_long (argc,
                                                 (char* const*)argv,
                                                shortOpts,
                                               	longOpts,
                                                indexptr );
		cout << "next_opt=" << next_opt << "\n" ;

		// error
		if (next_opt == '?') {
			cout << "option error ! \n";
			throw exception( ) ;
		}
		// sets attribute
		if (next_opt != -1 ){
			cout << "int_ptr=" << *indexptr << "\n" ;
			setAttribute (longOpts[*indexptr].val, argv);
		}
		/*
		cout << "optopt=" << optopt << "\n" ;
		cout << "optind=" << optind << "\n";
		cout << "argv=" << argv[optind]<< "\n";
		cout << "-------------------------------\n";
		*/

	} while (next_opt != -1);

	// check to be done only if the help message has not been requested
	if (!help) {
		// checks the JDL file option for the submission and list-match
		if ( cmdType == JOBSUBMIT ||
			cmdType == JOBMATCH  ){
			if ( optind == (argc-1) ){
				ifstream file(argv[optind]);
				if (!file.good()) {
					cout << "no such JDL file ! \n";
					throw exception( ) ;
				}
				jdlFile = new string(argv[ optind ]) ;
			} else {
				cout << "no JDL file option !\n" ;
				throw exception( ) ;
			}
		}
		// JobId option in job-attach
		if ( cmdType == JOBATTACH ){
			if ( optind == (argc-1) ){
				jobIds.push_back(argv[optind]);
			} else {
				cout << "no JOBID option !\n" ;
				throw exception( ) ;
			}
		}
		// list of jobids in status, logginginfo and cancel
		if ( cmdType == JOBSTATUS  ||
		cmdType == JOBLOGINFO ||
		cmdType == JOBCANCEL ) {
			if (optind == argc ){
				cout << "no JOBID option !\n" ;
				throw exception( ) ;
			}
			for (int i = optind ; i < argc ; i++ ){
				jobIds.push_back(argv[i]);
			}
		}
	} else {
		switch (cmdType){
			case (JOBSUBMIT ) :{
				submit_usage(argv[0]);
				break;
			} ;
			case (JOBSTATUS ) :{
				status_usage(argv[0]);
				break;
			} ;
			case (JOBLOGINFO ) :{
				loginfo_usage(argv[0]);
				break;
			} ;
			case (JOBCANCEL ) :{
				cancel_usage(argv[0]);
				break;
			} ;
			case (JOBOUTPUT ) :{
				output_usage(argv[0]);
				break;
			} ;
			case ( JOBMATCH) :{
				lsmatch_usage(argv[0]);
				break;
			} ;
			case ( JOBATTACH) :{
				attach_usage(argv[0]);
				break;
			} ;
			default :{
				break;
			} ;
		}
		exit(0);
	}
};
} // glite
} // wms
} // client
} // utilities
