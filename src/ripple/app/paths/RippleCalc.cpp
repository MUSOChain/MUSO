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

#include <MUSO/app/paths/Flow.h>
#include <MUSO/app/paths/MUSOCalc.h>
#include <MUSO/app/paths/Tuning.h>
#include <MUSO/app/paths/impl/FlowDebugInfo.h>
#include <MUSO/basics/Log.h>
#include <MUSO/ledger/View.h>
#include <MUSO/protocol/Feature.h>

namespace MUSO {
namespace path {

MUSOCalc::Output
MUSOCalc::MUSOCalculate(
    PaymentSandbox& view,

    // Compute paths using this ledger entry set.  Up to caller to actually
    // apply to ledger.

    // Issuer:
    //      MUSO: MUSOAccount()
    //  non-MUSO: uSrcAccountID (for any issuer) or another account with
    //           trust node.
    STAmount const& saMaxAmountReq,  // --> -1 = no limit.

    // Issuer:
    //      MUSO: MUSOAccount()
    //  non-MUSO: uDstAccountID (for any issuer) or another account with
    //           trust node.
    STAmount const& saDstAmountReq,

    AccountID const& uDstAccountID,
    AccountID const& uSrcAccountID,

    // A set of paths that are included in the transaction that we'll
    // explore for liquidity.
    STPathSet const& spsPaths,
    Logs& l,
    Input const* const pInputs)
{
    Output flowOut;
    PaymentSandbox flowSB(&view);
    auto j = l.journal("Flow");

    if (!view.rules().enabled(featureFlow))
    {
        // The new payment engine was enabled several years ago. New transaction
        // should never use the old rules. Assume this is a replay
        j.fatal()
            << "Old payment rules are required for this transaction. Assuming "
               "this is a replay and running with the new rules.";
    }

    {
        bool const defaultPaths =
            !pInputs ? true : pInputs->defaultPathsAllowed;

        bool const partialPayment =
            !pInputs ? false : pInputs->partialPaymentAllowed;

        auto const limitQuality = [&]() -> boost::optional<Quality> {
            if (pInputs && pInputs->limitQuality &&
                saMaxAmountReq > beast::zero)
                return Quality{Amounts(saMaxAmountReq, saDstAmountReq)};
            return boost::none;
        }();

        auto const sendMax = [&]() -> boost::optional<STAmount> {
            if (saMaxAmountReq >= beast::zero ||
                saMaxAmountReq.getCurrency() != saDstAmountReq.getCurrency() ||
                saMaxAmountReq.getIssuer() != uSrcAccountID)
            {
                return saMaxAmountReq;
            }
            return boost::none;
        }();

        bool const ownerPaysTransferFee =
            view.rules().enabled(featureOwnerPaysFee);

        try
        {
            flowOut = flow(
                flowSB,
                saDstAmountReq,
                uSrcAccountID,
                uDstAccountID,
                spsPaths,
                defaultPaths,
                partialPayment,
                ownerPaysTransferFee,
                /* offerCrossing */ false,
                limitQuality,
                sendMax,
                j,
                nullptr);
        }
        catch (std::exception& e)
        {
            JLOG(j.error()) << "Exception from flow: " << e.what();

            // return a tec so the tx is stored
            path::MUSOCalc::Output exceptResult;
            exceptResult.setResult(tecINTERNAL);
            return exceptResult;
        }
    }

    j.debug() << "MUSOCalc Result> "
              << " actualIn: " << flowOut.actualAmountIn
              << ", actualOut: " << flowOut.actualAmountOut
              << ", result: " << flowOut.result()
              << ", dstAmtReq: " << saDstAmountReq
              << ", sendMax: " << saMaxAmountReq;

    flowSB.apply(view);
    return flowOut;
}

}  // namespace path
}  // namespace MUSO
