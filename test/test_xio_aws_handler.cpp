/***************************************************************************
* Copyright (c) Wolf Vollprecht, Sylvain Corlay and Johan Mabille          *
* Copyright (c) QuantStack                                                 *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <cstdint>
#include <sstream>
#include <exception>

#include "gtest/gtest.h"
#include "xtensor/xview.hpp"
#include "xtensor-io/xfile_array.hpp"
#include "xtensor-io/xio_binary.hpp"
#include "xtensor-io/xio_aws_handler.hpp"

namespace xt
{
    TEST(xio_aws_handler, read)
    {
        Aws::SDKOptions options;
        Aws::InitAPI(options);

        xio_aws_handler<xio_binary_config> h;

        Aws::S3::S3Client client;

        xio_aws_config c = {client, "elevation-tiles-prod"};
        h.configure_io(c);
        xarray<char> a0;
        std::string path = "main.js";
        h.read(a0, path);
        xarray<char> a1 = {'/', '*', 'j', 's', 'l', 'i', 'n', 't', ' ', 'b', 'r' ,'o', 'w', 's', 'e', 'r', ':', ' ', 't', 'r', 'u', 'e', '*', '/'};
        EXPECT_TRUE(xt::all(xt::equal(xt::view(a0, xt::range(0, 24)), a1)));

        Aws::ShutdownAPI(options);
    }

    TEST(xio_aws_handler, xfile_array)
    {
        Aws::SDKOptions options;
        Aws::InitAPI(options);

        Aws::S3::S3Client client;
        xio_aws_config c = {client, "elevation-tiles-prod"};
        xfile_array<char, xio_aws_handler<xio_binary_config>> a0("main.js", c);

        xarray<char> a1 = {'/', '*', 'j', 's', 'l', 'i', 'n', 't', ' ', 'b', 'r' ,'o', 'w', 's', 'e', 'r', ':', ' ', 't', 'r', 'u', 'e', '*', '/'};
        EXPECT_TRUE(xt::all(xt::equal(xt::view(a0, xt::range(0, 24)), a1)));

        Aws::ShutdownAPI(options);
    }
}
