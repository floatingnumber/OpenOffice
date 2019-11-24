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



#ifndef _SIGNATUREENGINE_HXX
#define _SIGNATUREENGINE_HXX

#include <com/sun/star/xml/crypto/sax/XReferenceResolvedListener.hpp>
#include <com/sun/star/xml/crypto/sax/XReferenceResolvedBroadcaster.hpp>
#ifndef _COM_SUN_STAR_XML_CRYPTO_SAX_XSIGNATURECOLLECTOR_HPP_
#include <com/sun/star/xml/crypto/sax/XReferenceCollector.hpp>
#endif
#include <com/sun/star/xml/crypto/sax/XKeyCollector.hpp>
#include <com/sun/star/xml/crypto/sax/XMissionTaker.hpp>
#include <com/sun/star/xml/crypto/sax/XSAXEventKeeper.hpp>
#include <com/sun/star/xml/crypto/XXMLSecurityContext.hpp>
#include <com/sun/star/xml/crypto/XXMLSignature.hpp>
#include <com/sun/star/xml/crypto/XUriBinding.hpp>
#include <com/sun/star/io/XInputStream.hpp>
                                       
#include <cppuhelper/implbase2.hxx>

#include "securityengine.hxx"

#ifndef INCLUDED_VECTOR
#include <vector>
#define INCLUDED_VECTOR
#endif

class SignatureEngine : public cppu::ImplInheritanceHelper2
<
	SecurityEngine, 
	com::sun::star::xml::crypto::sax::XReferenceCollector,
	com::sun::star::xml::crypto::XUriBinding
>
/****** signatureengine.hxx/CLASS SignatureEngine *****************************
 *
 *   NAME
 *	SignatureEngine -- Base class of SignatureCreator and SignatureVerifier
 *
 *   FUNCTION
 *	Maintains common members and methods related with signature operation.
 *
 *   HISTORY
 *	05.01.2004 -	Interface supported: XReferenceCollector
 *
 *   AUTHOR
 *	Michael Mi
 *	Email: michael.mi@sun.com
 ******************************************************************************/
{
protected:
	
	/*
	 * the Signature bridge component, which performs signature generation
	 * and verification based on xmlsec library.
	 */
	com::sun::star::uno::Reference< 
		com::sun::star::xml::crypto::XXMLSignature > m_xXMLSignature;
		
	/*
	 * a collection of ElementCollector's ids. Each ElementCollector
	 * represents one element signed by this signature.
	 */
	std::vector< sal_Int32 > m_vReferenceIds;

	/*
	 * remembers how many references this signature has.
	 */
	sal_Int32 m_nTotalReferenceNumber;

	/*
	 * a collection of Uri binding.
	 *
	 * the m_vUris is used to hold the Uri strings, and the m_vXInputStreams is used
	 * to hold corresponding binded XInputStream interface.
	 */
	std::vector< rtl::OUString > m_vUris;
	std::vector< com::sun::star::uno::Reference<
		com::sun::star::io::XInputStream > > m_vXInputStreams;

protected:
	SignatureEngine( );
	virtual ~SignatureEngine() {};
	
	virtual void tryToPerform( ) 
		throw (com::sun::star::uno::Exception, com::sun::star::uno::RuntimeException);
	virtual void clearUp( ) const;	
	virtual bool checkReady() const;
	
	/*
	 * starts the main function. This method will be implemented by any sub-class.
	 * For a SignatureCreator, it performs signing operation;
	 * for a SignatureVerifier, verification operation is performed.
	 */
	virtual void startEngine( const com::sun::star::uno::Reference< 
                              com::sun::star::xml::crypto::XXMLSignatureTemplate >&)
		throw (com::sun::star::uno::Exception, com::sun::star::uno::RuntimeException)
		{};
	
public:
	/* XReferenceCollector */
	virtual void SAL_CALL setReferenceCount( sal_Int32 count )
		throw (com::sun::star::uno::Exception, com::sun::star::uno::RuntimeException);
    		
	virtual void SAL_CALL setReferenceId( sal_Int32 id )
		throw (com::sun::star::uno::Exception, com::sun::star::uno::RuntimeException);

	/* XUriBinding */
	virtual void SAL_CALL setUriBinding(
		const rtl::OUString& uri,
		const com::sun::star::uno::Reference<
			com::sun::star::io::XInputStream >& aInputStream )
		throw (com::sun::star::uno::Exception, com::sun::star::uno::RuntimeException);
	virtual com::sun::star::uno::Reference< com::sun::star::io::XInputStream >
		SAL_CALL getUriBinding( const rtl::OUString& uri )
		throw (com::sun::star::uno::Exception, com::sun::star::uno::RuntimeException);
};

#endif

