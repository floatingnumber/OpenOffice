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



#ifndef SUBCOMPONENTRECOVERY_HXX
#define SUBCOMPONENTRECOVERY_HXX

#include "subcomponents.hxx"

/** === begin UNO includes === **/
#include <com/sun/star/sdb/application/XDatabaseDocumentUI.hpp>
#include <com/sun/star/embed/XStorage.hpp>
/** === end UNO includes === **/

#include <comphelper/componentcontext.hxx>

//........................................................................
namespace dbaccess
{
//........................................................................

	//====================================================================
	//= SubComponentRecovery
	//====================================================================
    class DBACCESS_DLLPRIVATE SubComponentRecovery
    {
    public:
        SubComponentRecovery( const ::comphelper::ComponentContext& i_rContext, const ::com::sun::star::uno::Reference< ::com::sun::star::sdb::application::XDatabaseDocumentUI >& i_rController,
                const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent >& i_rComponent )
            :m_rContext( i_rContext )
            ,m_xDocumentUI( i_rController, ::com::sun::star::uno::UNO_SET_THROW )
            ,m_xComponent( i_rComponent )
            ,m_eType( UNKNOWN )
            ,m_aCompDesc()
        {
            impl_identifyComponent_throw();
        }

        SubComponentRecovery( const ::comphelper::ComponentContext& i_rContext, const ::com::sun::star::uno::Reference< ::com::sun::star::sdb::application::XDatabaseDocumentUI >& i_rController,
                const SubComponentType i_eType )
            :m_rContext( i_rContext )
            ,m_xDocumentUI( i_rController, ::com::sun::star::uno::UNO_SET_THROW )
            ,m_xComponent()
            ,m_eType( i_eType )
            ,m_aCompDesc()
        {
        }

        // only to be used after being constructed with a component
        void    saveToRecoveryStorage(
                    const ::com::sun::star::uno::Reference< ::com::sun::star::embed::XStorage >& i_rRecoveryStorage,
                    MapCompTypeToCompDescs& io_mapCompDescs
                );

        // only to be used after being constructed with a type
        ::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent >
                recoverFromStorage(
                    const ::com::sun::star::uno::Reference< ::com::sun::star::embed::XStorage >& i_rRecoveryStorage,
                    const ::rtl::OUString& i_rComponentName,
                    const bool i_bForEditing
                );

        static const ::rtl::OUString getComponentsStorageName( const SubComponentType i_eType );

    private:
        void    impl_saveSubDocument_throw(
                    const ::com::sun::star::uno::Reference< ::com::sun::star::embed::XStorage >& i_rObjectStorage
                );

        void    impl_saveQueryDesign_throw(
                    const ::com::sun::star::uno::Reference< ::com::sun::star::embed::XStorage >& i_rObjectStorage
                );

        ::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent >
                impl_recoverSubDocument_throw(
                    const ::com::sun::star::uno::Reference< ::com::sun::star::embed::XStorage >& i_rRecoveryStorage,
                    const ::rtl::OUString& i_rComponentName,
                    const bool i_bForEditing
                );

        ::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent >
                impl_recoverQueryDesign_throw(
                    const ::com::sun::star::uno::Reference< ::com::sun::star::embed::XStorage >& i_rRecoveryStorage,
                    const ::rtl::OUString& i_rComponentName,
                    const bool i_bForEditing
                );

        void    impl_identifyComponent_throw();

    private:
        const ::comphelper::ComponentContext&   m_rContext;
        ::com::sun::star::uno::Reference< ::com::sun::star::sdb::application::XDatabaseDocumentUI >
                                                m_xDocumentUI;
        const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XComponent >
                                                m_xComponent;
        SubComponentType                        m_eType;
        SubComponentDescriptor                  m_aCompDesc;
    };


//........................................................................
} // namespace dbaccess
//........................................................................

#endif // SUBCOMPONENTRECOVERY_HXX
