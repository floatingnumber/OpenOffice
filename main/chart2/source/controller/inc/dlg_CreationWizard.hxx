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



#ifndef _CHART2_CREATION_WIZARD_HXX
#define _CHART2_CREATION_WIZARD_HXX

#include "ServiceMacros.hxx"
#include "TimerTriggeredControllerLock.hxx"
#include "TabPageNotifiable.hxx"

#include <com/sun/star/chart2/XChartDocument.hpp>
#include <svtools/roadmapwizard.hxx>
#include <com/sun/star/uno/XComponentContext.hpp>

// for auto_ptr
#include <memory>

//.............................................................................
namespace chart
{
//.............................................................................

class RangeChooserTabPage;
class DataSourceTabPage;
class ChartTypeTemplateProvider;
class DialogModel;

class CreationWizard : public 
    svt::RoadmapWizard
    , public TabPageNotifiable
{
public:
    CreationWizard( Window* pParent, 
        const ::com::sun::star::uno::Reference<
        ::com::sun::star::frame::XModel >& xChartModel
        , const ::com::sun::star::uno::Reference<
        ::com::sun::star::uno::XComponentContext >& xContext
        , sal_Int32 nOnePageOnlyIndex=-1 );//if nOnePageOnlyIndex is an index of an exsisting  page starting with 0, then only this page is displayed without next/previous and roadmap
    virtual ~CreationWizard();

    bool isClosable();

    // TabPageNotifiable
    virtual void setInvalidPage( TabPage * pTabPage );
    virtual void setValidPage( TabPage * pTabPage );

protected:
    virtual	sal_Bool		leaveState( WizardState _nState );
    virtual WizardState		determineNextState(WizardState nCurrentState) const;
    virtual	void			enterState(WizardState nState);

    virtual String  getStateDisplayName( WizardState nState ) const;

private:
	//no default constructor
	CreationWizard();

    virtual svt::OWizardPage* createPage(WizardState nState);

    ::com::sun::star::uno::Reference<
                       ::com::sun::star::chart2::XChartDocument >   m_xChartModel;
    ::com::sun::star::uno::Reference<
                       ::com::sun::star::uno::XComponentContext>    m_xCC;
    bool m_bIsClosable;
    sal_Int32 m_nOnePageOnlyIndex;//if nOnePageOnlyIndex is an index of an exsisting page starting with 0, then only this page is displayed without next/previous and roadmap
    ChartTypeTemplateProvider*   m_pTemplateProvider;
    ::std::auto_ptr< DialogModel > m_apDialogModel;

    WizardState m_nFirstState;
    WizardState m_nLastState;

    TimerTriggeredControllerLock   m_aTimerTriggeredControllerLock;

//     RangeChooserTabPage * m_pRangeChooserTabePage;
//     DataSourceTabPage *   m_pDataSourceTabPage;
    bool                  m_bCanTravel;
};
//.............................................................................
} //namespace chart
//.............................................................................
#endif
