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
#include <s2_luidl/tk_punct.hxx>


// NOT FULLY DECLARED SERVICES
#include <parser/parserinfo.hxx>
#include <s2_luidl/tokintpr.hxx>


using csi::uidl::TokPunctuation;


lux::EnumValueMap			G_aTokPunctuation_EV_TokenId_Values;
TokPunctuation::EV_TokenId  ev_none(TokPunctuation::e_none,"");
TokPunctuation::EV_TokenId  BracketOpen(TokPunctuation::BracketOpen,"(");
TokPunctuation::EV_TokenId  BracketClose(TokPunctuation::BracketClose,")");
TokPunctuation::EV_TokenId  ArrayBracketOpen(TokPunctuation::ArrayBracketOpen,"[");
TokPunctuation::EV_TokenId  ArrayBracketClose(TokPunctuation::ArrayBracketClose,"]");
TokPunctuation::EV_TokenId  CurledBracketOpen(TokPunctuation::CurledBracketOpen,"{");
TokPunctuation::EV_TokenId  CurledBracketClose(TokPunctuation::CurledBracketClose,"}");
TokPunctuation::EV_TokenId  Semicolon(TokPunctuation::Semicolon,";");
TokPunctuation::EV_TokenId  Colon(TokPunctuation::Colon,":");
TokPunctuation::EV_TokenId  DoubleColon(TokPunctuation::DoubleColon,"::");
TokPunctuation::EV_TokenId  Comma(TokPunctuation::Comma,",");
TokPunctuation::EV_TokenId  Minus(TokPunctuation::Minus,"-");
TokPunctuation::EV_TokenId  Fullstop(TokPunctuation::Fullstop,".");
TokPunctuation::EV_TokenId  Lesser(TokPunctuation::Lesser,"<");
TokPunctuation::EV_TokenId  Greater(TokPunctuation::Greater,">");




namespace lux
{
template<> EnumValueMap &
TokPunctuation::EV_TokenId::Values_()		{ return G_aTokPunctuation_EV_TokenId_Values; }
}




namespace csi
{
namespace uidl
{

void
TokPunctuation::Trigger( TokenInterpreter &	io_rInterpreter ) const
{
	io_rInterpreter.Process_Punctuation(*this);
}

const char *
TokPunctuation::Text() const
{
	return eTag.Text();
}

void
Tok_EOL::Trigger( TokenInterpreter &	io_rInterpreter ) const
{
	io_rInterpreter.Process_EOL();
}

const char *
Tok_EOL::Text() const
{
	return "\r\n";
}

void
Tok_EOF::Trigger( TokenInterpreter & ) const
{
	csv_assert(false);
//	io_rInterpreter.Process_EOF();
}

const char *
Tok_EOF::Text() const
{
	return "";
}


}   // namespace uidl
}   // namespace csi
