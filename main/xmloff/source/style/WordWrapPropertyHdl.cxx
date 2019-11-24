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
#include "precompiled_xmloff.hxx"
#include <xmloff/xmlimp.hxx>
#include <xmloff/WordWrapPropertyHdl.hxx>
#include <xmloff/xmltoken.hxx>
#include <xmloff/xmluconv.hxx>
#include <comphelper/extract.hxx>
#include <rtl/ustring.hxx>
#include <rtl/ustrbuf.hxx>
#include <com/sun/star/uno/Any.hxx>

using ::rtl::OUString;
using ::rtl::OUStringBuffer;

using namespace ::com::sun::star::uno;

///////////////////////////////////////////////////////////////////////////////
//
// class XMLWordWrapPropertyHdl
//

XMLWordWrapPropertyHdl::XMLWordWrapPropertyHdl( SvXMLImport* pImport )
: mpImport( pImport )
{
}

XMLWordWrapPropertyHdl::~XMLWordWrapPropertyHdl()
{
	// Nothing to do
}

sal_Bool XMLWordWrapPropertyHdl::importXML( const OUString& rStrImpValue, Any& rValue, const SvXMLUnitConverter& ) const
{
	sal_Bool bValue = sal_False, bRetValue = sal_False;
	if( rStrImpValue == GetXMLToken( xmloff::token::XML_WRAP ) )
	{
		bValue = sal_True;
		bRetValue = sal_True;
	}
	if( rStrImpValue == GetXMLToken( xmloff::token::XML_NO_WRAP ) )
	{
		bValue = sal_False;
		bRetValue = sal_True;
	}
	if ( bRetValue && mpImport )
	{
		sal_Int32 nUPD, nBuildId;
		if( mpImport->getBuildIds( nUPD, nBuildId ) )
		{
		    if( nUPD == 300 )
		    {
			    if( ( nBuildId > 0 ) && (nBuildId < 9316 ) )
					bValue = bValue ? sal_False : sal_True;		// treat OOo 3.0 beta1 as OOo 2.x
		    }
            else if( ( nUPD == 680 ) || ( nUPD >= 640 && nUPD <= 645 ) )
				bValue = bValue ? sal_False : sal_True;
		}
		rValue <<= bValue;
	}
	return bRetValue;
}

sal_Bool XMLWordWrapPropertyHdl::exportXML( OUString& rStrExpValue, const Any& rValue, const SvXMLUnitConverter& ) const
{
	if( ::cppu::any2bool( rValue ) )
	{
		rStrExpValue = GetXMLToken( xmloff::token::XML_WRAP );
	}
	else
	{
		rStrExpValue = GetXMLToken( xmloff::token::XML_NO_WRAP );
	}
	return sal_True;
}

