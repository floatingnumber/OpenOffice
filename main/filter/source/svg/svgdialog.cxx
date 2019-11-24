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
#include "precompiled_filter.hxx"

#include "svgdialog.hxx" 
#include "impsvgdialog.hxx" 

#include <vcl/svapp.hxx>
#include <vcl/dialog.hxx>

#ifndef _COM_SUN_STAR_VIEW_XRENDERABLE_HPP_
#include <com/sun/star/view/XRenderable.hpp>
#endif
#ifndef	_COM_SUN_STAR_FRAME_XCONTROLLER_HPP_
#include <com/sun/star/frame/XController.hpp>
#endif
#ifndef _COM_SUN_STAR_VIEW_XSELECTIONSUPPLIER_HPP_
#include <com/sun/star/view/XSelectionSupplier.hpp>
#endif

#define SVG_DIALOG_SERVICE_NAME         "com.sun.star.comp.Draw.SVGFilterDialog"
#define SVG_DIALOG_IMPLEMENTATION_NAME  SVG_DIALOG_SERVICE_NAME
#define SVG_FILTER_DATA_NAME            "FilterData"

using namespace ::rtl;
using namespace ::vcl;
using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::frame;
using namespace ::com::sun::star::view;
using namespace ::com::sun::star::document;

// -----------------------
// - SVGDialog functions -
// -----------------------

OUString SVGDialog_getImplementationName ()
	throw (RuntimeException)
{
	return OUString ( RTL_CONSTASCII_USTRINGPARAM ( SVG_DIALOG_IMPLEMENTATION_NAME ) );
}

// -----------------------------------------------------------------------------

Sequence< sal_Int8 > SAL_CALL SVGDialog_getImplementationId() 
    throw(RuntimeException)
{
    static const ::cppu::OImplementationId aId;
    
    return( aId.getImplementationId() );
}

// -----------------------------------------------------------------------------

Sequence< OUString > SAL_CALL SVGDialog_getSupportedServiceNames() 
    throw (RuntimeException)
{
	Sequence< OUString > aRet( 1 );
    
    aRet.getArray()[ 0 ] = OUString ( RTL_CONSTASCII_USTRINGPARAM ( SVG_DIALOG_SERVICE_NAME ) );
    
    return aRet;
}

// -----------------------------------------------------------------------------

Reference< XInterface > SAL_CALL SVGDialog_createInstance( const Reference< XMultiServiceFactory > & rSMgr )
    throw( Exception )
{
    return( static_cast< cppu::OWeakObject* >( new SVGDialog( rSMgr ) ) );
}

// -------------
// - SVGDialog -
// -------------

SVGDialog::SVGDialog( const Reference< XMultiServiceFactory > &rxMSF ) :
    OGenericUnoDialog( rxMSF )
{
	ByteString aResMgrName( "svgfilter" );

	aResMgrName.Append( ByteString::CreateFromInt32( 0/*SOLARUPD*/ ) );
	
    // !!! KA: enable ResMgr
    // mapResMgr.reset( ResMgr::CreateResMgr( aResMgrName.GetBuffer(), Application::GetSettings().GetUILocale() ) );
}

// -----------------------------------------------------------------------------

SVGDialog::~SVGDialog()
{
}

// -----------------------------------------------------------------------------

Any SAL_CALL SVGDialog::queryInterface( const Type& rType ) 
    throw (RuntimeException)
{
    Any aReturn( OGenericUnoDialog::queryInterface( rType ) );

	if( !aReturn.hasValue() )
    {   
        aReturn = ::cppu::queryInterface( rType, static_cast< XPropertyAccess* >( this ),
                                                 static_cast< XExporter* >( this ) );
    }

    return( aReturn );
}

// -----------------------------------------------------------------------------

void SAL_CALL SVGDialog::acquire() 
    throw ()
{
    OWeakObject::acquire();
}

// -----------------------------------------------------------------------------

void SAL_CALL SVGDialog::release() 
    throw ()
{
    OWeakObject::release();
}

// -----------------------------------------------------------------------------

Sequence< sal_Int8 > SAL_CALL SVGDialog::getImplementationId() 
    throw(RuntimeException)
{
    return SVGDialog_getImplementationId();
}

// -----------------------------------------------------------------------------

OUString SAL_CALL SVGDialog::getImplementationName() 
	throw (RuntimeException)
{
	return SVGDialog_getImplementationName();
}

// -----------------------------------------------------------------------------

Sequence< OUString > SAL_CALL SVGDialog::getSupportedServiceNames()
    throw (RuntimeException)
{
    return SVGDialog_getSupportedServiceNames();
}

// -----------------------------------------------------------------------------

Dialog*	SVGDialog::createDialog( Window* pParent )
{
    return( ( /*KA: *mapResMgr.get() &&*/ mxSrcDoc.is() ) ? 
            new ImpSVGDialog( pParent/*KA: , *mapResMgr*/, maFilterData ) : 
            NULL );
}

// -----------------------------------------------------------------------------

void SVGDialog::executedDialog( sal_Int16 nExecutionResult )
{
    if( nExecutionResult && m_pDialog )
        maFilterData = static_cast< ImpSVGDialog* >( m_pDialog )->GetFilterData();

    destroyDialog();
}

// -----------------------------------------------------------------------------

Reference< XPropertySetInfo > SAL_CALL SVGDialog::getPropertySetInfo() 
    throw(RuntimeException)
{
    return( Reference< XPropertySetInfo >( createPropertySetInfo( getInfoHelper() ) ) );
}

//-------------------------------------------------------------------------
::cppu::IPropertyArrayHelper& SVGDialog::getInfoHelper()
{
    return( *const_cast< SVGDialog *>(this)->getArrayHelper() );
}

//------------------------------------------------------------------------------
::cppu::IPropertyArrayHelper* SVGDialog::createArrayHelper() const
{
	Sequence< Property > aProps;
	
    describeProperties(aProps);
	
    return new ::cppu::OPropertyArrayHelper( aProps );
}

// -----------------------------------------------------------------------------

Sequence< PropertyValue > SAL_CALL SVGDialog::getPropertyValues()
    throw ( RuntimeException )
{
    sal_Int32 i, nCount;

    for( i = 0, nCount = maMediaDescriptor.getLength(); i < nCount; ++i )
	{
        if( maMediaDescriptor[ i ].Name.equalsAscii( SVG_FILTER_DATA_NAME ) ) 
            break;
	}

    if( i == nCount )
    {        
		maMediaDescriptor.realloc( ++nCount );
        maMediaDescriptor[ i ].Name = String( RTL_CONSTASCII_USTRINGPARAM( SVG_FILTER_DATA_NAME ) );
    }
    
	maMediaDescriptor[ i ].Value <<= maFilterData;
    
    return( maMediaDescriptor );
}

// -----------------------------------------------------------------------------

void SAL_CALL SVGDialog::setPropertyValues( const Sequence< PropertyValue >& rProps ) 
    throw ( UnknownPropertyException, PropertyVetoException, IllegalArgumentException, WrappedTargetException, RuntimeException )
{
    maMediaDescriptor = rProps;

	for( sal_Int32 i = 0, nCount = maMediaDescriptor.getLength(); i < nCount; i++ )
	{
        if( maMediaDescriptor[ i ].Name.equalsAscii( SVG_FILTER_DATA_NAME ) ) 
		{
			maMediaDescriptor[ i ].Value >>= maFilterData;
			break;
		}
	}
}

// -----------------------------------------------------------------------------

void SAL_CALL SVGDialog::setSourceDocument( const Reference< XComponent >& xDoc ) 
    throw(IllegalArgumentException, RuntimeException)
{
    mxSrcDoc = xDoc;
}
