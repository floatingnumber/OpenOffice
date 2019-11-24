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


package com.sun.star.report.pentaho.loader;

import com.sun.star.report.InputRepository;

import java.io.IOException;
import java.io.InputStream;

import org.pentaho.reporting.libraries.resourceloader.ResourceKey;
import org.pentaho.reporting.libraries.resourceloader.ResourceLoadingException;
import org.pentaho.reporting.libraries.resourceloader.ResourceManager;
import org.pentaho.reporting.libraries.resourceloader.loader.AbstractResourceData;


public class InputRepositoryResourceData extends AbstractResourceData
{

    private final InputRepository inputRepository;
    private final ResourceKey key;
    private final String resourceIdentifer;

    public InputRepositoryResourceData(final ResourceKey key,
            final InputRepository repository)
    {
        this.key = key;
        this.inputRepository = repository;
        final InputResourceKey rkey = (InputResourceKey) key.getIdentifier();
        final String identifier = rkey.getPath();
        this.resourceIdentifer = identifier.substring("sun:oo://".length());
    }

    public Object getAttribute(final String key)
    {
        // we dont support attributes here ..
        return null;
    }

    public ResourceKey getKey()
    {
        return key;
    }

    public InputStream getResourceAsStream(final ResourceManager caller)
            throws ResourceLoadingException
    {
        try
        {
            return inputRepository.createInputStream(resourceIdentifer);
        }
        catch (IOException e)
        {
            throw new ResourceLoadingException("Failed to create input stream for " + resourceIdentifer, e);
        }
    }

    public long getVersion(final ResourceManager caller)
    {
        return inputRepository.getVersion(resourceIdentifer);
    }
}
