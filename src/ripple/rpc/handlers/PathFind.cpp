//------------------------------------------------------------------------------
/*
    This file is part of MUSO: https://github.com/MUSO/MUSO
    Copyright (c) 2012-2014 MUSO Labs Inc.

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

#include <MUSO/app/ledger/LedgerMaster.h>
#include <MUSO/app/main/Application.h>
#include <MUSO/app/misc/NetworkOPs.h>
#include <MUSO/app/paths/PathRequests.h>
#include <MUSO/net/RPCErr.h>
#include <MUSO/protocol/ErrorCodes.h>
#include <MUSO/protocol/jss.h>
#include <MUSO/resource/Fees.h>
#include <MUSO/rpc/Context.h>

namespace MUSO {

Json::Value
doPathFind(RPC::JsonContext& context)
{
    if (context.app.config().PATH_SEARCH_MAX == 0)
        return rpcError(rpcNOT_SUPPORTED);

    auto lpLedger = context.ledgerMaster.getClosedLedger();

    if (!context.params.isMember(jss::subcommand) ||
        !context.params[jss::subcommand].isString())
    {
        return rpcError(rpcINVALID_PARAMS);
    }

    if (!context.infoSub)
        return rpcError(rpcNO_EVENTS);

    auto sSubCommand = context.params[jss::subcommand].asString();

    if (sSubCommand == "create")
    {
        context.loadType = Resource::feeHighBurdenRPC;
        context.infoSub->clearPathRequest();
        return context.app.getPathRequests().makePathRequest(
            context.infoSub, lpLedger, context.params);
    }

    if (sSubCommand == "close")
    {
        PathRequest::pointer request = context.infoSub->getPathRequest();

        if (!request)
            return rpcError(rpcNO_PF_REQUEST);

        context.infoSub->clearPathRequest();
        return request->doClose(context.params);
    }

    if (sSubCommand == "status")
    {
        PathRequest::pointer request = context.infoSub->getPathRequest();

        if (!request)
            return rpcError(rpcNO_PF_REQUEST);

        return request->doStatus(context.params);
    }

    return rpcError(rpcINVALID_PARAMS);
}

}  // namespace MUSO
