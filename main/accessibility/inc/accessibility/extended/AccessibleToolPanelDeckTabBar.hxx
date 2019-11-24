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



#ifndef ACC_ACCESSIBLETOOLPANELTABBAR_HXX
#define ACC_ACCESSIBLETOOLPANELTABBAR_HXX

/** === begin UNO includes === **/
/** === end UNO includes === **/

#include <cppuhelper/implbase1.hxx>
#include <toolkit/awt/vclxaccessiblecomponent.hxx>

#include <boost/scoped_ptr.hpp>

namespace svt
{
    class IToolPanelDeck;
    class PanelTabBar;
}

//......................................................................................................................
namespace accessibility
{
//......................................................................................................................

	//==================================================================================================================
	//= AccessibleToolPanelTabBar
	//==================================================================================================================
    class AccessibleToolPanelTabBar_Impl;
    typedef VCLXAccessibleComponent   AccessibleToolPanelTabBar_Base;
    class AccessibleToolPanelTabBar : public AccessibleToolPanelTabBar_Base
	{
    public:
        AccessibleToolPanelTabBar(
            const ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible >& i_rAccessibleParent,
            ::svt::IToolPanelDeck& i_rPanelDeck,
            ::svt::PanelTabBar& i_rTabBar
        );

        using AccessibleToolPanelTabBar_Base::NotifyAccessibleEvent;

    protected:
        virtual ~AccessibleToolPanelTabBar();

        // XAccessibleContext
		virtual sal_Int32 SAL_CALL getAccessibleChildCount(  ) throw (::com::sun::star::uno::RuntimeException);
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > SAL_CALL getAccessibleChild( sal_Int32 i ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > SAL_CALL getAccessibleParent(  ) throw (::com::sun::star::uno::RuntimeException);
		virtual sal_Int16 SAL_CALL getAccessibleRole(  ) throw (::com::sun::star::uno::RuntimeException);

		// XAccessibleComponent
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > SAL_CALL getAccessibleAtPoint( const ::com::sun::star::awt::Point& aPoint ) throw (::com::sun::star::uno::RuntimeException);

        // OComponentHelper
	    virtual void SAL_CALL disposing();

        // VCLXAccessibleComponent
        virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > GetChildAccessible( const VclWindowEvent& i_rVclWindowEvent );
        virtual void FillAccessibleStateSet( ::utl::AccessibleStateSetHelper& i_rStateSet );

    private:
        ::boost::scoped_ptr< AccessibleToolPanelTabBar_Impl > m_pImpl;
	};

//......................................................................................................................
} // namespace accessibility
//......................................................................................................................

#endif // ACC_ACCESSIBLETOOLPANELTABBAR_HXX
