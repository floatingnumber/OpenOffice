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



#ifndef _XMLSECURITY_DOCUMENTSIGNATUREHELPER_HXX
#define _XMLSECURITY_DOCUMENTSIGNATUREHELPER_HXX

#include <com/sun/star/uno/Reference.h>
#include <rtl/ustring.hxx>
#include "xmlsecurity/sigstruct.hxx"

#ifndef _STLP_VECTOR
#include <vector>
#endif


namespace com {
namespace sun {
namespace star {
namespace io {
    class XStream; }
namespace embed {
    class XStorage; }
}}}

namespace css = com::sun::star;


/**********************************************************
 DocumentSignatureHelper

 Helper class for signing and verifieng document signatures

 Functions:
 1. help to create a list of content to be signed/verified
 
 **********************************************************/

enum DocumentSignatureMode { SignatureModeDocumentContent, SignatureModeMacros, SignatureModePackage };

enum DocumentSignatureAlgorithm
{
    OOo2Document,
    OOo3_0Document,
    OOo3_2Document
};

struct SignatureStreamHelper
{
    css::uno::Reference < css::embed::XStorage >    xSignatureStorage;
    css::uno::Reference < css::io::XStream >        xSignatureStream;
};


class DocumentSignatureHelper
{
public:

    static SignatureStreamHelper OpenSignatureStream( 
        const css::uno::Reference < css::embed::XStorage >& rxStore, sal_Int32 nOpenMode, 
        DocumentSignatureMode eDocSigMode );
    static std::vector< rtl::OUString > CreateElementList( 
        const css::uno::Reference < css::embed::XStorage >& rxStore, 
        const ::rtl::OUString rRootStorageName, DocumentSignatureMode eMode, 
        const DocumentSignatureAlgorithm mode);
    static bool isODFPre_1_2(const ::rtl::OUString & sODFVersion);
    static bool isOOo3_2_Signature(const SignatureInformation & sigInfo);
    static DocumentSignatureAlgorithm getDocumentAlgorithm(
        const ::rtl::OUString & sODFVersion, const SignatureInformation & sigInfo);
    static bool checkIfAllFilesAreSigned( const ::std::vector< ::rtl::OUString > & sElementList, 
        const SignatureInformation & sigInfo, const DocumentSignatureAlgorithm alg);
	static bool equalsReferenceUriManifestPath(
        const ::rtl::OUString & rUri, const ::rtl::OUString & rPath);
    static ::rtl::OUString GetDocumentContentSignatureDefaultStreamName();
	static ::rtl::OUString GetScriptingContentSignatureDefaultStreamName();
    static ::rtl::OUString GetPackageSignatureDefaultStreamName(); 

};

#endif // _XMLSECURITY_XMLSIGNATUREHELPER_HXX
