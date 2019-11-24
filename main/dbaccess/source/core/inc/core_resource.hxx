/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifndef _DBA_CORE_RESOURCE_HXX_
#define _DBA_CORE_RESOURCE_HXX_

#ifndef _RTL_USTRING_HXX_
#include <rtl/ustring.hxx>
#endif
#include <osl/mutex.hxx>

class ResMgr;
//.........................................................................
namespace dbaccess
{

#define DBA_RES( id )                       ResourceManager::loadString( id )
#define DBA_RES_PARAM( id, ascii, replace ) ResourceManager::loadString( id, ascii, replace )

#define DBACORE_RESSTRING( id ) DBA_RES( id )
        // (compatibility)

	//==================================================================
	//= ResourceManager
	//= handling ressources within the DBA-Core library
	//==================================================================
	class ResourceManager
	{
        friend class OModuleClient;
        static ::osl::Mutex	s_aMutex;		/// access safety
        static sal_Int32	s_nClients;		/// number of registered clients
		static ResMgr*	m_pImpl;        

	private:
		// no instantiation allowed
		ResourceManager() { }
		~ResourceManager() { }

	protected:
		static void ensureImplExists();
	    /// register a client for the module
	    static void	registerClient();
	    /// revoke a client for the module
	    static void	revokeClient();

	public:
		/** loads the string with the specified resource id
		*/
		static ::rtl::OUString  loadString(sal_uInt16 _nResId);

        /** loads a string from the resource file, substituting a placeholder with a given string

            @param  _nResId
                the resource ID of the string to load
            @param  _pPlaceholderAscii
                the ASCII representation of the placeholder string
            @param  _rReplace
                the string which should substitute the placeholder
        */
        static ::rtl::OUString  loadString(
                sal_uInt16              _nResId,
                const sal_Char*         _pPlaceholderAscii,
                const ::rtl::OUString&  _rReplace
        );

        /** loads a string from the resource file, substituting two placeholders with given strings

            @param  _nResId
                the resource ID of the string to load
            @param  _pPlaceholderAscii1
                the ASCII representation of the first placeholder string
            @param  _rReplace1
                the string which should substitute the first placeholder
            @param  _pPlaceholderAscii2
                the ASCII representation of the second placeholder string
            @param  _rReplace2
                the string which should substitute the second placeholder
        */
        static ::rtl::OUString  loadString(
                sal_uInt16              _nResId,
                const sal_Char*         _pPlaceholderAscii1,
                const ::rtl::OUString&  _rReplace1,
                const sal_Char*         _pPlaceholderAscii2,
                const ::rtl::OUString&  _rReplace2
        );
	};

    //=========================================================================
    //= OModuleClient
    //=========================================================================
    /** base class for objects which uses any global module-specific ressources 
    */
    class OModuleClient
    {
    public:
	    OModuleClient()		{ ResourceManager::registerClient(); }
	    ~OModuleClient()	{ ResourceManager::revokeClient(); }
    };


//.........................................................................
}
//.........................................................................

#endif // _DBA_CORE_RESOURCE_HXX_

