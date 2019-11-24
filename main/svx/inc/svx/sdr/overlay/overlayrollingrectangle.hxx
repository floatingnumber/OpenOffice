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



#ifndef _SDR_OVERLAY_OVERLAYROOLINGRECTANGLE_HXX
#define _SDR_OVERLAY_OVERLAYROOLINGRECTANGLE_HXX

#include <svx/sdr/overlay/overlayobject.hxx>

//////////////////////////////////////////////////////////////////////////////

namespace sdr
{
	namespace overlay
	{
		class OverlayRollingRectangleStriped : public OverlayObjectWithBasePosition
		{
		protected:
			// second position in pixel
			basegfx::B2DPoint						maSecondPosition;

			// bitfield
			// Flag to switch on/off long lines to the OutputDevice bounds
			unsigned								mbExtendedLines : 1;

			// Flag to switch on/off the bounds itself
			unsigned								mbShowBounds : 1;

			// geometry creation for OverlayObject
			virtual drawinglayer::primitive2d::Primitive2DSequence createOverlayObjectPrimitive2DSequence();

		public:
			OverlayRollingRectangleStriped(
				const basegfx::B2DPoint& rBasePos,
				const basegfx::B2DPoint& rSecondPos,
				bool bExtendedLines = false,
				bool bShowBounds = true);
			virtual ~OverlayRollingRectangleStriped();

			// change second position
			const basegfx::B2DPoint& getSecondPosition() const { return maSecondPosition; }
			void setSecondPosition(const basegfx::B2DPoint& rNew);

			// change extended lines
			bool getExtendedLines() const { return mbExtendedLines; }
			void setExtendedLines(bool bNew);

			// change show bounds
			bool getShowBounds() const { return mbShowBounds; }
			void setShowBounds(bool bNew);

			// react on stripe definition change
			virtual void stripeDefinitionHasChanged();
		};
	} // end of namespace overlay
} // end of namespace sdr

//////////////////////////////////////////////////////////////////////////////

#endif //_SDR_OVERLAY_OVERLAYROOLINGRECTANGLE_HXX

// eof
