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

// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_svx.hxx"

#include <svx/sdr/attribute/sdrallfillattributeshelper.hxx>
#include <svx/sdr/primitive2d/sdrattributecreator.hxx>
#include <svx/sdr/primitive2d/sdrdecompositiontools.hxx>
#include <basegfx/polygon/b2dpolygontools.hxx>
#include <basegfx/matrix/b2dhommatrix.hxx>
#include <basegfx/polygon/b2dpolygon.hxx>
#include <drawinglayer/attribute/fillhatchattribute.hxx>
#include <drawinglayer/attribute/sdrfillgraphicattribute.hxx>
#include <svx/xfillit0.hxx>
#include <vcl/graph.hxx>

//////////////////////////////////////////////////////////////////////////////

namespace drawinglayer
{
    namespace attribute
    {
        void SdrAllFillAttributesHelper::createPrimitive2DSequence(
            const basegfx::B2DRange& rPaintRange,
            const basegfx::B2DRange& rDefineRange)
        {
            // reset and remember new target range for object geometry
            maLastPaintRange = rPaintRange;
            maLastDefineRange = rDefineRange;

            if(isUsed())
            {
                maPrimitives.realloc(1);
                maPrimitives[0] = drawinglayer::primitive2d::createPolyPolygonFillPrimitive(
                    basegfx::B2DPolyPolygon(
                        basegfx::tools::createPolygonFromRect(
                            maLastPaintRange)), 
                        maLastDefineRange,
                    maFillAttribute.get() ? *maFillAttribute.get() : drawinglayer::attribute::SdrFillAttribute(),
                    maFillGradientAttribute.get() ? *maFillGradientAttribute.get() : drawinglayer::attribute::FillGradientAttribute());
            }
        }

        SdrAllFillAttributesHelper::SdrAllFillAttributesHelper()
        :   maLastPaintRange(),
            maLastDefineRange(),
            maFillAttribute(),
            maFillGradientAttribute(),
            maPrimitives()
        {
        }

        SdrAllFillAttributesHelper::SdrAllFillAttributesHelper(const Color& rColor)
        :   maLastPaintRange(),
            maLastDefineRange(),
            maFillAttribute(),
            maFillGradientAttribute(),
            maPrimitives()
        {
            maFillAttribute.reset(
                new drawinglayer::attribute::SdrFillAttribute(
                    0.0,
                    Color(rColor.GetRGBColor()).getBColor(),
                    drawinglayer::attribute::FillGradientAttribute(),
                    drawinglayer::attribute::FillHatchAttribute(),
                    drawinglayer::attribute::SdrFillGraphicAttribute()));
        }

        SdrAllFillAttributesHelper::SdrAllFillAttributesHelper(const SfxItemSet& rSet)
        :   maLastPaintRange(),
            maLastDefineRange(),
            maFillAttribute(
                new drawinglayer::attribute::SdrFillAttribute(
                    drawinglayer::primitive2d::createNewSdrFillAttribute(rSet))),
            maFillGradientAttribute(
                new drawinglayer::attribute::FillGradientAttribute(
                    drawinglayer::primitive2d::createNewTransparenceGradientAttribute(rSet))),
            maPrimitives()
        {
        }

        SdrAllFillAttributesHelper::~SdrAllFillAttributesHelper()
        {
        }

        bool SdrAllFillAttributesHelper::isUsed() const
        {
            // only depends on fill, FillGradientAttribute alone defines no fill
            return maFillAttribute.get() && !maFillAttribute->isDefault();
        }

        bool SdrAllFillAttributesHelper::isTransparent() const
        {
            if(hasSdrFillAttribute() && 0.0 != maFillAttribute->getTransparence())
            {
                return true;
            }

            if(hasFillGradientAttribute() && !maFillGradientAttribute->isDefault())
            {
                return true;
            }

            if(hasSdrFillAttribute())
            {
                const Graphic& rGraphic = getFillAttribute().getFillGraphic().getFillGraphic();

                return rGraphic.IsSupportedGraphic() && rGraphic.IsTransparent();
            }

            return false;
        }

        const drawinglayer::attribute::SdrFillAttribute& SdrAllFillAttributesHelper::getFillAttribute() const 
        { 
            if(!maFillAttribute.get())
            {
                const_cast< SdrAllFillAttributesHelper* >(this)->maFillAttribute.reset(new drawinglayer::attribute::SdrFillAttribute());
            }

            return *maFillAttribute.get(); 
        }

        const drawinglayer::attribute::FillGradientAttribute& SdrAllFillAttributesHelper::getFillGradientAttribute() const 
        { 
            if(!maFillGradientAttribute.get())
            {
                const_cast< SdrAllFillAttributesHelper* >(this)->maFillGradientAttribute.reset(new drawinglayer::attribute::FillGradientAttribute());
            }

            return *maFillGradientAttribute.get(); 
        }

        const drawinglayer::primitive2d::Primitive2DSequence& SdrAllFillAttributesHelper::getPrimitive2DSequence(
            const basegfx::B2DRange& rPaintRange,
            const basegfx::B2DRange& rDefineRange) const
        {
            if(maPrimitives.getLength() && (maLastPaintRange != rPaintRange || maLastDefineRange != rDefineRange))
            {
                const_cast< SdrAllFillAttributesHelper* >(this)->maPrimitives.realloc(0);
            }

            if(!maPrimitives.getLength())
            {
                const_cast< SdrAllFillAttributesHelper* >(this)->createPrimitive2DSequence(rPaintRange, rDefineRange);
            }

            return maPrimitives;
        }

        basegfx::BColor SdrAllFillAttributesHelper::getAverageColor(const basegfx::BColor& rFallback) const
        {
            basegfx::BColor aRetval(rFallback);

            if(maFillAttribute.get() && !maFillAttribute->isDefault())
            {
                const drawinglayer::attribute::FillGradientAttribute& rFillGradientAttribute = maFillAttribute->getGradient();
                const drawinglayer::attribute::FillHatchAttribute& rFillHatchAttribute = maFillAttribute->getHatch();
                const drawinglayer::attribute::SdrFillGraphicAttribute& rSdrFillGraphicAttribute = maFillAttribute->getFillGraphic();
                const drawinglayer::attribute::FillGradientAttribute& rFillTransparenceGradientAttribute = getFillGradientAttribute();
                double fTransparence(maFillAttribute->getTransparence());

                if(!rFillTransparenceGradientAttribute.isDefault())
                {
                    const double fTransA = rFillTransparenceGradientAttribute.getStartColor().luminance();
                    const double fTransB = rFillTransparenceGradientAttribute.getEndColor().luminance();

                    fTransparence = (fTransA + fTransB) * 0.5;
                }

                if(!rFillGradientAttribute.isDefault())
                {
                    // gradient fill
                    const basegfx::BColor& rStart = rFillGradientAttribute.getStartColor();
                    const basegfx::BColor& rEnd = rFillGradientAttribute.getEndColor();

                    aRetval = basegfx::interpolate(rStart, rEnd, 0.5);
                }
                else if(!rFillHatchAttribute.isDefault())
                {
                    // hatch fill
                    const basegfx::BColor& rColor = rFillHatchAttribute.getColor();

                    if(rFillHatchAttribute.isFillBackground())
                    {
                        const basegfx::BColor& rBackgroundColor = maFillAttribute->getColor();

                        // mix colors 50%/50%
                        aRetval = basegfx::interpolate(rColor, rBackgroundColor, 0.5);
                    }
                    else
                    {
                        // mix color with fallback color
                        aRetval = basegfx::interpolate(rColor, rFallback, 0.5);
                    }
                }
                else if(!rSdrFillGraphicAttribute.isDefault())
                {
                    // graphic fill

                    // not used yet by purpose (see SwPageFrm::GetDrawBackgrdColor()),
                    // use fallback (already set)
                }
                else
                {
                    // color fill
                    aRetval = maFillAttribute->getColor();
                }

                if(!basegfx::fTools::equalZero(fTransparence))
                {
                    // blend into transparency
                    aRetval = basegfx::interpolate(aRetval, rFallback, fTransparence);
                }
            }

            return aRetval.clamp();
        }

        bool SdrAllFillAttributesHelper::needCompleteRepaint() const
        {
            if(!isUsed() || !hasSdrFillAttribute())
            {
                // not used or no fill
                return false;
            }

            const drawinglayer::attribute::SdrFillAttribute& rSdrFillAttribute = getFillAttribute();

            if(!rSdrFillAttribute.getHatch().isDefault())
            {
                // hatch is always top-left aligned, needs no full refreshes
                return false;
            }

            if(!rSdrFillAttribute.getGradient().isDefault())
            {
                // gradients always scale with the object
                return true;
            }

            if(!rSdrFillAttribute.getFillGraphic().isDefault())
            {
                // some graphic constellations may not need this, but since most do
                // (stretch to fill, all but top-left aligned, ...) claim to do by default
                return true;
            }

            // color fill
            return false;
        }
    } // end of namespace attribute
} // end of namespace drawinglayer

//////////////////////////////////////////////////////////////////////////////
// eof
