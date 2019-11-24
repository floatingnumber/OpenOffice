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

#include "precompiled_sc.hxx"
#include "AccessibleGlobal.hxx"
#include "AccessibleFilterMenuItem.hxx"
#include "dpcontrol.hxx"

#include <com/sun/star/accessibility/XAccessible.hpp>
#include <com/sun/star/accessibility/XAccessibleStateSet.hpp>
#include <com/sun/star/accessibility/AccessibleRole.hpp>
#include <com/sun/star/accessibility/AccessibleEventId.hpp>
#include <com/sun/star/accessibility/AccessibleEventObject.hpp>
#include <com/sun/star/accessibility/AccessibleStateType.hpp>
#include <com/sun/star/accessibility/TextSegment.hpp>

using namespace ::com::sun::star;
using namespace ::com::sun::star::accessibility;
using namespace ::com::sun::star::accessibility::AccessibleStateType;

using ::com::sun::star::uno::Any;
using ::com::sun::star::uno::Reference;
using ::com::sun::star::uno::Sequence;
using ::com::sun::star::uno::UNO_QUERY;
using ::com::sun::star::lang::IndexOutOfBoundsException;
using ::com::sun::star::uno::RuntimeException;
using ::rtl::OUString;

ScAccessibleFilterMenuItem::ScAccessibleFilterMenuItem(
    const Reference<XAccessible>& rxParent, ScMenuFloatingWindow* pWin, const OUString& rName, size_t nMenuPos) :
    ScAccessibleContextBase(rxParent, AccessibleRole::MENU_ITEM),
    mpWindow(pWin),
    maName(rName),
    mnMenuPos(nMenuPos),
    mbEnabled(true)
{
    SetName(rName);
}

ScAccessibleFilterMenuItem::~ScAccessibleFilterMenuItem()
{
}

sal_Int32 ScAccessibleFilterMenuItem::getAccessibleChildCount()
    throw (RuntimeException)
{
    return 0;
}

Reference<XAccessible> ScAccessibleFilterMenuItem::getAccessibleChild(sal_Int32 /*nIndex*/)
    throw (RuntimeException, IndexOutOfBoundsException)
{
    throw IndexOutOfBoundsException();
}

Reference<XAccessibleStateSet> ScAccessibleFilterMenuItem::getAccessibleStateSet()
    throw (RuntimeException)
{
    updateStateSet();
    return mxStateSet;
}

OUString ScAccessibleFilterMenuItem::getImplementationName()
    throw (RuntimeException)
{
    return OUString::createFromAscii("ScAccessibleFilterMenuItem");
}

// XAccessibleAction

sal_Int32 ScAccessibleFilterMenuItem::getAccessibleActionCount() throw (RuntimeException)
{
    return 1;
}

sal_Bool ScAccessibleFilterMenuItem::doAccessibleAction(sal_Int32 /*nIndex*/)
    throw (IndexOutOfBoundsException, RuntimeException)
{
    mpWindow->executeMenuItem(mnMenuPos);
    return true;
}

OUString ScAccessibleFilterMenuItem::getAccessibleActionDescription(sal_Int32 /*nIndex*/)
    throw (IndexOutOfBoundsException, RuntimeException)
{
    return OUString::createFromAscii("click");
}

Reference<XAccessibleKeyBinding> ScAccessibleFilterMenuItem::getAccessibleActionKeyBinding(
    sal_Int32 /*nIndex*/) throw (IndexOutOfBoundsException, RuntimeException)
{
    return Reference<XAccessibleKeyBinding>();
}

Any SAL_CALL ScAccessibleFilterMenuItem::queryInterface( uno::Type const & rType )
	throw (RuntimeException)
{
	Any any = ScAccessibleContextBase::queryInterface(rType);
    if (any.hasValue())
        return any;

    return ScAccessibleFilterMenuItem_BASE::queryInterface(rType);
}

void SAL_CALL ScAccessibleFilterMenuItem::acquire() throw ()
{
	ScAccessibleContextBase::acquire();
}

void SAL_CALL ScAccessibleFilterMenuItem::release() throw ()
{
	ScAccessibleContextBase::release();
}

bool ScAccessibleFilterMenuItem::isSelected() const
{
    return mpWindow->isMenuItemSelected(mnMenuPos);
}

bool ScAccessibleFilterMenuItem::isFocused() const
{
    return isSelected();
}

void ScAccessibleFilterMenuItem::setEnabled(bool bEnabled)
{
    mbEnabled = bEnabled;
}

Rectangle ScAccessibleFilterMenuItem::GetBoundingBoxOnScreen() const
    throw (RuntimeException)
{
    if (!mpWindow->IsVisible())
        return Rectangle();

    Point aPos = mpWindow->OutputToAbsoluteScreenPixel(Point(0,0));
    Point aMenuPos;
    Size aMenuSize;
    mpWindow->getMenuItemPosSize(mnMenuPos, aMenuPos, aMenuSize);
    Rectangle aRect(aPos + aMenuPos, aMenuSize);
    return aRect;
}

Rectangle ScAccessibleFilterMenuItem::GetBoundingBox() const
    throw (RuntimeException)
{
    if (!mpWindow->IsVisible())
        return Rectangle();

    Point aMenuPos;
    Size aMenuSize;
    mpWindow->getMenuItemPosSize(mnMenuPos, aMenuPos, aMenuSize);
    Rectangle aRect(aMenuPos, aMenuSize);
    return aRect;
}

void ScAccessibleFilterMenuItem::updateStateSet()
{
    if (!mxStateSet.is())
        mxStateSet.set(new ScAccessibleStateSet);

    ScAccessibleStateSet* p = static_cast<ScAccessibleStateSet*>(
        mxStateSet.get());

    p->clear();

    p->insert(ENABLED);
    p->insert(FOCUSABLE);
    p->insert(SELECTABLE);
    p->insert(SENSITIVE);
    p->insert(OPAQUE);

    if (isFocused())
        p->insert(FOCUSED);

    if (isSelected())
        p->insert(SELECTED);
}

