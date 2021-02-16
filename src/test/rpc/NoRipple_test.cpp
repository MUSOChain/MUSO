//------------------------------------------------------------------------------
/*
    This file is part of MUSO: https://github.com/MUSO/MUSO
    Copyright (c) 2016 MUSO Labs Inc.

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

#include <MUSO/protocol/Feature.h>
#include <MUSO/protocol/jss.h>
#include <test/jtx.h>

namespace MUSO {

namespace test {

class NoMUSO_test : public beast::unit_test::suite
{
public:
    void
    testSetAndClear()
    {
        testcase("Set and clear noMUSO");

        using namespace jtx;
        Env env(*this);

        auto const gw = Account("gateway");
        auto const alice = Account("alice");

        env.fund(MUSO(10000), gw, alice);

        auto const USD = gw["USD"];

        Json::Value account_gw;
        account_gw[jss::account] = gw.human();
        Json::Value account_alice;
        account_alice[jss::account] = alice.human();

        for (auto SetOrClear : {true, false})
        {
            // Create a trust line with no-MUSO flag setting
            env(trust(
                gw,
                USD(100),
                alice,
                SetOrClear ? tfSetNoMUSO : tfClearNoMUSO));
            env.close();

            // Check no-MUSO flag on sender 'gateway'
            Json::Value lines{
                env.rpc("json", "account_lines", to_string(account_gw))};
            auto const& gline0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(gline0[jss::no_MUSO].asBool() == SetOrClear);

            // Check no-MUSO peer flag on destination 'alice'
            lines = env.rpc("json", "account_lines", to_string(account_alice));
            auto const& aline0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(aline0[jss::no_MUSO_peer].asBool() == SetOrClear);
        }
    }

    void
    testNegativeBalance(FeatureBitset features)
    {
        testcase("Set noMUSO on a line with negative balance");

        using namespace jtx;
        auto const gw = Account("gateway");
        auto const alice = Account("alice");
        auto const bob = Account("bob");
        auto const carol = Account("carol");

        // fix1578 changes the return code.  Verify expected behavior
        // without and with fix1578.
        for (auto const tweakedFeatures :
             {features - fix1578, features | fix1578})
        {
            Env env(*this, tweakedFeatures);

            env.fund(MUSO(10000), gw, alice, bob, carol);
            env.close();

            env.trust(alice["USD"](100), bob);
            env.trust(bob["USD"](100), carol);
            env.close();

            // After this payment alice has a -50 USD balance with bob, and
            // bob has a -50 USD balance with carol.  So neither alice nor
            // bob should be able to clear the noMUSO flag.
            env(pay(alice, carol, carol["USD"](50)), path(bob));
            env.close();

            TER const terNeg{
                tweakedFeatures[fix1578] ? TER{tecNO_PERMISSION}
                                         : TER{tesSUCCESS}};

            env(trust(alice, bob["USD"](100), bob, tfSetNoMUSO), ter(terNeg));
            env(trust(bob, carol["USD"](100), carol, tfSetNoMUSO),
                ter(terNeg));
            env.close();

            Json::Value params;
            params[jss::source_account] = alice.human();
            params[jss::destination_account] = carol.human();
            params[jss::destination_amount] = [] {
                Json::Value dest_amt;
                dest_amt[jss::currency] = "USD";
                dest_amt[jss::value] = "1";
                dest_amt[jss::issuer] = Account("carol").human();
                return dest_amt;
            }();

            auto const resp =
                env.rpc("json", "MUSO_path_find", to_string(params));
            BEAST_EXPECT(resp[jss::result][jss::alternatives].size() == 1);

            auto getAccountLines = [&env](Account const& acct) {
                Json::Value jv;
                jv[jss::account] = acct.human();
                auto const r = env.rpc("json", "account_lines", to_string(jv));
                return r[jss::result][jss::lines];
            };
            {
                auto const aliceLines = getAccountLines(alice);
                BEAST_EXPECT(aliceLines.size() == 1);
                BEAST_EXPECT(aliceLines[0u][jss::no_MUSO].asBool() == false);

                auto const bobLines = getAccountLines(bob);
                BEAST_EXPECT(bobLines.size() == 2);
                BEAST_EXPECT(bobLines[0u][jss::no_MUSO].asBool() == false);
                BEAST_EXPECT(bobLines[1u][jss::no_MUSO].asBool() == false);
            }

            // Now carol sends the 50 USD back to alice.  Then alice and
            // bob can set the noMUSO flag.
            env(pay(carol, alice, alice["USD"](50)), path(bob));
            env.close();

            env(trust(alice, bob["USD"](100), bob, tfSetNoMUSO));
            env(trust(bob, carol["USD"](100), carol, tfSetNoMUSO));
            env.close();
            {
                auto const aliceLines = getAccountLines(alice);
                BEAST_EXPECT(aliceLines.size() == 1);
                BEAST_EXPECT(aliceLines[0u].isMember(jss::no_MUSO));

                auto const bobLines = getAccountLines(bob);
                BEAST_EXPECT(bobLines.size() == 2);
                BEAST_EXPECT(bobLines[0u].isMember(jss::no_MUSO_peer));
                BEAST_EXPECT(bobLines[1u].isMember(jss::no_MUSO));
            }
        }
    }

    void
    testPairwise(FeatureBitset features)
    {
        testcase("pairwise NoMUSO");

        using namespace jtx;
        Env env(*this, features);

        auto const alice = Account("alice");
        auto const bob = Account("bob");
        auto const carol = Account("carol");

        env.fund(MUSO(10000), alice, bob, carol);

        env(trust(bob, alice["USD"](100)));
        env(trust(carol, bob["USD"](100)));

        env(trust(bob, alice["USD"](100), alice, tfSetNoMUSO));
        env(trust(bob, carol["USD"](100), carol, tfSetNoMUSO));
        env.close();

        Json::Value params;
        params[jss::source_account] = alice.human();
        params[jss::destination_account] = carol.human();
        params[jss::destination_amount] = [] {
            Json::Value dest_amt;
            dest_amt[jss::currency] = "USD";
            dest_amt[jss::value] = "1";
            dest_amt[jss::issuer] = Account("carol").human();
            return dest_amt;
        }();

        Json::Value const resp{
            env.rpc("json", "MUSO_path_find", to_string(params))};
        BEAST_EXPECT(resp[jss::result][jss::alternatives].size() == 0);

        env(pay(alice, carol, bob["USD"](50)), ter(tecPATH_DRY));
    }

    void
    testDefaultMUSO(FeatureBitset features)
    {
        testcase("Set default MUSO on an account and check new trustlines");

        using namespace jtx;
        Env env(*this, features);

        auto const gw = Account("gateway");
        auto const alice = Account("alice");
        auto const bob = Account("bob");

        env.fund(MUSO(10000), gw, noMUSO(alice, bob));

        env(fset(bob, asfDefaultMUSO));

        auto const USD = gw["USD"];

        env(trust(gw, USD(100), alice, 0));
        env(trust(gw, USD(100), bob, 0));

        {
            Json::Value params;
            params[jss::account] = gw.human();
            params[jss::peer] = alice.human();

            auto lines = env.rpc("json", "account_lines", to_string(params));
            auto const& line0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(line0[jss::no_MUSO_peer].asBool() == true);
        }
        {
            Json::Value params;
            params[jss::account] = alice.human();
            params[jss::peer] = gw.human();

            auto lines = env.rpc("json", "account_lines", to_string(params));
            auto const& line0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(line0[jss::no_MUSO].asBool() == true);
        }
        {
            Json::Value params;
            params[jss::account] = gw.human();
            params[jss::peer] = bob.human();

            auto lines = env.rpc("json", "account_lines", to_string(params));
            auto const& line0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(line0[jss::no_MUSO].asBool() == false);
        }
        {
            Json::Value params;
            params[jss::account] = bob.human();
            params[jss::peer] = gw.human();

            auto lines = env.rpc("json", "account_lines", to_string(params));
            auto const& line0 = lines[jss::result][jss::lines][0u];
            BEAST_EXPECT(line0[jss::no_MUSO_peer].asBool() == false);
        }
    }

    void
    run() override
    {
        testSetAndClear();

        auto withFeatsTests = [this](FeatureBitset features) {
            testNegativeBalance(features);
            testPairwise(features);
            testDefaultMUSO(features);
        };
        using namespace jtx;
        auto const sa = supported_amendments();
        withFeatsTests(sa - featureFlowCross);
        withFeatsTests(sa);
    }
};

BEAST_DEFINE_TESTSUITE(NoMUSO, app, MUSO);

}  // namespace test
}  // namespace MUSO
