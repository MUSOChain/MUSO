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

#ifndef MUSO_TX_CHANGE_H_INCLUDED
#define MUSO_TX_CHANGE_H_INCLUDED

#include <MUSO/app/main/Application.h>
#include <MUSO/app/misc/AmendmentTable.h>
#include <MUSO/app/misc/NetworkOPs.h>
#include <MUSO/app/tx/impl/Transactor.h>
#include <MUSO/basics/Log.h>
#include <MUSO/protocol/Indexes.h>

namespace MUSO {

class Change : public Transactor
{
public:
    static constexpr ConsequencesFactoryType ConsequencesFactory{Normal};

    explicit Change(ApplyContext& ctx) : Transactor(ctx)
    {
    }

    static NotTEC
    preflight(PreflightContext const& ctx);

    TER
    doApply() override;
    void
    preCompute() override;

    static FeeUnit64
    calculateBaseFee(ReadView const& view, STTx const& tx)
    {
        return FeeUnit64{0};
    }

    static TER
    preclaim(PreclaimContext const& ctx);

private:
    TER
    applyAmendment();

    TER
    applyFee();

    TER
    applyUNLModify();
};

}  // namespace MUSO

#endif