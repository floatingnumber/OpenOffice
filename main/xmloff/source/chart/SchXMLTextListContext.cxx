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

#include "SchXMLImport.hxx"
#include "SchXMLTextListContext.hxx"
#include "SchXMLParagraphContext.hxx"

#include "xmloff/xmlnmspe.hxx"
#include <xmloff/xmltoken.hxx>
#include <xmloff/nmspmap.hxx>

using ::rtl::OUString;
using ::com::sun::star::uno::Sequence;
using ::com::sun::star::uno::Reference;
using namespace com::sun::star;
using namespace ::xmloff::token;

//-------------------------------------------------
class SchXMLListItemContext : public SvXMLImportContext
{
public:
	SchXMLListItemContext( SvXMLImport& rImport, const OUString& rLocalName, OUString& rText );
	virtual ~SchXMLListItemContext();
	virtual void StartElement( const Reference< xml::sax::XAttributeList >& xAttrList );
	virtual void EndElement();

	virtual SvXMLImportContext* CreateChildContext(
		sal_uInt16 nPrefix,
		const ::rtl::OUString& rLocalName,
		const com::sun::star::uno::Reference< com::sun::star::xml::sax::XAttributeList >& xAttrList );

private:
    ::rtl::OUString& m_rText;
};

SchXMLListItemContext::SchXMLListItemContext(
        SvXMLImport& rImport
        , const OUString& rLocalName
		, OUString& rText )
        : SvXMLImportContext( rImport, XML_NAMESPACE_TEXT, rLocalName )
        , m_rText( rText )
{
}

SchXMLListItemContext::~SchXMLListItemContext()
{}

void SchXMLListItemContext::StartElement( const Reference< xml::sax::XAttributeList >& /*xAttrList*/ )
{
}

void SchXMLListItemContext::EndElement()
{
}

SvXMLImportContext* SchXMLListItemContext::CreateChildContext(
    sal_uInt16 nPrefix, const OUString& rLocalName,
    const uno::Reference< xml::sax::XAttributeList >& )
{
	SvXMLImportContext* pContext = 0;
	if( nPrefix == XML_NAMESPACE_TEXT && IsXMLToken( rLocalName, XML_P ) )
		pContext = new SchXMLParagraphContext( GetImport(), rLocalName, m_rText );
	else
		pContext = new SvXMLImportContext( GetImport(), nPrefix, rLocalName );
	return pContext;
}

//-------------------------------------------------

SchXMLTextListContext::SchXMLTextListContext(
        SvXMLImport& rImport
        , const OUString& rLocalName
		, Sequence< OUString>& rTextList )
        : SvXMLImportContext( rImport, XML_NAMESPACE_TEXT, rLocalName )
        , m_rTextList( rTextList )
        , m_aTextVector()
{
}

SchXMLTextListContext::~SchXMLTextListContext()
{
}

void SchXMLTextListContext::StartElement( const Reference< xml::sax::XAttributeList >& /*xAttrList*/ )
{
}

void SchXMLTextListContext::EndElement()
{
    sal_Int32 nCount = m_aTextVector.size();
    m_rTextList.realloc(nCount);
    for( sal_Int32 nN=0; nN<nCount; nN++ )
        m_rTextList[nN]=m_aTextVector[nN];
}

SvXMLImportContext* SchXMLTextListContext::CreateChildContext(
    sal_uInt16 nPrefix, const OUString& rLocalName,
    const uno::Reference< xml::sax::XAttributeList >& )
{
	SvXMLImportContext* pContext = 0;
	if( nPrefix == XML_NAMESPACE_TEXT && IsXMLToken( rLocalName, XML_LIST_ITEM ) )
    {
        m_aTextVector.push_back( OUString() );
    	pContext = new SchXMLListItemContext( GetImport(), rLocalName, m_aTextVector.back() );
    }
	else
		pContext = new SvXMLImportContext( GetImport(), nPrefix, rLocalName );
	return pContext;
}
