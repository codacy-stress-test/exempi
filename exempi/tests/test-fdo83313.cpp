/*
 * exempi - test-fd083313.cpp
 *
 * Copyright (C) 2007-2022 Hubert Figuière
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>

#include <boost/test/included/unit_test.hpp>

#include "utils.h"
#include "xmpconsts.h"
#include "xmp.h"

boost::unit_test::test_suite* init_unit_test_suite(int argc, char * argv[])
{
  prepare_test(argc, argv, "fdo83313.jpg");

  return nullptr;
}

BOOST_AUTO_TEST_CASE(test_fd083313)
{
  BOOST_CHECK(xmp_init());

  XmpFilePtr xf = xmp_files_open_new(g_testfile.c_str(), XMP_OPEN_READ);

  BOOST_CHECK(xf != NULL);
  if (xf == NULL) {
    exit(128);
  }

  XmpPtr xmp = xmp_files_get_new_xmp(xf);

  BOOST_CHECK(xmp != NULL);

  BOOST_CHECK(xmp_free(xmp));
  BOOST_CHECK(xmp_files_free(xf));

  xmp_terminate();

  BOOST_CHECK(!g_lt->check_leaks());
  BOOST_CHECK(!g_lt->check_errors());
}
