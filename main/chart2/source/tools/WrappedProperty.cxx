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
#include "precompiled_chart2.hxx"

#include "WrappedProperty.hxx"
#include "macros.hxx"
#include <com/sun/star/drawing/LineStyle.hpp>

using namespace ::com::sun::star;
using ::com::sun::star::uno::Any;
using ::com::sun::star::uno::Reference;
using ::rtl::OUString;


//.............................................................................
namespace chart
{
//.............................................................................

WrappedProperty::WrappedProperty( const OUString& rOuterName, const OUString& rInnerName)
                         : m_aOuterName( rOuterName )
                         , m_aInnerName( rInnerName )
{
}
WrappedProperty::~WrappedProperty()
{
}

const OUString& WrappedProperty::getOuterName() const
{
    return m_aOuterName;
}

//virtual
::rtl::OUString WrappedProperty::getInnerName() const
{
    return m_aInnerName;
}

Any WrappedProperty::convertInnerToOuterValue( const Any& rInnerValue ) const
{
    return rInnerValue;
}
Any WrappedProperty::convertOuterToInnerValue( const Any& rOuterValue ) const
{
    return rOuterValue;
}

void WrappedProperty::setPropertyValue( const Any& rOuterValue, const Reference< beans::XPropertySet >& xInnerPropertySet ) const
                throw (beans::UnknownPropertyException, beans::PropertyVetoException, lang::IllegalArgumentException, lang::WrappedTargetException, uno::RuntimeException)
{
    if(xInnerPropertySet.is())
        xInnerPropertySet->setPropertyValue( this->getInnerName(), this->convertOuterToInnerValue( rOuterValue ) );
}

Any WrappedProperty::getPropertyValue( const Reference< beans::XPropertySet >& xInnerPropertySet ) const
                        throw (beans::UnknownPropertyException, lang::WrappedTargetException, uno::RuntimeException)
{
    Any aRet;
    if( xInnerPropertySet.is() )
    {
        aRet = xInnerPropertySet->getPropertyValue( this->getInnerName() );
        aRet = this->convertInnerToOuterValue( aRet );
    }
    return aRet;
}

void WrappedProperty::setPropertyToDefault( const Reference< beans::XPropertyState >& xInnerPropertyState ) const
                        throw (::com::sun::star::beans::UnknownPropertyException, ::com::sun::star::uno::RuntimeException)
{
    if( xInnerPropertyState.is() && !this->getInnerName().isEmpty() )
        xInnerPropertyState->setPropertyToDefault(this->getInnerName());
    else
    {
        Reference< beans::XPropertySet > xInnerProp( xInnerPropertyState, uno::UNO_QUERY );
        setPropertyValue( getPropertyDefault( xInnerPropertyState ), xInnerProp );
    }
}

Any WrappedProperty::getPropertyDefault( const Reference< beans::XPropertyState >& xInnerPropertyState ) const
                        throw (beans::UnknownPropertyException, lang::WrappedTargetException, uno::RuntimeException)
{
    Any aRet;
    if( xInnerPropertyState.is() )
    {
        aRet = xInnerPropertyState->getPropertyDefault( this->getInnerName() );
        aRet = this->convertInnerToOuterValue( aRet );    
    }
    return aRet;
}

beans::PropertyState WrappedProperty::getPropertyState( const Reference< beans::XPropertyState >& xInnerPropertyState ) const
                        throw (beans::UnknownPropertyException, uno::RuntimeException)
{
    beans::PropertyState aState = beans::PropertyState_DIRECT_VALUE;
    rtl::OUString aInnerName( this->getInnerName() );
    if( xInnerPropertyState.is() && !aInnerName.isEmpty() )
        aState = xInnerPropertyState->getPropertyState( aInnerName );
    else
    {
        try
        {
            Reference< beans::XPropertySet > xInnerProp( xInnerPropertyState, uno::UNO_QUERY );
            uno::Any aValue = this->getPropertyValue( xInnerProp );
            if( !aValue.hasValue() )
                aState = beans::PropertyState_DEFAULT_VALUE;
            else
            {
                uno::Any aDefault = this->getPropertyDefault( xInnerPropertyState );
                if( aValue == aDefault )
                    aState = beans::PropertyState_DEFAULT_VALUE;
            }
        }
        catch( beans::UnknownPropertyException& ex )
        {
            ASSERT_EXCEPTION( ex );
        }
    }
    return aState;
}

//.............................................................................
} //namespace chart
//.............................................................................
