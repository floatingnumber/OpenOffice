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


#ifndef SW_VBA_HEADERFOOTER_HXX
#define SW_VBA_HEADERFOOTER_HXX

#include <ooo/vba/word/XHeaderFooter.hpp>
#include <ooo/vba/word/XRange.hpp>
#include <vbahelper/vbahelperinterface.hxx>
#include <cppuhelper/implbase1.hxx>

typedef InheritedHelperInterfaceImpl1< ooo::vba::word::XHeaderFooter > SwVbaHeaderFooter_BASE;

class SwVbaHeaderFooter : public SwVbaHeaderFooter_BASE
{
private:
    css::uno::Reference< css::frame::XModel > mxModel;
    css::uno::Reference< css::beans::XPropertySet > mxPageStyleProps;
    sal_Bool mbHeader;
    sal_Int32 mnIndex;

public:
	SwVbaHeaderFooter( const css::uno::Reference< ooo::vba::XHelperInterface >& rParent, const css::uno::Reference< css::uno::XComponentContext >& rContext, const css::uno::Reference< css::frame::XModel >& xModel, const css::uno::Reference< css::beans::XPropertySet >& xProps, sal_Bool isHeader, sal_Int32 index ) throw ( css::uno::RuntimeException );
	virtual ~SwVbaHeaderFooter(){}

    // Attributes
    virtual ::sal_Bool SAL_CALL getIsHeader() throw (css::uno::RuntimeException);
    virtual ::sal_Bool SAL_CALL getLinkToPrevious() throw (css::uno::RuntimeException);
    virtual void SAL_CALL setLinkToPrevious( ::sal_Bool _linktoprevious ) throw (css::uno::RuntimeException);
    virtual css::uno::Reference< ::ooo::vba::word::XRange > SAL_CALL getRange() throw (css::uno::RuntimeException);
    virtual css::uno::Any SAL_CALL Shapes( const css::uno::Any& aIndex ) throw (css::uno::RuntimeException);

	// XHelperInterface
	virtual rtl::OUString& getServiceImplName();
	virtual css::uno::Sequence<rtl::OUString> getServiceNames();
};
#endif /* SW_VBA_HEADERFOOTER_HXX */
