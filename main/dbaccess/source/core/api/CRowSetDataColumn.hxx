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


#ifndef DBACCESS_CORE_API_CROWSETDATACOLUMN_HXX
#define DBACCESS_CORE_API_CROWSETDATACOLUMN_HXX

#include "datacolumn.hxx"
#include "RowSetRow.hxx"
#include "RowSetCacheIterator.hxx"
#include "columnsettings.hxx"

#include <connectivity/CommonTools.hxx>
#include <comphelper/proparrhlp.hxx>

namespace dbaccess
{
	class ORowSetDataColumn;
	typedef ::comphelper::OPropertyArrayUsageHelper<ORowSetDataColumn> ORowSetDataColumn_PROP;

	class ORowSetDataColumn :	public ODataColumn,
								public OColumnSettings,
								public ORowSetDataColumn_PROP
	{
	protected:
		ORowSetCacheIterator		m_aColumnValue;
		::com::sun::star::uno::Any	m_aOldValue;

        ::rtl::OUString             m_sLabel;
		::rtl::OUString				m_aDescription;		// description
		ORowSetBase*				m_pRowSet;

		virtual ~ORowSetDataColumn();
	public:
		ORowSetDataColumn(const ::com::sun::star::uno::Reference < ::com::sun::star::sdbc::XResultSetMetaData >& _xMetaData,
						  const ::com::sun::star::uno::Reference < ::com::sun::star::sdbc::XRow >& _xRow,
						  const ::com::sun::star::uno::Reference < ::com::sun::star::sdbc::XRowUpdate >& _xRowUpdate,
						  sal_Int32 _nPos,
                          const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XDatabaseMetaData >& _rxDBMeta,
						  const ::rtl::OUString& _rDescription,
                          const ::rtl::OUString& i_sLabel,
						  const ORowSetCacheIterator& _rColumnValue);

		
		// com::sun::star::lang::XTypeProvider
		virtual ::com::sun::star::uno::Sequence< sal_Int8 > SAL_CALL getImplementationId() throw (::com::sun::star::uno::RuntimeException);
		// comphelper::OPropertyArrayUsageHelper
		virtual ::cppu::IPropertyArrayHelper* createArrayHelper( ) const;

		// cppu::OPropertySetHelper
		virtual ::cppu::IPropertyArrayHelper& SAL_CALL getInfoHelper();

		virtual sal_Bool SAL_CALL convertFastPropertyValue( ::com::sun::star::uno::Any & rConvertedValue,
															::com::sun::star::uno::Any & rOldValue,
															sal_Int32 nHandle,
															const ::com::sun::star::uno::Any& rValue ) throw (::com::sun::star::lang::IllegalArgumentException);
		virtual void SAL_CALL getFastPropertyValue( ::com::sun::star::uno::Any& rValue, sal_Int32 nHandle ) const;
		virtual void SAL_CALL setFastPropertyValue_NoBroadcast(sal_Int32 nHandle,const ::com::sun::star::uno::Any& rValue )throw (::com::sun::star::uno::Exception);

		virtual void fireValueChange(const ::connectivity::ORowSetValue& _rOldValue);
    protected:
        using ODataColumn::getFastPropertyValue;
	};
	// -------------------------------------------------------------------------
//	typedef connectivity::ORefVector< ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySet> >
//			ORowSetDataColumns_COLLECTION;

	typedef connectivity::sdbcx::OCollection ORowSetDataColumns_BASE;
	class ORowSetDataColumns : public ORowSetDataColumns_BASE
	{
		::vos::ORef< ::connectivity::OSQLColumns> m_aColumns;
	protected:
		virtual connectivity::sdbcx::ObjectType createObject(const ::rtl::OUString& _rName);
		virtual void impl_refresh() throw(::com::sun::star::uno::RuntimeException);
	public:
		ORowSetDataColumns(
						sal_Bool _bCase,
						const ::vos::ORef< ::connectivity::OSQLColumns>& _rColumns,
						::cppu::OWeakObject& _rParent,
						::osl::Mutex& _rMutex,
						const ::std::vector< ::rtl::OUString> &_rVector
						);
		virtual ~ORowSetDataColumns();
		// only the name is identical to ::cppu::OComponentHelper
		virtual void SAL_CALL disposing(void);
		void assign(const ::vos::ORef< ::connectivity::OSQLColumns>& _rColumns,const ::std::vector< ::rtl::OUString> &_rVector);
	};
}

#endif

