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

#include <tools/debug.hxx>
#include <com/sun/star/drawing/XLayerSupplier.hpp>
#include <com/sun/star/container/XIndexAccess.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <xmloff/xmltoken.hxx>
#include "xmloff/xmlnmspe.hxx"
#include <xmloff/xmlexp.hxx>
#include <xmloff/xmlement.hxx>
#include <xmloff/nmspmap.hxx>
#include "layerexp.hxx"

using ::rtl::OUString;
using ::rtl::OUStringBuffer;
using ::com::sun::star::uno::Reference;

using namespace ::cppu;
using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::drawing;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::container;
using namespace ::xmloff::token;

void SdXMLayerExporter::exportLayer( SvXMLExport& rExport )
{
	Reference< XLayerSupplier > xLayerSupplier( rExport.GetModel(), UNO_QUERY );
	if( !xLayerSupplier.is() )
		return;

	Reference< XIndexAccess > xLayerManager( xLayerSupplier->getLayerManager(), UNO_QUERY );
	if( !xLayerManager.is() )
		return;

	const sal_Int32 nCount = xLayerManager->getCount();
	if( nCount == 0 )
		return;

	const OUString strName( RTL_CONSTASCII_USTRINGPARAM( "Name" ) );
	const OUString strTitle( RTL_CONSTASCII_USTRINGPARAM( "Title" ) );
	const OUString strDescription( RTL_CONSTASCII_USTRINGPARAM( "Description" ) );

	OUString sTmp;

	SvXMLElementExport aElem( rExport, XML_NAMESPACE_DRAW, XML_LAYER_SET, sal_True, sal_True );

	for( sal_Int32 nIndex = 0; nIndex < nCount; nIndex++ )
	{
		try
		{
			Reference< XPropertySet> xLayer( xLayerManager->getByIndex( nIndex ), UNO_QUERY_THROW );
			xLayer->getPropertyValue( strName ) >>= sTmp;
			if(sTmp.getLength())
				rExport.AddAttribute( XML_NAMESPACE_DRAW, XML_NAME, sTmp );

			SvXMLElementExport aEle( rExport, XML_NAMESPACE_DRAW, XML_LAYER, sal_True, sal_True );

			// title property (as <svg:title> element)
			xLayer->getPropertyValue(strTitle) >>= sTmp;
			if(sTmp.getLength())
			{
				SvXMLElementExport aEventElemt(rExport, XML_NAMESPACE_SVG, XML_TITLE, sal_True, sal_False);
				rExport.Characters(sTmp);
			}

			// description property (as <svg:desc> element)
			xLayer->getPropertyValue(strDescription) >>= sTmp;
			if(sTmp.getLength() > 0)
			{
				SvXMLElementExport aDesc(rExport, XML_NAMESPACE_SVG, XML_DESC, sal_True, sal_False);
				rExport.Characters(sTmp);
			}
		}	  
		catch( Exception& )	
		{
			DBG_ERROR("SdXMLayerExporter::exportLayer(), exception caught during export of one layer!");
		}	  
	}
}
