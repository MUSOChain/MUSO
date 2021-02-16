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

#ifndef MUSO_APP_PATHS_MUSOLINECACHE_H_INCLUDED
#define MUSO_APP_PATHS_MUSOLINECACHE_H_INCLUDED

#include <MUSO/app/ledger/Ledger.h>
#include <MUSO/app/paths/MUSOState.h>
#include <MUSO/basics/hardened_hash.h>
#include <cstddef>
#include <memory>
#include <mutex>
#include <vector>

namespace MUSO {

// Used by Pathfinder
class MUSOLineCache
{
public:
    explicit MUSOLineCache(std::shared_ptr<ReadView const> const& l);

    std::shared_ptr<ReadView const> const&
    getLedger() const
    {
        return mLedger;
    }

    std::vector<MUSOState::pointer> const&
    getMUSOLines(AccountID const& accountID);

private:
    std::mutex mLock;

    MUSO::hardened_hash<> hasher_;
    std::shared_ptr<ReadView const> mLedger;

    struct AccountKey
    {
        AccountID account_;
        std::size_t hash_value_;

        AccountKey(AccountID const& account, std::size_t hash)
            : account_(account), hash_value_(hash)
        {
        }

        AccountKey(AccountKey const& other) = default;

        AccountKey&
        operator=(AccountKey const& other) = default;

        bool
        operator==(AccountKey const& lhs) const
        {
            return hash_value_ == lhs.hash_value_ && account_ == lhs.account_;
        }

        std::size_t
        get_hash() const
        {
            return hash_value_;
        }

        struct Hash
        {
            explicit Hash() = default;

            std::size_t
            operator()(AccountKey const& key) const noexcept
            {
                return key.get_hash();
            }
        };
    };

    hash_map<AccountKey, std::vector<MUSOState::pointer>, AccountKey::Hash>
        lines_;
};

}  // namespace MUSO

#endif
