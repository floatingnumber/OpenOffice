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



#ifndef _UCBHELPER_SIMPLENAMECLASHRESOLVEREQUEST_HXX
#define _UCBHELPER_SIMPLENAMECLASHRESOLVEREQUEST_HXX

#include <rtl/ref.hxx>
#include <ucbhelper/interactionrequest.hxx>
#include "ucbhelper/ucbhelperdllapi.h"

namespace ucbhelper {

/**
  * This class implements a simple name clash resolve interaction request.
  * Instances can be passed directly to XInteractionHandler::handle(...). Each
  * instance contains a NameClashResolveRequest and two interaction
  * continuations: "Abort" and "SupplyName". Another continuation
  * ("ReplaceExistingData") may be supplied optionally.
  *
  * @see com::sun::star::ucb::NameClashResolveRequest
  * @see InteractionAbort
  * @see InteractioneplaceExistingData
  * @see InteractionSupplyName
  */
class UCBHELPER_DLLPUBLIC SimpleNameClashResolveRequest : public ucbhelper::InteractionRequest
{
    rtl::Reference< InteractionSupplyName > m_xNameSupplier;

public:
    /**
      * Constructor.
      *
      * @param rTargetFolderURL contains the URL of the folder that contains
      *        the clashing resource.
      * @param rClashingName contains the clashing name.
      * @param rProposedNewName contains a proposal for the new name or is
      *        empty.
      * @param bSupportsOverwriteData indicates whether an
      *        InteractionReplaceExistingData continuation shall be supplied
      *        with the interaction request.
      */
    SimpleNameClashResolveRequest( const rtl::OUString & rTargetFolderURL,
                                   const rtl::OUString & rClashingName,
                                   const rtl::OUString & rProposedNewName,
                                   sal_Bool bSupportsOverwriteData = sal_True );
    /**
      * This method returns the new name that was supplied by the interaction
      * handler.
      *
      * @return the new name, if supplied.
      */
    const rtl::OUString getNewName() const
    { return m_xNameSupplier->getName(); }

};

} // namespace ucbhelper

#endif /* !_UCBHELPER_SIMPLENAMECLASHRESOLVEREQUEST_HXX */
