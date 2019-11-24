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



package ifc.util;

import lib.MultiMethodTest;

import com.sun.star.lang.XServiceInfo;
import com.sun.star.lang.XTypeProvider;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;
import com.sun.star.util.XCloneable;

/**
* Testing <code>com.sun.star.util.XCloneable</code>
* interface methods :
* <ul>
*  <li><code> createClone()</code></li>
* </ul> <p>
* @see com.sun.star.util.XCloneable
*/
public class _XCloneable extends MultiMethodTest {

    // oObj filled by MultiMethodTest
    public XCloneable oObj = null ;
    protected XCloneable clone = null;
    
    /**
     * calls the method. <p>
     * Has <b>OK</b> status if no exception has occurred. <p>
     */
    public void _createClone() {
        boolean result = true;
        clone = oObj.createClone();

        //check if the implementaionname equals
        result &= checkImplementationName(oObj,clone);
        
        //check ImplementationID
        result &= checkImplementationID(oObj, clone);
        
        tRes.tested("createClone()", result) ;
    }
    
    protected byte[] getImplementationID(XInterface ifc) {
        byte[] res = new byte[0];
        XTypeProvider provider = (XTypeProvider) 
                    UnoRuntime.queryInterface(XTypeProvider.class, ifc);
        if (provider != null) {
            res = provider.getImplementationId();
        }
        return res;
    } 
    
    protected boolean checkImplementationID(XInterface org, XInterface clone) {
        boolean res = getImplementationID(org).equals(
                                            getImplementationID(clone));
        if (res && getImplementationID(org).length > 0) {
            log.println("ImplementationID equals the clone has the same id as the original Object");
            log.println("------------------------------------------------------------------------");
        }
        return !res;
    }    
    
    protected String getImplementationName(XInterface ifc) {
        String res = "";
        XServiceInfo info = (XServiceInfo) 
                    UnoRuntime.queryInterface(XServiceInfo.class, ifc);
        if (info != null) {
            res = info.getImplementationName();
        }
        return res;
    }
    
    protected boolean checkImplementationName(XInterface org, XInterface clone) {
        boolean res = getImplementationName(org).equals(
                                            getImplementationName(clone));
        if (!res) {
            log.println("ImplementationName differs: ");
            log.println("Expected: "+getImplementationName(org));
            log.println("Gained: "+getImplementationName(clone));
            log.println("----------------------------------------");
        }
        return res;
    }

}  // finish class _XCloneable

