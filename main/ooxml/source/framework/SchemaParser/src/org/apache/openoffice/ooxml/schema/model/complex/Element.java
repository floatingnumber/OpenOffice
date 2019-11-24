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

package org.apache.openoffice.ooxml.schema.model.complex;

import org.apache.openoffice.ooxml.schema.model.base.INodeVisitor;
import org.apache.openoffice.ooxml.schema.model.base.Location;
import org.apache.openoffice.ooxml.schema.model.base.Node;
import org.apache.openoffice.ooxml.schema.model.base.NodeType;
import org.apache.openoffice.ooxml.schema.model.base.QualifiedName;

/** Representation of the 'element' XML schema element.
 */
public class Element
    extends Node
{
    /** Create a new element that represents a transition to aTypeName when a
     *  start tag of aElementName is processed.
     */
    public Element (
        final Node aParent,
        final QualifiedName aElementName,
        final QualifiedName aTypeName,
        final Location aLocation)
    {
        super(aParent, aElementName, aLocation);
        maElementName = aElementName;
        maTypeName = aTypeName;
    }
    
    
    
    
    public QualifiedName GetElementName ()
    {
        return maElementName;
    }
    
    
    
    
    public QualifiedName GetTypeName ()
    {
        return maTypeName;
    }
    
    
    
    
    @Override
    public int compareTo (final Node aOther)
    {
        if (aOther.GetNodeType() != NodeType.Element)
            throw new RuntimeException("can not compare Element object to non-Element object");
        else
            return maElementName.compareTo(((Element)aOther).maElementName);
    }
    
    
    
    
    @Override
    public NodeType GetNodeType ()
    {
        return NodeType.Element;
    }

    
    
    
    @Override
    public void AcceptVisitor (final INodeVisitor aVisitor)
    {
        aVisitor.Visit(this);
    }

    
    
    
    @Override
    public String toString ()
    {
        return "element " + maElementName.GetDisplayName() +" -> " + maTypeName.GetDisplayName();
    }
    
    
    
    
    private final QualifiedName maElementName;
    private final QualifiedName maTypeName;
}
