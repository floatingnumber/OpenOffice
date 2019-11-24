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



#ifndef IMPDIALOG_HXX
#define IMPDIALOG_HXX

#include "pdffilter.hxx"

#include "vcl/dialog.hxx"
#include "vcl/button.hxx"
#include "vcl/fixed.hxx"
#include "vcl/field.hxx"
#include "vcl/edit.hxx"
#include "vcl/lstbox.hxx"
#include "vcl/combobox.hxx"
#include "vcl/group.hxx"
#include "vcl/pdfwriter.hxx"

#include "svtools/FilterConfigItem.hxx"

#include "sfx2/tabdlg.hxx"

#include "com/sun/star/beans/NamedValue.hpp"

// ----------------
// - ImpPDFDialog -
// ----------------

class ResMgr;
class ImpPDFTabGeneralPage;
class ImpPDFTabViewerPage;
class ImpPDFTabOpnFtrPage;
class ImpPDFTabLinksPage;

class PDFFilterResId : public ResId
{
public:
    PDFFilterResId( sal_uInt32 nId );
};

class ImplErrorDialog : public ModalDialog
{
    FixedImage      maFI;
    FixedText       maProcessText;
    ListBox         maErrors;
    FixedText       maExplanation;
    
    OKButton        maButton;
    
    DECL_LINK( SelectHdl, ListBox* );
    public:
    ImplErrorDialog( const std::set< vcl::PDFWriter::ErrorCode >& );
    ~ImplErrorDialog();
};

////////////////////////////////////////////////////////////////////////
//class tabbed dialog
class ImpPDFTabDialog : public SfxTabDialog
{
private:
    com::sun::star::uno::Reference< com::sun::star::lang::XMultiServiceFactory > mxMSF;
    FilterConfigItem            maConfigItem;
    FilterConfigItem            maConfigI18N;

    Any                         maSelection;

protected:
//the following data are the configuration used throughout the dialog and pages
   	sal_Bool                    mbIsPresentation;
    sal_Bool                    mbIsWriter;
    sal_Bool                    mbSelectionPresent;
    sal_Bool                    mbUseCTLFont;
    sal_Bool                    mbUseLosslessCompression;
    sal_Int32                   mnQuality;
    sal_Bool                    mbReduceImageResolution;
    sal_Int32                   mnMaxImageResolution;
    sal_Bool                    mbUseTaggedPDF;
    sal_Int32                   mnPDFTypeSelection;    
    sal_Bool                    mbExportNotes;
	sal_Bool					mbExportNotesPages;
    sal_Bool                    mbUseTransitionEffects;
    sal_Bool                    mbIsSkipEmptyPages;
    sal_Bool                    mbAddStream;
    sal_Bool                    mbEmbedStandardFonts;
    sal_Int32                   mnFormsType;
    sal_Bool                    mbExportFormFields;
    sal_Bool                    mbAllowDuplicateFieldNames;
    sal_Bool                    mbExportBookmarks;
    sal_Int32                   mnOpenBookmarkLevels;

    sal_Bool                    mbHideViewerToolbar;
    sal_Bool                    mbHideViewerMenubar;
    sal_Bool                    mbHideViewerWindowControls;
    sal_Bool                    mbResizeWinToInit;
    sal_Bool                    mbCenterWindow;
    sal_Bool                    mbOpenInFullScreenMode;
    sal_Bool                    mbDisplayPDFDocumentTitle;
    sal_Int32                   mnMagnification;
    sal_Int32                   mnInitialView;
    sal_Int32                   mnZoom;
    sal_Int32                   mnInitialPage;
 
    sal_Int32                   mnPageLayout;
    sal_Bool                    mbFirstPageLeft;

    sal_Bool					mbEncrypt;

    sal_Bool					mbRestrictPermissions;
    com::sun::star::uno::Sequence< com::sun::star::beans::NamedValue > maPreparedOwnerPassword;
    sal_Int32					mnPrint;
    sal_Int32					mnChangesAllowed;
    sal_Bool					mbCanCopyOrExtract;
    sal_Bool					mbCanExtractForAccessibility;
    com::sun::star::uno::Reference< com::sun::star::beans::XMaterialHolder > mxPreparedPasswords;

    sal_Bool                    mbIsRangeChecked;
    String                      msPageRange;
    sal_Bool                    mbSelectionIsChecked;

	sal_Bool                    mbExportRelativeFsysLinks;
	sal_Int32                   mnViewPDFMode;    
	sal_Bool                    mbConvertOOoTargets;
	sal_Bool                    mbExportBmkToPDFDestination;

public:

    friend class				ImpPDFTabGeneralPage;
    friend class				ImpPDFTabViewerPage;
    friend class				ImpPDFTabOpnFtrPage;
    friend class				ImpPDFTabSecurityPage;
    friend class                ImpPDFTabLinksPage;

    ImpPDFTabDialog( Window* pParent,
                     Sequence< PropertyValue >& rFilterData,
                     const Reference< XComponent >& rDoc,
                     const Reference< lang::XMultiServiceFactory >& xFact
                     );
    ~ImpPDFTabDialog(); 

    Sequence< PropertyValue >   GetFilterData();
    const com::sun::star::uno::Reference< com::sun::star::lang::XMultiServiceFactory >& getServiceFactory() const { return mxMSF; }

protected:
    virtual void                PageCreated( sal_uInt16 _nId,
                                             SfxTabPage& _rPage );
    virtual short               Ok();
};

//class tab page general
class ImpPDFTabGeneralPage : public SfxTabPage
{
    friend class                ImpPDFTabLinksPage;

    FixedLine                   maFlPages;
    RadioButton                 maRbAll;
    RadioButton                 maRbRange;
    RadioButton                 maRbSelection;
    Edit                        maEdPages;

    FixedLine                   maFlCompression;
    RadioButton                 maRbLosslessCompression;
    RadioButton                 maRbJPEGCompression;
    FixedText                   maFtQuality;
    MetricField                 maNfQuality;
    CheckBox                    maCbReduceImageResolution;
    ComboBox                    maCoReduceImageResolution;

    FixedLine                   maFlGeneral;
    CheckBox                    maCbPDFA1b;
    CheckBox                    maCbTaggedPDF;
    sal_Bool                    mbTaggedPDFUserSelection;
    
    CheckBox                    maCbExportFormFields;
    sal_Bool                    mbExportFormFieldsUserSelection;
    sal_Bool                    mbEmbedStandardFontsUserSelection;
    FixedText                   maFtFormsFormat;
    ListBox                     maLbFormsFormat;
    CheckBox                    maCbAllowDuplicateFieldNames;
    
    CheckBox                    maCbExportBookmarks;
    CheckBox                    maCbExportNotes;
	CheckBox					maCbExportNotesPages;

    CheckBox                    maCbExportEmptyPages;
    CheckBox                    maCbAddStream;
    CheckBox                    maCbEmbedStandardFonts;

    sal_Bool                    mbIsPresentation;
    sal_Bool                    mbIsWriter;

const ImpPDFTabDialog*          mpaParent;

    DECL_LINK( TogglePagesHdl, void* );
    DECL_LINK( ToggleCompressionHdl, void* );
    DECL_LINK( ToggleReduceImageResolutionHdl, void* );
    DECL_LINK( ToggleAddStreamHdl, void* );
    DECL_LINK( ToggleExportFormFieldsHdl, void* );

public:
    DECL_LINK( ToggleExportPDFAHdl, void* );

    ImpPDFTabGeneralPage( Window* pParent,
                          const SfxItemSet& rSet );

    ~ImpPDFTabGeneralPage();
    static SfxTabPage*          Create( Window* pParent,
                                        const SfxItemSet& rAttrSet);

    void                        GetFilterConfigItem( ImpPDFTabDialog* paParent );
    void                        SetFilterConfigItem( const ImpPDFTabDialog* paParent );
    sal_Bool                    IsPdfaSelected() { return maCbPDFA1b.IsChecked(); };
};

//class tab page viewer
class ImpPDFTabOpnFtrPage : public SfxTabPage
{
    FixedLine                   maFlInitialView;
    RadioButton                 maRbOpnPageOnly;
    RadioButton                 maRbOpnOutline;
    RadioButton                 maRbOpnThumbs;
    FixedText                   maFtInitialPage;
    NumericField                maNumInitialPage;

    FixedLine                   maFlMagnification;
    RadioButton                 maRbMagnDefault;
    RadioButton                 maRbMagnFitWin;
    RadioButton                 maRbMagnFitWidth;
    RadioButton                 maRbMagnFitVisible;
    RadioButton                 maRbMagnZoom;
    MetricField                 maNumZoom;

    FixedLine			        maFlPageLayout;
    RadioButton                 maRbPgLyDefault;
    RadioButton                 maRbPgLySinglePage;
    RadioButton                 maRbPgLyContinue;
    RadioButton                 maRbPgLyContinueFacing;
    CheckBox                    maCbPgLyFirstOnLeft;

    sal_Bool                    mbUseCTLFont;

    DECL_LINK( ToggleRbPgLyContinueFacingHdl, void* );
    DECL_LINK( ToggleRbMagnHdl, void* );

public:
    ImpPDFTabOpnFtrPage( Window* pParent,
                         const SfxItemSet& rSet );

    ~ImpPDFTabOpnFtrPage();
    static SfxTabPage*          Create( Window* pParent,
                                        const SfxItemSet& rAttrSet );

    void                        GetFilterConfigItem( ImpPDFTabDialog* paParent);
    void                        SetFilterConfigItem( const ImpPDFTabDialog* paParent );
};

//class tab page viewer
class ImpPDFTabViewerPage : public SfxTabPage
{
    FixedLine                   maFlWindowOptions;
    CheckBox                    maCbResWinInit;
    CheckBox                    maCbCenterWindow;
    CheckBox                    maCbOpenFullScreen;
    CheckBox                    maCbDispDocTitle;

    FixedLine                   maFlUIOptions;
    CheckBox                    maCbHideViewerMenubar;
    CheckBox                    maCbHideViewerToolbar;
    CheckBox                    maCbHideViewerWindowControls;

    FixedLine                   maFlTransitions;
    CheckBox                    maCbTransitionEffects;
    sal_Bool                    mbIsPresentation;
    
    FixedLine                   maFlBookmarks;
    RadioButton                 maRbAllBookmarkLevels;
    RadioButton                 maRbVisibleBookmarkLevels;
    NumericField                maNumBookmarkLevels;
    
    DECL_LINK( ToggleRbBookmarksHdl, void* );
public:
    ImpPDFTabViewerPage( Window* pParent,
                         const SfxItemSet& rSet );

    ~ImpPDFTabViewerPage();
    static SfxTabPage*          Create( Window* pParent,
                                        const SfxItemSet& rAttrSet );

    void                        GetFilterConfigItem( ImpPDFTabDialog* paParent);
    void                        SetFilterConfigItem( const ImpPDFTabDialog* paParent );
};

//class security tab page
class ImpPDFTabSecurityPage : public SfxTabPage
{
    FixedLine                   maFlGroup;
	PushButton					maPbSetPwd;
	FixedText					maFtUserPwd;
    String                      maUserPwdSet;
    String                      maUserPwdUnset;
    String                      maUserPwdPdfa;
    String                      maStrSetPwd;

	FixedText					maFtOwnerPwd;
    String                      maOwnerPwdSet;
    String                      maOwnerPwdUnset;
    String                      maOwnerPwdPdfa;

	FixedLine					maFlPrintPermissions;
	RadioButton					maRbPrintNone;
	RadioButton					maRbPrintLowRes;
	RadioButton					maRbPrintHighRes;

	FixedLine					maFlChangesAllowed;
	RadioButton					maRbChangesNone;
	RadioButton					maRbChangesInsDel;
	RadioButton					maRbChangesFillForm;
	RadioButton					maRbChangesComment;
	RadioButton					maRbChangesAnyNoCopy;

	CheckBox					maCbEnableCopy;
	CheckBox					maCbEnableAccessibility;

	String						msUserPwdTitle;

	bool                        mbHaveOwnerPassword;
	bool                        mbHaveUserPassword;
    com::sun::star::uno::Sequence< com::sun::star::beans::NamedValue > maPreparedOwnerPassword;
	String						msOwnerPwdTitle;

    com::sun::star::uno::Reference< com::sun::star::beans::XMaterialHolder > mxPreparedPasswords;

	long nWidth;

	DECL_LINK( ClickmaPbSetPwdHdl, void* );
    
    void enablePermissionControls();

	void						ImplPwdPushButton( const String &, String & );

public:
	ImpPDFTabSecurityPage( Window* pParent,
						   const SfxItemSet& rSet );

	~ImpPDFTabSecurityPage();
	static SfxTabPage*		Create( Window* pParent,
									const SfxItemSet& rAttrSet );

    void	GetFilterConfigItem( ImpPDFTabDialog* paParent);
	void	SetFilterConfigItem( const ImpPDFTabDialog* paParent );
    void    ImplPDFASecurityControl( sal_Bool bEnableSecurity );
    bool    hasPassword() const { return mbHaveOwnerPassword || mbHaveUserPassword; }
};

//class to implement the relative link stuff
class ImpPDFTabLinksPage : public SfxTabPage
{
    CheckBox                    maCbExprtBmkrToNmDst;
    CheckBox                    maCbOOoToPDFTargets;    
    CheckBox                    maCbExportRelativeFsysLinks;

    FixedLine                   maFlDefaultTitle;
	RadioButton                 maRbOpnLnksDefault;
    sal_Bool                    mbOpnLnksDefaultUserState;
    RadioButton                 maRbOpnLnksLaunch;
    sal_Bool                    mbOpnLnksLaunchUserState;
	RadioButton                 maRbOpnLnksBrowser;
    sal_Bool                    mbOpnLnksBrowserUserState;

	long nWidth;
    
    DECL_LINK( ClickRbOpnLnksDefaultHdl, void* );
    DECL_LINK( ClickRbOpnLnksBrowserHdl, void* );

public:
	ImpPDFTabLinksPage( Window* pParent,
						   const SfxItemSet& rSet );

	~ImpPDFTabLinksPage();
	static SfxTabPage*		Create( Window* pParent,
									const SfxItemSet& rAttrSet );

	void	GetFilterConfigItem( ImpPDFTabDialog* paParent);
	void	SetFilterConfigItem( const ImpPDFTabDialog* paParent );

    void    ImplPDFALinkControl( sal_Bool bEnableLaunch );
};

#endif // IMPDIALOG_HXX


