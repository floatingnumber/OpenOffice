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



#ifndef _UCBHELPER_SIMPLEINTERACTIONREQUEST_HXX
#define _UCBHELPER_SIMPLEINTERACTIONREQUEST_HXX

#include <ucbhelper/interactionrequest.hxx>
#include "ucbhelper/ucbhelperdllapi.h"

namespace ucbhelper {

/** These are the constants that can be passed to the constructor of class
  * SimpleInteractionRequest and that are returned by method
  * SimpleInteractionRequest::getResponse().
  */

/** The request was not (yet) handled by the interaction handler. */
static const sal_Int32 CONTINUATION_UNKNOWN    = 0;

/** The interaction handler selected XInteractionAbort. */
static const sal_Int32 CONTINUATION_ABORT      = 1;

/** The interaction handler selected XInteractionRetry. */
static const sal_Int32 CONTINUATION_RETRY      = 2;

/** The interaction handler selected XInteractionApprove. */
static const sal_Int32 CONTINUATION_APPROVE    = 4;

/** The interaction handler selected XInteractionDisapprove. */
static const sal_Int32 CONTINUATION_DISAPPROVE = 8;

/**
  * This class implements a simple interaction request. The user must not deal
  * with XInteractionContinuations directly, but can use constants that are
  * mapped internally to the according objects. This class encapsulates the
  * standard Interaction Continuations "Abort", "Retry", "Approve" and
  * "Disaprrove". Instances can be passed directly to
  * XInteractionHandler::handle(...).
  *
  * @see InteractionRequest
  * @see InteractionAbort
  * @see InteractionRetry
  * @see InteractionApprove
  * @see InteractionDisapprove
  */
class UCBHELPER_DLLPUBLIC SimpleInteractionRequest : public ucbhelper::InteractionRequest
{
public:
    /**
      * Constructor.
      *
      * @param rRequest is the exception describing the error.
      * @param nContinuations contains the possible "answers" for the request.
      *        This can be any of the CONTINUATION_* constants combinations
      *        listed above.
	  */
    SimpleInteractionRequest( const com::sun::star::uno::Any & rRequest,
                              const sal_Int32 nContinuations );

    /**
      * After passing this request to XInteractionHandler::handle, this method
      * returns the continuation that was chosen by the interaction handler.
	  *
      * @return the continuation chosen by an interaction handler or
      *         CONTINUATION_UNKNOWN, if the request was not (yet) handled.
	  */
    sal_Int32 getResponse() const;
};

} // namespace ucbhelper

#endif /* !_UCBHELPER_SIMPLEINTERACTIONREQUEST_HXX */
