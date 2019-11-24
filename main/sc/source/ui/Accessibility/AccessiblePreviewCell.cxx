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
#include "scitems.hxx"
#include <editeng/eeitem.hxx>
#include <tools/gen.hxx>
#include "AccessibleText.hxx"
#include "editsrc.hxx"
#include "AccessiblePreviewCell.hxx"
#include "AccessibilityHints.hxx"
#include "prevwsh.hxx"
#include "unoguard.hxx"
#include "prevloc.hxx"
#include "document.hxx"
#include <svx/AccessibleTextHelper.hxx>
#include <unotools/accessiblestatesethelper.hxx>
#include <editeng/brshitem.hxx>
#include <vcl/window.hxx>
#include <toolkit/helper/convert.hxx>

#include <com/sun/star/accessibility/AccessibleStateType.hpp>

using namespace	::com::sun::star;
using namespace	::com::sun::star::accessibility;

//=====  internal  ============================================================

ScAccessiblePreviewCell::ScAccessiblePreviewCell( const ::com::sun::star::uno::Reference<
						        ::com::sun::star::accessibility::XAccessible>& rxParent,
							ScPreviewShell* pViewShell, /* const */ ScAddress& rCellAddress,
							sal_Int32 nIndex ) :
	ScAccessibleCellBase( rxParent, ( pViewShell ? pViewShell->GetDocument() : NULL ), rCellAddress, nIndex ),
	mpViewShell( pViewShell ),
    mpTextHelper(NULL)
{
	if (mpViewShell)
		mpViewShell->AddAccessibilityObject(*this);
}

ScAccessiblePreviewCell::~ScAccessiblePreviewCell()
{
	if (!ScAccessibleContextBase::IsDefunc() && !rBHelper.bInDispose)
	{
		// increment refcount to prevent double call off dtor
		osl_incrementInterlockedCount( &m_refCount );
		// call dispose to inform object which have a weak reference to this object
		dispose();
	}
}

void SAL_CALL ScAccessiblePreviewCell::disposing()
{
    ScUnoGuard aGuard;
	if (mpViewShell)
	{
		mpViewShell->RemoveAccessibilityObject(*this);
		mpViewShell = NULL;
	}

	if (mpTextHelper)
		DELETEZ(mpTextHelper);

	ScAccessibleCellBase::disposing();
}

void ScAccessiblePreviewCell::Notify( SfxBroadcaster& rBC, const SfxHint& rHint )
{
	if (rHint.ISA( SfxSimpleHint ))
	{
		const SfxSimpleHint& rRef = (const SfxSimpleHint&)rHint;
        if (rRef.GetId() == SC_HINT_ACC_VISAREACHANGED)
        {
            if (mpTextHelper)
                mpTextHelper->UpdateChildren();
        }
	}

	ScAccessibleContextBase::Notify(rBC, rHint);
}

//=====  XAccessibleComponent  ============================================

uno::Reference< XAccessible > SAL_CALL ScAccessiblePreviewCell::getAccessibleAtPoint( const awt::Point& rPoint )
								throw (uno::RuntimeException)
{
    uno::Reference<XAccessible> xRet;
    if (containsPoint(rPoint))
    {
 	    ScUnoGuard aGuard;
        IsObjectValid();

    	if(!mpTextHelper)
	    	CreateTextHelper();

        xRet = mpTextHelper->GetAt(rPoint);
    }

    return xRet;
}

void SAL_CALL ScAccessiblePreviewCell::grabFocus() throw (uno::RuntimeException)
{
 	ScUnoGuard aGuard;
    IsObjectValid();
	if (getAccessibleParent().is())
	{
		uno::Reference<XAccessibleComponent> xAccessibleComponent(getAccessibleParent()->getAccessibleContext(), uno::UNO_QUERY);
		if (xAccessibleComponent.is())
			xAccessibleComponent->grabFocus();
	}
}

//=====  XAccessibleContext  ==============================================

sal_Int32 SAL_CALL ScAccessiblePreviewCell::getAccessibleChildCount() throw(uno::RuntimeException)
{
	ScUnoGuard aGuard;
    IsObjectValid();
	if (!mpTextHelper)
		CreateTextHelper();
	return mpTextHelper->GetChildCount();
}

uno::Reference< XAccessible > SAL_CALL ScAccessiblePreviewCell::getAccessibleChild(sal_Int32 nIndex)
					        throw (uno::RuntimeException, lang::IndexOutOfBoundsException)
{
	ScUnoGuard aGuard;
    IsObjectValid();
	if (!mpTextHelper)
		CreateTextHelper();
	return mpTextHelper->GetChild(nIndex);
}

uno::Reference<XAccessibleStateSet> SAL_CALL ScAccessiblePreviewCell::getAccessibleStateSet()
						    throw(uno::RuntimeException)
{
	ScUnoGuard aGuard;

	uno::Reference<XAccessibleStateSet> xParentStates;
	if (getAccessibleParent().is())
	{
		uno::Reference<XAccessibleContext> xParentContext = getAccessibleParent()->getAccessibleContext();
		xParentStates = xParentContext->getAccessibleStateSet();
	}
	utl::AccessibleStateSetHelper* pStateSet = new utl::AccessibleStateSetHelper();
	if (IsDefunc(xParentStates))
		pStateSet->AddState(AccessibleStateType::DEFUNC);
    else
    {
	    pStateSet->AddState(AccessibleStateType::ENABLED);
	    pStateSet->AddState(AccessibleStateType::MULTI_LINE);
	    if (IsOpaque(xParentStates))
		    pStateSet->AddState(AccessibleStateType::OPAQUE);
	    if (isShowing())
		    pStateSet->AddState(AccessibleStateType::SHOWING);
	    pStateSet->AddState(AccessibleStateType::TRANSIENT);
	    if (isVisible())
		    pStateSet->AddState(AccessibleStateType::VISIBLE);
        // #111635# MANAGES_DESCENDANTS (for paragraphs)
        pStateSet->AddState(AccessibleStateType::MANAGES_DESCENDANTS);
    }
	return pStateSet;
}

//=====  XServiceInfo  ====================================================

rtl::OUString SAL_CALL ScAccessiblePreviewCell::getImplementationName() throw(uno::RuntimeException)
{
	return rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("ScAccessiblePreviewCell"));
}

uno::Sequence<rtl::OUString> SAL_CALL ScAccessiblePreviewCell::getSupportedServiceNames()
													throw(uno::RuntimeException)
{
	uno::Sequence< ::rtl::OUString > aSequence = ScAccessibleContextBase::getSupportedServiceNames();
    sal_Int32 nOldSize(aSequence.getLength());
    aSequence.realloc(nOldSize + 1);
    ::rtl::OUString* pNames = aSequence.getArray();

	pNames[nOldSize] = rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.table.AccessibleCellView"));

	return aSequence;
}

//=====  XTypeProvider  =======================================================

uno::Sequence<sal_Int8> SAL_CALL
	ScAccessiblePreviewCell::getImplementationId(void)
    throw (uno::RuntimeException)
{
    ScUnoGuard aGuard;
    IsObjectValid();
	static uno::Sequence<sal_Int8> aId;
	if (aId.getLength() == 0)
	{
		aId.realloc (16);
		rtl_createUuid (reinterpret_cast<sal_uInt8 *>(aId.getArray()), 0, sal_True);
	}
	return aId;
}

//====  internal  =========================================================

Rectangle ScAccessiblePreviewCell::GetBoundingBoxOnScreen() const throw (uno::RuntimeException)
{
	Rectangle aCellRect;
	if (mpViewShell)
	{
        mpViewShell->GetLocationData().GetCellPosition( maCellAddress, aCellRect );
		Window* pWindow = mpViewShell->GetWindow();
		if (pWindow)
		{
			Rectangle aRect = pWindow->GetWindowExtentsRelative(NULL);
			aCellRect.setX(aCellRect.getX() + aRect.getX());
			aCellRect.setY(aCellRect.getY() + aRect.getY());
		}
	}
	return aCellRect;
}

Rectangle ScAccessiblePreviewCell::GetBoundingBox() const throw (uno::RuntimeException)
{
	Rectangle aCellRect;
	if (mpViewShell)
    {
        mpViewShell->GetLocationData().GetCellPosition( maCellAddress, aCellRect );
        uno::Reference<XAccessible> xAccParent = const_cast<ScAccessiblePreviewCell*>(this)->getAccessibleParent();
        if (xAccParent.is())
        {
            uno::Reference<XAccessibleContext> xAccParentContext = xAccParent->getAccessibleContext();
            uno::Reference<XAccessibleComponent> xAccParentComp (xAccParentContext, uno::UNO_QUERY);
            if (xAccParentComp.is())
            {
                Rectangle aParentRect (VCLRectangle(xAccParentComp->getBounds()));
	            aCellRect.setX(aCellRect.getX() - aParentRect.getX());
	            aCellRect.setY(aCellRect.getY() - aParentRect.getY());
            }
        }
    }
	return aCellRect;
}

sal_Bool ScAccessiblePreviewCell::IsDefunc(
	const uno::Reference<XAccessibleStateSet>& rxParentStates)
{
	return ScAccessibleContextBase::IsDefunc() || (mpDoc == NULL) || (mpViewShell == NULL) || !getAccessibleParent().is() ||
		 (rxParentStates.is() && rxParentStates->contains(AccessibleStateType::DEFUNC));
}

sal_Bool ScAccessiblePreviewCell::IsEditable(
    const uno::Reference<XAccessibleStateSet>& /* rxParentStates */)
{
	return sal_False;
}

sal_Bool ScAccessiblePreviewCell::IsOpaque(
    const uno::Reference<XAccessibleStateSet>& /* rxParentStates */)
{
	// test whether there is a background color
	//!	could be moved to ScAccessibleCellBase

	sal_Bool bOpaque(sal_True);
	if (mpDoc)
	{
		const SvxBrushItem* pItem = (const SvxBrushItem*)mpDoc->GetAttr(
			maCellAddress.Col(), maCellAddress.Row(), maCellAddress.Tab(), ATTR_BACKGROUND);
		if (pItem)
			bOpaque = pItem->GetColor() != COL_TRANSPARENT;
	}
	return bOpaque;
}

void ScAccessiblePreviewCell::CreateTextHelper()
{
	if (!mpTextHelper)
	{
		::std::auto_ptr < ScAccessibleTextData > pAccessiblePreviewCellTextData
			(new ScAccessiblePreviewCellTextData(mpViewShell, maCellAddress));
		::std::auto_ptr< SvxEditSource > pEditSource (new ScAccessibilityEditSource(pAccessiblePreviewCellTextData));

        mpTextHelper = new ::accessibility::AccessibleTextHelper( pEditSource );
        mpTextHelper->SetEventSource( this );

        // #111635# paragraphs in preview are transient
        ::accessibility::AccessibleTextHelper::VectorOfStates aChildStates;
        aChildStates.push_back( AccessibleStateType::TRANSIENT );
        mpTextHelper->SetAdditionalChildStates( aChildStates );
	}
}

