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
#include "precompiled_framework.hxx"

//_________________________________________________________________________________________________________________
//	my own includes
//_________________________________________________________________________________________________________________

#include <limits.h>

#include "framework/bmkmenu.hxx"
#include <general.h>
#include <macros/debug/assertion.hxx>
#include <framework/imageproducer.hxx>
#include <framework/menuconfiguration.hxx>

//_________________________________________________________________________________________________________________
//	interface includes
//_________________________________________________________________________________________________________________
#include <com/sun/star/uno/Reference.h>
#include <com/sun/star/util/URL.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
#ifndef _UNOTOOLS_PROCESSFACTORY_HXX
#include <comphelper/processfactory.hxx>
#endif
#include <com/sun/star/util/XURLTransformer.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/util/DateTime.hpp>

//_________________________________________________________________________________________________________________
//	includes of other projects
//_________________________________________________________________________________________________________________
#include <tools/config.hxx>
#include <vcl/svapp.hxx>
#include <unotools/dynamicmenuoptions.hxx>
#include <svtools/menuoptions.hxx>
#include <rtl/logfile.hxx>

//_________________________________________________________________________________________________________________
//	namespace
//_________________________________________________________________________________________________________________

using namespace ::comphelper;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::util;
using namespace ::com::sun::star::frame;
using namespace ::com::sun::star::beans;

namespace framework
{

void GetMenuEntry(
	Sequence< PropertyValue >&	aDynamicMenuEntry,
	::rtl::OUString&			rTitle,
	::rtl::OUString&			rURL,
	::rtl::OUString&			rFrame,
	::rtl::OUString&			rImageId );

class BmkMenu_Impl
{
	private:
		static sal_uInt16		 m_nMID;

	public:
		BmkMenu*			 m_pRoot;
		sal_Bool                 m_bInitialized;

		BmkMenu_Impl( BmkMenu* pRoot );
		BmkMenu_Impl();
		~BmkMenu_Impl();

		static sal_uInt16		GetMID();
};

sal_uInt16 BmkMenu_Impl::m_nMID = BMKMENU_ITEMID_START;

BmkMenu_Impl::BmkMenu_Impl( BmkMenu* pRoot ) :
	m_pRoot(pRoot),
	m_bInitialized(sal_False)
{
}

BmkMenu_Impl::BmkMenu_Impl() :
	m_pRoot(0),
	m_bInitialized(sal_False)
{
}

BmkMenu_Impl::~BmkMenu_Impl()
{
}

sal_uInt16 BmkMenu_Impl::GetMID()
{
	m_nMID++;
	if( !m_nMID )
        m_nMID = BMKMENU_ITEMID_START;
	return m_nMID;
}

// ------------------------------------------------------------------------

BmkMenu::BmkMenu( com::sun::star::uno::Reference< XFrame >& xFrame, BmkMenu::BmkMenuType nType, BmkMenu* pRoot )
    :AddonMenu(xFrame)
    ,m_nType( nType )
{
    _pImp = new BmkMenu_Impl( pRoot );
    Initialize();
}

BmkMenu::BmkMenu( Reference< XFrame >& xFrame, BmkMenu::BmkMenuType nType )
    :AddonMenu(xFrame)
    ,m_nType( nType )
{
    _pImp = new BmkMenu_Impl();
    Initialize();
}

BmkMenu::~BmkMenu()
{
	delete _pImp;
}

void BmkMenu::Initialize()
{
    RTL_LOGFILE_CONTEXT( aLog, "framework (cd100003) ::BmkMenu::Initialize" );

    if( _pImp->m_bInitialized )
		return;

    _pImp->m_bInitialized = sal_True;

	Sequence< Sequence< PropertyValue > > aDynamicMenuEntries;

	if ( m_nType == BmkMenu::BMK_NEWMENU )
		aDynamicMenuEntries = SvtDynamicMenuOptions().GetMenu( E_NEWMENU );
	else if ( m_nType == BmkMenu::BMK_WIZARDMENU )
		aDynamicMenuEntries = SvtDynamicMenuOptions().GetMenu( E_WIZARDMENU );

	const StyleSettings& rSettings = Application::GetSettings().GetStyleSettings();
	sal_Bool bShowMenuImages = rSettings.GetUseImagesInMenus();

	::rtl::OUString aTitle;
	::rtl::OUString aURL;
	::rtl::OUString aTargetFrame;
	::rtl::OUString aImageId;

	sal_Bool bIsHiContrastMode = rSettings.GetHighContrastMode();

	sal_uInt32 i, nCount = aDynamicMenuEntries.getLength();
	for ( i = 0; i < nCount; ++i )
	{
		GetMenuEntry( aDynamicMenuEntries[i], aTitle, aURL, aTargetFrame, aImageId );

		if ( !aTitle.getLength() && !aURL.getLength() )
			continue;

        if ( aURL == ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "private:separator" )))
			InsertSeparator();
		else
		{
			sal_Bool	bImageSet = sal_False;
			sal_uInt16		nId = CreateMenuId();

			if ( bShowMenuImages )
			{
				if ( aImageId.getLength() > 0 )
				{
					Image aImage = GetImageFromURL( m_xFrame, aImageId, sal_False, bIsHiContrastMode );
					if ( !!aImage )
					{
						bImageSet = sal_True;
						InsertItem( nId, aTitle, aImage );
					}
				}

				if ( !bImageSet )
				{
					Image aImage = GetImageFromURL( m_xFrame, aURL, sal_False, bIsHiContrastMode );
					if ( !aImage )
						InsertItem( nId, aTitle );
					else
						InsertItem( nId, aTitle, aImage );
				}
			}
			else
				InsertItem( nId, aTitle );

			// Store values from configuration to the New and Wizard menu entries to enable
			// sfx2 based code to support high contrast mode correctly!
			MenuConfiguration::Attributes* pUserAttributes = new MenuConfiguration::Attributes( aTargetFrame, aImageId );
			SetUserValue( nId, (sal_uIntPtr)pUserAttributes );

			SetItemCommand( nId, aURL );
		}
	}
}

sal_uInt16 BmkMenu::CreateMenuId()
{
    return BmkMenu_Impl::GetMID();
}

void GetMenuEntry
(
	Sequence< PropertyValue >& aDynamicMenuEntry,
	::rtl::OUString& rTitle,
	::rtl::OUString& rURL,
    ::rtl::OUString& rFrame,
	::rtl::OUString& rImageId
)
{
	for ( int i = 0; i < aDynamicMenuEntry.getLength(); i++ )
	{
		if ( aDynamicMenuEntry[i].Name == DYNAMICMENU_PROPERTYNAME_URL )
			aDynamicMenuEntry[i].Value >>= rURL;
		else if ( aDynamicMenuEntry[i].Name == DYNAMICMENU_PROPERTYNAME_TITLE )
			aDynamicMenuEntry[i].Value >>= rTitle;
		else if ( aDynamicMenuEntry[i].Name == DYNAMICMENU_PROPERTYNAME_IMAGEIDENTIFIER )
			aDynamicMenuEntry[i].Value >>= rImageId;
		else if ( aDynamicMenuEntry[i].Name == DYNAMICMENU_PROPERTYNAME_TARGETNAME )
			aDynamicMenuEntry[i].Value >>= rFrame;
	}
}

}

