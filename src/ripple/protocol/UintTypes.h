//------------------------------------------------------------------------------
/*
    This file is part of MUSO: https://github.com/MUSO/MUSO
    Copyright (c) 2014 MUSO Labs Inc.

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

#ifndef MUSO_PROTOCOL_UINTTYPES_H_INCLUDED
#define MUSO_PROTOCOL_UINTTYPES_H_INCLUDED

#include <MUSO/basics/UnorderedContainers.h>
#include <MUSO/basics/base_uint.h>
#include <MUSO/beast/utility/Zero.h>
#include <MUSO/protocol/AccountID.h>

namespace MUSO {
namespace detail {

class CurrencyTag
{
public:
    explicit CurrencyTag() = default;
};

class DirectoryTag
{
public:
    explicit DirectoryTag() = default;
};

class NodeIDTag
{
public:
    explicit NodeIDTag() = default;
};

}  // namespace detail

/** Directory is an index into the directory of offer books.
    The last 64 bits of this are the quality. */
using Directory = base_uint<256, detail::DirectoryTag>;

/** Currency is a hash representing a specific currency. */
using Currency = base_uint<160, detail::CurrencyTag>;

/** NodeID is a 160-bit hash representing one node. */
using NodeID = base_uint<160, detail::NodeIDTag>;

/** MUSO currency. */
Currency const&
MUSOCurrency();

/** A placeholder for empty currencies. */
Currency const&
noCurrency();

/** We deliberately disallow the currency that looks like "MUSO" because too
    many people were using it instead of the correct MUSO currency. */
Currency const&
badCurrency();

inline bool
isMUSO(Currency const& c)
{
    return c == beast::zero;
}

/** Returns "", "MUSO", or three letter ISO code. */
std::string
to_string(Currency const& c);

/** Tries to convert a string to a Currency, returns true on success.

    @note This function will return success if the resulting currency is
          badCurrency(). This legacy behavior is unfortunate; changing this
          will require very careful checking everywhere and may mean having
          to rewrite some unit test code.
*/
bool
to_currency(Currency&, std::string const&);

/** Tries to convert a string to a Currency, returns noCurrency() on failure.

    @note This function can return badCurrency(). This legacy behavior is
          unfortunate; changing this will require very careful checking
          everywhere and may mean having to rewrite some unit test code.
*/
Currency
to_currency(std::string const&);

inline std::ostream&
operator<<(std::ostream& os, Currency const& x)
{
    os << to_string(x);
    return os;
}

}  // namespace MUSO

namespace std {

template <>
struct hash<MUSO::Currency> : MUSO::Currency::hasher
{
    explicit hash() = default;
};

template <>
struct hash<MUSO::NodeID> : MUSO::NodeID::hasher
{
    explicit hash() = default;
};

template <>
struct hash<MUSO::Directory> : MUSO::Directory::hasher
{
    explicit hash() = default;
};

}  // namespace std

#endif
