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



#ifndef _SDR_ATTRIBUTE_SDRLINEFILLSHADOWTEXTATTRIBUTE_HXX
#define _SDR_ATTRIBUTE_SDRLINEFILLSHADOWTEXTATTRIBUTE_HXX

#include <sal/types.h>
#include <svx/sdr/attribute/sdrlineshadowtextattribute.hxx>
#include <drawinglayer/attribute/sdrfillattribute.hxx>
#include <drawinglayer/attribute/fillgradientattribute.hxx>

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
	namespace attribute
	{
		class SdrLineFillShadowTextAttribute : public SdrLineShadowTextAttribute
		{
			// add fill and transGradient attributes
			SdrFillAttribute			maFill;						// fill attributes (if used)
			FillGradientAttribute		maFillFloatTransGradient;	// fill float transparence gradient (if used)

		public:
			SdrLineFillShadowTextAttribute(
				const SdrLineAttribute& rLine, 
                const SdrFillAttribute& rFill, 
                const SdrLineStartEndAttribute& rLineStartEnd, 
				const SdrShadowAttribute& rShadow, 
                const FillGradientAttribute& rFillFloatTransGradient, 
                const SdrTextAttribute& rTextAttribute);
			SdrLineFillShadowTextAttribute();
			SdrLineFillShadowTextAttribute(const SdrLineFillShadowTextAttribute& rCandidate);
			SdrLineFillShadowTextAttribute& operator=(const SdrLineFillShadowTextAttribute& rCandidate);

            // checks if the incarnation is default constructed
            bool isDefault() const;

			// compare operator
			bool operator==(const SdrLineFillShadowTextAttribute& rCandidate) const;

			// data access
			const SdrFillAttribute& getFill() const { return maFill; }
			const FillGradientAttribute& getFillFloatTransGradient() const { return maFillFloatTransGradient; }
		};
	} // end of namespace attribute
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////

#endif // _SDR_ATTRIBUTE_SDRLINEFILLSHADOWTEXTATTRIBUTE_HXX

// eof
