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

#ifndef MUSO_LEDGER_SANDBOX_H_INCLUDED
#define MUSO_LEDGER_SANDBOX_H_INCLUDED

#include <MUSO/ledger/RawView.h>
#include <MUSO/ledger/detail/ApplyViewBase.h>

namespace MUSO {

/** Discardable, editable view to a ledger.

    The sandbox inherits the flags of the base.

    @note Presented as ApplyView to clients.
*/
class Sandbox : public detail::ApplyViewBase
{
public:
    Sandbox() = delete;
    Sandbox(Sandbox const&) = delete;
    Sandbox&
    operator=(Sandbox&&) = delete;
    Sandbox&
    operator=(Sandbox const&) = delete;

    Sandbox(Sandbox&&) = default;

    Sandbox(ReadView const* base, ApplyFlags flags) : ApplyViewBase(base, flags)
    {
    }

    Sandbox(ApplyView const* base) : Sandbox(base, base->flags())
    {
    }

    void
    apply(RawView& to)
    {
        items_.apply(to);
    }
};

}  // namespace MUSO

#endif
