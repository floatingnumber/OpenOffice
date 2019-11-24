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

#include <precomp.h>
#include <ary/idl/i_exception.hxx>
#include <ary/idl/ik_exception.hxx>


// NOT FULLY DECLARED SERVICES
#include <cosv/tpl/processor.hxx>
#include <sci_impl.hxx>
#include "i2s_calculator.hxx"



namespace ary
{
namespace idl
{

Exception::Exception( const String &      i_sName,
                      Ce_id               i_nOwner,
                      Type_id             i_nBase )
    :   sName(i_sName),
        nOwner(i_nOwner),
        nBase(i_nBase),
        aElements()
{
}

Exception::~Exception()
{
}

void
Exception::do_Accept( csv::ProcessorIfc & io_processor ) const
{
    csv::CheckedCall(io_processor, *this);
}

ClassId
Exception::get_AryClass() const
{
    return class_id;
}

const String &
Exception::inq_LocalName() const
{
    return sName;
}

Ce_id
Exception::inq_NameRoom() const
{
    return nOwner;
}

Ce_id
Exception::inq_Owner() const
{
    return nOwner;
}

E_SightLevel
Exception::inq_SightLevel() const
{
    return sl_File;
}


namespace ifc_exception
{

inline const Exception &
exception_cast( const CodeEntity &  i_ce )
{
    csv_assert( i_ce.AryClass() == Exception::class_id );
	return static_cast< const Exception& >(i_ce);
}

Type_id
attr::Base( const CodeEntity & i_ce )
{
    return exception_cast(i_ce).nBase;
}

void
attr::Get_Elements( Dyn_CeIterator &    o_result,
                    const CodeEntity &  i_ce )
{
    o_result = new SCI_Vector<Ce_id>( exception_cast(i_ce).aElements );
}


void
xref::Get_Derivations( Dyn_CeIterator &    o_result,
                       const CodeEntity &  i_ce )
{
    o_result = new SCI_Vector<Ce_id>(i_ce.Secondaries().List(exception_2s_Derivations));
}

void
xref::Get_RaisingFunctions( Dyn_CeIterator &    o_result,
                            const CodeEntity &  i_ce )
{
    o_result = new SCI_Vector<Ce_id>(i_ce.Secondaries().List(exception_2s_RaisingFunctions));
}


} // namespace ifc_exception


}   //  namespace   idl
}   //  namespace   ary
