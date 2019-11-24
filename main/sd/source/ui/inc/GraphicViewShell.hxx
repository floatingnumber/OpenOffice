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



#ifndef SD_GRAPHIC_VIEW_SHELL_HXX
#define SD_GRAPHIC_VIEW_SHELL_HXX

#include "DrawViewShell.hxx"

class Window;

namespace sd {

/** View shell of the Draw application.

    <p>This class is an example of how not to do it: specialization by
    inheritance.  A graphic view shell is similar to a draw view shell
    but lacks some of its features.  Thus is should be at most a base
    class of DrawViewShell.  There even is special case code in
    ViewShell that turns off some of the features for GraphicViewShell
    instances.</p>
*/
class GraphicViewShell
    : public DrawViewShell
{
public:
	TYPEINFO();

	SFX_DECL_VIEWFACTORY(GraphicViewShell);
	SFX_DECL_INTERFACE(SD_IF_SDGRAPHICVIEWSHELL)

    /** Create a new view shell for the Draw application.
        @param rViewShellBase
            The new object will be stacked on this view shell base.
        @param pFrameView
            The frame view that makes it possible to pass information from
            one view shell to the next.
    */
    GraphicViewShell (
        SfxViewFrame* pFrame, 
        ViewShellBase& rViewShellBase,
        ::Window* pParentWindow,
        FrameView* pFrameView = NULL);

	virtual ~GraphicViewShell (void);

    /** This method is overloaded in order to have the layer mode always
        active.
    */
	virtual void ChangeEditMode (EditMode eMode, bool bIsLayerModeActive);

protected:
	void ConstructGraphicViewShell (void);
    virtual void ArrangeGUIElements (void);

private:
    DECL_LINK(TabBarSplitHandler, TabBar*);
};

} // end of namespace sd

#endif
