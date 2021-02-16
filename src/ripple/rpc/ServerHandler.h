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

#ifndef MUSO_RPC_SERVERHANDLER_H_INCLUDED
#define MUSO_RPC_SERVERHANDLER_H_INCLUDED

#include <MUSO/basics/BasicConfig.h>
#include <MUSO/beast/utility/Journal.h>
#include <MUSO/core/Config.h>
#include <MUSO/core/JobQueue.h>
#include <MUSO/core/Stoppable.h>
#include <MUSO/resource/ResourceManager.h>
#include <MUSO/rpc/impl/ServerHandlerImp.h>
#include <MUSO/server/Port.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>
#include <memory>
#include <vector>

namespace MUSO {

using ServerHandler = ServerHandlerImp;

ServerHandler::Setup
setup_ServerHandler(Config const& c, std::ostream&& log);

std::unique_ptr<ServerHandler>
make_ServerHandler(
    Application& app,
    Stoppable& parent,
    boost::asio::io_service&,
    JobQueue&,
    NetworkOPs&,
    Resource::Manager&,
    CollectorManager& cm);

}  // namespace MUSO

#endif
