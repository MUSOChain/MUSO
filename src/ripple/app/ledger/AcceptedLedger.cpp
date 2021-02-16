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

#include <MUSO/app/ledger/AcceptedLedger.h>
#include <MUSO/app/main/Application.h>
#include <MUSO/basics/Log.h>
#include <MUSO/basics/chrono.h>

namespace MUSO {

AcceptedLedger::AcceptedLedger(
    std::shared_ptr<ReadView const> const& ledger,
    Application& app)
    : mLedger(ledger)
{
    auto insertAll = [&](auto const& txns) {
        for (auto const& item : txns)
        {
            insert(std::make_shared<AcceptedLedgerTx>(
                ledger,
                item.first,
                item.second,
                app.accountIDCache(),
                app.logs()));
        }
    };

    if (app.config().reporting())
        insertAll(flatFetchTransactions(*ledger, app));
    else
        insertAll(ledger->txs);
}

void
AcceptedLedger::insert(AcceptedLedgerTx::ref at)
{
    assert(mMap.find(at->getIndex()) == mMap.end());
    mMap.insert(std::make_pair(at->getIndex(), at));
}

AcceptedLedgerTx::pointer
AcceptedLedger::getTxn(int i) const
{
    map_t::const_iterator it = mMap.find(i);

    if (it == mMap.end())
        return AcceptedLedgerTx::pointer();

    return it->second;
}

}  // namespace MUSO
