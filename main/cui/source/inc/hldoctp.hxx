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


#ifndef _SVX_TABPAGE_DOC_HYPERLINK_HXX
#define _SVX_TABPAGE_DOC_HYPERLINK_HXX

#include "hltpbase.hxx"

/*************************************************************************
|*
|* Tabpage : Hyperlink - Document
|*
\************************************************************************/

class SvxHyperlinkDocTp : public SvxHyperlinkTabPageBase
{
private:
	FixedLine			maGrpDocument;
	FixedText			maFtPath;
	SvxHyperURLBox		maCbbPath;
	ImageButton			maBtFileopen;

	FixedLine			maGrpTarget;
	FixedText			maFtTarget;
	Edit				maEdTarget;
	FixedText			maFtURL;
	FixedText			maFtFullURL;
	ImageButton			maBtBrowse;

	String				maStrURL;

	sal_Bool				mbMarkWndOpen;

	DECL_LINK (ClickFileopenHdl_Impl  , void * );		// Button : Fileopen
	DECL_LINK (ClickTargetHdl_Impl    , void * );		// Button : Target

	DECL_LINK (ModifiedPathHdl_Impl	 , void * );		// Contens of combobox "Path" modified
	DECL_LINK (ModifiedTargetHdl_Impl, void * );		// Contens of editfield "Target" modified

	DECL_LINK (LostFocusPathHdl_Impl,  void * );		// Combobox "path" lost its focus

	DECL_LINK (TimeoutHdl_Impl      ,  Timer * );		// Handler for timer -timeout

	enum EPathType { Type_Unknown, Type_Invalid, 
		             Type_ExistsFile, Type_File, 
					 Type_ExistsDir, Type_Dir };
	EPathType GetPathType ( String& aStrPath );

protected:
	void FillDlgFields     ( String& aStrURL );
	void GetCurentItemData ( String& aStrURL, String& aStrName, 
		                     String& aStrIntName, String& aStrFrame, 
							 SvxLinkInsertMode& eMode );
	virtual sal_Bool   ShouldOpenMarkWnd () {return mbMarkWndOpen;}
	virtual void   SetMarkWndShouldOpen (sal_Bool bOpen) {mbMarkWndOpen=bOpen;}
	String GetCurrentURL 	();

public:
	SvxHyperlinkDocTp ( Window *pParent, const SfxItemSet& rItemSet);
	~SvxHyperlinkDocTp ();

	static  IconChoicePage* Create( Window* pWindow, const SfxItemSet& rItemSet );

	virtual void		SetMarkStr ( String& aStrMark );

	virtual void		SetInitFocus();
};


#endif // _SVX_TABPAGE_DOC_HYPERLINK_HXX
