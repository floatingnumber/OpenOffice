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



package ifc.ucb;

import lib.MultiMethodTest;

import com.sun.star.ucb.XContentProvider;
import com.sun.star.ucb.XParameterizedContentProvider;

/**
* Testing <code>com.sun.star.ucb.XParameterizedContentProvider</code>
* interface methods :
* <ul>
*  <li><code> registerInstance()</code></li>
*  <li><code> deregisterInstance()</code></li>
* </ul> <p>
* Test is <b> NOT </b> multithread compilant. <p>
* @see com.sun.star.ucb.XParameterizedContentProvider
*/
public class _XParameterizedContentProvider extends MultiMethodTest {

    public static XParameterizedContentProvider oObj = null;

    /**
     * Registers an instance with some arguments. <p>
     * Has <b>OK</b> status if the method returns non <code>null</code>
     * provider.
     */
    public void _registerInstance() {
        try {
            XContentProvider CP = oObj.registerInstance(
                        "\"vnd.sun.star.pkg://file\".*",
                        "uno:pipe,name=ucb_soffice;<PIPE>;urp;UCB.Factory",
                        true);
            tRes.tested("registerInstance()",CP != null);
        } catch (com.sun.star.lang.IllegalArgumentException e) {
            log.println("Exception while checking 'registerInstance'");
            e.printStackTrace(log);
            tRes.tested("registerInstance()",false);
        }
    }

    /**
     * Deregisters the instance registered before. <p>
     * Has <b>OK</b> status if the method returns non <code>null</code>
     * provider. <p>
     *
     * The following method tests are to be completed successfully before :
     * <ul>
     *  <li> <code> registerInstance() </code> : to deregister it here. </li>
     * </ul>
     */
    public void _deregisterInstance() {
        requiredMethod("registerInstance()");
        try {
            XContentProvider CP = oObj.deregisterInstance(
                        "\"vnd.sun.star.pkg://file\".*",
                        "uno:pipe,name=ucb_soffice;<PIPE>;urp;UCB.Factory");
            tRes.tested("deregisterInstance()",CP != null);
        } catch (com.sun.star.lang.IllegalArgumentException e) {
            log.println("Exception while checking 'deregisterInstance'");
            e.printStackTrace(log);
            tRes.tested("deregisterInstance()",false);
        }
    }

}

