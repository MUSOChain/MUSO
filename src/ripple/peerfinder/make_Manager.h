//------------------------------------------------------------------------------
/*
    This file is part of MUSO: https://github.com/MUSO/MUSO
    Copyright (c) 2012, 2013 MUSO Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef MUSO_PEERFINDER_MAKE_MANAGER_H_INCLUDED
#define MUSO_PEERFINDER_MAKE_MANAGER_H_INCLUDED

#include <MUSO/peerfinder/PeerfinderManager.h>
#include <boost/asio/io_service.hpp>
#include <memory>

namespace MUSO {
namespace PeerFinder {

/** Create a new Manager. */
std::unique_ptr<Manager>
make_Manager(
    Stoppable& parent,
    boost::asio::io_service& io_service,
    clock_type& clock,
    beast::Journal journal,
    BasicConfig const& config,
    beast::insight::Collector::ptr const& collector);

}  // namespace PeerFinder
}  // namespace MUSO

#endif
