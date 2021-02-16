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

#include <MUSO/app/ledger/AcceptedLedger.h>
#include <MUSO/app/ledger/InboundLedgers.h>
#include <MUSO/app/ledger/LedgerMaster.h>
#include <MUSO/app/main/Application.h>
#include <MUSO/app/misc/NetworkOPs.h>
#include <MUSO/basics/UptimeClock.h>
#include <MUSO/core/DatabaseCon.h>
#include <MUSO/json/json_value.h>
#include <MUSO/ledger/CachedSLEs.h>
#include <MUSO/net/RPCErr.h>
#include <MUSO/nodestore/Database.h>
#include <MUSO/nodestore/DatabaseShard.h>
#include <MUSO/protocol/ErrorCodes.h>
#include <MUSO/protocol/jss.h>
#include <MUSO/rpc/Context.h>
#include <MUSO/shamap/ShardFamily.h>

namespace MUSO {

static void
textTime(
    std::string& text,
    UptimeClock::time_point& seconds,
    const char* unitName,
    std::chrono::seconds unitVal)
{
    auto i = seconds.time_since_epoch() / unitVal;

    if (i == 0)
        return;

    seconds -= unitVal * i;

    if (!text.empty())
        text += ", ";

    text += std::to_string(i);
    text += " ";
    text += unitName;

    if (i > 1)
        text += "s";
}

Json::Value
getCountsJson(Application& app, int minObjectCount)
{
    auto objectCounts = CountedObjects::getInstance().getCounts(minObjectCount);

    Json::Value ret(Json::objectValue);

    for (auto const& [k, v] : objectCounts)
    {
        ret[k] = v;
    }

    if (!app.config().reporting() && app.config().useTxTables())
    {
        int dbKB = getKBUsedAll(app.getLedgerDB().getSession());

        if (dbKB > 0)
            ret[jss::dbKBTotal] = dbKB;

        dbKB = getKBUsedDB(app.getLedgerDB().getSession());

        if (dbKB > 0)
            ret[jss::dbKBLedger] = dbKB;

        dbKB = getKBUsedDB(app.getTxnDB().getSession());

        if (dbKB > 0)
            ret[jss::dbKBTransaction] = dbKB;

        {
            std::size_t c = app.getOPs().getLocalTxCount();
            if (c > 0)
                ret[jss::local_txs] = static_cast<Json::UInt>(c);
        }
    }

    ret[jss::write_load] = app.getNodeStore().getWriteLoad();

    ret[jss::historical_perminute] =
        static_cast<int>(app.getInboundLedgers().fetchRate());
    ret[jss::SLE_hit_rate] = app.cachedSLEs().rate();
    ret[jss::ledger_hit_rate] = app.getLedgerMaster().getCacheHitRate();
    ret[jss::AL_hit_rate] = app.getAcceptedLedgerCache().getHitRate();

    ret[jss::fullbelow_size] =
        static_cast<int>(app.getNodeFamily().getFullBelowCache(0)->size());
    ret[jss::treenode_cache_size] =
        app.getNodeFamily().getTreeNodeCache(0)->getCacheSize();
    ret[jss::treenode_track_size] =
        app.getNodeFamily().getTreeNodeCache(0)->getTrackSize();

    std::string uptime;
    auto s = UptimeClock::now();
    using namespace std::chrono_literals;
    textTime(uptime, s, "year", 365 * 24h);
    textTime(uptime, s, "day", 24h);
    textTime(uptime, s, "hour", 1h);
    textTime(uptime, s, "minute", 1min);
    textTime(uptime, s, "second", 1s);
    ret[jss::uptime] = uptime;

    if (auto shardStore = app.getShardStore())
    {
        auto shardFamily{dynamic_cast<ShardFamily*>(app.getShardFamily())};
        auto const [cacheSz, trackSz] = shardFamily->getTreeNodeCacheSize();
        Json::Value& jv = (ret[jss::shards] = Json::objectValue);

        jv[jss::fullbelow_size] = shardFamily->getFullBelowCacheSize();
        jv[jss::treenode_cache_size] = cacheSz;
        jv[jss::treenode_track_size] = trackSz;
        ret[jss::write_load] = shardStore->getWriteLoad();
        jv[jss::node_writes] = std::to_string(shardStore->getStoreCount());
        jv[jss::node_reads_total] = shardStore->getFetchTotalCount();
        jv[jss::node_reads_hit] = shardStore->getFetchHitCount();
        jv[jss::node_written_bytes] =
            std::to_string(shardStore->getStoreSize());
        jv[jss::node_read_bytes] = shardStore->getFetchSize();
    }
    else
    {
        app.getNodeStore().getCountsJson(ret);
    }

    return ret;
}

// {
//   min_count: <number>  // optional, defaults to 10
// }
Json::Value
doGetCounts(RPC::JsonContext& context)
{
    int minCount = 10;

    if (context.params.isMember(jss::min_count))
        minCount = context.params[jss::min_count].asUInt();

    return getCountsJson(context.app, minCount);
}

}  // namespace MUSO
