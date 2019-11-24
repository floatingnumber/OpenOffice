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



#ifndef _TOOLS_WEAKBASE_HXX_
#define _TOOLS_WEAKBASE_HXX_

#include <tools/weakbase.h>

/// see weakbase.h for documentation

namespace tools
{

template< class reference_type >
inline WeakReference< reference_type >::WeakReference()
{
	mpWeakConnection = new WeakConnection<reference_type>( 0 );
	mpWeakConnection->acquire();
}

template< class reference_type >
inline WeakReference< reference_type >::WeakReference( reference_type* pReference )
{
	if( pReference )
		mpWeakConnection = pReference->getWeakConnection();
	else
		mpWeakConnection = new WeakConnection<reference_type>( 0 );

	mpWeakConnection->acquire();
}

template< class reference_type >
inline WeakReference< reference_type >::WeakReference( const WeakReference< reference_type >& rWeakRef )
{
	mpWeakConnection = rWeakRef.mpWeakConnection;
	mpWeakConnection->acquire();
}

template< class reference_type >
inline WeakReference< reference_type >::~WeakReference()
{
	mpWeakConnection->release();
}

template< class reference_type >
inline bool WeakReference< reference_type >::is() const
{
	return mpWeakConnection->mpReference != 0;
}

template< class reference_type >
inline reference_type * WeakReference< reference_type >::get() const
{
	return mpWeakConnection->mpReference;
}

template< class reference_type >
inline void WeakReference< reference_type >::reset( reference_type* pReference )
{
	mpWeakConnection->release();

	if( pReference )
		mpWeakConnection = pReference->getWeakConnection();
	else
		mpWeakConnection = new WeakConnection<reference_type>( 0 );

	mpWeakConnection->acquire();
}

template< class reference_type >
inline reference_type * WeakReference< reference_type >::operator->() const
{
	OSL_PRECOND(mpWeakConnection, "tools::WeakReference::operator->() : null body");
	return mpWeakConnection->mpReference;
}

template< class reference_type >
inline sal_Bool WeakReference< reference_type >::operator==(const reference_type * pReferenceObject) const
{
	return mpWeakConnection->mpReference == pReferenceObject;
}

template< class reference_type >
inline sal_Bool WeakReference< reference_type >::operator==(const WeakReference<reference_type> & handle) const
{
	return mpWeakConnection == handle.mpWeakConnection;
}

template< class reference_type >
inline sal_Bool	WeakReference< reference_type >::operator!=(const WeakReference<reference_type> & handle) const
{
	return mpWeakConnection != handle.mpWeakConnection;
}

template< class reference_type >
inline sal_Bool	WeakReference< reference_type >::operator<(const WeakReference<reference_type> & handle) const
{
	return mpWeakConnection->mpReference < handle.mpWeakConnection->mpReference;
}

template< class reference_type >
inline sal_Bool	WeakReference< reference_type >::operator>(const WeakReference<reference_type> & handle) const
{
	return mpWeakConnection->mpReference > handle.mpWeakConnection->mpReference;
}

template< class reference_type >
inline WeakReference<reference_type>& WeakReference<reference_type>::operator= (
    const WeakReference<reference_type>& rReference)
{
    if (&rReference != this)
    {
        mpWeakConnection->release();

        mpWeakConnection = rReference.mpWeakConnection;
        mpWeakConnection->acquire();
    }
    return *this;
}

template< class reference_type >
inline WeakBase< reference_type >::WeakBase()
{
	mpWeakConnection = 0;
}

template< class reference_type >
inline WeakBase< reference_type >::~WeakBase()
{
	if( mpWeakConnection )
	{
		mpWeakConnection->mpReference = 0;
		mpWeakConnection->release();
		mpWeakConnection = 0;
	}
}

template< class reference_type >
inline void WeakBase< reference_type >::clearWeak()
{
	if( mpWeakConnection )
		mpWeakConnection->mpReference = 0;
}

template< class reference_type >
inline WeakConnection< reference_type >* WeakBase< reference_type >::getWeakConnection()
{
	if( !mpWeakConnection )
	{
		mpWeakConnection = new WeakConnection< reference_type >( static_cast< reference_type* >( this ) );
		mpWeakConnection->acquire();
	}
	return mpWeakConnection;
}

}

#endif // _TOOLS_WEAKBASE_HXX_

