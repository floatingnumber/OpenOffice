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



#ifndef INCLUDED_SLIDESHOW_VIEWUPDATE_HXX
#define INCLUDED_SLIDESHOW_VIEWUPDATE_HXX

#include "disposable.hxx"
#include "view.hxx"
#include <boost/shared_ptr.hpp>

/* Definition of ViewUpdate interface */

namespace slideshow
{
    namespace internal
    {
        /** Interface for something that can perform view updates.

            Use this interface for objects that perform view updates
            from time to time, e.g. slide content. Availability of
            updates can be queried, and subsequently performed.
         */
        class ViewUpdate : public Disposable
        {
        public:
            /** Perform the update action on all views

                @return true, if the update was performed
                successfully, false otherwise.
             */
            virtual bool update() = 0;

            /** Perform the update action on given view only

                @return true, if the update was performed
                successfully, false otherwise.
             */
            virtual bool update( ViewSharedPtr const& rView ) = 0;

            /** Query whether updates are pending

                @return true, if updates are pending. Calling update()
                subsequently will perform the pending update then.
             */
            virtual bool needsUpdate() const = 0;
        };

        typedef ::boost::shared_ptr< ViewUpdate > ViewUpdateSharedPtr;
    }
}

#endif /* INCLUDED_SLIDESHOW_VIEWUPDATE_HXX */
