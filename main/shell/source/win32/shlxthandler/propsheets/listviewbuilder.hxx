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



#ifndef LISTVIEWBUILDER_HXX_INCLUDED
#define LISTVIEWBUILDER_HXX_INCLUDED

//------------------------------------
// include 
//------------------------------------

#if defined _MSC_VER
#pragma warning(push, 1)
#endif
#include <windows.h>
#if defined _MSC_VER
#pragma warning(pop)
#endif 

#include <string>
#include <memory>
#include "document_statistic.hxx"

//------------------------------------
//
//------------------------------------

class list_view_builder;
typedef std::auto_ptr<list_view_builder> list_view_builder_ptr;

// factory method for list_view_builder
list_view_builder_ptr create_list_view_builder(
    HWND hwnd_lv, const std::wstring& col1, const std::wstring& col2);
        
//------------------------------------
//
//------------------------------------

class list_view_builder
{    
public:        
    virtual ~list_view_builder();
            
    void build(statistic_group_list_t& gl);
    
protected:
    list_view_builder(
        HWND hwnd_list_view, 
        const std::wstring& column1_title, 
        const std::wstring& column2_title);
        
    virtual void setup_list_view();
    virtual void insert_group(const std::wstring& title);
    virtual void insert_item(const std::wstring& title, const std::wstring& value, bool is_editable);    
        
    HWND get_list_view() const;
    int get_current_row() const;
    
    int  row_index_;
    
private:
    HWND hwnd_list_view_;    
    std::wstring column1_title_;
    std::wstring column2_title_;
    
    friend list_view_builder_ptr create_list_view_builder(HWND hwnd_lv, const std::wstring& col1, const std::wstring& col2);
};

//------------------------------------
//
//------------------------------------

class winxp_list_view_builder : public list_view_builder
{
protected:
    winxp_list_view_builder(
        HWND hwnd_list_view,
        const std::wstring& column1_title, 
        const std::wstring& column2_title);
  
    virtual void setup_list_view();
    virtual void insert_group(const std::wstring& name);  
    virtual void insert_item(const std::wstring& title, const std::wstring& value, bool is_editable);
            
private:
    int  group_count_;    
    int  row_count_;
    
    friend list_view_builder_ptr create_list_view_builder(HWND hwnd_lv, const std::wstring& col1, const std::wstring& col2);
};

#endif
