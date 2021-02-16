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

#ifndef MUSO_PROTOCOL_BOOK_H_INCLUDED
#define MUSO_PROTOCOL_BOOK_H_INCLUDED

#include <MUSO/protocol/Issue.h>
#include <boost/utility/base_from_member.hpp>

namespace MUSO {

/** Specifies an order book.
    The order book is a pair of Issues called in and out.
    @see Issue.
*/
class Book
{
public:
    Issue in;
    Issue out;

    Book()
    {
    }

    Book(Issue const& in_, Issue const& out_) : in(in_), out(out_)
    {
    }
};

bool
isConsistent(Book const& book);

std::string
to_string(Book const& book);

std::ostream&
operator<<(std::ostream& os, Book const& x);

template <class Hasher>
void
hash_append(Hasher& h, Book const& b)
{
    using beast::hash_append;
    hash_append(h, b.in, b.out);
}

Book
reversed(Book const& book);

/** Ordered comparison. */
int
compare(Book const& lhs, Book const& rhs);

/** Equality comparison. */
/** @{ */
bool
operator==(Book const& lhs, Book const& rhs);
bool
operator!=(Book const& lhs, Book const& rhs);
/** @} */

/** Strict weak ordering. */
/** @{ */
bool
operator<(Book const& lhs, Book const& rhs);
bool
operator>(Book const& lhs, Book const& rhs);
bool
operator>=(Book const& lhs, Book const& rhs);
bool
operator<=(Book const& lhs, Book const& rhs);
/** @} */

}  // namespace MUSO

//------------------------------------------------------------------------------

namespace std {

template <>
struct hash<MUSO::Issue>
    : private boost::base_from_member<std::hash<MUSO::Currency>, 0>,
      private boost::base_from_member<std::hash<MUSO::AccountID>, 1>
{
private:
    using currency_hash_type =
        boost::base_from_member<std::hash<MUSO::Currency>, 0>;
    using issuer_hash_type =
        boost::base_from_member<std::hash<MUSO::AccountID>, 1>;

public:
    explicit hash() = default;

    using value_type = std::size_t;
    using argument_type = MUSO::Issue;

    value_type
    operator()(argument_type const& value) const
    {
        value_type result(currency_hash_type::member(value.currency));
        if (!isMUSO(value.currency))
            boost::hash_combine(
                result, issuer_hash_type::member(value.account));
        return result;
    }
};

//------------------------------------------------------------------------------

template <>
struct hash<MUSO::Book>
{
private:
    using hasher = std::hash<MUSO::Issue>;

    hasher m_hasher;

public:
    explicit hash() = default;

    using value_type = std::size_t;
    using argument_type = MUSO::Book;

    value_type
    operator()(argument_type const& value) const
    {
        value_type result(m_hasher(value.in));
        boost::hash_combine(result, m_hasher(value.out));
        return result;
    }
};

}  // namespace std

//------------------------------------------------------------------------------

namespace boost {

template <>
struct hash<MUSO::Issue> : std::hash<MUSO::Issue>
{
    explicit hash() = default;

    using Base = std::hash<MUSO::Issue>;
    // VFALCO NOTE broken in vs2012
    // using Base::Base; // inherit ctors
};

template <>
struct hash<MUSO::Book> : std::hash<MUSO::Book>
{
    explicit hash() = default;

    using Base = std::hash<MUSO::Book>;
    // VFALCO NOTE broken in vs2012
    // using Base::Base; // inherit ctors
};

}  // namespace boost

#endif
