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



package ifc.text;

import lib.MultiMethodTest;
import util.utils;

import com.sun.star.beans.PropertyValue;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.text.XPagePrintable;

/**
 * Testing <code>com.sun.star.text.XPagePrintable</code>
 * interface methods :
 * <ul>
 *  <li><code> getPagePrintSettings()</code></li>
 *  <li><code> setPagePrintSettings()</code></li>
 *  <li><code> printPages()</code></li>
 * </ul> <p>
 * Test is <b> NOT </b> multithread compilant. <p>
 * @see com.sun.star.text.XPagePrintable
 */
public class _XPagePrintable extends MultiMethodTest {

    public static XPagePrintable oObj = null;
    public PropertyValue[] PrintSettings = new PropertyValue[0];

    /**
     * Types of print settings properties by order they returned by
     * <code>getPagePrintSettings()</code>.
     */
    public String[] types = new String[]{"Short","Short","Integer","Integer",
        "Integer","Integer","Integer","Integer","Boolean"};

    /**
     * Calls the method and examines the returned array of properties. <p>
     *
     * Has <b>OK</b> status if all properties' types are correspond
     * to their expected values of the <code>types</code> array.
     *
     * @see #types
     */
    public void _getPagePrintSettings() {
        boolean res = true;
        PrintSettings = oObj.getPagePrintSettings();

        for (int i=0;i<PrintSettings.length;i++) {
            String the_type = PrintSettings[i].Value.getClass().toString();
            if (!the_type.endsWith(types[i])) {
                log.println("Name: "+PrintSettings[i].Name);
                log.println("Value: "+PrintSettings[i].Value);
                log.println("Type"+the_type);
                log.println("Expected: java.lang."+types[i]);
                res = false;
            }
        }

        tRes.tested("getPagePrintSettings()",res);
    }

    /**
     * Changes a property 'IsLandscape' in existsing print settings,
     * and sets these settings back. <p>
     *
     * Has <b>OK</b> status if settings gotten again has the changed
     * 'IsLandscape' property value. <p>
     *
     * The following method tests are to be completed successfully before :
     * <ul>
     *  <li> <code> getPagePrintSettings() </code> : to have existing
     *   print settings. </li>
     * </ul>
     */
    public void _setPagePrintSettings() {
        requiredMethod("getPagePrintSettings()");
        boolean res = true;

        Boolean landscape = (Boolean) PrintSettings[8].Value;
        Boolean newlandscape = new Boolean(!landscape.booleanValue());
        PrintSettings[8].Value = newlandscape;
        oObj.setPagePrintSettings(PrintSettings);
        res = (oObj.getPagePrintSettings()[8].Value.equals(newlandscape));

        tRes.tested("setPagePrintSettings()",res);
    }

    /**
     * Creates print options for printing into file situated in the SOffice
     * temporary directory. If the file already exists it is deleted.
     * Then calls the method. <p>
     *
     * Has <b>OK</b> status if the file to which printing must be performed
     * is exists.
     */
    public void _printPages() {
        boolean res = true;

        try {
            XMultiServiceFactory xMSF = (XMultiServiceFactory)tParam.getMSF();
            
            String printFile = utils.getOfficeTemp(xMSF) + "XPagePrintable.prt";
            log.println("Printing to : "+ printFile);

            PropertyValue[] PrintOptions = new PropertyValue[1];
            PropertyValue firstProp = new PropertyValue();
            firstProp.Name = "FileName";

            firstProp.Value = printFile;
            firstProp.State = com.sun.star.beans.PropertyState.DEFAULT_VALUE;
            PrintOptions[0] = firstProp;
            
            if (! util.utils.deleteFile(xMSF, printFile)){
                log.println("ERROR: could not remove '" + printFile + "'");
                res = false;
            }
            
            oObj.printPages(PrintOptions);
            
            util.utils.shortWait(tParam.getInt(util.PropertyName.SHORT_WAIT));
            
            if (! util.utils.fileExists(xMSF, printFile)){
                log.println("ERROR: could not find '" + printFile + "'");
                res = false;
            }
            
        } catch (com.sun.star.lang.IllegalArgumentException ex) {
            log.println("Exception while checking 'printPages'");
            res = false;
            ex.printStackTrace(log);
        }

        tRes.tested("printPages()",res);
    }

}  // finish class _XPagePrintable

