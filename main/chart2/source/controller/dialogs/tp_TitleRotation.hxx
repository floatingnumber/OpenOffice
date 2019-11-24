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


#ifndef _CHART2_TP_TITLEROTATION_HXX
#define _CHART2_TP_TITLEROTATION_HXX

// header for SfxTabPage
#include <sfx2/tabdlg.hxx>
#include <svx/dialcontrol.hxx>
#include <svx/wrapfield.hxx>
#include <svx/orienthelper.hxx>
#include <vcl/fixed.hxx>
#include "TextDirectionListBox.hxx"

//.............................................................................
namespace chart
{
//.............................................................................

class SchAlignmentTabPage : public SfxTabPage
{
private:
    FixedLine               aFlAlign;
    svx::DialControl        aCtrlDial;
    FixedText               aFtRotate;
    svx::WrapField          aNfRotate;
    TriStateBox             aCbStacked;
    svx::OrientationHelper  aOrientHlp;
    FixedText               aFtTextDirection;
    TextDirectionListBox    aLbTextDirection;

public:
    SchAlignmentTabPage(Window* pParent, const SfxItemSet& rInAttrs, bool bWithRotation = true);
	virtual ~SchAlignmentTabPage();

	static SfxTabPage* Create(Window* pParent, const SfxItemSet& rInAttrs);
    static SfxTabPage* CreateWithoutRotation(Window* pParent, const SfxItemSet& rInAttrs);
	virtual sal_Bool FillItemSet(SfxItemSet& rOutAttrs);
	virtual void Reset(const SfxItemSet& rInAttrs);
};

//.............................................................................
} //namespace chart
//.............................................................................

#endif
