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




#ifndef DBACCESS_DOCUMENTEVENTNOTIFIER_HXX
#define DBACCESS_DOCUMENTEVENTNOTIFIER_HXX

/** === begin UNO includes === **/
#include <com/sun/star/document/XEventListener.hpp>
#include <com/sun/star/document/XDocumentEventListener.hpp>
/** === end UNO includes === **/

#include <rtl/ref.hxx>

namespace cppu
{
    class OWeakObject;
}

//........................................................................
namespace dbaccess
{
//........................................................................

    class DocumentEventNotifier_Impl;
	//====================================================================
	//= DocumentEventNotifier
	//====================================================================
	class DocumentEventNotifier
	{
    public:
        DocumentEventNotifier( ::cppu::OWeakObject& _rBroadcasterDocument, ::osl::Mutex& _rMutex );
        ~DocumentEventNotifier();

        void addLegacyEventListener( const ::com::sun::star::uno::Reference< ::com::sun::star::document::XEventListener >& _Listener );
        void removeLegacyEventListener( const ::com::sun::star::uno::Reference< ::com::sun::star::document::XEventListener >& _Listener );
        void addDocumentEventListener( const ::com::sun::star::uno::Reference< ::com::sun::star::document::XDocumentEventListener >& _Listener );
        void removeDocumentEventListener( const ::com::sun::star::uno::Reference< ::com::sun::star::document::XDocumentEventListener >& _Listener );

        /** disposes the instance
            @precond
                the mutex is not locked
        */
        void    disposing();

        /** tells the instance that its document is completely initialized now.

            Before you call this method, no notification will actually happen

            @precond
                the mutex is locked
        */
        void    onDocumentInitialized();

        /** notifies a document event described by the given parameters

            @precond
                the mutex is not locked
            @precond
                ->onDocumentInitialized has been called
        */
        void    notifyDocumentEvent(
                    const ::rtl::OUString& _EventName,
                    const ::com::sun::star::uno::Reference< ::com::sun::star::frame::XController2 >& _rxViewController = NULL,
                    const ::com::sun::star::uno::Any& _Supplement = ::com::sun::star::uno::Any()
                );

        /** notifies a document event, described by the given parameters, asynchronously

            Note that no event is actually notified before you called ->onDocumentInitialized.

            @precond
                the mutex is locked
        */
        void    notifyDocumentEventAsync(
                    const ::rtl::OUString& _EventName,
                    const ::com::sun::star::uno::Reference< ::com::sun::star::frame::XController2 >& _ViewController = NULL,
                    const ::com::sun::star::uno::Any& _Supplement = ::com::sun::star::uno::Any()
                );

        /** notifies a document event to all registered listeners

            @precond
                the mutex is not locked
            @precond
                ->onDocumentInitialized has been called
        */
        void    notifyDocumentEvent(
                    const sal_Char* _pAsciiEventName,
                    const ::com::sun::star::uno::Reference< ::com::sun::star::frame::XController2 >& _rxViewController = NULL,
                    const ::com::sun::star::uno::Any& _rSupplement = ::com::sun::star::uno::Any()
                )
        {
            notifyDocumentEvent( ::rtl::OUString::createFromAscii( _pAsciiEventName ), _rxViewController, _rSupplement );
        }

        /** notifies a document event to all registered listeners, asynchronously

            Note that no event is actually notified before you called ->onDocumentInitialized.

            @precond
                the mutex is locked
        */
        void    notifyDocumentEventAsync(
                    const sal_Char* _pAsciiEventName,
                    const ::com::sun::star::uno::Reference< ::com::sun::star::frame::XController2 >& _rxViewController = NULL,
                    const ::com::sun::star::uno::Any& _rSupplement = ::com::sun::star::uno::Any()
                )
        {
            notifyDocumentEventAsync( ::rtl::OUString::createFromAscii( _pAsciiEventName ), _rxViewController, _rSupplement );
        }

    private:
        ::rtl::Reference< DocumentEventNotifier_Impl >   m_pImpl;
	};

//........................................................................
} // namespace dbaccess
//........................................................................

#endif // DBACCESS_DOCUMENTEVENTNOTIFIER_HXX
