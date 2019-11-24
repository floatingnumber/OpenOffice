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



#ifndef SDEXT_PRESENTER_PRESENTER_CANVAS_HELPER_HXX
#define SDEXT_PRESENTER_PRESENTER_CANVAS_HELPER_HXX

#include "PresenterTheme.hxx"
#include <com/sun/star/awt/Point.hpp>
#include <com/sun/star/awt/Rectangle.hpp>
#include <com/sun/star/rendering/TextDirection.hpp>
#include <com/sun/star/rendering/XCanvas.hpp>
#include <com/sun/star/rendering/XCanvasFont.hpp>
#include <com/sun/star/rendering/XPolyPolygon2D.hpp>
#include <rtl/ref.hxx>
#include <boost/noncopyable.hpp>

namespace css = ::com::sun::star;

namespace sdext { namespace presenter {

class PresenterController;

/** Collection of functions to ease the life of a canvas user.
*/
class PresenterCanvasHelper
    : ::boost::noncopyable
{
public:
    PresenterCanvasHelper (void);
    ~PresenterCanvasHelper (void);
    
    void Paint (
        const SharedBitmapDescriptor& rpBitmap,
        const css::uno::Reference<css::rendering::XCanvas>& rxCanvas,
        const css::awt::Rectangle& rRepaintBox,
        const css::awt::Rectangle& rBackgroundBoundingBox,
        const css::awt::Rectangle& rContentBoundingBox) const;

    static void PaintRectangle (
        const SharedBitmapDescriptor& rpBitmap,
        const css::uno::Reference<css::rendering::XCanvas>& rxCanvas,
        const css::awt::Rectangle& rRepaintBox,
        const css::awt::Rectangle& rBackgroundBoundingBox,
        const css::awt::Rectangle& rContentBoundingBox,
        const css::rendering::ViewState& rDefaultViewState,
        const css::rendering::RenderState& rDefaultRenderState);

    static void SetDeviceColor(
        css::rendering::RenderState& rRenderState,
        const css::util::Color aColor);

    static css::geometry::RealRectangle2D GetTextBoundingBox (
        const css::uno::Reference<css::rendering::XCanvasFont>& rxFont,
        const ::rtl::OUString& rsText,
        const sal_Int8 = css::rendering::TextDirection::WEAK_LEFT_TO_RIGHT);

    static css::geometry::RealSize2D GetTextSize (
        const css::uno::Reference<css::rendering::XCanvasFont>& rxFont,
        const ::rtl::OUString& rsText,
        const sal_Int8 = css::rendering::TextDirection::WEAK_LEFT_TO_RIGHT);

private:
    const css::rendering::ViewState maDefaultViewState;
    const css::rendering::RenderState maDefaultRenderState;

    static void PaintTiledBitmap (
        const css::uno::Reference<css::rendering::XBitmap>& rxTexture,
        const css::uno::Reference<css::rendering::XCanvas>& rxCanvas,
        const css::awt::Rectangle& rRepaintBox,
        const css::uno::Reference<css::rendering::XPolyPolygon2D>& rxPolygon,
        const css::awt::Rectangle& rHole,
        const css::rendering::ViewState& rDefaultViewState,
        const css::rendering::RenderState& rDefaultRenderState);

    static void PaintBitmap (
        const css::uno::Reference<css::rendering::XBitmap>& rxBitmap,
        const css::awt::Point& rLocation,
        const css::uno::Reference<css::rendering::XCanvas>& rxCanvas,
        const css::awt::Rectangle& rRepaintBox,
        const css::uno::Reference<css::rendering::XPolyPolygon2D>& rxPolygon,
        const css::rendering::ViewState& rDefaultViewState,
        const css::rendering::RenderState& rDefaultRenderState);

    static void PaintColor (
        const css::util::Color nColor,
        const css::uno::Reference<css::rendering::XCanvas>& rxCanvas,
        const css::awt::Rectangle& rRepaintBox,
        const css::uno::Reference<css::rendering::XPolyPolygon2D>& rxPolygon,
        const css::rendering::ViewState& rDefaultViewState,
        const css::rendering::RenderState& rDefaultRenderState);
};

} }

#endif
