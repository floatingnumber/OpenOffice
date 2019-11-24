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

package com.sun.star.script.framework.log;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

public class LogUtils {

    private static boolean m_bDebugEnabled = false;

    static
    {
        String debugFlag =
            System.getProperties().getProperty("ScriptJavaRuntimeDebug");

        if (debugFlag != null && debugFlag.length() > 0)
        {
            m_bDebugEnabled = debugFlag.equalsIgnoreCase("true");
        }
    }

    // Ensure that instances of this class cannot be created
    private LogUtils() {
    }

    /**
    *  Print Debug Output
    *
    * @param  msg  message to be displayed
    */
    public static void DEBUG(String msg)
    {
        if (m_bDebugEnabled)
        {
            System.out.println(msg);
        }
    }

    public static String getTrace( Exception e )
    {
        ByteArrayOutputStream baos = null;
        PrintStream ps = null;
        String result = "";
        try
        {
            baos = new ByteArrayOutputStream( );
            ps = new PrintStream( baos );
            e.printStackTrace( ps );
        }
        finally
        {
            try
            { 
                if ( baos != null )
                {
                    baos.close();
                }
                if ( ps != null )
                {
                    ps.close();
                }
            }
            catch ( Exception excp )
            {
            }
        }
        return result;
    }
}
