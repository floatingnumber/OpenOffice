<!--***********************************************************
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
 ***********************************************************-->


<xsl:stylesheet 
    version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0" 
    xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0" 
    xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0" 
    xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0" 
    xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0" 
    xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0" 
    xmlns:xlink="http://www.w3.org/1999/xlink" 
    xmlns:dc="http://purl.org/dc/elements/1.1/" 
    xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0" 
    xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0" 
    xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0" 
    xmlns:chart="urn:oasis:names:tc:opendocument:xmlns:chart:1.0" 
    xmlns:dr3d="urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0" 
    xmlns:math="http://www.w3.org/1998/Math/MathML" 
    xmlns:form="urn:oasis:names:tc:opendocument:xmlns:form:1.0" 
    xmlns:script="urn:oasis:names:tc:opendocument:xmlns:script:1.0" 
    xmlns:config="urn:oasis:names:tc:opendocument:xmlns:config:1.0" 
    xmlns:ooo="http://openoffice.org/2004/office" 
    xmlns:ooow="http://openoffice.org/2004/writer" 
    xmlns:oooc="http://openoffice.org/2004/calc" 
    xmlns:dom="http://www.w3.org/2001/xml-events" 
    xmlns:xforms="http://www.w3.org/2002/xforms" 
    xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
    xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#"  
    xmlns:rng="http://relaxng.org/ns/structure/1.0"
    xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#" 
    xmlns:UML = 'org.omg.xmi.namespace.UML' xml:space="default">
  <xsl:output method="text" />
  <xsl:param name="prefix"/>
  
  <xsl:include href="factorytools.xsl"/>

  <xsl:template name="fasttokens">
    <xsl:for-each select="/model/fasttoken">
      <xsl:text>
const sal_Int32 OOXML_</xsl:text>
<xsl:value-of select="translate(., '-', '_')"/>
<xsl:text> = </xsl:text>
<xsl:value-of select="position() - 1"/>
<xsl:text>;</xsl:text>
    </xsl:for-each>
    <xsl:text>
const sal_Int32 OOXML_FAST_TOKENS_END = </xsl:text>
<xsl:value-of select="count(/model/fasttoken)"/>
<xsl:text>;&#xa;</xsl:text>
  </xsl:template>

  <xsl:template match="/">
    <out xml:space="preserve">
      <xsl:call-template name="licenseheader"/>
      <xsl:text>
#ifndef INCLUDED_OOXML_FAST_TOKENS_HXX
#define INCLUDED_OOXML_FAST_TOKENS_HXX

#include &lt;map&gt;
#include &lt;vector&gt;
#include &lt;boost/shared_ptr.hpp&gt;
#include &lt;string&gt;

#include &lt;resourcemodel/WW8ResourceModel.hxx&gt;

namespace writerfilter {
namespace ooxml {
using namespace ::std;
using namespace ::com::sun::star;
</xsl:text>
<xsl:call-template name="fasttokens"/>
<xsl:text>
string fastTokenToId(sal_uInt32 nToken);

static const sal_uInt32 F_Attribute = 1U &lt;&lt; 31;

}}
#endif //INCLUDED_OOXML_FAST_TOKENS_HXX&#xa;</xsl:text></out></xsl:template>

  <xsl:template match="*"/>
</xsl:stylesheet>
