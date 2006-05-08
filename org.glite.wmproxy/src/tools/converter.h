#ifndef GLITE_WMS_WMPROXY_TOOLS_CONVERTER_H
#define GLITE_WMS_WMPROXY_TOOLS_CONVERTER_H

#include "log.h"
#include "authorizer/wmpgaclmanager.h"

#include <vector>
#include <string>

namespace glite {
namespace wms {
namespace wmproxy {
namespace tools {

struct userOpts {
	std::string* input ;
	std::string* output ;
	std::string* log;
	bool noint;
	bool debug ;
	bool create ;
	bool add ;
	bool remove;
	bool dn ;
	bool fqan ;
	bool version ;
	bool help;
};


class Converter {
        public :
		/**
		* Default constructor
		*/
		Converter();

		/**
		* Default destructor
		*/
		~Converter () ;
		/**
		* Returns a message with the version of this tool
		*/
		const std::string version() ;
		/**
		* Prints on the standard error
		* a message with the usage instructions for this tool
		*/
		void usage(const std::string &exe);
		/**
		* Handles the errors occuring during the excution: prints the error messages
		* and stop the execution of the program
		* @param msg the string with error message
		* @param desc the string with some details about the error
		*/
		void error (const std::string &msg, const std::string &details="") ;
		/**
		* Returns the pointer to the userOptions struct
		*/
		userOpts* getUserOptions( ) ;
		/**
		* Creates the log file to the specified path
		*@param path the location of the log file
		*/
		Log* createLogFile (const std::string &path);
		/**
		* Creates the log file to the default-path
		*@param path the location of the log file
		*/
		Log* createLogFile ( );

		/**
		* Checks whether the input string corresponds to one of the user options
		* @param opt the name of the input option to be checked
		* @return a negative value if the inputs string doesn't match any user option strings
		*/
		const int checkOpts(const std::string& opt) ;
		/**
		* Reads the input options
		* @param arg the number of input string
		* @param opts this method put the read values of the user options in this struct
		* @param msg at the end of the execution this string contains the list of the read options
		* @param errors the description of any error occurred during the execution
		* @return a negative value in case of any error occured,
		* ("errors" contains the description of the error), 0 otherwise
		*/
		const int readOptions(int argc,char **argv, userOpts &opts, std::string &msg, std::string &errors);
		/*
		*: Loads the grid-mapfile located in "path" and
		* puts the entries in the mapEntries vector
		* @param path the path location of the gridmapfile to be loaded
		* @param entries this method put all the entries of the file in this vector
		* @param errors the description of any error occurred during the execution
		* @return a negative value in case of any error occured
		* ("errors" contains the description of the error), 0 otherwise
		*/
		static int getEntry(std::string &entry) ;
		/**
		*/
		int readMapFile(const std::string &path, std::vector<std::string> &entries, std::string &errors) ;
		void setUpGaclFile (const std::string &path, const bool& create,std::vector<std::string> &dns, std::vector<std::string> &fqans) ;
		void addEntries (const std::vector<std::string> &map, const glite::wms::wmproxy::authorizer::GaclManager::WMPgaclCredType &credential= 					glite::wms::wmproxy::authorizer::GaclManager::WMPGACL_UNDEFCRED_TYPE) ;
		void removeOldEntries(const std::vector<std::string> &map, std::vector<std::string> &gacl, const glite::wms::wmproxy::authorizer::GaclManager::WMPgaclCredType &credential=glite::wms::wmproxy::authorizer::GaclManager::WMPGACL_UNDEFCRED_TYPE) ;
		static glite::wms::wmproxy::authorizer::GaclManager::WMPgaclCredType Converter::checkCredentialType (const std::string &raw) ;
		int saveGacl ( ) ;
		std::string entries (int n, bool new_word=false);
		std::string getResultMsg ( );
		inline bool add( ){return addOper;};
		inline bool remove( ){return rmOper;};

	private:
/**
*  Yes/No-question
*/
bool answerYes (const std::string& question, bool defaultAnswer, bool defaultValue);

		Log* logFile;
		userOpts usrOpts;
		glite::wms::wmproxy::authorizer::GaclManager *usrGacl;
		std::string gridMapFile;
		// new gacl file flag
		bool newGacl ;
		bool existingGacl;
		// operation counters
		int readEntries;
		int addedEntries;
		int removedEntries;
		int notReplacedEntries;
		// operation types
		bool addOper ;
		bool rmOper ;

};
}}}} // ending namespaces
#endif
