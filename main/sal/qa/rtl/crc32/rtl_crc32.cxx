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




// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_sal.hxx"
// autogenerated file with codegen.pl

#include "gtest/gtest.h"
#include <rtl/crc.h>

namespace rtl_CRC32
{

class test : public ::testing::Test
{
public:
    // initialise your test code values here.
    void SetUp()
    {
    }

    void TearDown()
    {
    }
}; // class test

TEST_F(test, rtl_crc32_001)
{
    sal_uInt32 nCRC = 0;

    char buf[] = {0};
    int num = 0;

    nCRC = rtl_crc32(nCRC, buf, num);

    ASSERT_TRUE(nCRC == 0) << "empty crc buffer";
}

TEST_F(test, rtl_crc32_002)
{
    sal_uInt32 nCRC = 0;

    char buf[] = {0,0};
    int num = sizeof(buf);

    nCRC = rtl_crc32(nCRC, buf, num);

    ASSERT_TRUE(nCRC != 0) << "buffer contain 2 empty bytes, crc is zero";
}

TEST_F(test, rtl_crc32_002_1)
{
    sal_uInt32 nCRC = 0;

    char buf[] = {0,0,0};
    int num = sizeof(buf);

    nCRC = rtl_crc32(nCRC, buf, num);

    ASSERT_TRUE(nCRC != 0) << "buffer contain 3 empty bytes, crc is zero";
}

/**
 * crc32 check:
 * Build checksum on two buffers with same size but different content,
 * the result (crc32 checksum) must differ
 */

TEST_F(test, rtl_crc32_003)
{
    sal_uInt32 nCRC1 = 0;
    char buf1[] = {2};
    int num1 = sizeof(buf1);

    nCRC1 = rtl_crc32(nCRC1, buf1, num1);

    sal_uInt32 nCRC2 = 0;
    char buf2[] = {3};
    int num2 = sizeof(buf2);

    nCRC2 = rtl_crc32(nCRC2, buf2, num2);

    ASSERT_TRUE(nCRC1 != nCRC2) << "checksum should differ for buf1 and buf2";
}

/** check if the crc32 only use as much values, as given
 *
 */
TEST_F(test, rtl_crc32_003_1)
{
    sal_uInt32 nCRC1 = 0;
    char buf1[] = {2,1};
    int num1 = sizeof(buf1) - 1;

    nCRC1 = rtl_crc32(nCRC1, buf1, num1);

    sal_uInt32 nCRC2 = 0;
    char buf2[] = {2,2};
    int num2 = sizeof(buf2) - 1;

    nCRC2 = rtl_crc32(nCRC2, buf2, num2);

    ASSERT_TRUE(nCRC1 == nCRC2) << "checksum leave it's bounds";
}

/** check if the crc32 differ at same content in reverse order
 *
 */
TEST_F(test, rtl_crc32_003_2)
{
    sal_uInt32 nCRC1 = 0;
    char buf1[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    int num1 = sizeof(buf1);

    nCRC1 = rtl_crc32(nCRC1, buf1, num1);

    sal_uInt32 nCRC2 = 0;
    char buf2[] = {20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
    int num2 = sizeof(buf2);

    nCRC2 = rtl_crc32(nCRC2, buf2, num2);

    ASSERT_TRUE(nCRC1 != nCRC2) << "checksum should differ";
}

// -----------------------------------------------------------------------------
} // namespace rtl_CRC32

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
