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
#include "CreateElemTContext.hxx"
#include "MutableAttrList.hxx"
#ifndef _XMLOFF_TRANSFORMERBASE_HXX
#include "TransformerBase.hxx"
#endif
#include "TransformerActions.hxx"
#include "TContextVector.hxx"
#include "FlatTContext.hxx"
#include "AttrTransformerAction.hxx"
#include <xmloff/nmspmap.hxx>

using ::rtl::OUString;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::xml::sax;
using namespace ::xmloff::token;

TYPEINIT1( XMLCreateElemTransformerContext, XMLTransformerContext );

XMLCreateElemTransformerContext::XMLCreateElemTransformerContext( 
		XMLTransformerBase& rImp, 
		const OUString& rQName,
	   sal_uInt16 nActionMap ) :
	XMLTransformerContext( rImp, rQName ),
	m_nActionMap( nActionMap )
{
}

XMLCreateElemTransformerContext::~XMLCreateElemTransformerContext()
{
}

void XMLCreateElemTransformerContext::StartElement( 
		const Reference< XAttributeList >& rAttrList )
{
	Reference< XAttributeList > xAttrList( rAttrList );

	XMLTransformerContextVector aChildContexts;

	XMLMutableAttributeList *pMutableAttrList = 0;
	XMLTransformerActions *pActions = 
		GetTransformer().GetUserDefinedActions( m_nActionMap );
	OSL_ENSURE( pActions, "go no actions" );
	if( pActions )
	{
		sal_Int16 nAttrCount = xAttrList.is() ? xAttrList->getLength() : 0;
		for( sal_Int16 i=0; i < nAttrCount; ++i )
		{
			const OUString& rAttrName = xAttrList->getNameByIndex( i );
			const OUString& rAttrValue = xAttrList->getValueByIndex( i );
			OUString aLocalName;
			sal_uInt16 nPrefix = 
				GetTransformer().GetNamespaceMap().GetKeyByAttrName( rAttrName, 
														   &aLocalName );

			XMLTransformerActions::key_type aKey( nPrefix, aLocalName );
			XMLTransformerActions::const_iterator aIter =
					pActions->find( aKey );
			if( !(aIter == pActions->end() ) )
			{
				if( !pMutableAttrList )
				{
					pMutableAttrList = new XMLMutableAttributeList( xAttrList );
					xAttrList = pMutableAttrList;
				}
				sal_uInt32 nAction = (*aIter).second.m_nActionType;
				switch( nAction )
				{
				case XML_ATACTION_MOVE_TO_ELEM:
					{
						OUString aElemQName( 
							GetTransformer().GetNamespaceMap().GetQNameByKey( 
								(*aIter).second.GetQNamePrefixFromParam1(), 
								::xmloff::token::GetXMLToken( 
								(*aIter).second.GetQNameTokenFromParam1()) ) );
						XMLTransformerContext *pContext = 
							new XMLPersTextContentTContext( GetTransformer(), 
													   aElemQName );
						pContext->Characters( rAttrValue );
						XMLTransformerContextVector::value_type aVal( 
								pContext );
						aChildContexts.push_back( aVal );
						pMutableAttrList->RemoveAttributeByIndex( i );
						--i;
						--nAttrCount;
					}
					break;
				default:
					OSL_ENSURE( sal_False, "unknown action" );
					break;
				}
			}
		}
	}
	XMLTransformerContext::StartElement( xAttrList );

	XMLTransformerContextVector::iterator aIter = aChildContexts.begin();

	for( ; aIter != aChildContexts.end(); ++aIter )
	{
		(*aIter)->Export();
	}
}
