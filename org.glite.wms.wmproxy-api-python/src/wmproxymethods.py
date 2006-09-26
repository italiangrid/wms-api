
"""
Python API for Wmproxy Web Service
"""

from WMPClient import WMPSOAPProxy
import SOAPpy  #Error Type
import os  #getDefaultProxy method
import socket # socket error mapping

class Config:
	"""
	Used for debug messages.
	you can always change this value from an external program (please do not edit this file)
	"""
	DEBUGMODE=0

### Static methods/classes: ###
def parseStructType(struct,*fields):
	"""
	with fielsd: Parses a SOAPPy.Types.structType returning a dictionary of parsed attributes
	without fields: Parses a SOAPPy.Types.structType returning a list of parsed attributes
	"""
	if fields:
		result= {}
		for field in fields:
			try:
				result[field]=struct.__getitem__(field)
			except:
				if Config.DEBUGMODE:
					print "\n *** WARNING: parseStructType: Unable to find field: " + field
					print "Available struct is:" , struct
				result[field]=""
		return result
	else:
		result=[]
		for item in struct:
			result.append(item)
		return result


class JobIdStruct:
	"""
	Output Structure for Register and Submission operation
	each JobIdStructure must have:
	- an identifier, th jobid itself (string)
	moreover it can have:
	- a path corresponding to the WMPROXY jobid InputSandbox location (string)
	- a name, if it is a node of a dag (string)
	- one or more children if it is a dag (list of JobIdStructure(s))
	"""
	def __init__ (self,soapStruct):
		"""
		Default constructor
		"""
		self.children = []
		self.nodeName = ""
		self.path     = ""
		# Mandatory field jobid:
		self.jobid=soapStruct.__getitem__("id")
		# Optional fields (children, name,path)
		try:
			# try and parse children:
			children =soapStruct.__getitem__("childrenJob")
			if type(children)==list:
				# iterate over children
				for child in children:
					self.children.append( JobIdStruct(child) )
			else:
				# Only one child: append it
				self.children.append(JobIdStruct(children))
		except AttributeError:
			# no children found: it is not a dag
			pass
		try:
			self.nodeName = soapStruct.__getitem__("name")
		except AttributeError:
			# no node name found: it is not a node
			self.nodeName = ""
			pass
		try:
			self.path = soapStruct.__getitem__("path")
		except AttributeError:
			# no path found: attribute not present
			# (an old server might have been contacted)
			self.path= ""
			pass


	def __repr__(self):
		"""
		Return, the istance itself
		"""
		return "<JobIdStruct instance: " + self.jobid + ">"
	def getChildren(self):
		"""
		Return, if the istance represents a dag, the list of all its sons (as a list of JobIdStruct)
		"""
		return self.children
	def toString(self):
		"""
		Return the JobId string representation
		"""
		result = ""
		if self.nodeName:
			result+="Node Name: " + self.nodeName +"\n"
		if self.path:
			result+="ISB remote path: " + self.path +"\n"
		result+="JobId: " + self.jobid
		for child in self.children():
			result += "\n\t" + child.toString()
		return result
	def getJobId(self):
		"""
		Equal to toString
		"""
		return self.jobid
	def getNodeName(self):
		"""
		Return, if the istance represents a dag node, the name of the node
		"""
		return self.nodeName
	def getPath(self):
		"""
		Return, if avaliable, the path
		"""
		return self.path



class BaseException(Exception):
	"""
	Base Exception Class deinfe a structure for
	all exception thrown in this module
	"""
	origin  = ""
	errType = ""
	methodName =""
	description=""
	args=[]

	def __repr__(self):
		return "<BaseException instance: " + self.toString() + ">"

	def toString(self):
		result =""
		if self.errType:
			result+=self.errType
		if self.origin:
			result+=" raised by "+self.origin
		if self.methodName:
			result+=" (method "+self.methodName+") "
		if self.description:
			result+="\n" + self.description
		if Config.DEBUGMODE:
			for fc in self.args:
				try:
					result+="\n\t "+ fc
				except:
					result+="\n\t "+ repr(fc)
		return result


class HTTPException(BaseException):
	"""
	Specify a structure for HTTP protocol exceptions
	"""
	def __init__(self, err):
		self.origin  = "HTTP Server"
		self.errType = "Error"
		self.error   = err
		self.methodName =""
		self.description=""
		self.args=[]


class SocketException(BaseException):
	"""
	Socket-Connection Error
	input: a socket.err error
	"""
	def __init__(self, err):
		self.origin  = "Socket Connection"
		self.errType = "Error"
		self.errorCode   = 105
		self.args=[]
		for ar in err.args:
			self.args.append(ar)

class WMPException(BaseException):
	"""
	Specify a structure for Wmproxy Server exceptions
	"""
	def parseWmp(self, err):
		error = err[2][0]
		self.errorCode   = error["ErrorCode"]
		self.timestamp   = error["Timestamp"]
		self.methodName  = error["methodName"]
		self.description = error["Description"]
		for ar in error["FaultCause"]:
			self.args.append(ar)

	def parseExt(self, err):
		self.origin  = err[0]
		self.errType = err[1]
		self.methodName =""
		error = err[2][0]
		if error:
			self.description =error[0]
			self.args.append(self.description)
		else:
			self.args.append(self.origin)
			self.args.append(self.errType)
	def __init__(self, err):
		self.error   = err
		self.origin  = err[0]
		self.errType = err[1]
		self.args=[]
		try:
			self.parseWmp(err)
		except:
			try:
				self.parseExt(err)
			except:
				raise err


class ApiException(BaseException):
	"""
	Exception raised directly from api client
	"""
	def __init__(self, method, message):
		self.origin  = "Wmproxy Api Python"
		self.errType = "Method not supported"
		self.errorCode   = 105
		self.methodName  = method
		self.description = message


def getDefaultProxy():
	""" retrieve PROXY Default Certificate File name"""
	try:
			return os.environ['X509_USER_PROXY']
	except:
			return '/tmp/x509up_u'+ repr(os.getuid())


"""
Converters:
"""

def b2b(b_in):
	"""
	Boolean to SOAPPy boolean
	"""
	return SOAPpy.Types.booleanType(b_in)

def getJDLConverter(i_in):
	"""
	used to convert an integer into a getJDL Request value as follows:
	i_in ==  0 retrieves ORIGINAL   jdl
	i_in !=  0 retrieves REGISTERED jdl
	This method is used internally by getJDL method
	"""
	if int (i_in):
		return SOAPpy.Types.untypedType('REGISTERED')
	else:
		return SOAPpy.Types.untypedType('ORIGINAL')


class Wmproxy:
	"""
	Provide all WMProxy web services
	"""
	def __init__(self, url, ns="", proxy=""):
		"""
		Default Constructor
		"""
		self.url=url
		self.ns=ns
		self.proxy=proxy
		self.remote=""
		self.init=0
	"""
	Static Methods
	"""
	def getDefaultNs(self):
		"""
		retrieve Wmproxy default namespace string representation (static method)
		"""
		return "http://glite.org/wms/wmproxy"
	def getGrstNs(self):
		"""
		GridSite Delegation namespace string representation  (static method)
		PutProxy and getProxyReq services requires gridsite specific namespace
		WARNING: for backward compatibility with WMPROXY server (version <= 1.x.x)
		deprecated PutProxy and getProxyReq sevices are still provided with
		wmproxy default namespace
		"""
		return "http://www.gridsite.org/namespaces/delegation-1"


	def close(self):
		"""
		Default Destructor
		reset values
		"""
		self.ns=""
		self.proxy=""
		self.remote=""
		self.init=0
	def soapInit(self):
		"""
		Perform initialisation  (if necessary)
		Establish connection with remote server
		"""
		if self.init==0:
			if not self.proxy:
				self.proxy=getDefaultProxy()
			if not self.ns:
				self.ns=self.getDefaultNs()
			self.remote = WMPSOAPProxy(self.url,namespace=self.ns, key_file=self.proxy, cert_file=self.proxy)
			self.init=1

	def setUrl(self,url):
		"""
		Change/Set the url where to load the WSDL from
		IN =  url(string)
		"""
		self.init=0
		self.url=url

	def setNamespace(self,ns):
		"""
		Change/Set the Wmproxy web server namespace
		IN =  url (string)
		"""
		self.init=0
		self.ns=ns

	def setProxy(self,proxy):
		"""
		Change/Set the proxty certificate file location
		IN =  proxy (string)
		"""
		self.init=0
		self.proxy=proxy

	def methods(self):
		self.soapInit()
		return self.remote.methods

	"""
	ACTUAL WEB SERVICE METHODS
	"""
	def jobListMatch(self, jdl, delegationId):
		"""
		Method:  jobListMatch
		IN =  jdl (string)
		IN =  delegationId (string)
		OUT = CEIdAndRankList (StringAndLongList)

		return the list of CE Ids satisfying the job Requirements specified in the JDL,
		ordered according to the decreasing Rank.
		"""
		try:
			self.soapInit()
			result = []
			jlm = self.remote.jobListMatch(jdl, delegationId)[0]
			for i in range (len(jlm)):
				result.append(parseStructType(jlm[i]))
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getCollectionTemplate(self, jobNumber, requirements, rank):
		"""
		Method:  getCollectionTemplate
		IN =  jobNumber (int)
		IN =  requirements (string)
		IN =  rank (string)
		OUT = jdl (string)

		return a JDL template for a collection of jobs, that is a set of independent jobs that can be submitted,
		controlled and monitored as a single entity.
		"""
		try:
			self.soapInit()
			return self.remote.getCollectionTemplate(jobNumber, requirements, rank)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def jobPurge(self, jobId):
		"""
		Method:  jobPurge
		IN =  jobId (string)

		Remove from the WM managed space all files related to the  job identified by the jobId provided as input.
		This only applies to job related files that are managed by the WM.
		E.g. Input/Output sandbox files that have been specified in the JDL through a URI will be not subjected to this management.
		"""
		try:
			self.soapInit()
			self.remote.jobPurge(jobId)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def jobStart(self, jobId):
		"""
		Method:  jobStart
		IN =  jobId (string)

		Trigger the submission a previously registered job.
		It starts the actual processing of the registered job within the Workload Manager.
		It is assumed that when this operation is called, all the work preparatory to the job
		(e.g. input sandbox upload, registration of input data to the Data Management service etc.)
		has been completed by the client.
		To better clarify, an example of the correct sequence of operations for submitting a job could be:
			1.  jobId = jobRegist(JDL)
			2.  destURI = getSandboxDestURI(jobID)
			3.  transfer InputSandbox file to destURI (e.g. using GridFTP)
			4.  jobStart(jobId)
		Note that the jobStart operation is not allowed on subjobs of a complex object,
		i.e. the input parameter must be either the id of a simple job or the main id of a complex object.
		"""
		try:
			self.soapInit()
			self.remote.jobStart(jobId)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)


	def addACLItems(self, jobId, items):
		"""
		Method:  addACLItems
		IN =  jobId (string)
		IN =  items (StringList)

		This operations adds a list of items to the job Access Control List.
		Items Already present will be ignored.
		"""
		try:
			self.soapInit()
			self.remote.addACLItems(jobId, items)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)


	def getACLItems(self, jobId):
		"""
		Method:  getACLItems
		IN =  jobId (string)
		OUT = a list of strings containing the ACL Items to add.

		This operation returns the list of the Items contained in the job Access Control List
		present inside the Gacl authorization file specific fo the job.
		"""
		try:
			self.soapInit()
			return parseStructType(self.remote.getACLItems(jobId))
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def removeACLItem(self, jobId, item):
		"""
		Method:  removeACLItem
		IN =  jobId (string)
		IN =  item (string)
		This operation remove an item from the job Access Control List. Removal of the item
		representing the user that has registered the job are not allowed (a fault will be
		returned to the caller).
		"""
		try:
			self.soapInit()
			self.remote.removeACLItem(jobId, item)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)


	def getSandboxBulkDestURI(self, jobId, protocol=""):
		"""
		Method:  getSandboxBulkDestURI
		IN =  jobId (string)
		IN =  protocol (as a string) to use in the returned URIs. This parameter has been added since WMPROXY server version 2.2.0
		OUT = A dictonary containing, for each jobid (string), its destUris in  available protocols (list of strings)

		This operation returns the list of destination URIs associated to a compound job
		(i.e. a DAG a Collection or a parametric jobs) and all of its sub-jobs in a vector of structures each one containing:
		- the job id
		- the corresponding list of destination URIs (can be more than one if different transfer protocols are supported, e.g. gsiftp, https etc.)
		The vector contains an element (structure above) for the compound job id provided (at first position)
		and one further element for any sub nodes. It contains only one element if the job id provided as imnput is the identifier of a simple job.
		The location is created in the storage managed by the WMS and the corresponding URI is returned to the operation caller if no problems has been arised during creation.
		Files of the job input sandbox that have been referenced in the JDL as relative or absolute paths are expected to be found in the returned location when the job lands on the CE.
		Note that the WMS service only provides a the URI of a location where the job input sandbox files can be stored but does not perform any file transfer.
		File upload is indeed responsibility of the client (through the GridFTP/HTTPS server available on the WMS node).
		The user can also specify in the JDL the complete URI of files that are stored on a GridFTP/HTTPS server
		(e.g. managed by her organisation);
		those files will be directly downloaded (by the JobWrapper) on the WN where the job will run without transiting on the WMS machine.
		The same applies to the output sandbox files list, i.e. the user can specify in the JDL the complete URIs for the files of the output sandbox;
		those files will be directly uploaded (by the JobWrapper) from the WN to the specified GridFTP/HTTPS servers without transiting on the WMS machine.
		"""
		destUris={}
		try:
			self.soapInit()
			if protocol:
				dests= self.remote.getSandboxBulkDestURI(jobId, protocol)
			else:
				dests= self.remote.getSandboxBulkDestURI(jobId)
			if type(dests[0]) == type([]):
				dests=dests[0]
			for dest in dests:
				uri = dest.__getitem__("Item")
				uris=[]
				if type(dests[0]) == type([]):
					uri = uris
				else:
					uris.append(uri)
				destUris[ dest.__getitem__("id")] = uris
			return destUris
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getSandboxDestURI(self, jobId,protocol=""):
		"""
		Method:  getSandboxDestURI
		TBD test better
		IN =  jobId (string)
		IN =  protocol (as a string) to use in the returned URIs. This parameter has been added since WMPROXY server version 2.2.0
		OUT = dstUri in all available protocols( list of strings)

		This operation returns a destination URI associated to the job, identified by the jobId provided as input,
		where the job input sandbox files can be uploaded by the client on the WMS node.
		The location is created in the storage managed by the WM and the corresponding URI is returned to the operation caller if no problems has been arised during creation.
		Files of the job input sandbox that have been referenced in the JDL as relative or absolute paths are expected to be found in the returned location when the job lands on the CE.
		Note that the WM service only provides a the URI of a location where the job input sandbox files can be stored
		but does not perform any file transfer.
		File upload is indeed responsibility of the client (through the GridFTP service available on the WMS node).
		The user can also specify in the JDL the complete URI of files that are stored on a GridFTP server
		(e.g. managed by her organisation);
		those files will be directly downloaded (by the JobWrapper) on the WN
		where the job will run without transiting on the WM machine.
		The same applies to the output sandbox files list,
		i.e. the user can specify in the JDL the complete URIs for the files of the output sandbox;
		those files will be directly uploaded (by the JobWrapper) from the WN to the specified GridFTP servers without transiting on the WMS machine.
		"""
		try:
			self.soapInit()
			if protocol:
				return self.remote.getSandboxDestURI(jobId,protocol)[0]
			else:
				return self.remote.getSandboxDestURI(jobId)[0]
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def jobCancel(self, jobId):
		"""
		Method:  jobCancel
		IN =  jobId (string)

		This operation cancels a previously submitted job identified by its JobId.
		If the job is still maaged by the WM then it is removed from the WM tasks queue. If the job has been already sent to the CE,
		the WM simply forwards the request to the CE.
		For suspending/eleasing and sending signals to a submitted job the user has to chek that the job has been scheduled to a CE
		and access directly the corresponding operations made avalable by the CE service.
		"""
		try:
			self.soapInit()
			self.remote.jobCancel(jobId)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getStringParametricJobTemplate(self, attributes, param, requirements, rank):
		"""
		Method:  getStringParametricJobTemplate
		IN =  attributes (StringList)
		IN =  param (StringList)
		IN =  requirements (string)
		IN =  rank (string)
		OUT = jdl (string)

		This operation returns a JDL template for a parametric of job, which is a job having one or more parametric attributes in the JDL.
		The parametric attributes vary their values according to the "Parameter" attribute specified in the JDL itself
		(in this case the parametere has to be a list of strings).
		The submission of a Parametric job results in the submission  of a set of jobs having the same descritpion apart from the value of the parametric attributes.
		They can be however controlled and monitored as a single entity.
		"""
		try:
			self.soapInit()
			return self.remote.getStringParametricJobTemplate(attributes, param, requirements, rank)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getJobTemplate(self, jobType, executable, arguments, requirements, rank):
		"""
		Method (not yet supported):  getJobTemplateFalse
		IN =  jobType (JobTypeList)
		IN =  executable (string)
		IN =  arguments (string)
		IN =  requirements (string)
		IN =  rank (string)
		OUT = jdl (string)

		This operation returns a JDL template for the requested job type.
		"""
		raise ApiException("getJobTemplate","not yet supported")
		try:
			self.soapInit()
			return self.remote.getJobTemplate(jobType, executable, arguments, requirements, rank)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getFreeQuota(self):
		"""
		Method:  getFreeQuota
		OUT = softLimit (long)
		OUT = hardLimit (long)

		return a dictionary with soft&hard Limits
		This operation returns the remaining free part of available user disk quota (in bytes).
		"""
		try:
			self.soapInit()
			return parseStructType(self.remote.getFreeQuota(),'softLimit','hardLimit')
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def putProxy(self, delegationID, proxy, ns =""):
		"""
		Method:  putProxy
		IN =  delegationID (string)
		IN =  ns  if namespace different from currently used,
			only this method will be affected
		IN =  proxy (string)

		ProxyOperationException: Proxy exception: Provided delegation id not valid
		WARNING: for backward compatibility putProxy is provided with both namespaces:
		defaultWmproxy namespace for WMPROXY servers (version <= 1.x.x)
		gridsite namespace for WMPROXY servers (version > 1.x.x)
		see getDefaultProxy() and  getDefaultNs() methods
		This operation finishes the delegation procedure by sending the signed proxy certificate to the server.
		"""
		try:
			if ns!=self.ns:
				oldNs=self.ns
				self.setNamespace(ns)
			self.soapInit()
			self.setNamespace(oldNs)
			self.remote.putProxy(delegationID, proxy)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getProxyReq(self, delegationID, ns =""):
		"""
		Method:  getProxyReq
		IN =  delegationID (string)
		IN =  ns  if namespace different from currently used,
			only this method will be affected
		OUT =  The new proxy certificate in PEM format wiht base64 enconding request (string)

		WARNING: for backward compatibility getProxyReq is provided with both namespaces:
		defaultWmproxy namespace for WMPROXY servers (version <= 1.x.x)
		gridsite namespace for WMPROXY servers (version > 1.x.x)
		see getDefaultProxy() and  getDefaultNs() methods
		This operation starts the delegation procedure by asking for a certificate signing request from the server.
		The server answers with a certificate signing request which includes the public key for the new delegated credentials.
		"""
		try:
			if ns!=self.ns:
				oldNs=self.ns
				self.setNamespace(ns)
			self.soapInit()
			self.setNamespace(oldNs)
			return self.remote.getProxyReq(delegationID)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)


	def getNewProxyReq(self, ns =""):
		"""
		Method:  getNewProxyReq
		IN =  ns  if namespace different from currently used,
			only this method will be affected
		OUT = Server side generated ID of the new delegation session (string)
		OUT = The new proxy certificate in PEM format wiht base64 enconding request (string)

		WARNING: This method is only available with WMPROXY servers verson >

		This operation starts the delegation procedure by asking for a certificate signing request from the server.
		The server answers with a certificate signing request which includes the public key for the new delegated credentials.
		putProxy() has to be called to finish the procedure.
		"""
		try:
			if ns!=self.ns:
				oldNs=self.ns
				self.setNamespace(ns)
			self.soapInit()
			self.setNamespace(oldNs)
			return parseStructType(self.remote.getNewProxyReq(),'delegationID','proxyRequest')
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def renewProxyReq(self, delegationID, ns =""):
		"""
		Method:  renewProxyReq
		IN =  delegationID (string)
		IN =  ns  if namespace different from currently used,
			only this method will be affected
		OUT = The new proxy certificate in PEM format wiht base64 enconding request (string)
		WARNING: This method is only available with WMPROXY servers verson >
		This operation restarts the delegation procedure by asking for a certificate signing request from the server for an already 		existing delegation ID.
		The server answers with a certificate signing request which includes the public key for new delegated credentials.
		putProxy() has to be called to finish the procedure.
		"""
		try:
			if ns!=self.ns:
				oldNs=self.ns
				self.setNamespace(ns)
			self.soapInit()
			self.setNamespace(oldNs)
			return self.remote.renewProxyReq(delegationID)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getTerminationTime(self, delegationID, ns =""):
		"""
		Method:  getTerminationTime
		IN =  delegationID (string)
		IN =  ns  if namespace different from currently used,
			only this method will be affected
		OUT = The new proxy certificate in PEM format wiht base64 enconding request (string)
		WARNING: This method is only available with WMPROXY servers verson >
		This operation returns the termination (expiration) date and time of the credential, associated with the given delegaion ID. 		If there was no delegation ID, then generate one by hashing the client DN and client VOMS attributes.
		"""
		try:
			if ns!=self.ns:
				oldNs=self.ns
				self.setNamespace(ns)
			self.soapInit()
			self.setNamespace(oldNs)
			return self.remote.getTerminationTime(delegationID)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def destroy(self, delegationID, ns =""):
		"""
		Method:  destroy
		IN =  delegationID (string)
		IN =  ns  if namespace different from currently used,
			only this method will be affected
		WARNING: This method is only available with WMPROXY servers verson >
		Destroys the delegated credentials associated with the given delegation ID immediately.
		If there was no delegation ID, then generate one by hashing the client DN and client VOMS attributes.
		"""
		try:
			if ns!=self.ns:
				oldNs=self.ns
				self.setNamespace(ns)
			self.soapInit()
			self.setNamespace(oldNs)
			self.remote.destroy(delegationID)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getVersion(self):
		"""
		Method:  getVersion
		OUT = version (string)

		This operation gets the version of the service.
		Format of the version string is "major.minor.patch"
		"""
		try:
			self.soapInit()
			return self.remote.getVersion()
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getIntParametricJobTemplate(self, attributes, param, parameterStart, parameterStep, requirements, rank):
		"""
		Method:  getIntParametricJobTemplate
		IN =  attributes (StringList)
		IN =  param (int)
		IN =  parameterStart (int)
		IN =  parameterStep (int)
		IN =  requirements (string)
		IN =  rank (string)
		OUT = jdl (string)

		This operation returns a JDL template for a parametric of job, which is a job having one or more parametric attributes in the JDL.
		The parametric attributes vary their values according to the "Parameter" attribute specified in the JDL itself
		(in this case the parametere has to be an integer).
		The submission of a Parametric job results in the submission  of a set of jobs having the same descritpion apart from the parametrised attribute.
		They can be however controlled and monitored as a single entity.
		"""
		try:
			self.soapInit()
			return self.remote.getIntParametricJobTemplate(attributes, param, parameterStart, parameterStep, requirements, rank)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getDAGTemplate(self, dependencies, requirements, rank):
		"""
		Method (not yes supported):  getDAGTemplate
		IN =  dependencies (GraphStructType)
		IN =  requirements (string)
		IN =  rank (string)
		OUT = jdl (string)

		This operation returns a JDL template for a DAG.
		"""
		raise ApiException("getDAGTemplate","not yet supported")
		try:
			self.soapInit()
			return self.remote.getDAGTemplate(dependencies, requirements, rank)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def jobSubmit(self, jdl, delegationId):
		"""
		Method:  jobSubmit
		IN =  jdl (string)
		IN =  delegationId (string)
		OUT = jobIdStruct (JobIdStruct)

		This operation submits a job. The JDL description of the job provided by the client is validated by the service, registered to the LB and finally passed to the Workload Manager.
		The unique identifier assigned to the job is returned to the client.
		This operation assumes that all the work preparatory to the job
		(e.g. input sandbox upload, registration of input data to the Data Management service etc.) has been completed by the client.
		Usage of this operation (instead of jobRegister + jobStart) is indeed recommended when the job identifier is not needed prior to its submission
		(e.g. jobs without input sandbox or with a sandbox entirely available on a GridFTP server managed by the client).
		The service supports submission of simple jobs, parametric jobs, partitionable jobs, DAGs and collections of jobs;
		the description is always provided through a single JDL description (see "GLite JDL Attributes" document for details).
		When a clients requests for submission of a complex object, i.e. parametric and partitionable jobs, DAGs and collections of jobs
		(all those requests represent in fact a set of jobs),
		the operations returns a structure containing the main identifier of the complex object and the identifiers of all related sub jobs.
		"""
		try:
			self.soapInit()
			return JobIdStruct(self.remote.jobSubmit(jdl, delegationId))
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def jobRegister(self, jdl, delegationId):
		"""
		Method:  jobRegister
		IN =  jdl (string)
		IN =  delegationId (string)
		OUT = jobIdStruct (JobIdStruct)

		This operation registers a job for submission. The JDL description of the job provided by the client is first validated by the service and then registered to the LB.
		The unique identifier assigned to the job is returned to the client.
		Note that this operation only registers the job and assign it with an identifier.
		The actual submission of the job has to be triggered by a call to the jobStart operation after all preparation activities,
		such as the Input sandbox files upload, have been completed.
		The service supports registration of simple jobs, parametric jobs, partitionable jobs, DAGs and collections of jobs;
		the description is always provided through a single JDL description (see "GLite JDL Attributes" document for details).
		When a clients requests for registration of a complex object, i.e. parametric and partitionable jobs, DAGs and collections of jobs
		(all those requests represent in fact a set of jobs),
		the operations returns a structure containing the main identifier of the complex object and the identifiers of all related sub jobs.
		"""
		try:
			self.soapInit()
			return JobIdStruct(self.remote.jobRegister(jdl, delegationId))
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def removeACLItem(self, jobId, item):
		"""
		Method (not yet supported):  removeACLItem
		IN =  jobId (string)
		IN =  item (string)

		This operation remove an item from the job Access Control List. Removal of the item representing the user that has registered the job are not allowed
		(a fault will be returned to the caller).
		"""
		raise ApiException("removeACLItem","not yet supported")
		try:
			self.soapInit()
			self.remote.removeACLItem(jobId, item)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getMaxInputSandboxSize(self):
		"""
		Method:  getMaxInputSandboxSize
		OUT = size (long)

		This operation returns the maximum Input sandbox size (in bytes) a user can count-on for a job submission if using the space managed by the WM.
		This is a static value in the WM configuration (on a job-basis) set by the VO administrator.
		No assumption should be made on the input sandboxes space managed by the WM. It is managed  transparently to the user; it can be either local to the WM or remote.
		"""
		try:
			self.soapInit()
			return int(self.remote.getMaxInputSandboxSize())
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getTotalQuota(self):
		"""
		Method:  getTotalQuota
		AuthorizationException: LCMAPS failed to map user credential
		OUT = softLimit (long)
		OUT = hardLimit (long)

		This operation returns the available user space quota on the storage managed by the WM.
		The fault GetQuotaManagementFault is returned if the quota management is not active on the WM.
		"""
		try:
			self.soapInit()
			return parseStructType(self.remote.getTotalQuota(),'softLimit''hardLimit')
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getOutputFileList(self, jobId,protocol=""):
		"""
		Method:  getOutputFileList
		IN =  jobId (string)
		IN =  protocol (as a string) to use in the returned URIs. This parameter has been added since WMPROXY server version 2.2.0
		OUT = OutputFileAndSizeList (StringAndLongList)

		This operation returns the list of URIs where the output files created during job execution have been stored in the WM managed space and the corresponding sizes in bytes.
		This only applies for files of the Output Sandbox that are managed by the WM (i.e. not specified as URI in the JDL).
		"""
		try:
			self.soapInit()
			outputFileList=[]
			if protocol:
				outputStruct=parseStructType(self.remote.getOutputFileList(jobId,protocol))
			else:
				outputStruct=parseStructType(self.remote.getOutputFileList(jobId,protocol))
			if outputStruct:
				outputStruct=outputStruct[0]
				for ofl in outputStruct:
					outputFileList.append(parseStructType(ofl,"name","size"))
			return outputFileList
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getPerusalFiles(self, jobId, file, allChunks,protocol=""):
		"""
		Method: getPerusalFiles
		IN =  jobId (string)
		IN =  file (string)
		IN =  allChunks (boolean)
		IN =  protocol (as a string) to use in the returned URIs. This parameter has been added since WMPROXY server version 2.2.0
		OUT = fileList (StringList)

		This operation gets the URIs of perusal files generated during job execution for the specified file file.
		If allChunks is set to true all perusal URIs will be returned; also the URIs already requested with a
		previous getPerusalFiles operation. Default value is false.
		"""
		try:
			self.soapInit()
			if allChunks:
				allChunks=True
			else:
				allChunks=False
			if protocol:
				files = parseStructType(self.remote.getPerusalFiles( jobId, file, b2b(allChunks), protocol) )
			else:
				files = parseStructType(self.remote.getPerusalFiles( jobId, file, b2b(allChunks), protocol) )
			if not files:
				return []
			else:
				files=files[0]
				if type(files) == type("str"):
					return [files]
				else:
					return files
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

        def enableFilePerusal(self, jobId, fileList):
		"""
		Method: enableFilePerusal
		IN =  jobId (string)
		IN =  fileList (StringList)

		This operation enables file perusal functionalities if not disabled with the specific
		jdl attribute during job register operation.
		Calling this operation, the user enables perusal for job identified by jobId,
		for files specified with fileList.
		An empty fileList disables perusal.
		"""
		try:
			self.soapInit()
			self.remote.enableFilePerusal(jobId, fileList)
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getJobProxyInfo(self, jobId):
		"""
		Method:  getJobProxyInfo
		IN =  jobId (string)
		OUT = list of strings containing Delegated Proxy information

		This operation returns the Job Proxy information
		"""
		try:
			self.soapInit()
			result =self.remote.getJobProxyInfo(jobId)
			return parseStructType(self.remote.getJobProxyInfo(jobId))
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getDelegatedProxyInfo(self, delegationId):
		"""
		Method:  getDelegatedProxyInfo
		IN =  delegationId (string)
		OUT = list of strings containing Delegated Proxy information

		This operation returns the Delegated Proxy information
		"""
		try:
			self.soapInit()
			return parseStructType(self.remote.getDelegatedProxyInfo(delegationId))
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getJDL(self, jobId, jdlType):
		"""
		Method:  getJDL
		IN = jobId (string)
		IN = jdlType (int)
		OUT = JDL as a string

		This operation returns the Delegated Proxy information
		jdlType enum values are considered as follows:
		0 = ORIGINAL, otherwise = REGISTERED

		"""
		try:
			self.soapInit()
			return self.remote.getJDL(jobId, getJDLConverter(jdlType))
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)

	def getTransferProtocols(self):
		"""
		Method:  getJDL
		OUT = A vector of string containing the protocols.

		This operation returns the server available transfer protocols.
		Input: no input.

		"""
		try:
			self.soapInit()
			protocols = parseStructType(self.remote.getTransferProtocols())
			if protocols:
				return protocols[0]
			else:
				return []
		except SOAPpy.Types.faultType, err:
			raise WMPException(err)
		except SOAPpy.Errors.HTTPError, err:
			raise HTTPException(err)
		except socket.error, err:
			raise SocketException(err)
