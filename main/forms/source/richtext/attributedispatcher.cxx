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
#include "precompiled_forms.hxx"
#include "attributedispatcher.hxx"

/** === begin UNO includes === **/
/** === end UNO includes === **/
#include <editeng/editview.hxx>

//........................................................................
namespace frm
{
//........................................................................

    using namespace ::com::sun::star::uno;
    using namespace ::com::sun::star::frame;
    using namespace ::com::sun::star::lang;
    using namespace ::com::sun::star::util;
    using namespace ::com::sun::star::beans;

    //====================================================================
	//= OAttributeDispatcher
	//====================================================================
	//--------------------------------------------------------------------
    OAttributeDispatcher::OAttributeDispatcher( EditView& _rView, AttributeId _nAttributeId, const URL& _rURL,
            IMultiAttributeDispatcher* _pMasterDispatcher )
        :ORichTextFeatureDispatcher( _rView, _rURL )
        ,m_pMasterDispatcher( _pMasterDispatcher )
        ,m_nAttributeId( _nAttributeId )
    {
        OSL_ENSURE( m_pMasterDispatcher, "OAttributeDispatcher::OAttributeDispatcher: invalid master dispatcher!" );
    }

	//--------------------------------------------------------------------
    OAttributeDispatcher::~OAttributeDispatcher( )
    {
        acquire();
        dispose();
    }

    //--------------------------------------------------------------------
    void OAttributeDispatcher::disposing( ::osl::ClearableMutexGuard& _rClearBeforeNotify )
    {
        m_pMasterDispatcher = NULL;
        ORichTextFeatureDispatcher::disposing( _rClearBeforeNotify );
    }

    //--------------------------------------------------------------------
    void OAttributeDispatcher::fillFeatureEventFromAttributeState( FeatureStateEvent& _rEvent, const AttributeState& _rState ) const
    {
        if ( _rState.eSimpleState == eChecked )
            _rEvent.State <<= (sal_Bool)sal_True;
        else if ( _rState.eSimpleState == eUnchecked )
            _rEvent.State <<= (sal_Bool)sal_False;
    }

    //--------------------------------------------------------------------
    FeatureStateEvent OAttributeDispatcher::buildStatusEvent() const
    {
        FeatureStateEvent aEvent( ORichTextFeatureDispatcher::buildStatusEvent() );
        aEvent.IsEnabled = getEditView() ? !getEditView()->IsReadOnly() : sal_False;

        AttributeState aState;
        if ( m_pMasterDispatcher )
            aState = m_pMasterDispatcher->getState( m_nAttributeId );

        fillFeatureEventFromAttributeState( aEvent, aState );

        return aEvent;
    }

    //--------------------------------------------------------------------
    void SAL_CALL OAttributeDispatcher::dispatch( const URL& _rURL, const Sequence< PropertyValue >& _rArguments ) throw (RuntimeException)
    {
        ::osl::MutexGuard aGuard( m_aMutex );

        checkDisposed();

        (void)_rURL;
        (void)_rArguments;

        OSL_ENSURE( _rURL.Complete == getFeatureURL().Complete, "OAttributeDispatcher::dispatch: invalid URL!" );
#if OSL_DEBUG_LEVEL > 0
        if ( _rArguments.getLength() )
        {
            ::rtl::OString sMessage( "OAttributeDispatcher::dispatch: found arguments, but can't handle arguments at all" );
            sMessage += "\n (URL: ";
            sMessage += ::rtl::OString( _rURL.Complete.getStr(), _rURL.Complete.getLength(), RTL_TEXTENCODING_ASCII_US );
            sMessage += ")";
            DBG_ERROR( sMessage.getStr() );
        }
#endif

        if ( m_pMasterDispatcher )
            m_pMasterDispatcher->executeAttribute( m_nAttributeId, NULL );
    }
    
    //--------------------------------------------------------------------
    void OAttributeDispatcher::onAttributeStateChanged( AttributeId _nAttributeId, const AttributeState& /*_rState*/ )
    {
        OSL_ENSURE( _nAttributeId == m_nAttributeId, "OAttributeDispatcher::onAttributeStateChanged: wrong attribute!" );
        (void)_nAttributeId;

        FeatureStateEvent aEvent( buildStatusEvent() );
        ::cppu::OInterfaceIteratorHelper aIter( getStatusListeners() );
    	while ( aIter.hasMoreElements() )
            doNotify( static_cast< XStatusListener* >( aIter.next() ), aEvent );
    }

//........................................................................
}   // namespace frm
//........................................................................
