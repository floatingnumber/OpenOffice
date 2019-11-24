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



#ifndef _SVX_ACCESSIBILITY_ACCESSIBLE_SELECTION_BASE_HXX
#define _SVX_ACCESSIBILITY_ACCESSIBLE_SELECTION_BASE_HXX

#include <comphelper/accessibleselectionhelper.hxx>
#include "editeng/editengdllapi.h"

namespace accessibility
{
/** @descr
        This base class provides a base implementation of the
        <type>XAccessibleSelection</type> interface.
        The following methods have to be implemented if this
        class is used:

        <method>implGetMutex</method>,
        <method>implGetAccessibleContext</method>,
        <method>implIsSelected</method>,
        <method>implSelect</method>,
*/
    class EDITENG_DLLPUBLIC AccessibleSelectionBase : public ::comphelper::OCommonAccessibleSelection,
                                    public ::com::sun::star::accessibility::XAccessibleSelection
    {
    protected:

        virtual ::osl::Mutex&   implGetMutex() = 0;

    public:

        // XAccessibleSelection - default implementations
        virtual void SAL_CALL selectAccessibleChild( sal_Int32 nChildIndex ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
        virtual sal_Bool SAL_CALL isAccessibleChildSelected( sal_Int32 nChildIndex ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
        virtual void SAL_CALL clearAccessibleSelection(  ) throw (::com::sun::star::uno::RuntimeException);
        virtual void SAL_CALL selectAllAccessibleChildren(  ) throw (::com::sun::star::uno::RuntimeException);
        virtual sal_Int32 SAL_CALL getSelectedAccessibleChildCount(  ) throw (::com::sun::star::uno::RuntimeException);
        virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > SAL_CALL getSelectedAccessibleChild( sal_Int32 nSelectedChildIndex ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
        virtual void SAL_CALL deselectAccessibleChild( sal_Int32 nSelectedChildIndex ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);

    public:

                                AccessibleSelectionBase();
        virtual                 ~AccessibleSelectionBase();
    };

}

#endif // _SVX_ACCESSIBILITY_ACCESSIBLE_SELECTION_BASE_HXX
