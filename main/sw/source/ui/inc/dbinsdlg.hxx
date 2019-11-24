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



#ifndef _DBINSDLG_HXX
#define _DBINSDLG_HXX


#ifndef _BUTTON_HXX //autogen
#include <vcl/button.hxx>
#endif
#ifndef _GROUP_HXX //autogen
#include <vcl/group.hxx>
#endif
#ifndef _FIXED_HXX //autogen
#include <vcl/fixed.hxx>
#endif
#ifndef _EDIT_HXX //autogen
#include <vcl/edit.hxx>
#endif
#include <vcl/lstbox.hxx>
#include <svtools/svmedit.hxx>
#include <sfx2/basedlgs.hxx>
#include <svl/svarray.hxx>
#include <unotools/configitem.hxx>
#include <numfmtlb.hxx>
#include <swdbdata.hxx>
#include <com/sun/star/uno/Reference.h>
#include <com/sun/star/uno/Sequence.h>
namespace com{namespace sun{namespace star{
	namespace sdbcx{
	class XColumnsSupplier;
	}
	namespace sdbc{
	class XDataSource;
	class XConnection;
	class XResultSet;
	}
}}}

class SwTableAutoFmt;
class SwView;
class SfxItemSet;
class SwTableRep;
class _DB_Columns;

struct SwInsDBColumn
{
	rtl::OUString sColumn, sUsrNumFmt;
	sal_Int32 nDBNumFmt;
	sal_uInt32 nUsrNumFmt;
	LanguageType eUsrNumFmtLng;
	sal_uInt16 nCol;
	sal_Bool bHasFmt : 1;
	sal_Bool bIsDBFmt : 1;

	SwInsDBColumn( const String& rStr, sal_uInt16 nColumn )
        : sColumn( rStr ),
        nDBNumFmt( 0 ),
        nUsrNumFmt( 0 ),
        eUsrNumFmtLng( LANGUAGE_SYSTEM ),
        nCol( nColumn ),
        bHasFmt(sal_False),
        bIsDBFmt(sal_True)
	{}

	int operator==( const SwInsDBColumn& rCmp ) const
		{ return sColumn == rCmp.sColumn; }
	int operator<( const SwInsDBColumn& rCmp ) const;
};

typedef SwInsDBColumn* SwInsDBColumnPtr;
SV_DECL_PTRARR_SORT_DEL( SwInsDBColumns, SwInsDBColumnPtr, 32, 32 )


class SwInsertDBColAutoPilot : public SfxModalDialog, public utl::ConfigItem
{
	FixedText 		aFtInsertData;
	RadioButton 	aRbAsTable;
	RadioButton 	aRbAsField;
	RadioButton 	aRbAsText;

    FixedLine       aFlHead;
	FixedText 		aFtDbColumn;

	ListBox 		aLbTblDbColumn;
	ListBox 		aLbTxtDbColumn;

    FixedLine       aFlFormat;
	RadioButton 	aRbDbFmtFromDb;
	RadioButton 	aRbDbFmtFromUsr;
	NumFormatListBox aLbDbFmtFromUsr;

	/* ----- Page Text/Field ------- */
    ImageButton     aIbDbcolToEdit;
	MultiLineEdit 	aEdDbText;
	FixedText 		aFtDbParaColl;
	ListBox 		aLbDbParaColl;

	/* ----- Page Table ------------ */
    ImageButton     aIbDbcolAllTo;
    ImageButton     aIbDbcolOneTo;
    ImageButton     aIbDbcolOneFrom;
    ImageButton     aIbDbcolAllFrom;
	FixedText 		aFtTableCol;
	ListBox 		aLbTableCol;
	CheckBox 		aCbTableHeadon;
	RadioButton 	aRbHeadlColnms;
	RadioButton 	aRbHeadlEmpty;
	PushButton 		aPbTblFormat;
	PushButton 		aPbTblAutofmt;

	OKButton 		aBtOk;
	CancelButton 	aBtCancel;
	HelpButton 		aBtHelp;

    FixedLine       aFlBottom;

    SwInsDBColumns  aDBColumns;
	const SwDBData	aDBData;

	Link			aOldNumFmtLnk;
	String			sNoTmpl;

	SwView* 		pView;
	SwTableAutoFmt*	pTAutoFmt;

	SfxItemSet*		pTblSet;
	SwTableRep* 	pRep;
	sal_uInt16			nGBFmtLen;

	DECL_LINK( PageHdl, Button* );
	DECL_LINK( AutoFmtHdl, PushButton* );
	DECL_LINK( TblFmtHdl, PushButton* );
	DECL_LINK( DBFormatHdl, Button* );
	DECL_LINK( TblToFromHdl, Button* );
	DECL_LINK( SelectHdl, ListBox* );
	DECL_LINK( DblClickHdl, ListBox* );
	DECL_LINK( HeaderHdl, Button* );

	sal_Bool SplitTextToColArr( const String& rTxt, _DB_Columns& rColArr, sal_Bool bInsField );
        using SfxModalDialog::Notify;
	virtual void Notify( const ::com::sun::star::uno::Sequence< rtl::OUString >& aPropertyNames );
	virtual void			Commit();
	void 					Load();

	// setze die Tabellen - Eigenschaften
	void SetTabSet();

public:
	SwInsertDBColAutoPilot( SwView& rView,
		::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XDataSource> rxSource,
		com::sun::star::uno::Reference<com::sun::star::sdbcx::XColumnsSupplier>,
		const SwDBData& rData  );

	virtual ~SwInsertDBColAutoPilot();

    void DataToDoc( const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any >& rSelection,
		::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XDataSource> rxSource,
		::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection> xConnection,
		::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XResultSet > xResultSet);

};

#endif
