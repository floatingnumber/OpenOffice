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



#ifndef _SVTOOLS_TOOLBARMENU_HXX_
#define _SVTOOLS_TOOLBARMENU_HXX_

#include "svtools/svtdllapi.h"

#include <com/sun/star/frame/FeatureStateEvent.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/frame/XFrame.hpp>

#include <vector>

#include <rtl/ref.hxx>

#include <vcl/ctrl.hxx>
#include <vcl/menu.hxx>
#include <vcl/dockwin.hxx>

class ValueSet;

namespace svtools {

class ToolbarMenuEntry;
struct ToolbarMenu_Impl;

class SVT_DLLPUBLIC ToolbarMenu : public DockingWindow
{
	friend class ToolbarMenuStatusListener;
	friend struct ToolbarMenu_Impl;
public:
					ToolbarMenu( const ::com::sun::star::uno::Reference< ::com::sun::star::frame::XFrame >& rFrame,
                                 Window* pParentWindow,
                                 WinBits nBits );
                    
                    ToolbarMenu( const ::com::sun::star::uno::Reference< ::com::sun::star::frame::XFrame >& rFrame,
                                 Window* pParentWindow,
                                 const ResId& rResId );
                    
                    ~ToolbarMenu();

    virtual void    MouseMove( const MouseEvent& rMEvt );
    virtual void    MouseButtonDown( const MouseEvent& rMEvt );
    virtual void    MouseButtonUp( const MouseEvent& rMEvt );
    virtual void    KeyInput( const KeyEvent& rKEvent );
    virtual void    Command( const CommandEvent& rCEvt );
    virtual void    Paint( const Rectangle& rRect );
    virtual void    RequestHelp( const HelpEvent& rHEvt );
	virtual void	GetFocus();
    virtual void	LoseFocus();

	void			appendEntry( int nEntryId, const String& rStr, MenuItemBits nItemBits = 0 );
	void			appendEntry( int nEntryId, const Image& rImage, MenuItemBits nItemBits = 0 );
	void			appendEntry( int nEntryId, const String& rStr, const Image& rImage, MenuItemBits nItemBits = 0 );
	void			appendEntry( int nEntryId, Control* pControl, MenuItemBits nItemBits = 0 );
	void			appendSeparator();

	/** creates an empty ValueSet that is initialized and can be inserted with appendEntry. */
	ValueSet*		createEmptyValueSetControl();

	void			checkEntry( int nEntryId, bool bCheck = true );
	bool			isEntryChecked( int nEntryId ) const;

	void			enableEntry( int nEntryId, bool bEnable = true );
	bool			isEntryEnabled( int nEntryId ) const;

	void			setEntryText( int nEntryId, const String& rStr );
	const String&	getEntryText( int nEntryId ) const;

	void			setEntryImage( int nEntryId, const Image& rImage );
	const Image&	getEntryImage( int nEntryId ) const;

	const Size&		getMenuSize() const;

	void			SetSelectHdl( const Link& rLink );
	const Link& 	GetSelectHdl() const;

	int				getSelectedEntryId() const;
	int				getHighlightedEntryId() const;

	void			highlightFirstEntry();

protected:
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > CreateAccessible();

	// todo: move to new base class that will replace SfxPopupWindow
    void AddStatusListener( const rtl::OUString& rCommandURL );
    void RemoveStatusListener( const rtl::OUString& rCommandURL );
    void UpdateStatus( const rtl::OUString& rCommandURL );

	bool IsInPopupMode();
	void EndPopupMode();

    // XStatusListener (subclasses must override this one to get the status updates
	virtual void SAL_CALL statusChanged( const ::com::sun::star::frame::FeatureStateEvent& Event ) throw ( ::com::sun::star::uno::RuntimeException );

    void            StateChanged( StateChangedType nType );
    void            DataChanged( const DataChangedEvent& rDCEvt );
	
	::com::sun::star::uno::Reference< ::com::sun::star::frame::XFrame >	GetFrame() const;
private:
	DECL_LINK( HighlightHdl, Control * );

	void initStatusListener();

	void			implInit( const ::com::sun::star::uno::Reference< ::com::sun::star::frame::XFrame >& rFrame );
	void			initWindow();

	Size			implCalcSize();

	void			appendEntry( ToolbarMenuEntry* pEntry );

	void			implPaint( ToolbarMenuEntry* pThisOnly = NULL, bool bHighlight = false );
    
	void			implHighlightEntry( int nHighlightEntry, bool bHighlight );
	void			implHighlightEntry( const MouseEvent& rMEvt, bool bMBDown );

	void			implChangeHighlightEntry( int nEntry );
	void			implSelectEntry( int nSelectedEntry );

	ToolbarMenuEntry*	implCursorUpDown( bool bUp, bool bHomeEnd );
	ToolbarMenuEntry*	implGetEntry( int nEntry ) const;
	ToolbarMenuEntry*	implSearchEntry( int nEntryId ) const;

	ToolbarMenu_Impl*	mpImpl;
};

} // namespace svtools

#endif
