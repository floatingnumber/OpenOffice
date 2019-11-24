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



#include "vbatextboxshape.hxx"
#include "vbacharacters.hxx"
#include <com/sun/star/text/XSimpleText.hpp>
#include <vector>

using namespace com::sun::star;
using namespace ooo::vba;

ScVbaTextBoxShape::ScVbaTextBoxShape( const uno::Reference< uno::XComponentContext >& xContext, const uno::Reference< drawing::XShape >& xShape, const uno::Reference< drawing::XShapes >& xShapes, const uno::Reference< frame::XModel >& xModel ) : TextBoxShapeImpl_BASE( uno::Reference< XHelperInterface >(), xContext, xShape, xShapes, xModel, ScVbaShape::getType( xShape )  )
{
    m_xTextRange.set( xShape , uno::UNO_QUERY_THROW );
    m_xModel.set( xModel );
}

rtl::OUString SAL_CALL 
ScVbaTextBoxShape::getText() throw (css::uno::RuntimeException)
{
    return m_xTextRange->getString();
}

void SAL_CALL 
ScVbaTextBoxShape::setText( const rtl::OUString& _text ) throw (css::uno::RuntimeException)
{
    m_xTextRange->setString( _text );
}

uno::Reference< excel::XCharacters > SAL_CALL 
ScVbaTextBoxShape::characters( const uno::Any& Start, const uno::Any& Length ) throw (uno::RuntimeException)
{
    ScDocShell* pDocShell = excel::getDocShell( m_xModel );
    ScDocument* pDoc = pDocShell ? pDocShell->GetDocument() : NULL;

    if ( !pDoc )
        throw uno::RuntimeException( rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "Failed to access document from shell" ) ), uno::Reference< uno::XInterface >() );
    uno::Reference< text::XSimpleText > xSimple( m_xTextRange, uno::UNO_QUERY_THROW );

    ScVbaPalette aPalette( pDoc->GetDocumentShell() );
    return  new ScVbaCharacters( this, mxContext, aPalette, xSimple, Start, Length, sal_True );
}
