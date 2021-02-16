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

#include <MUSO/basics/base_uint.h>
#include <MUSO/beast/utility/rngfill.h>
#include <MUSO/crypto/csprng.h>
#include <MUSO/json/json_value.h>
#include <MUSO/net/RPCErr.h>
#include <MUSO/protocol/ErrorCodes.h>
#include <MUSO/protocol/jss.h>

namespace MUSO {

namespace RPC {
struct JsonContext;
}

// Result:
// {
//   random: <uint256>
// }
Json::Value
doRandom(RPC::JsonContext& context)
{
    // TODO(tom): the try/catch is almost certainly redundant, we catch at the
    // top level too.
    try
    {
        uint256 rand;
        beast::rngfill(rand.begin(), rand.size(), crypto_prng());

        Json::Value jvResult;
        jvResult[jss::random] = to_string(rand);
        return jvResult;
    }
    catch (std::exception const&)
    {
        return rpcError(rpcINTERNAL);
    }
}

}  // namespace MUSO
