//------------------------------------------------------------------------------
/*
    This file is part of MUSO: https://github.com/MUSO/MUSO
    Copyright (c) 2019 MUSO Labs Inc.

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

#include <MUSO/protocol/jss.h>
#include <test/jtx/acctdelete.h>

namespace MUSO {
namespace test {
namespace jtx {

// Delete account.  If successful transfer remaining MUSO to dest.
Json::Value
acctdelete(jtx::Account const& account, jtx::Account const& dest)
{
    Json::Value jv;
    jv[sfAccount.jsonName] = account.human();
    jv[sfDestination.jsonName] = dest.human();
    jv[sfTransactionType.jsonName] = jss::AccountDelete;
    return jv;
}

}  // namespace jtx
}  // namespace test
}  // namespace MUSO
