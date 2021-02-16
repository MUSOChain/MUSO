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

#include <MUSO/app/main/Application.h>
#include <MUSO/basics/Log.h>
#include <MUSO/json/json_value.h>
#include <MUSO/net/RPCErr.h>
#include <MUSO/protocol/ErrorCodes.h>
#include <MUSO/protocol/jss.h>
#include <MUSO/rpc/Context.h>
#include <boost/algorithm/string/predicate.hpp>

namespace MUSO {

Json::Value
doLogLevel(RPC::JsonContext& context)
{
    // log_level
    if (!context.params.isMember(jss::severity))
    {
        // get log severities
        Json::Value ret(Json::objectValue);
        Json::Value lev(Json::objectValue);

        lev[jss::base] =
            Logs::toString(Logs::fromSeverity(context.app.logs().threshold()));
        std::vector<std::pair<std::string, std::string>> logTable(
            context.app.logs().partition_severities());
        using stringPair = std::map<std::string, std::string>::value_type;
        for (auto const& [k, v] : logTable)
            lev[k] = v;

        ret[jss::levels] = lev;
        return ret;
    }

    LogSeverity const sv(
        Logs::fromString(context.params[jss::severity].asString()));

    if (sv == lsINVALID)
        return rpcError(rpcINVALID_PARAMS);

    auto severity = Logs::toSeverity(sv);
    // log_level severity
    if (!context.params.isMember(jss::partition))
    {
        // set base log threshold
        context.app.logs().threshold(severity);
        return Json::objectValue;
    }

    // log_level partition severity base?
    if (context.params.isMember(jss::partition))
    {
        // set partition threshold
        std::string partition(context.params[jss::partition].asString());

        if (boost::iequals(partition, "base"))
            context.app.logs().threshold(severity);
        else
            context.app.logs().get(partition).threshold(severity);

        return Json::objectValue;
    }

    return rpcError(rpcINVALID_PARAMS);
}

}  // namespace MUSO
