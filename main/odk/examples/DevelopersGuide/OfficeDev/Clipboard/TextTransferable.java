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



import com.sun.star.datatransfer.DataFlavor;
import com.sun.star.datatransfer.UnsupportedFlavorException;
import com.sun.star.datatransfer.XTransferable;
import com.sun.star.uno.Type;

//---------------------------------------
// A simple transferable containing only
// one format, unicode text
//---------------------------------------

public class TextTransferable implements XTransferable
{ 
	public TextTransferable(String aText)
	{
		text = aText;
	}

	// XTransferable methods

	public Object getTransferData(DataFlavor aFlavor) throws UnsupportedFlavorException
	{
		if ( !aFlavor.MimeType.equalsIgnoreCase( UNICODE_CONTENT_TYPE ) )
			throw new UnsupportedFlavorException();

		return text;
	}

	public DataFlavor[] getTransferDataFlavors()
	{
		DataFlavor[] adf = new DataFlavor[1];

		DataFlavor uniflv = new DataFlavor(
			UNICODE_CONTENT_TYPE, 
			"Unicode Text", 
			new Type(String.class) );
	
		adf[0] = uniflv;

		return adf;
	}

	public boolean isDataFlavorSupported(DataFlavor aFlavor)
	{
		return aFlavor.MimeType.equalsIgnoreCase(UNICODE_CONTENT_TYPE);
	}

// members

	private final String text;
	private final String UNICODE_CONTENT_TYPE = "text/plain;charset=utf-16";
}
