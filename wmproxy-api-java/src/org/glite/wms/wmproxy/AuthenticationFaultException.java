/*
Copyright (c) Members of the EGEE Collaboration. 2004.
See http://www.eu-egee.org/partners/ for details on the
copyright holders.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
 * Copyright (c) Members of the EGEE Collaboration. 2004.
 * See http://eu-egee.org/partners/ for details on the copyright holders.
 * For license conditions see the license file or http://eu-egee.org/license.html
 */
/*
 * 
 * Authors: Marco Sottilaro (marco.sottilaro@datamat.it)
 */


package org.glite.wms.wmproxy;

/**
 * This exception is thrown to indicate that an authentication error occurs while accessing the WMProxy services.
 * An authentication error can happen, for example, when the user credentials is either invalid or expired.
*/

public class AuthenticationFaultException extends BaseException {
	/**
	* Constructs an Exception with no specified detail message.
	*/
	public AuthenticationFaultException () {
		super();
	}

	/**
	* Constructs an Exception with the specified detail message.
	*/
	public AuthenticationFaultException(String message) {
		super(message);
	}
}
