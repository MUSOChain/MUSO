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

#ifndef MUSO_RESOURCE_KEY_H_INCLUDED
#define MUSO_RESOURCE_KEY_H_INCLUDED

#include <MUSO/beast/net/IPEndpoint.h>
#include <MUSO/resource/impl/Kind.h>
#include <cassert>

namespace MUSO {
namespace Resource {

// The consumer key
struct Key
{
    Kind kind;
    beast::IP::Endpoint address;

    Key() = delete;

    Key(Kind k, beast::IP::Endpoint const& addr) : kind(k), address(addr)
    {
    }

    struct hasher
    {
        std::size_t
        operator()(Key const& v) const
        {
            return m_addr_hash(v.address);
        }

    private:
        beast::uhash<> m_addr_hash;
    };

    struct key_equal
    {
        explicit key_equal() = default;

        bool
        operator()(Key const& lhs, Key const& rhs) const
        {
            return lhs.kind == rhs.kind && lhs.address == rhs.address;
        }

    private:
    };
};

}  // namespace Resource
}  // namespace MUSO

#endif
