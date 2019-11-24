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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_ftp.hxx"

/**************************************************************************
								TODO
 **************************************************************************

 *************************************************************************/
#include "ftpcontentidentifier.hxx"
#include "ftpcontentprovider.hxx"

using namespace ftp;
using namespace com::sun::star::uno;
using namespace com::sun::star::ucb;
using namespace com::sun::star::lang;


FTPContentIdentifier::FTPContentIdentifier(
	const rtl::OUString& ident
)
	: m_ident(ident)
{
}


FTPContentIdentifier::~FTPContentIdentifier()
{
}


Any SAL_CALL
FTPContentIdentifier::queryInterface(
	const Type& rType
) 
	throw( 
		RuntimeException 
	)
{
	Any aRet = 
		::cppu::queryInterface(rType,
							   SAL_STATIC_CAST(XTypeProvider*,this),
							   SAL_STATIC_CAST(XContentIdentifier*,this));
	
	return aRet.hasValue() ? aRet : OWeakObject::queryInterface( rType );
}


void SAL_CALL FTPContentIdentifier::acquire( void ) throw() {
	OWeakObject::acquire();
}


void SAL_CALL FTPContentIdentifier::release( void ) throw() {
	OWeakObject::release();
}


Sequence<sal_Int8> SAL_CALL
FTPContentIdentifier::getImplementationId()
	throw(RuntimeException)
{
	static cppu::OImplementationId* pId = NULL;
	if(!pId)
    {
		osl::Guard< osl::Mutex > aGuard( osl::Mutex::getGlobalMutex() );
		if ( !pId ) 
		{
			static cppu::OImplementationId id( sal_False );
			pId = &id;
		}
    }
	return (*pId).getImplementationId();
}


Sequence<Type> SAL_CALL
FTPContentIdentifier::getTypes(
	void )
	throw(RuntimeException)
{
	static cppu::OTypeCollection* pCollection = NULL;
	if ( !pCollection ) {
		osl::Guard< osl::Mutex > aGuard( osl::Mutex::getGlobalMutex() );
		if ( !pCollection )
		{
			static cppu::OTypeCollection collection(
				getCppuType(
					static_cast<Reference<XTypeProvider>*>(0)),
				getCppuType(
					static_cast<Reference<XContentIdentifier>*>(0)));
			pCollection = &collection;
		}
	}
	return (*pCollection).getTypes();
}


rtl::OUString SAL_CALL
FTPContentIdentifier::getContentIdentifier( 
) 
	throw (
		com::sun::star::uno::RuntimeException
	)
{
	return m_ident;
}


rtl::OUString SAL_CALL
FTPContentIdentifier::getContentProviderScheme(  
) 
	throw (
		com::sun::star::uno::RuntimeException
	)
{
	return rtl::OUString::createFromAscii("ftp");
}






