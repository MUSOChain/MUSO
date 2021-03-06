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
#include <MUSO/app/misc/NetworkOPs.h>
#include <MUSO/json/json_value.h>
#include <MUSO/net/RPCErr.h>
#include <MUSO/protocol/jss.h>
#include <MUSO/rpc/Context.h>

namespace MUSO {

Json::Value
doLedgerCurrent(RPC::JsonContext& context)
{
    Json::Value jvResult;
    jvResult[jss::ledger_current_index] =
        context.ledgerMaster.getCurrentLedgerIndex();
    return jvResult;
}

}  // namespace MUSO
