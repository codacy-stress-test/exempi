/*
 * exempi - testcpp.cpp
 *
 * Copyright (C) 2011-2022 Hubert Figuière
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1 Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2 Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * 3 Neither the name of the Authors, nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software wit hout specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <boost/test/included/unit_test.hpp>

#include "utils.h"
#include "xmp.h"
#include "xmpconsts.h"
#include "xmp++.hpp"

boost::unit_test::test_suite* init_unit_test_suite(int, char **)
{
  return nullptr;
}

BOOST_AUTO_TEST_CASE(test_cpp)
{
  // this test is totally useless without leak checking.
  // but for compiling.
  {
    xmp::ScopedPtr<XmpStringPtr> ptr(xmp_string_new());
  }

  {
    xmp::ScopedPtr<XmpFilePtr> ptr(xmp_files_new());
  }

  {
    xmp::ScopedPtr<XmpPtr> ptr(xmp_new_empty());
    {
      xmp::ScopedPtr<XmpIteratorPtr> iterptr(
        xmp_iterator_new(ptr, NS_EXIF, NULL, XMP_ITER_JUSTLEAFNODES));
    }
  }

  BOOST_CHECK(!g_lt->check_leaks());
  BOOST_CHECK(!g_lt->check_errors());
}
