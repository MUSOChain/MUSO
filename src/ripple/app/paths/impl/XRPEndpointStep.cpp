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

#include <MUSO/app/paths/Credit.h>
#include <MUSO/app/paths/impl/AmountSpec.h>
#include <MUSO/app/paths/impl/StepChecks.h>
#include <MUSO/app/paths/impl/Steps.h>
#include <MUSO/basics/IOUAmount.h>
#include <MUSO/basics/Log.h>
#include <MUSO/basics/MUSOAmount.h>
#include <MUSO/ledger/PaymentSandbox.h>
#include <MUSO/protocol/Feature.h>
#include <MUSO/protocol/Quality.h>

#include <boost/container/flat_set.hpp>

#include <numeric>
#include <sstream>

namespace MUSO {

template <class TDerived>
class MUSOEndpointStep
    : public StepImp<MUSOAmount, MUSOAmount, MUSOEndpointStep<TDerived>>
{
private:
    AccountID acc_;
    bool const isLast_;
    beast::Journal const j_;

    // Since this step will always be an endpoint in a strand
    // (either the first or last step) the same cache is used
    // for cachedIn and cachedOut and only one will ever be used
    boost::optional<MUSOAmount> cache_;

    boost::optional<EitherAmount>
    cached() const
    {
        if (!cache_)
            return boost::none;
        return EitherAmount(*cache_);
    }

public:
    MUSOEndpointStep(StrandContext const& ctx, AccountID const& acc)
        : acc_(acc), isLast_(ctx.isLast), j_(ctx.j)
    {
    }

    AccountID const&
    acc() const
    {
        return acc_;
    }

    boost::optional<std::pair<AccountID, AccountID>>
    directStepAccts() const override
    {
        if (isLast_)
            return std::make_pair(MUSOAccount(), acc_);
        return std::make_pair(acc_, MUSOAccount());
    }

    boost::optional<EitherAmount>
    cachedIn() const override
    {
        return cached();
    }

    boost::optional<EitherAmount>
    cachedOut() const override
    {
        return cached();
    }

    DebtDirection
    debtDirection(ReadView const& sb, StrandDirection dir) const override
    {
        return DebtDirection::issues;
    }

    std::pair<boost::optional<Quality>, DebtDirection>
    qualityUpperBound(ReadView const& v, DebtDirection prevStepDir)
        const override;

    std::pair<MUSOAmount, MUSOAmount>
    revImp(
        PaymentSandbox& sb,
        ApplyView& afView,
        boost::container::flat_set<uint256>& ofrsToRm,
        MUSOAmount const& out);

    std::pair<MUSOAmount, MUSOAmount>
    fwdImp(
        PaymentSandbox& sb,
        ApplyView& afView,
        boost::container::flat_set<uint256>& ofrsToRm,
        MUSOAmount const& in);

    std::pair<bool, EitherAmount>
    validFwd(PaymentSandbox& sb, ApplyView& afView, EitherAmount const& in)
        override;

    // Check for errors and violations of frozen constraints.
    TER
    check(StrandContext const& ctx) const;

protected:
    MUSOAmount
    MUSOLiquidImpl(ReadView& sb, std::int32_t reserveReduction) const
    {
        return MUSO::MUSOLiquid(sb, acc_, reserveReduction, j_);
    }

    std::string
    logStringImpl(char const* name) const
    {
        std::ostringstream ostr;
        ostr << name << ": "
             << "\nAcc: " << acc_;
        return ostr.str();
    }

private:
    template <class P>
    friend bool
    operator==(MUSOEndpointStep<P> const& lhs, MUSOEndpointStep<P> const& rhs);

    friend bool
    operator!=(MUSOEndpointStep const& lhs, MUSOEndpointStep const& rhs)
    {
        return !(lhs == rhs);
    }

    bool
    equal(Step const& rhs) const override
    {
        if (auto ds = dynamic_cast<MUSOEndpointStep const*>(&rhs))
        {
            return *this == *ds;
        }
        return false;
    }
};

//------------------------------------------------------------------------------

// Flow is used in two different circumstances for transferring funds:
//  o Payments, and
//  o Offer crossing.
// The rules for handling funds in these two cases are almost, but not
// quite, the same.

// Payment MUSOEndpointStep class (not offer crossing).
class MUSOEndpointPaymentStep : public MUSOEndpointStep<MUSOEndpointPaymentStep>
{
public:
    using MUSOEndpointStep<MUSOEndpointPaymentStep>::MUSOEndpointStep;

    MUSOAmount
    MUSOLiquid(ReadView& sb) const
    {
        return MUSOLiquidImpl(sb, 0);
        ;
    }

    std::string
    logString() const override
    {
        return logStringImpl("MUSOEndpointPaymentStep");
    }
};

// Offer crossing MUSOEndpointStep class (not a payment).
class MUSOEndpointOfferCrossingStep
    : public MUSOEndpointStep<MUSOEndpointOfferCrossingStep>
{
private:
    // For historical reasons, offer crossing is allowed to dig further
    // into the MUSO reserve than an ordinary payment.  (I believe it's
    // because the trust line was created after the MUSO was removed.)
    // Return how much the reserve should be reduced.
    //
    // Note that reduced reserve only happens if the trust line does not
    // currently exist.
    static std::int32_t
    computeReserveReduction(StrandContext const& ctx, AccountID const& acc)
    {
        if (ctx.isFirst && !ctx.view.read(keylet::line(acc, ctx.strandDeliver)))
            return -1;
        return 0;
    }

public:
    MUSOEndpointOfferCrossingStep(StrandContext const& ctx, AccountID const& acc)
        : MUSOEndpointStep<MUSOEndpointOfferCrossingStep>(ctx, acc)
        , reserveReduction_(computeReserveReduction(ctx, acc))
    {
    }

    MUSOAmount
    MUSOLiquid(ReadView& sb) const
    {
        return MUSOLiquidImpl(sb, reserveReduction_);
    }

    std::string
    logString() const override
    {
        return logStringImpl("MUSOEndpointOfferCrossingStep");
    }

private:
    std::int32_t const reserveReduction_;
};

//------------------------------------------------------------------------------

template <class TDerived>
inline bool
operator==(
    MUSOEndpointStep<TDerived> const& lhs,
    MUSOEndpointStep<TDerived> const& rhs)
{
    return lhs.acc_ == rhs.acc_ && lhs.isLast_ == rhs.isLast_;
}

template <class TDerived>
std::pair<boost::optional<Quality>, DebtDirection>
MUSOEndpointStep<TDerived>::qualityUpperBound(
    ReadView const& v,
    DebtDirection prevStepDir) const
{
    return {
        Quality{STAmount::uRateOne},
        this->debtDirection(v, StrandDirection::forward)};
}

template <class TDerived>
std::pair<MUSOAmount, MUSOAmount>
MUSOEndpointStep<TDerived>::revImp(
    PaymentSandbox& sb,
    ApplyView& afView,
    boost::container::flat_set<uint256>& ofrsToRm,
    MUSOAmount const& out)
{
    auto const balance = static_cast<TDerived const*>(this)->MUSOLiquid(sb);

    auto const result = isLast_ ? out : std::min(balance, out);

    auto& sender = isLast_ ? MUSOAccount() : acc_;
    auto& receiver = isLast_ ? acc_ : MUSOAccount();
    auto ter = accountSend(sb, sender, receiver, toSTAmount(result), j_);
    if (ter != tesSUCCESS)
        return {MUSOAmount{beast::zero}, MUSOAmount{beast::zero}};

    cache_.emplace(result);
    return {result, result};
}

template <class TDerived>
std::pair<MUSOAmount, MUSOAmount>
MUSOEndpointStep<TDerived>::fwdImp(
    PaymentSandbox& sb,
    ApplyView& afView,
    boost::container::flat_set<uint256>& ofrsToRm,
    MUSOAmount const& in)
{
    assert(cache_);
    auto const balance = static_cast<TDerived const*>(this)->MUSOLiquid(sb);

    auto const result = isLast_ ? in : std::min(balance, in);

    auto& sender = isLast_ ? MUSOAccount() : acc_;
    auto& receiver = isLast_ ? acc_ : MUSOAccount();
    auto ter = accountSend(sb, sender, receiver, toSTAmount(result), j_);
    if (ter != tesSUCCESS)
        return {MUSOAmount{beast::zero}, MUSOAmount{beast::zero}};

    cache_.emplace(result);
    return {result, result};
}

template <class TDerived>
std::pair<bool, EitherAmount>
MUSOEndpointStep<TDerived>::validFwd(
    PaymentSandbox& sb,
    ApplyView& afView,
    EitherAmount const& in)
{
    if (!cache_)
    {
        JLOG(j_.error()) << "Expected valid cache in validFwd";
        return {false, EitherAmount(MUSOAmount(beast::zero))};
    }

    assert(in.native);

    auto const& MUSOIn = in.MUSO;
    auto const balance = static_cast<TDerived const*>(this)->MUSOLiquid(sb);

    if (!isLast_ && balance < MUSOIn)
    {
        JLOG(j_.warn()) << "MUSOEndpointStep: Strand re-execute check failed."
                        << " Insufficient balance: " << to_string(balance)
                        << " Requested: " << to_string(MUSOIn);
        return {false, EitherAmount(balance)};
    }

    if (MUSOIn != *cache_)
    {
        JLOG(j_.warn()) << "MUSOEndpointStep: Strand re-execute check failed."
                        << " ExpectedIn: " << to_string(*cache_)
                        << " CachedIn: " << to_string(MUSOIn);
    }
    return {true, in};
}

template <class TDerived>
TER
MUSOEndpointStep<TDerived>::check(StrandContext const& ctx) const
{
    if (!acc_)
    {
        JLOG(j_.debug()) << "MUSOEndpointStep: specified bad account.";
        return temBAD_PATH;
    }

    auto sleAcc = ctx.view.read(keylet::account(acc_));
    if (!sleAcc)
    {
        JLOG(j_.warn()) << "MUSOEndpointStep: can't send or receive MUSO from "
                           "non-existent account: "
                        << acc_;
        return terNO_ACCOUNT;
    }

    if (!ctx.isFirst && !ctx.isLast)
    {
        return temBAD_PATH;
    }

    auto& src = isLast_ ? MUSOAccount() : acc_;
    auto& dst = isLast_ ? acc_ : MUSOAccount();
    auto ter = checkFreeze(ctx.view, src, dst, MUSOCurrency());
    if (ter != tesSUCCESS)
        return ter;

    if (ctx.view.rules().enabled(fix1781))
    {
        auto const issuesIndex = isLast_ ? 0 : 1;
        if (!ctx.seenDirectIssues[issuesIndex].insert(MUSOIssue()).second)
        {
            JLOG(j_.debug())
                << "MUSOEndpointStep: loop detected: Index: " << ctx.strandSize
                << ' ' << *this;
            return temBAD_PATH_LOOP;
        }
    }

    return tesSUCCESS;
}

//------------------------------------------------------------------------------

namespace test {
// Needed for testing
bool
MUSOEndpointStepEqual(Step const& step, AccountID const& acc)
{
    if (auto xs =
            dynamic_cast<MUSOEndpointStep<MUSOEndpointPaymentStep> const*>(&step))
    {
        return xs->acc() == acc;
    }
    return false;
}
}  // namespace test

//------------------------------------------------------------------------------

std::pair<TER, std::unique_ptr<Step>>
make_MUSOEndpointStep(StrandContext const& ctx, AccountID const& acc)
{
    TER ter = tefINTERNAL;
    std::unique_ptr<Step> r;
    if (ctx.offerCrossing)
    {
        auto offerCrossingStep =
            std::make_unique<MUSOEndpointOfferCrossingStep>(ctx, acc);
        ter = offerCrossingStep->check(ctx);
        r = std::move(offerCrossingStep);
    }
    else  // payment
    {
        auto paymentStep = std::make_unique<MUSOEndpointPaymentStep>(ctx, acc);
        ter = paymentStep->check(ctx);
        r = std::move(paymentStep);
    }
    if (ter != tesSUCCESS)
        return {ter, nullptr};

    return {tesSUCCESS, std::move(r)};
}

}  // namespace MUSO
