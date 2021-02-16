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

#ifndef MUSO_PATH_IMPL_AMOUNTSPEC_H_INCLUDED
#define MUSO_PATH_IMPL_AMOUNTSPEC_H_INCLUDED

#include <MUSO/basics/IOUAmount.h>
#include <MUSO/basics/MUSOAmount.h>
#include <MUSO/protocol/STAmount.h>

namespace MUSO {

struct AmountSpec
{
    explicit AmountSpec() = default;

    bool native;
    union
    {
        MUSOAmount MUSO;
        IOUAmount iou;
    };
    boost::optional<AccountID> issuer;
    boost::optional<Currency> currency;

    friend std::ostream&
    operator<<(std::ostream& stream, AmountSpec const& amt)
    {
        if (amt.native)
            stream << to_string(amt.MUSO);
        else
            stream << to_string(amt.iou);
        if (amt.currency)
            stream << "/(" << *amt.currency << ")";
        if (amt.issuer)
            stream << "/" << *amt.issuer << "";
        return stream;
    }
};

struct EitherAmount
{
#ifndef NDEBUG
    bool native = false;
#endif

    union
    {
        IOUAmount iou;
        MUSOAmount MUSO;
    };

    EitherAmount() = default;

    explicit EitherAmount(IOUAmount const& a) : iou(a)
    {
    }

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
    // ignore warning about half of iou amount being uninitialized
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
    explicit EitherAmount(MUSOAmount const& a) : MUSO(a)
    {
#ifndef NDEBUG
        native = true;
#endif
    }
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif

    explicit EitherAmount(AmountSpec const& a)
    {
#ifndef NDEBUG
        native = a.native;
#endif
        if (a.native)
            MUSO = a.MUSO;
        else
            iou = a.iou;
    }

#ifndef NDEBUG
    friend std::ostream&
    operator<<(std::ostream& stream, EitherAmount const& amt)
    {
        if (amt.native)
            stream << to_string(amt.MUSO);
        else
            stream << to_string(amt.iou);
        return stream;
    }
#endif
};

template <class T>
T&
get(EitherAmount& amt)
{
    static_assert(sizeof(T) == -1, "Must used specialized function");
    return T(0);
}

template <>
inline IOUAmount&
get<IOUAmount>(EitherAmount& amt)
{
    assert(!amt.native);
    return amt.iou;
}

template <>
inline MUSOAmount&
get<MUSOAmount>(EitherAmount& amt)
{
    assert(amt.native);
    return amt.MUSO;
}

template <class T>
T const&
get(EitherAmount const& amt)
{
    static_assert(sizeof(T) == -1, "Must used specialized function");
    return T(0);
}

template <>
inline IOUAmount const&
get<IOUAmount>(EitherAmount const& amt)
{
    assert(!amt.native);
    return amt.iou;
}

template <>
inline MUSOAmount const&
get<MUSOAmount>(EitherAmount const& amt)
{
    assert(amt.native);
    return amt.MUSO;
}

inline AmountSpec
toAmountSpec(STAmount const& amt)
{
    assert(amt.mantissa() < std::numeric_limits<std::int64_t>::max());
    bool const isNeg = amt.negative();
    std::int64_t const sMant =
        isNeg ? -std::int64_t(amt.mantissa()) : amt.mantissa();
    AmountSpec result;

    result.native = isMUSO(amt);
    if (result.native)
    {
        result.MUSO = MUSOAmount(sMant);
    }
    else
    {
        result.iou = IOUAmount(sMant, amt.exponent());
        result.issuer = amt.issue().account;
        result.currency = amt.issue().currency;
    }

    return result;
}

inline EitherAmount
toEitherAmount(STAmount const& amt)
{
    if (isMUSO(amt))
        return EitherAmount{amt.MUSO()};
    return EitherAmount{amt.iou()};
}

inline AmountSpec
toAmountSpec(EitherAmount const& ea, boost::optional<Currency> const& c)
{
    AmountSpec r;
    r.native = (!c || isMUSO(*c));
    r.currency = c;
    assert(ea.native == r.native);
    if (r.native)
    {
        r.MUSO = ea.MUSO;
    }
    else
    {
        r.iou = ea.iou;
    }
    return r;
}

}  // namespace MUSO

#endif
