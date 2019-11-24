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


#ifndef INCLUDED_SLIDESHOW_ANIMATIONBASENODE_HXX
#define INCLUDED_SLIDESHOW_ANIMATIONBASENODE_HXX

#include <com/sun/star/animations/XAnimate.hpp>

#include "basecontainernode.hxx"
#include "activitiesfactory.hxx"
#include "shapeattributelayer.hxx"
#include "shapeattributelayerholder.hxx"
#include "attributableshape.hxx"
#include "shapesubset.hxx"

namespace slideshow {
namespace internal {

/** Common base class for all leaf animation nodes.
    
    This class basically holds the target shape
*/
class AnimationBaseNode : public BaseNode
{
public:
    AnimationBaseNode(
        ::com::sun::star::uno::Reference<
        ::com::sun::star::animations::XAnimationNode> const& xNode, 
        ::boost::shared_ptr<BaseContainerNode> const& pParent,
        NodeContext const& rContext );
    
#if defined(VERBOSE) && defined(DBG_UTIL)
    virtual void showState() const;
#endif
    
protected:
    virtual void dispose();
    
    ::com::sun::star::uno::Reference<
        ::com::sun::star::animations::XAnimate> const& getXAnimateNode() const
        { return mxAnimateNode; }
    
    /// Create parameter struct for ActivitiesFactory
    ActivitiesFactory::CommonParameters fillCommonParameters() const;
    ::basegfx::B2DVector const&         getSlideSize() const { return maSlideSize; }
    AttributableShapeSharedPtr          getShape() const;
    
private:
    virtual bool hasPendingAnimation() const;
    
private: // state transition callbacks
    virtual bool init_st();
    virtual bool resolve_st();
    virtual void activate_st();
    virtual void deactivate_st( NodeState eDestState );
    virtual AnimationActivitySharedPtr createActivity() const = 0;
    
private:
    /** Returns true, if this is a subset animation, and
        the subset is autogenerated (e.g. from an
        iteration)
    */
    bool isDependentSubsettedShape() const
        { return mpShapeSubset && !mbIsIndependentSubset; }
    
    ShapeAttributeLayerHolder const & getAttributeLayerHolder() const
        { return maAttributeLayerHolder; }
    
private:
    ::com::sun::star::uno::Reference<
        ::com::sun::star::animations::XAnimate>     mxAnimateNode;
    ShapeAttributeLayerHolder                       maAttributeLayerHolder;
    ::basegfx::B2DVector                            maSlideSize;
    AnimationActivitySharedPtr                      mpActivity;
    
    /// When valid, this node has a plain target shape
    AttributableShapeSharedPtr                      mpShape;
    /// When valid, this is a subsetted target shape 
    ShapeSubsetSharedPtr                            mpShapeSubset;
    SubsettableShapeManagerSharedPtr                mpSubsetManager;
    bool                                            mbIsIndependentSubset;
};

} // namespace internal
} // namespace presentation

#endif /* INCLUDED_SLIDESHOW_ANIMATIONBASENODE_HXX */

