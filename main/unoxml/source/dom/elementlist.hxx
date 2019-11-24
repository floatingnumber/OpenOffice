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



#ifndef DOM_ELEMENTLIST_HXX
#define DOM_ELEMENTLIST_HXX

#include <vector>

#include <boost/scoped_array.hpp>

#include <libxml/tree.h>

#include <sal/types.h>
#include <rtl/ref.hxx>

#include <com/sun/star/uno/Reference.h>
#include <com/sun/star/uno/Exception.hpp>
#include <com/sun/star/xml/dom/XNode.hpp>
#include <com/sun/star/xml/dom/XNodeList.hpp>
#include <com/sun/star/xml/dom/events/XEvent.hpp>
#include <com/sun/star/xml/dom/events/XEventListener.hpp>

#include <cppuhelper/implbase2.hxx>


using ::rtl::OUString;
using namespace com::sun::star::uno;
using namespace com::sun::star::xml::dom;
using namespace com::sun::star::xml::dom::events;

namespace DOM
{
    class CElement;

    typedef std::vector< xmlNodePtr > nodevector_t;

    class CElementList
        : public cppu::WeakImplHelper2< XNodeList,
                com::sun::star::xml::dom::events::XEventListener >
    {
    private:
        ::rtl::Reference<CElement> const m_pElement;
        ::osl::Mutex & m_rMutex;
        ::boost::scoped_array<xmlChar> const m_pName;
        ::boost::scoped_array<xmlChar> const m_pURI;
        bool m_bRebuild;
        nodevector_t m_nodevector;

        void buildlist(xmlNodePtr pNode, sal_Bool start=sal_True);
        void registerListener(CElement & rElement);

    public:
        CElementList(::rtl::Reference<CElement> const& pElement,
                ::osl::Mutex & rMutex,
                OUString const& rName, OUString const*const pURI = 0);

        /**
        The number of nodes in the list.
        */
        virtual sal_Int32 SAL_CALL getLength() throw (RuntimeException);
        /**
        Returns the indexth item in the collection.
        */
        virtual Reference< XNode > SAL_CALL item(sal_Int32 index)
            throw (RuntimeException);

        // XEventListener
        virtual void SAL_CALL handleEvent(const Reference< XEvent >& evt)
            throw (RuntimeException);
    };
}

#endif
