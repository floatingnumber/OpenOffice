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



#ifndef _FTP_FTPINTREQ_HXX
#define _FTP_FTPINTREQ_HXX

#include <cppuhelper/weak.hxx>
#include <ucbhelper/macros.hxx>
#include <rtl/ustring.hxx>
#include <com/sun/star/uno/XInterface.hpp>
#include <com/sun/star/lang/XTypeProvider.hpp>
#include <com/sun/star/task/XInteractionDisapprove.hpp>
#include <com/sun/star/task/XInteractionApprove.hpp>
#include <com/sun/star/task/XInteractionRequest.hpp>


namespace ftp {


    class XInteractionApproveImpl
        : public cppu::OWeakObject,
          public com::sun::star::lang::XTypeProvider,
          public com::sun::star::task::XInteractionApprove
    {
    public:

        XInteractionApproveImpl();

        virtual com::sun::star::uno::Any SAL_CALL
        queryInterface(
            const com::sun::star::uno::Type& rType )
            throw( com::sun::star::uno::RuntimeException);

        virtual void SAL_CALL
        acquire(
            void )
            throw();

        virtual void SAL_CALL
        release(
            void )
            throw();


        // XTypeProvider

        XTYPEPROVIDER_DECL()

        virtual void SAL_CALL select()
            throw (com::sun::star::uno::RuntimeException);

        bool isSelected() const;

    private:

        bool          m_bSelected;
    };




    class XInteractionDisapproveImpl
        : public cppu::OWeakObject,
          public com::sun::star::lang::XTypeProvider,
          public com::sun::star::task::XInteractionDisapprove
    {
    public:

        XInteractionDisapproveImpl();

        virtual com::sun::star::uno::Any SAL_CALL
        queryInterface(
            const com::sun::star::uno::Type& rType )
            throw( com::sun::star::uno::RuntimeException);

        virtual void SAL_CALL
        acquire(
            void )
            throw();

        virtual void SAL_CALL
        release(
            void )
            throw();


        // XTypeProvider

        XTYPEPROVIDER_DECL()

        virtual void SAL_CALL select()
            throw (com::sun::star::uno::RuntimeException);

    private:

        bool          m_bSelected;
    };



    class XInteractionRequestImpl
        : public cppu::OWeakObject,
          public com::sun::star::lang::XTypeProvider,
          public com::sun::star::task::XInteractionRequest
    {
    public:

        XInteractionRequestImpl(const rtl::OUString& aName);

        virtual com::sun::star::uno::Any SAL_CALL
        queryInterface(
            const com::sun::star::uno::Type& rType )
            throw( com::sun::star::uno::RuntimeException);

        virtual void SAL_CALL
        acquire(
            void )
            throw();

        virtual void SAL_CALL
        release(
            void )
            throw();


        // XTypeProvider

        XTYPEPROVIDER_DECL()

        com::sun::star::uno::Any SAL_CALL
        getRequest(  )
            throw (com::sun::star::uno::RuntimeException);

        com::sun::star::uno::Sequence<
            com::sun::star::uno::Reference<
            com::sun::star::task::XInteractionContinuation > > SAL_CALL
        getContinuations(  )
            throw (com::sun::star::uno::RuntimeException);

        bool approved() const;

    private:

        XInteractionApproveImpl* p1;
        XInteractionDisapproveImpl* p2;

        rtl::OUString m_aName;

        com::sun::star::uno::Sequence<
            com::sun::star::uno::Reference<
            com::sun::star::task::XInteractionContinuation > > m_aSeq;
    };

}


#endif
