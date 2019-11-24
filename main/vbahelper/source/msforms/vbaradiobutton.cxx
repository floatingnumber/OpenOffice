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



#include "vbaradiobutton.hxx"
#include "vbanewfont.hxx"

using namespace com::sun::star;
using namespace ooo::vba;


const static rtl::OUString LABEL( RTL_CONSTASCII_USTRINGPARAM("Label") );
const static rtl::OUString STATE( RTL_CONSTASCII_USTRINGPARAM("State") );
ScVbaRadioButton::ScVbaRadioButton( const uno::Reference< XHelperInterface >& xParent, const uno::Reference< uno::XComponentContext >& xContext, const uno::Reference< uno::XInterface >& xControl, const uno::Reference< frame::XModel >& xModel, AbstractGeometryAttributes* pGeomHelper ) : RadioButtonImpl_BASE( xParent, xContext, xControl, xModel, pGeomHelper )
{
}

// Attributes
rtl::OUString SAL_CALL 
ScVbaRadioButton::getCaption() throw (css::uno::RuntimeException)
{
    rtl::OUString Label;
    m_xProps->getPropertyValue( LABEL ) >>= Label;
    return Label;
}

void SAL_CALL 
ScVbaRadioButton::setCaption( const rtl::OUString& _caption ) throw (::com::sun::star::uno::RuntimeException)
{
    m_xProps->setPropertyValue( LABEL, uno::makeAny( _caption ) );
}

uno::Any SAL_CALL 
ScVbaRadioButton::getValue() throw (css::uno::RuntimeException)
{
    sal_Int16 nValue = -1;
    m_xProps->getPropertyValue( STATE ) >>= nValue;
    if( nValue != 0 )
        nValue = -1;
//    return uno::makeAny( nValue ); 
// I must be missing something MSO says value should be -1 if selected, 0 if not
// selected
    return uno::makeAny( ( nValue == -1 ) ? sal_True : sal_False ); 

}

void SAL_CALL 
ScVbaRadioButton::setValue( const uno::Any& _value ) throw (uno::RuntimeException)
{
    sal_Int16 nValue = 0;
    sal_Bool bValue = sal_False;
    if( _value >>= nValue )
    {
        if( nValue == -1)
        nValue = 1;
    }
    else if ( _value >>= bValue )
    {
        if ( bValue )
            nValue = 1;
    }
    m_xProps->setPropertyValue( STATE, uno::makeAny( nValue ) );
}

uno::Reference< msforms::XNewFont > SAL_CALL ScVbaRadioButton::getFont() throw (uno::RuntimeException)
{
    return new VbaNewFont( this, mxContext, m_xProps );
}

rtl::OUString&
ScVbaRadioButton::getServiceImplName()
{
	static rtl::OUString sImplName( RTL_CONSTASCII_USTRINGPARAM("ScVbaRadioButton") );
	return sImplName;
}

uno::Sequence< rtl::OUString > 
ScVbaRadioButton::getServiceNames()
{
	static uno::Sequence< rtl::OUString > aServiceNames;
	if ( aServiceNames.getLength() == 0 )
	{
		aServiceNames.realloc( 1 );
		aServiceNames[ 0 ] = rtl::OUString( RTL_CONSTASCII_USTRINGPARAM("ooo.vba.msforms.RadioButton" ) );
	}
	return aServiceNames;
}
