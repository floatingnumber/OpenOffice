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
#include "precompiled_sw.hxx"

#ifdef SW_DLLIMPLEMENTATION
#undef SW_DLLIMPLEMENTATION
#endif


#include <vcl/msgbox.hxx>

#include "wrtsh.hxx"
#include "view.hxx"
#include "itabenum.hxx"
#include "instable.hxx"
#include "tblafmt.hxx"
#include "modcfg.hxx"
#include "swmodule.hxx"
#include <svx/htmlmode.hxx>
#include <viewopt.hxx>

#include "table.hrc"
#include "instable.hrc"

#include "swabstdlg.hxx"

namespace swui
{
	SwAbstractDialogFactory * GetFactory();
}

#define ROW_COL_PROD 16384

void SwInsTableDlg::GetValues( String& rName, sal_uInt16& rRow, sal_uInt16& rCol,
                                SwInsertTableOptions& rInsTblOpts, String& rAutoName,
								SwTableAutoFmt *& prTAFmt )
{
	sal_uInt16 nInsMode = 0;
	rName = aNameEdit.GetText();
	rRow = (sal_uInt16)aRowEdit.GetValue();
	rCol = (sal_uInt16)aColEdit.GetValue();

	if (aBorderCB.IsChecked())
        nInsMode |= tabopts::DEFAULT_BORDER;
	if (aHeaderCB.IsChecked())
        nInsMode |= tabopts::HEADLINE;
	if (aRepeatHeaderCB.IsEnabled() && aRepeatHeaderCB.IsChecked())
        rInsTblOpts.mnRowsToRepeat = sal_uInt16( aRepeatHeaderNF.GetValue() );
	else
		rInsTblOpts.mnRowsToRepeat = 0;
	if (!aDontSplitCB.IsChecked())
        nInsMode |= tabopts::SPLIT_LAYOUT;
	if( pTAutoFmt )
    {
		prTAFmt = new SwTableAutoFmt( *pTAutoFmt );
        rAutoName = prTAFmt->GetName();
    }

	rInsTblOpts.mnInsMode = nInsMode;
}

// CTOR / DTOR -----------------------------------------------------------


SwInsTableDlg::SwInsTableDlg( SwView& rView )
	: SfxModalDialog( rView.GetWindow(), SW_RES(DLG_INSERT_TABLE) ),
    aNameFT                 (this, SW_RES(FT_NAME)),
	aNameEdit				(this, SW_RES(ED_NAME)),

    aFL                     (this, SW_RES(FL_TABLE)),
    aColLbl                 (this, SW_RES(FT_COL)),
	aColEdit				(this, SW_RES(ED_COL)),
	aRowLbl					(this, SW_RES(FT_ROW)),
	aRowEdit				(this, SW_RES(ED_ROW)),

    aOptionsFL              (this, SW_RES(FL_OPTIONS)),
    aHeaderCB               (this, SW_RES(CB_HEADER)),
	aRepeatHeaderCB			(this, SW_RES(CB_REPEAT_HEADER)),
	aRepeatHeaderFT			(this, SW_RES(FT_REPEAT_HEADER)),
	aRepeatHeaderBeforeFT	(this),
	aRepeatHeaderNF			(this, SW_RES(NF_REPEAT_HEADER)),
	aRepeatHeaderAfterFT	(this),
	aRepeatHeaderCombo		(this, SW_RES(WIN_REPEAT_HEADER), aRepeatHeaderNF, aRepeatHeaderBeforeFT, aRepeatHeaderAfterFT),

    aDontSplitCB            (this, SW_RES(CB_DONT_SPLIT)),
	aBorderCB				(this, SW_RES(CB_BORDER)),

    aOkBtn                  (this, SW_RES(BT_OK)),
	aCancelBtn				(this, SW_RES(BT_CANCEL)),
	aHelpBtn				(this, SW_RES(BT_HELP)),
	aAutoFmtBtn				(this, SW_RES(BT_AUTOFORMAT)),

    pShell(&rView.GetWrtShell()),
    pTAutoFmt( 0 ),
	nEnteredValRepeatHeaderNF( -1 )
{
	FreeResource();
	aNameEdit.SetText(pShell->GetUniqueTblName());
	aNameEdit.SetModifyHdl(LINK(this, SwInsTableDlg, ModifyName));
	aColEdit.SetModifyHdl(LINK(this, SwInsTableDlg, ModifyRowCol));
	aRowEdit.SetModifyHdl(LINK(this, SwInsTableDlg, ModifyRowCol));

	aRowEdit.SetMax(ROW_COL_PROD/aColEdit.GetValue());
	aColEdit.SetMax(ROW_COL_PROD/aRowEdit.GetValue());
	aAutoFmtBtn.SetClickHdl(LINK(this, SwInsTableDlg, AutoFmtHdl));

	sal_Bool bHTMLMode = 0 != (::GetHtmlMode(rView.GetDocShell())&HTMLMODE_ON);
	const SwModuleOptions* pModOpt = SW_MOD()->GetModuleConfig();

	SwInsertTableOptions aInsOpts = pModOpt->GetInsTblFlags(bHTMLMode);
	sal_uInt16 nInsTblFlags = aInsOpts.mnInsMode;

    aHeaderCB.Check( 0 != (nInsTblFlags & tabopts::HEADLINE) );
    aRepeatHeaderCB.Check(aInsOpts.mnRowsToRepeat > 0);
	if(bHTMLMode)
	{
		aDontSplitCB.Hide();
		aBorderCB.SetPosPixel(aDontSplitCB.GetPosPixel());
	}
	else
	{
        aDontSplitCB.Check( 0 == (nInsTblFlags & tabopts::SPLIT_LAYOUT) );
	}
    aBorderCB.Check( 0 != (nInsTblFlags & tabopts::DEFAULT_BORDER) );

	aRepeatHeaderNF.SetModifyHdl( LINK( this, SwInsTableDlg, ModifyRepeatHeaderNF_Hdl ) );
	aHeaderCB.SetClickHdl(LINK(this, SwInsTableDlg, CheckBoxHdl));
	aRepeatHeaderCB.SetClickHdl(LINK(this, SwInsTableDlg, ReapeatHeaderCheckBoxHdl));
	ReapeatHeaderCheckBoxHdl();
	CheckBoxHdl();

    sal_Int64 nMax = aRowEdit.GetValue();
	if( nMax <= 1 )
		nMax = 1;
	else
		--nMax;
	aRepeatHeaderNF.SetMax( nMax );

	aRepeatHeaderCombo.Arrange( aRepeatHeaderFT );
}

SwInsTableDlg::~SwInsTableDlg()
{
	delete pTAutoFmt;
}

IMPL_LINK_INLINE_START( SwInsTableDlg, ModifyName, Edit *, pEdit )
{
	String sTblName = pEdit->GetText();
	if(sTblName.Search(' ') != STRING_NOTFOUND)
	{
		sTblName.EraseAllChars( );
		pEdit->SetText(sTblName);
	}

	aOkBtn.Enable(pShell->GetTblStyle( sTblName ) == 0);
	return 0;
}
IMPL_LINK_INLINE_END( SwInsTableDlg, ModifyName, Edit *, EMPTYARG )

/*-----------------15.04.98 11:36-------------------

--------------------------------------------------*/
IMPL_LINK( SwInsTableDlg, ModifyRowCol, NumericField *, pField )
{
	if(pField == &aColEdit)
	{
        sal_Int64 nCol = aColEdit.GetValue();
        if(!nCol)
            nCol = 1;
		aRowEdit.SetMax(ROW_COL_PROD/nCol);
	}
	else
	{
        sal_Int64 nRow = aRowEdit.GetValue();
        if(!nRow)
            nRow = 1;
        aColEdit.SetMax(ROW_COL_PROD/nRow);

		// adjust depending NF for repeated rows
        sal_Int64 nMax = ( nRow == 1 )? 1 : nRow - 1 ;
        sal_Int64 nActVal = aRepeatHeaderNF.GetValue();

		aRepeatHeaderNF.SetMax( nMax );

		if( nActVal > nMax )
			aRepeatHeaderNF.SetValue( nMax );
		else if( nActVal < nEnteredValRepeatHeaderNF )
			aRepeatHeaderNF.SetValue( ( nEnteredValRepeatHeaderNF < nMax )? nEnteredValRepeatHeaderNF : nMax );
	}
	return 0;
}

IMPL_LINK( SwInsTableDlg, AutoFmtHdl, PushButton*, pButton )
{
    SwAbstractDialogFactory* pFact = swui::GetFactory();
    DBG_ASSERT(pFact, "SwAbstractDialogFactory fail!");

    AbstractSwAutoFormatDlg* pDlg = pFact->CreateSwAutoFormatDlg(pButton,pShell, DLG_AUTOFMT_TABLE, sal_False, pTAutoFmt );
    DBG_ASSERT(pDlg, "Dialogdiet fail!");
    if( RET_OK == pDlg->Execute())
        pDlg->FillAutoFmtOfIndex( pTAutoFmt );
    delete pDlg;
	return 0;
}

IMPL_LINK(SwInsTableDlg, CheckBoxHdl, CheckBox*, EMPTYARG)
{
	aRepeatHeaderCB.Enable(aHeaderCB.IsChecked());
	ReapeatHeaderCheckBoxHdl();

	return 0;
}

IMPL_LINK(SwInsTableDlg, ReapeatHeaderCheckBoxHdl, void*, EMPTYARG)
{
	aRepeatHeaderCombo.Enable(aHeaderCB.IsChecked() && aRepeatHeaderCB.IsChecked());

	return 0;
}

IMPL_LINK(SwInsTableDlg, ModifyRepeatHeaderNF_Hdl, void*, EMPTYARG)
{
	nEnteredValRepeatHeaderNF = aRepeatHeaderNF.GetValue();
	return 0;
}

