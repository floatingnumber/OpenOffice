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

#include "precompiled_sd.hxx"

#include "PreviewValueSet.hxx"
#include <vcl/image.hxx>


namespace sd { namespace sidebar {


PreviewValueSet::PreviewValueSet (::Window* pParent)
    : ValueSet (pParent, WB_TABSTOP),
      maPreviewSize(10,10),
      mnBorderWidth(3),
      mnBorderHeight(3),
      mnMaxColumnCount(-1)
{
	SetStyle (
        GetStyle() 
        & ~(WB_ITEMBORDER)// | WB_MENUSTYLEVALUESET)
        //        | WB_FLATVALUESET);
        );
	SetColCount(2);
	SetExtraSpacing (2);
}




PreviewValueSet::~PreviewValueSet (void)
{
}




void PreviewValueSet::SetPreviewSize (const Size& rSize)
{
    maPreviewSize = rSize;
}




void PreviewValueSet::SetRightMouseClickHandler (const Link& rLink)
{
    maRightMouseClickHandler = rLink;
}




void PreviewValueSet::MouseButtonDown (const MouseEvent& rEvent)
{
    if (rEvent.IsRight())
        maRightMouseClickHandler.Call(reinterpret_cast<void*>(
            &const_cast<MouseEvent&>(rEvent)));
    else
        ValueSet::MouseButtonDown (rEvent);

}




void PreviewValueSet::Resize (void)
{
    ValueSet::Resize ();

    Size aWindowSize (GetOutputSizePixel());
    if (aWindowSize.Width()>0 && aWindowSize.Height()>0)
    {
        Rearrange();
    }
}




void PreviewValueSet::Rearrange (bool bForceRequestResize)
{
    sal_uInt16 nNewColumnCount (CalculateColumnCount (
        GetOutputSizePixel().Width()));
    sal_uInt16 nNewRowCount (CalculateRowCount (nNewColumnCount));

    SetColCount(nNewColumnCount);
    SetLineCount(nNewRowCount);
}




sal_uInt16 PreviewValueSet::CalculateColumnCount (int nWidth) const
{
    int nColumnCount = 0;
    if (nWidth > 0)
    {
        nColumnCount = nWidth / (maPreviewSize.Width() + 2*mnBorderWidth);
        if (nColumnCount < 1)
            nColumnCount = 1;
        else if (mnMaxColumnCount>0 && nColumnCount>mnMaxColumnCount)
            nColumnCount = mnMaxColumnCount;
    }
    return (sal_uInt16)nColumnCount;
}




sal_uInt16 PreviewValueSet::CalculateRowCount (sal_uInt16 nColumnCount) const
{
    int nRowCount = 0;
    int nItemCount = GetItemCount();
    if (nColumnCount > 0)
    {
        nRowCount = (nItemCount+nColumnCount-1) / nColumnCount;
        if (nRowCount < 1)
            nRowCount = 1;
    }

    return (sal_uInt16)nRowCount;
}




sal_Int32 PreviewValueSet::GetPreferredWidth (sal_Int32 nHeight)
{
    int nPreferredWidth (maPreviewSize.Width() + 2*mnBorderWidth);

    // Get height of each row.
    int nItemHeight (maPreviewSize.Height() + 2*mnBorderHeight);

    // Calculate the row- and column count and from the later the preferred
    // width.
    int nRowCount = nHeight / nItemHeight;
    if (nRowCount > 0)
    {
        int nColumnCount = (GetItemCount()+nRowCount-1) / nRowCount;
        if (nColumnCount > 0)
            nPreferredWidth = (maPreviewSize.Width() + 2*mnBorderWidth) 
                * nColumnCount;
    }

    return nPreferredWidth;
}




sal_Int32 PreviewValueSet::GetPreferredHeight (sal_Int32 nWidth)
{
    int nRowCount (CalculateRowCount(CalculateColumnCount(nWidth)));
    int nItemHeight (maPreviewSize.Height());
    
    return nRowCount * (nItemHeight + 2*mnBorderHeight);
}




} } // end of namespace sd::sidebar
