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
#include "precompiled_scui.hxx"




//------------------------------------------------------------------

#define _TPHF_CXX
#include "scitems.hxx"
#include <sfx2/basedlgs.hxx>
#include <svl/style.hxx>
#include <vcl/svapp.hxx>
#include <vcl/msgbox.hxx>

#include "tphf.hxx"
#include "sc.hrc"
#include "globstr.hrc"
#include "tabvwsh.hxx"
#include "viewdata.hxx"
#include "document.hxx"
//CHINA001 #include "tphfedit.hxx"
#include "hfedtdlg.hxx"
#include "styledlg.hxx"
#include "scresid.hxx"
#include "scuitphfedit.hxx" //CHINA001 
#undef _TPHF_CXX



//==================================================================
// class ScHFPage
//==================================================================

ScHFPage::ScHFPage( Window* pParent, sal_uInt16 nResId,
					const SfxItemSet& rSet, sal_uInt16 nSetId )

	:	SvxHFPage	( pParent, nResId, rSet, nSetId ),
		aBtnEdit	( this, ScResId( RID_SCBTN_HFEDIT ) ),
		aDataSet 	( *rSet.GetPool(),
					   ATTR_PAGE_HEADERLEFT, ATTR_PAGE_FOOTERRIGHT,
					   ATTR_PAGE, ATTR_PAGE, 0 ),
		nPageUsage	( (sal_uInt16)SVX_PAGE_ALL ),
		pStyleDlg	( NULL )
{
	SetExchangeSupport();

	SfxViewShell*	pSh = SfxViewShell::Current();
	ScTabViewShell* pViewSh = PTR_CAST(ScTabViewShell,pSh);
	Point			aPos( aBackgroundBtn.GetPosPixel() );

	// aBackgroundBtn position not changed anymore

	aPos.X() += aBackgroundBtn.GetSizePixel().Width();
	aPos.X() += LogicToPixel( Size(3,0), MAP_APPFONT ).Width();
	aBtnEdit.SetPosPixel( aPos );
	aBtnEdit.Show();

	aDataSet.Put( rSet );

	if ( pViewSh )
	{
		ScViewData* pViewData = pViewSh->GetViewData();
		ScDocument* pDoc	  = pViewData->GetDocument();

		aStrPageStyle = pDoc->GetPageStyle( pViewData->GetTabNo() );
	}

	aBtnEdit.SetClickHdl 	( LINK( this, ScHFPage, BtnHdl ) );
	aTurnOnBox.SetClickHdl	( LINK( this, ScHFPage, TurnOnHdl ) );

	if ( nId == SID_ATTR_PAGE_HEADERSET )
		aBtnEdit.SetHelpId( HID_SC_HEADER_EDIT );
	else
		aBtnEdit.SetHelpId( HID_SC_FOOTER_EDIT );

	aBtnEdit.SetAccessibleRelationMemberOf(&aFrm);
}

//------------------------------------------------------------------

__EXPORT ScHFPage::~ScHFPage()
{
}

//------------------------------------------------------------------

void __EXPORT ScHFPage::Reset( const SfxItemSet& rSet )
{
	SvxHFPage::Reset( rSet );
	TurnOnHdl( 0 );
}

//------------------------------------------------------------------

sal_Bool __EXPORT ScHFPage::FillItemSet( SfxItemSet& rOutSet )
{
	sal_Bool bResult = SvxHFPage::FillItemSet( rOutSet );

	if ( nId == SID_ATTR_PAGE_HEADERSET )
	{
		rOutSet.Put( aDataSet.Get( ATTR_PAGE_HEADERLEFT ) );
		rOutSet.Put( aDataSet.Get( ATTR_PAGE_HEADERRIGHT ) );
	}
	else
	{
		rOutSet.Put( aDataSet.Get( ATTR_PAGE_FOOTERLEFT ) );
		rOutSet.Put( aDataSet.Get( ATTR_PAGE_FOOTERRIGHT ) );
	}

	return bResult;
}

//------------------------------------------------------------------

void __EXPORT ScHFPage::ActivatePage( const SfxItemSet& rSet )
{
	sal_uInt16				nPageWhich = GetWhich( SID_ATTR_PAGE );
	const SvxPageItem&	rPageItem  = (const SvxPageItem&)
									 rSet.Get(nPageWhich);

	nPageUsage = rPageItem.GetPageUsage();

	if ( pStyleDlg )
		aStrPageStyle = pStyleDlg->GetStyleSheet().GetName();

	aDataSet.Put( rSet.Get(ATTR_PAGE) );

	SvxHFPage::ActivatePage( rSet );
}

//------------------------------------------------------------------

int __EXPORT ScHFPage::DeactivatePage( SfxItemSet* pSetP )
{
    if ( LEAVE_PAGE == SvxHFPage::DeactivatePage( pSetP ) )
        if ( pSetP )
            FillItemSet( *pSetP );

	return LEAVE_PAGE;
}

//------------------------------------------------------------------

void ScHFPage::ActivatePage()
{
}

void ScHFPage::DeactivatePage()
{
}

//------------------------------------------------------------------
// Handler:
//------------------------------------------------------------------

IMPL_LINK( ScHFPage, TurnOnHdl, CheckBox*, EMPTYARG )
{
	SvxHFPage::TurnOnHdl( &aTurnOnBox );

	if ( aTurnOnBox.IsChecked() )
		aBtnEdit.Enable();
	else
		aBtnEdit.Disable();

	return 0;
}


//------------------------------------------------------------------

IMPL_LINK( ScHFPage, BtnHdl, PushButton*, EMPTYARG )
{
	//	Wenn der Bearbeiten-Dialog direkt aus dem Click-Handler des Buttons
	//	aufgerufen wird, funktioniert im Bearbeiten-Dialog unter OS/2 das
	//	GrabFocus nicht (Bug #41805#).
	//	Mit dem neuen StarView sollte dieser Workaround wieder raus koennen!

	Application::PostUserEvent( LINK( this, ScHFPage, HFEditHdl ) );
	return 0;
}

IMPL_LINK( ScHFPage, HFEditHdl, void*, EMPTYARG )
{
	SfxViewShell*	pViewSh = SfxViewShell::Current();

	if ( !pViewSh )
	{
		DBG_ERROR( "Current ViewShell not found." );
		return 0;
	}

	if (   aCntSharedBox.IsEnabled()
		&& !aCntSharedBox.IsChecked() )
	{
		sal_uInt16 nResId = ( nId == SID_ATTR_PAGE_HEADERSET )
							? RID_SCDLG_HFED_HEADER
							: RID_SCDLG_HFED_FOOTER;

		ScHFEditDlg* pDlg
			 = new ScHFEditDlg( pViewSh->GetViewFrame(), this,
								aDataSet, aStrPageStyle, nResId );

		if ( pDlg->Execute() == RET_OK )
		{
			aDataSet.Put( *pDlg->GetOutputItemSet() );
		}

		delete pDlg;
	}
	else
	{
		String				aText;
		SfxSingleTabDialog* pDlg = new SfxSingleTabDialog( this, aDataSet, 42 );
		sal_Bool bRightPage =   aCntSharedBox.IsChecked()
						 || ( SVX_PAGE_LEFT != SvxPageUsage(nPageUsage) );

		if ( nId == SID_ATTR_PAGE_HEADERSET )
		{
			aText = ScGlobal::GetRscString( STR_PAGEHEADER );
			if ( bRightPage )
				pDlg->SetTabPage( ScRightHeaderEditPage::Create( pDlg, aDataSet ) );
			else
				pDlg->SetTabPage( ScLeftHeaderEditPage::Create( pDlg, aDataSet ) );
		}
		else
		{
			aText = ScGlobal::GetRscString( STR_PAGEFOOTER );
			if ( bRightPage )
				pDlg->SetTabPage( ScRightFooterEditPage::Create( pDlg, aDataSet ) );
			else
				pDlg->SetTabPage( ScLeftFooterEditPage::Create( pDlg, aDataSet ) );
		}

		SvxNumType eNumType = ((const SvxPageItem&)aDataSet.Get(ATTR_PAGE)).GetNumType();
		((ScHFEditPage*)pDlg->GetTabPage())->SetNumType(eNumType);

		aText.AppendAscii(RTL_CONSTASCII_STRINGPARAM( " (" ));
		aText += ScGlobal::GetRscString( STR_PAGESTYLE );
		aText.AppendAscii(RTL_CONSTASCII_STRINGPARAM( ": " ));
		aText += aStrPageStyle;
		aText += ')';

		pDlg->SetText( aText );

		if ( pDlg->Execute() == RET_OK )
		{
			aDataSet.Put( *pDlg->GetOutputItemSet() );
		}

		delete pDlg;
	}

	return 0;
}

//==================================================================
// class ScHeaderPage
//==================================================================

ScHeaderPage::ScHeaderPage( Window* pParent, const SfxItemSet& rSet )
	: ScHFPage( pParent, RID_SVXPAGE_HEADER, rSet, SID_ATTR_PAGE_HEADERSET )
{
}

//------------------------------------------------------------------

SfxTabPage* __EXPORT ScHeaderPage::Create( Window* pParent, const SfxItemSet& rCoreSet )
{
	return ( new ScHeaderPage( pParent, rCoreSet ) );
}

//------------------------------------------------------------------

sal_uInt16* __EXPORT ScHeaderPage::GetRanges()
{
	return SvxHeaderPage::GetRanges();
}

//==================================================================
// class ScFooterPage
//==================================================================

ScFooterPage::ScFooterPage( Window* pParent, const SfxItemSet& rSet )
	: ScHFPage( pParent, RID_SVXPAGE_FOOTER, rSet, SID_ATTR_PAGE_FOOTERSET )
{
}

//------------------------------------------------------------------

SfxTabPage* __EXPORT ScFooterPage::Create( Window* pParent, const SfxItemSet& rCoreSet )
{
	return ( new ScFooterPage( pParent, rCoreSet ) );
}

//------------------------------------------------------------------

sal_uInt16* __EXPORT ScFooterPage::GetRanges()
{
	return SvxHeaderPage::GetRanges();
}




