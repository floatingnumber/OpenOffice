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



#ifndef CONNECTIVITY_SCONNECTION_HXX
#define CONNECTIVITY_SCONNECTION_HXX

#include <com/sun/star/sdbc/SQLWarning.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
#include "OSubComponent.hxx"
#include "OTypeInfo.hxx"
#include <com/sun/star/lang/DisposedException.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/lang/XUnoTunnel.hpp>
#include <com/sun/star/sdbc/XWarningsSupplier.hpp>
#include <com/sun/star/sdbc/XConnection.hpp>
#include <cppuhelper/compbase3.hxx>
#include <cppuhelper/weakref.hxx>

#include <map>

namespace connectivity
{
	namespace skeleton
	{

		typedef ::cppu::WeakComponentImplHelper3<   ::com::sun::star::sdbc::XConnection,
												::com::sun::star::sdbc::XWarningsSupplier,
                                                ::com::sun::star::lang::XServiceInfo
											> OMetaConnection_BASE;

		class OStatement_Base;
		class SkeletonDriver;
		class ODatabaseMetaData;

		typedef OMetaConnection_BASE				OConnection_BASE; // implements basics and text encoding
		typedef ::std::vector< ::connectivity::OTypeInfo>	TTypeInfoVector;
		typedef std::vector< ::com::sun::star::uno::WeakReferenceHelper > OWeakRefArray;

		class OConnection : public OBase_Mutex,
							public OConnection_BASE,
							public connectivity::skeleton::OSubComponent<OConnection, OConnection_BASE>
		{
			friend class connectivity::skeleton::OSubComponent<OConnection, OConnection_BASE>;

		protected:
			
			rtl_TextEncoding						m_nTextEncoding; // the encoding which is used for all text conversions		
			//====================================================================
			// Data attributes
			//====================================================================
			TTypeInfoVector							m_aTypeInfo;	//	vector containing an entry
																	//  for each row returned by
																	//  DatabaseMetaData.getTypeInfo.
            ::com::sun::star::uno::WeakReference< ::com::sun::star::sdbc::XDatabaseMetaData > m_xMetaData;

			OWeakRefArray							m_aStatements;	//	vector containing a list
														//  of all the Statement objects
														//  for this Connection

            ::com::sun::star::sdbc::SQLWarning      m_aLastWarning; //      Last SQLWarning generated by
														//  an operation
			::rtl::OUString							m_aURL;			//	URL of connection
			::rtl::OUString							m_sUser;		// the user name
			SkeletonDriver*							m_pDriver;		//	Pointer to the owning
																	//  driver object
													
			sal_Bool								m_bClosed;
			sal_Bool								m_bUseCatalog;	// should we use the catalog on filebased databases
			sal_Bool								m_bUseOldDateFormat;


            void                    buildTypeInfo() throw( ::com::sun::star::sdbc::SQLException);
			
		public:
            virtual void construct( const ::rtl::OUString& url,const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& info) throw(::com::sun::star::sdbc::SQLException);

			OConnection(SkeletonDriver*	_pDriver);
			virtual ~OConnection();

            void closeAllStatements () throw( ::com::sun::star::sdbc::SQLException);

			// OComponentHelper
			virtual void SAL_CALL disposing(void);
			// XInterface
			virtual void SAL_CALL release() throw();

			// XServiceInfo
			DECLARE_SERVICE_INFO();
			// XConnection
            virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XStatement > SAL_CALL createStatement(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XPreparedStatement > SAL_CALL prepareStatement( const ::rtl::OUString& sql ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XPreparedStatement > SAL_CALL prepareCall( const ::rtl::OUString& sql ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::rtl::OUString SAL_CALL nativeSQL( const ::rtl::OUString& sql ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL setAutoCommit( sal_Bool autoCommit ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual sal_Bool SAL_CALL getAutoCommit(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL commit(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL rollback(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual sal_Bool SAL_CALL isClosed(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XDatabaseMetaData > SAL_CALL getMetaData(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL setReadOnly( sal_Bool readOnly ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual sal_Bool SAL_CALL isReadOnly(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL setCatalog( const ::rtl::OUString& catalog ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::rtl::OUString SAL_CALL getCatalog(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL setTransactionIsolation( sal_Int32 level ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual sal_Int32 SAL_CALL getTransactionIsolation(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual ::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess > SAL_CALL getTypeMap(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
            virtual void SAL_CALL setTypeMap( const ::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess >& typeMap ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
			// XCloseable
			virtual void SAL_CALL close(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
			// XWarningsSupplier
			virtual ::com::sun::star::uno::Any SAL_CALL getWarnings(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
			virtual void SAL_CALL clearWarnings(  ) throw(::com::sun::star::sdbc::SQLException, ::com::sun::star::uno::RuntimeException);
			//
			
			// should we use the catalog on filebased databases
			inline sal_Bool			isCatalogUsed()		const { return m_bUseCatalog; }
			inline ::rtl::OUString	getUserName()		const { return m_sUser; }
			inline SkeletonDriver*	getDriver()			const { return m_pDriver;}
			inline rtl_TextEncoding getTextEncoding()	const { return m_nTextEncoding; }
		};
	}
}
#endif // CONNECTIVITY_SCONNECTION_HXX

