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


#ifndef CHART_STOCKDATAINTERPRETER_HXX
#define CHART_STOCKDATAINTERPRETER_HXX

#include "DataInterpreter.hxx"

#include "StockChartTypeTemplate.hxx"

namespace chart
{

class StockDataInterpreter : public DataInterpreter
{
public:
    explicit StockDataInterpreter(
        StockChartTypeTemplate::StockVariant eVariant,
        const ::com::sun::star::uno::Reference<
            ::com::sun::star::uno::XComponentContext > & xContext );
    virtual ~StockDataInterpreter();

protected:
    // ____ XDataInterpreter ____
    virtual ::com::sun::star::chart2::InterpretedData SAL_CALL interpretDataSource(
        const ::com::sun::star::uno::Reference< ::com::sun::star::chart2::data::XDataSource >& xSource,
        const ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& aArguments,
        const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Reference< ::com::sun::star::chart2::XDataSeries > >& aSeriesToReUse )
        throw (::com::sun::star::uno::RuntimeException);
    virtual sal_Bool SAL_CALL isDataCompatible(
        const ::com::sun::star::chart2::InterpretedData& aInterpretedData )
        throw (::com::sun::star::uno::RuntimeException);
    virtual ::com::sun::star::chart2::InterpretedData SAL_CALL reinterpretDataSeries(
        const ::com::sun::star::chart2::InterpretedData& aInterpretedData )
        throw (::com::sun::star::uno::RuntimeException);

private:
    StockChartTypeTemplate::StockVariant m_eStockVariant;

    StockChartTypeTemplate::StockVariant GetStockVariant() const;
};

} // namespace chart

// CHART_STOCKDATAINTERPRETER_HXX
#endif
