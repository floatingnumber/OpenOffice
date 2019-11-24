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



#ifndef SVT_DRAWERLAYOUTER_HXX
#define SVT_DRAWERLAYOUTER_HXX

#include "svtools/svtdllapi.h"
#include "svtools/toolpanel/refbase.hxx"
#include "svtools/toolpanel/toolpaneldeck.hxx"
#include "svtools/toolpanel/decklayouter.hxx"

#include <boost/shared_ptr.hpp>

//......................................................................................................................
namespace svt
{
//......................................................................................................................

    class ToolPanelViewShell;
    class ToolPanelDrawer;
    typedef ::boost::shared_ptr< ToolPanelDrawer >  PToolPanelDrawer;

	//==================================================================================================================
	//= ToolPanelDrawer
	//==================================================================================================================
    /** a class which implements a tool panel selector in the form of the classical drawers
    */
    class SVT_DLLPUBLIC DrawerDeckLayouter  :public RefBase
                                            ,public IDeckLayouter
                                            ,public IToolPanelDeckListener
	{
    public:
        DrawerDeckLayouter(
            ::Window& i_rParentWindow,
            IToolPanelDeck& i_rPanels
        );
        ~DrawerDeckLayouter();

        // IReference
        DECLARE_IREFERENCE()

        // IDeckLayouter
        virtual Rectangle   Layout( const Rectangle& i_rDeckPlayground );
        virtual void        Destroy();
        virtual void        SetFocusToPanelSelector();
        virtual size_t      GetAccessibleChildCount() const;
        virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible >
                            GetAccessibleChild(
                                const size_t i_nChildIndex,
                                const ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible >& i_rParentAccessible
                            );

        // IToolPanelDeckListener
        virtual void PanelInserted( const PToolPanel& i_pPanel, const size_t i_nPosition );
        virtual void PanelRemoved( const size_t i_nPosition );
        virtual void ActivePanelChanged( const ::boost::optional< size_t >& i_rOldActive, const ::boost::optional< size_t >& i_rNewActive );
        virtual void LayouterChanged( const PDeckLayouter& i_rNewLayouter );
        virtual void Dying();

    private:
        // triggers a re-arrance of the panel deck elements
        void    impl_triggerRearrange() const;
        size_t  impl_getPanelPositionFromWindow( const Window* i_pDrawerWindow ) const;
        void    impl_removeDrawer( const size_t i_nPosition );

        DECL_LINK( OnWindowEvent, VclSimpleEvent* );

private:
        Window&                             m_rParentWindow;
        IToolPanelDeck&                     m_rPanelDeck;
        ::std::vector< PToolPanelDrawer >   m_aDrawers;
        ::boost::optional< size_t >         m_aLastKnownActivePanel;
	};

//......................................................................................................................
} // namespace svt
//......................................................................................................................

#endif // SVT_DRAWERLAYOUTER_HXX
