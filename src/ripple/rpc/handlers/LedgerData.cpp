//------------------------------------------------------------------------------
/*
    This file is part of MUSO: https://github.com/MUSO/MUSO
    Copyright (c) 2012-2014 MUSO Labs Inc.

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

#include <MUSO/app/ledger/LedgerToJson.h>
#include <MUSO/ledger/ReadView.h>
#include <MUSO/protocol/ErrorCodes.h>
#include <MUSO/protocol/LedgerFormats.h>
#include <MUSO/protocol/jss.h>
#include <MUSO/rpc/Context.h>
#include <MUSO/rpc/GRPCHandlers.h>
#include <MUSO/rpc/Role.h>
#include <MUSO/rpc/impl/GRPCHelpers.h>
#include <MUSO/rpc/impl/RPCHelpers.h>
#include <MUSO/rpc/impl/Tuning.h>

namespace MUSO {

// Get state nodes from a ledger
//   Inputs:
//     limit:        integer, maximum number of entries
//     marker:       opaque, resume point
//     binary:       boolean, format
//     type:         string // optional, defaults to all ledger node types
//   Outputs:
//     ledger_hash:  chosen ledger's hash
//     ledger_index: chosen ledger's index
//     state:        array of state nodes
//     marker:       resume point, if any
Json::Value
doLedgerData(RPC::JsonContext& context)
{
    std::shared_ptr<ReadView const> lpLedger;
    auto const& params = context.params;

    auto jvResult = RPC::lookupLedger(lpLedger, context);
    if (!lpLedger)
        return jvResult;

    bool const isMarker = params.isMember(jss::marker);
    ReadView::key_type key = ReadView::key_type();
    if (isMarker)
    {
        Json::Value const& jMarker = params[jss::marker];
        if (!(jMarker.isString() && key.parseHex(jMarker.asString())))
            return RPC::expected_field_error(jss::marker, "valid");
    }

    bool const isBinary = params[jss::binary].asBool();

    int limit = -1;
    if (params.isMember(jss::limit))
    {
        Json::Value const& jLimit = params[jss::limit];
        if (!jLimit.isIntegral())
            return RPC::expected_field_error(jss::limit, "integer");

        limit = jLimit.asInt();
    }

    auto maxLimit = RPC::Tuning::pageLength(isBinary);
    if ((limit < 0) || ((limit > maxLimit) && (!isUnlimited(context.role))))
        limit = maxLimit;

    jvResult[jss::ledger_hash] = to_string(lpLedger->info().hash);
    jvResult[jss::ledger_index] = lpLedger->info().seq;

    if (!isMarker)
    {
        // Return base ledger data on first query
        jvResult[jss::ledger] = getJson(LedgerFill(
            *lpLedger, &context, isBinary ? LedgerFill::Options::binary : 0));
    }

    auto [rpcStatus, type] = RPC::chooseLedgerEntryType(params);
    if (rpcStatus)
    {
        jvResult.clear();
        rpcStatus.inject(jvResult);
        return jvResult;
    }
    Json::Value& nodes = jvResult[jss::state];

    auto e = lpLedger->sles.end();
    for (auto i = lpLedger->sles.upper_bound(key); i != e; ++i)
    {
        auto sle = lpLedger->read(keylet::unchecked((*i)->key()));
        if (limit-- <= 0)
        {
            // Stop processing before the current key.
            auto k = sle->key();
            jvResult[jss::marker] = to_string(--k);
            break;
        }

        if (type == ltINVALID || sle->getType() == type)
        {
            if (isBinary)
            {
                Json::Value& entry = nodes.append(Json::objectValue);
                entry[jss::data] = serializeHex(*sle);
                entry[jss::index] = to_string(sle->key());
            }
            else
            {
                Json::Value& entry =
                    nodes.append(sle->getJson(JsonOptions::none));
                entry[jss::index] = to_string(sle->key());
            }
        }
    }

    return jvResult;
}

std::pair<org::MUSOl::rpc::v1::GetLedgerDataResponse, grpc::Status>
doLedgerDataGrpc(
    RPC::GRPCContext<org::MUSOl::rpc::v1::GetLedgerDataRequest>& context)
{
    org::MUSOl::rpc::v1::GetLedgerDataRequest& request = context.params;
    org::MUSOl::rpc::v1::GetLedgerDataResponse response;
    grpc::Status status = grpc::Status::OK;

    std::shared_ptr<ReadView const> ledger;
    if (RPC::ledgerFromRequest(ledger, context))
    {
        grpc::Status errorStatus{
            grpc::StatusCode::NOT_FOUND, "ledger not found"};
        return {response, errorStatus};
    }

    ReadView::key_type key = ReadView::key_type();
    if (request.marker().size() != 0)
    {
        key = uint256::fromVoid(request.marker().data());
        if (key.size() != request.marker().size())
        {
            grpc::Status errorStatus{
                grpc::StatusCode::INVALID_ARGUMENT, "marker malformed"};
            return {response, errorStatus};
        }
    }

    auto e = ledger->sles.end();
    ReadView::key_type stopKey = ReadView::key_type();
    if (request.end_marker().size() != 0)
    {
        stopKey = uint256::fromVoid(request.end_marker().data());
        if (stopKey.size() != request.marker().size())
        {
            grpc::Status errorStatus{
                grpc::StatusCode::INVALID_ARGUMENT, "end marker malformed"};
            return {response, errorStatus};
        }
        e = ledger->sles.upper_bound(stopKey);
    }

    int maxLimit = RPC::Tuning::pageLength(true);

    for (auto i = ledger->sles.upper_bound(key); i != e; ++i)
    {
        auto sle = ledger->read(keylet::unchecked((*i)->key()));
        if (maxLimit-- <= 0)
        {
            // Stop processing before the current key.
            auto k = sle->key();
            --k;
            response.set_marker(k.data(), k.size());
            break;
        }
        auto stateObject = response.mutable_ledger_objects()->add_objects();
        Serializer s;
        sle->add(s);
        stateObject->set_data(s.peekData().data(), s.getLength());
        stateObject->set_key(sle->key().data(), sle->key().size());
    }
    return {response, status};
}

}  // namespace MUSO
