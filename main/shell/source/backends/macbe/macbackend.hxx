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



#ifndef _MACBACKEND_HXX_
#define _MACBACKEND_HXX_

#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <cppuhelper/implbase2.hxx>
#include <rtl/string.hxx>

// FIXME: stdio.h only for debugging...
#include <stdio.h>

namespace css = com::sun::star;
namespace uno = css::uno;
namespace lang = css::lang;

class MacOSXBackend : public ::cppu::WeakImplHelper2 <css::beans::XPropertySet, lang::XServiceInfo >
{

public:

    static MacOSXBackend* createInstance();

    // XServiceInfo
    virtual rtl::OUString SAL_CALL getImplementationName()
        throw (uno::RuntimeException);

    virtual sal_Bool SAL_CALL supportsService(const rtl::OUString& aServiceName)
        throw (uno::RuntimeException);

    virtual uno::Sequence<rtl::OUString> SAL_CALL getSupportedServiceNames()
        throw (uno::RuntimeException);

    /**
       Provides the implementation name.

       @return   implementation name
    */
    static rtl::OUString SAL_CALL getBackendName(void);

    /**
       Provides the supported services names

       @return   service names
    */
    static uno::Sequence<rtl::OUString> SAL_CALL getBackendServiceNames(void);

    // XPropertySet
    virtual css::uno::Reference< css::beans::XPropertySetInfo > SAL_CALL
    getPropertySetInfo() throw (css::uno::RuntimeException)
    { return css::uno::Reference< css::beans::XPropertySetInfo >(); }

    virtual void SAL_CALL setPropertyValue(
        rtl::OUString const &, css::uno::Any const &)
        throw (
            css::beans::UnknownPropertyException,
            css::beans::PropertyVetoException,
            css::lang::IllegalArgumentException,
            css::lang::WrappedTargetException, css::uno::RuntimeException);

    virtual css::uno::Any SAL_CALL getPropertyValue(
        rtl::OUString const & PropertyName)
        throw (
            css::beans::UnknownPropertyException,
            css::lang::WrappedTargetException, css::uno::RuntimeException);

    virtual void SAL_CALL addPropertyChangeListener(
        rtl::OUString const &,
        css::uno::Reference< css::beans::XPropertyChangeListener > const &)
        throw (
            css::beans::UnknownPropertyException,
            css::lang::WrappedTargetException, css::uno::RuntimeException)
    {}

    virtual void SAL_CALL removePropertyChangeListener(
        rtl::OUString const &,
        css::uno::Reference< css::beans::XPropertyChangeListener > const &)
        throw (
            css::beans::UnknownPropertyException,
            css::lang::WrappedTargetException, css::uno::RuntimeException)
    {}

    virtual void SAL_CALL addVetoableChangeListener(
        rtl::OUString const &,
        css::uno::Reference< css::beans::XVetoableChangeListener > const &)
        throw (
            css::beans::UnknownPropertyException,
            css::lang::WrappedTargetException, css::uno::RuntimeException)
    {}

    virtual void SAL_CALL removeVetoableChangeListener(
        rtl::OUString const &,
        css::uno::Reference< css::beans::XVetoableChangeListener > const &)
        throw (
            css::beans::UnknownPropertyException,
            css::lang::WrappedTargetException, css::uno::RuntimeException)
    {}

protected:

    /**
       Service constructor from a service factory.

       @param xContext   component context
    */
    MacOSXBackend();

    /** Destructor */
    ~MacOSXBackend(void);
};

#endif // _MACBACKEND_HXX_
