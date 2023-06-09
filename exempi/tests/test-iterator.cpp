/*
 * exempi - test3.cpp
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

#include <array>
#include <iostream>
#include <string>
#include <vector>

#include <boost/test/included/unit_test.hpp>
#include <boost/format.hpp>

#include "utils.h"
#include "xmp.h"
#include "xmpconsts.h"

using boost::unit_test::test_suite;

boost::unit_test::test_suite* init_unit_test_suite(int argc, char * argv[])
{
  prepare_test(argc, argv, "test1.xmp");
  return nullptr;
}

BOOST_AUTO_TEST_CASE(test_exempi_iterate)
{
  size_t len;
  char *buffer;

  FILE *f = fopen(g_testfile.c_str(), "rb");

  BOOST_CHECK(f != NULL);
  if (f == NULL) {
    exit(128);
  }
  fseek(f, 0, SEEK_END);
  len = ftell(f);
  fseek(f, 0, SEEK_SET);

  buffer = (char *)malloc(len + 1);
  size_t rlen = fread(buffer, 1, len, f);

  BOOST_CHECK(rlen == len);
  BOOST_CHECK(len != 0);
  BOOST_CHECK(xmp_init());

  XmpPtr xmp = xmp_new_empty();

  BOOST_CHECK(xmp_parse(xmp, buffer, len));

  BOOST_CHECK(xmp != NULL);

  XmpStringPtr the_schema = xmp_string_new();
  XmpStringPtr the_path = xmp_string_new();
  XmpStringPtr the_prop = xmp_string_new();
  uint32_t options;

  typedef std::array<std::string, 3> tuple3;

  {
    // leafnodes iteration
    XmpIteratorPtr iter =
      xmp_iterator_new(xmp, NS_DC, NULL, XMP_ITER_JUSTLEAFNODES);

    BOOST_CHECK(iter);
    std::vector<tuple3> props;

    while (xmp_iterator_next(iter, the_schema, the_path, the_prop, &options)) {
      props.push_back(tuple3 {{
          xmp_string_cstr(the_schema),
          xmp_string_cstr(the_path),
          xmp_string_cstr(the_prop)
        }});
    }

    BOOST_CHECK(props.size() == 7);
    for (auto prop_tuple : props) {
      BOOST_CHECK(prop_tuple[0] == NS_DC);
    }
    BOOST_CHECK(props[0][2] == "unknown");
    BOOST_CHECK(props[3][1] == "dc:subject[1]");

    BOOST_CHECK(xmp_iterator_free(iter));
  }

  {
    // leafname iteration
    XmpIteratorPtr iter =
      xmp_iterator_new(xmp, NS_DC, "rights", XMP_ITER_JUSTLEAFNAME);

    BOOST_CHECK(iter);
    std::vector<tuple3> props;

    while (xmp_iterator_next(iter, the_schema, the_path, the_prop, &options)) {
      props.push_back(tuple3 {{
          xmp_string_cstr(the_schema),
          xmp_string_cstr(the_path),
          xmp_string_cstr(the_prop)
        }});
    }

    BOOST_CHECK(props.size() == 3);

    BOOST_CHECK(props[0] == (tuple3{{NS_DC, "dc:rights", ""}}));
    BOOST_CHECK(props[1] == (tuple3{{"", "[1]", "2006, Hubert Figuiere"}}));
    BOOST_CHECK(props[2] == (tuple3{{NS_XML, "xml:lang", "x-default"}}));

    BOOST_CHECK(xmp_iterator_free(iter));
  }

  {
    // Iterator with property but no NS is invalid.
    XmpIteratorPtr iter =
      xmp_iterator_new(xmp, NULL, "rights", XMP_ITER_JUSTLEAFNODES);

    // Invalid iterator
    BOOST_CHECK(!iter);
  }

  {
    // Iterating namespaces is invalid.
    XmpIteratorPtr iter =
      xmp_iterator_new(xmp, NULL, NULL, XMP_ITER_NAMESPACES);

    // Invalid iterator
    BOOST_CHECK(!iter);
  }

  xmp_string_free(the_prop);
  xmp_string_free(the_path);
  xmp_string_free(the_schema);
  BOOST_CHECK(xmp_free(xmp));

  free(buffer);
  fclose(f);
  xmp_terminate();

  BOOST_CHECK(!g_lt->check_leaks());
  BOOST_CHECK(!g_lt->check_errors());
}
