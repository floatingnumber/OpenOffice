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


#ifndef CHART_SCATTERCHARTTYPETEMPLATE_HXX
#define CHART_SCATTERCHARTTYPETEMPLATE_HXX

#include "ChartTypeTemplate.hxx"
#include "OPropertySet.hxx"
#include "MutexContainer.hxx"
#include <comphelper/uno3.hxx>
#include <com/sun/star/chart2/CurveStyle.hpp>

namespace chart
{

class ScatterChartTypeTemplate :
        public MutexContainer,
        public ChartTypeTemplate,
        public ::property::OPropertySet
{
public:
    explicit ScatterChartTypeTemplate(
        ::com::sun::star::uno::Reference<
            ::com::sun::star::uno::XComponentContext > const & xContext,
        const ::rtl::OUString & rServiceName,
        bool bSymbols,
        bool bHasLines = true,
        sal_Int32 nDim = 2 );
	virtual ~ScatterChartTypeTemplate();

    /// XServiceInfo declarations
    APPHELPER_XSERVICEINFO_DECL()

    /// merge XInterface implementations
 	DECLARE_XINTERFACE()
    /// merge XTypeProvider implementations
 	DECLARE_XTYPEPROVIDER()

protected:
    // ____ OPropertySet ____
    virtual ::com::sun::star::uno::Any GetDefaultValue( sal_Int32 nHandle ) const
        throw(::com::sun::star::beans::UnknownPropertyException);
    virtual ::cppu::IPropertyArrayHelper & SAL_CALL getInfoHelper();

    // ____ XPropertySet ____
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::beans::XPropertySetInfo > SAL_CALL
        getPropertySetInfo()
        throw (::com::sun::star::uno::RuntimeException);

    // ____ XChartTypeTemplate ____
    virtual sal_Bool SAL_CALL supportsCategories()
        throw (::com::sun::star::uno::RuntimeException);
    virtual sal_Bool SAL_CALL matchesTemplate(
        const ::com::sun::star::uno::Reference<
            ::com::sun::star::chart2::XDiagram >& xDiagram,
        sal_Bool bAdaptProperties )
        throw (::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::chart2::XChartType > SAL_CALL
        getChartTypeForNewSeries( const ::com::sun::star::uno::Sequence<
            ::com::sun::star::uno::Reference<
                ::com::sun::star::chart2::XChartType > >& aFormerlyUsedChartTypes )
        throw (::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::chart2::XDataInterpreter > SAL_CALL getDataInterpreter()
        throw (::com::sun::star::uno::RuntimeException);
    virtual void SAL_CALL applyStyle(
        const ::com::sun::star::uno::Reference< ::com::sun::star::chart2::XDataSeries >& xSeries,
        ::sal_Int32 nChartTypeGroupIndex,
        ::sal_Int32 nSeriesIndex,
        ::sal_Int32 nSeriesCount )
        throw (::com::sun::star::uno::RuntimeException);

    // ____ ChartTypeTemplate ____
    virtual ::com::sun::star::uno::Reference< ::com::sun::star::chart2::XChartType >
                getChartTypeForIndex( sal_Int32 nChartTypeIndex );
    virtual sal_Int32 getDimension() const;
    virtual StackMode getStackMode( sal_Int32 nChartTypeIndex ) const;

private:
    bool               m_bHasSymbols;
    bool               m_bHasLines;
    sal_Int32          m_nDim;
};

} //  namespace chart

// CHART_SCATTERCHARTTYPETEMPLATE_HXX
#endif
