#[===================================================================[
   MUSOl_core
   core functionality, useable by some client software perhaps
#]===================================================================]

file (GLOB_RECURSE rb_headers
  src/MUSO/beast/*.h
  src/MUSO/beast/*.hpp)

add_library (MUSOl_core
  ${rb_headers}) ## headers added here for benefit of IDEs
if (unity)
  set_target_properties(MUSOl_core PROPERTIES UNITY_BUILD ON)
endif ()


#[===============================[
    beast/legacy FILES:
    TODO: review these sources for removal or replacement
#]===============================]
target_sources (MUSOl_core PRIVATE
  src/MUSO/beast/core/CurrentThreadName.cpp
  src/MUSO/beast/core/SemanticVersion.cpp
  src/MUSO/beast/hash/impl/xxhash.cpp
  src/MUSO/beast/insight/impl/Collector.cpp
  src/MUSO/beast/insight/impl/Groups.cpp
  src/MUSO/beast/insight/impl/Hook.cpp
  src/MUSO/beast/insight/impl/Metric.cpp
  src/MUSO/beast/insight/impl/NullCollector.cpp
  src/MUSO/beast/insight/impl/StatsDCollector.cpp
  src/MUSO/beast/net/impl/IPAddressConversion.cpp
  src/MUSO/beast/net/impl/IPAddressV4.cpp
  src/MUSO/beast/net/impl/IPAddressV6.cpp
  src/MUSO/beast/net/impl/IPEndpoint.cpp
  src/MUSO/beast/utility/src/beast_Journal.cpp
  src/MUSO/beast/utility/src/beast_PropertyStream.cpp)

#[===============================[
    core sources
#]===============================]
target_sources (MUSOl_core PRIVATE
  #[===============================[
    main sources:
      subdir: basics (partial)
  #]===============================]
  src/MUSO/basics/impl/base64.cpp
  src/MUSO/basics/impl/contract.cpp
  src/MUSO/basics/impl/CountedObject.cpp
  src/MUSO/basics/impl/FileUtilities.cpp
  src/MUSO/basics/impl/IOUAmount.cpp
  src/MUSO/basics/impl/Log.cpp
  src/MUSO/basics/impl/strHex.cpp
  src/MUSO/basics/impl/StringUtilities.cpp
  #[===============================[
    main sources:
      subdir: json
  #]===============================]
  src/MUSO/json/impl/JsonPropertyStream.cpp
  src/MUSO/json/impl/Object.cpp
  src/MUSO/json/impl/Output.cpp
  src/MUSO/json/impl/Writer.cpp
  src/MUSO/json/impl/json_reader.cpp
  src/MUSO/json/impl/json_value.cpp
  src/MUSO/json/impl/json_valueiterator.cpp
  src/MUSO/json/impl/json_writer.cpp
  src/MUSO/json/impl/to_string.cpp
  #[===============================[
    main sources:
      subdir: protocol
  #]===============================]
  src/MUSO/protocol/impl/AccountID.cpp
  src/MUSO/protocol/impl/Book.cpp
  src/MUSO/protocol/impl/BuildInfo.cpp
  src/MUSO/protocol/impl/ErrorCodes.cpp
  src/MUSO/protocol/impl/Feature.cpp
  src/MUSO/protocol/impl/Indexes.cpp
  src/MUSO/protocol/impl/InnerObjectFormats.cpp
  src/MUSO/protocol/impl/Issue.cpp
  src/MUSO/protocol/impl/Keylet.cpp
  src/MUSO/protocol/impl/LedgerFormats.cpp
  src/MUSO/protocol/impl/PublicKey.cpp
  src/MUSO/protocol/impl/Quality.cpp
  src/MUSO/protocol/impl/Rate2.cpp
  src/MUSO/protocol/impl/SField.cpp
  src/MUSO/protocol/impl/SOTemplate.cpp
  src/MUSO/protocol/impl/STAccount.cpp
  src/MUSO/protocol/impl/STAmount.cpp
  src/MUSO/protocol/impl/STArray.cpp
  src/MUSO/protocol/impl/STBase.cpp
  src/MUSO/protocol/impl/STBlob.cpp
  src/MUSO/protocol/impl/STInteger.cpp
  src/MUSO/protocol/impl/STLedgerEntry.cpp
  src/MUSO/protocol/impl/STObject.cpp
  src/MUSO/protocol/impl/STParsedJSON.cpp
  src/MUSO/protocol/impl/STPathSet.cpp
  src/MUSO/protocol/impl/STTx.cpp
  src/MUSO/protocol/impl/STValidation.cpp
  src/MUSO/protocol/impl/STVar.cpp
  src/MUSO/protocol/impl/STVector256.cpp
  src/MUSO/protocol/impl/SecretKey.cpp
  src/MUSO/protocol/impl/Seed.cpp
  src/MUSO/protocol/impl/Serializer.cpp
  src/MUSO/protocol/impl/Sign.cpp
  src/MUSO/protocol/impl/TER.cpp
  src/MUSO/protocol/impl/TxFormats.cpp
  src/MUSO/protocol/impl/UintTypes.cpp
  src/MUSO/protocol/impl/digest.cpp
  src/MUSO/protocol/impl/tokens.cpp
  #[===============================[
    main sources:
      subdir: crypto
  #]===============================]
  src/MUSO/crypto/impl/GenerateDeterministicKey.cpp
  src/MUSO/crypto/impl/RFC1751.cpp
  src/MUSO/crypto/impl/csprng.cpp
  src/MUSO/crypto/impl/ec_key.cpp
  src/MUSO/crypto/impl/openssl.cpp
  src/MUSO/crypto/impl/secure_erase.cpp)

add_library (MUSO::MUSOl_core ALIAS MUSOl_core)
target_include_directories (MUSOl_core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/MUSO>
    # this one is for beast/legacy files:
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/beast/extras>
    $<INSTALL_INTERFACE:include>)


target_compile_definitions(MUSOl_core
  PUBLIC
    BOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT
    HAS_UNCAUGHT_EXCEPTIONS=1)
target_compile_options (MUSOl_core
  PUBLIC
    $<$<BOOL:${is_gcc}>:-Wno-maybe-uninitialized>)
target_link_libraries (MUSOl_core
  PUBLIC
    OpenSSL::Crypto
    MUSO::boost
    MUSO::syslibs
    NIH::secp256k1
    NIH::ed25519-donna
    date::date
    MUSO::opts)
#[=================================[
   main/core headers installation
#]=================================]
install (
  FILES
    src/MUSO/basics/base64.h
    src/MUSO/basics/Blob.h
    src/MUSO/basics/Buffer.h
    src/MUSO/basics/CountedObject.h
    src/MUSO/basics/FileUtilities.h
    src/MUSO/basics/IOUAmount.h
    src/MUSO/basics/LocalValue.h
    src/MUSO/basics/Log.h
    src/MUSO/basics/MathUtilities.h
    src/MUSO/basics/safe_cast.h
    src/MUSO/basics/Slice.h
    src/MUSO/basics/StringUtilities.h
    src/MUSO/basics/ToString.h
    src/MUSO/basics/UnorderedContainers.h
    src/MUSO/basics/MUSOAmount.h
    src/MUSO/basics/algorithm.h
    src/MUSO/basics/base_uint.h
    src/MUSO/basics/chrono.h
    src/MUSO/basics/contract.h
    src/MUSO/basics/FeeUnits.h
    src/MUSO/basics/hardened_hash.h
    src/MUSO/basics/strHex.h
  DESTINATION include/MUSO/basics)
install (
  FILES
    src/MUSO/crypto/GenerateDeterministicKey.h
    src/MUSO/crypto/RFC1751.h
    src/MUSO/crypto/csprng.h
    src/MUSO/crypto/secure_erase.h
  DESTINATION include/MUSO/crypto)
install (
  FILES
    src/MUSO/crypto/impl/ec_key.h
    src/MUSO/crypto/impl/openssl.h
  DESTINATION include/MUSO/crypto/impl)
install (
  FILES
    src/MUSO/json/JsonPropertyStream.h
    src/MUSO/json/Object.h
    src/MUSO/json/Output.h
    src/MUSO/json/Writer.h
    src/MUSO/json/json_forwards.h
    src/MUSO/json/json_reader.h
    src/MUSO/json/json_value.h
    src/MUSO/json/json_writer.h
    src/MUSO/json/to_string.h
  DESTINATION include/MUSO/json)
install (
  FILES
    src/MUSO/json/impl/json_assert.h
  DESTINATION include/MUSO/json/impl)
install (
  FILES
    src/MUSO/protocol/AccountID.h
    src/MUSO/protocol/AmountConversions.h
    src/MUSO/protocol/Book.h
    src/MUSO/protocol/BuildInfo.h
    src/MUSO/protocol/ErrorCodes.h
    src/MUSO/protocol/Feature.h
    src/MUSO/protocol/HashPrefix.h
    src/MUSO/protocol/Indexes.h
    src/MUSO/protocol/InnerObjectFormats.h
    src/MUSO/protocol/Issue.h
    src/MUSO/protocol/KeyType.h
    src/MUSO/protocol/Keylet.h
    src/MUSO/protocol/KnownFormats.h
    src/MUSO/protocol/LedgerFormats.h
    src/MUSO/protocol/Protocol.h
    src/MUSO/protocol/PublicKey.h
    src/MUSO/protocol/Quality.h
    src/MUSO/protocol/Rate.h
    src/MUSO/protocol/SField.h
    src/MUSO/protocol/SOTemplate.h
    src/MUSO/protocol/STAccount.h
    src/MUSO/protocol/STAmount.h
    src/MUSO/protocol/STArray.h
    src/MUSO/protocol/STBase.h
    src/MUSO/protocol/STBitString.h
    src/MUSO/protocol/STBlob.h
    src/MUSO/protocol/STExchange.h
    src/MUSO/protocol/STInteger.h
    src/MUSO/protocol/STLedgerEntry.h
    src/MUSO/protocol/STObject.h
    src/MUSO/protocol/STParsedJSON.h
    src/MUSO/protocol/STPathSet.h
    src/MUSO/protocol/STTx.h
    src/MUSO/protocol/STValidation.h
    src/MUSO/protocol/STVector256.h
    src/MUSO/protocol/SecretKey.h
    src/MUSO/protocol/Seed.h
    src/MUSO/protocol/SeqProxy.h
    src/MUSO/protocol/Serializer.h
    src/MUSO/protocol/Sign.h
    src/MUSO/protocol/SystemParameters.h
    src/MUSO/protocol/TER.h
    src/MUSO/protocol/TxFlags.h
    src/MUSO/protocol/TxFormats.h
    src/MUSO/protocol/UintTypes.h
    src/MUSO/protocol/digest.h
    src/MUSO/protocol/jss.h
    src/MUSO/protocol/tokens.h
  DESTINATION include/MUSO/protocol)
install (
  FILES
    src/MUSO/protocol/impl/STVar.h
    src/MUSO/protocol/impl/secp256k1.h
  DESTINATION include/MUSO/protocol/impl)

#[===================================[
   beast/legacy headers installation
#]===================================]
install (
  FILES
    src/MUSO/beast/clock/abstract_clock.h
    src/MUSO/beast/clock/basic_seconds_clock.h
    src/MUSO/beast/clock/manual_clock.h
  DESTINATION include/MUSO/beast/clock)
install (
  FILES
    src/MUSO/beast/core/LexicalCast.h
    src/MUSO/beast/core/List.h
    src/MUSO/beast/core/SemanticVersion.h
  DESTINATION include/MUSO/beast/core)
install (
  FILES
    src/MUSO/beast/hash/hash_append.h
    src/MUSO/beast/hash/uhash.h
    src/MUSO/beast/hash/xxhasher.h
  DESTINATION include/MUSO/beast/hash)
install (
  FILES src/MUSO/beast/hash/impl/xxhash.h
  DESTINATION include/MUSO/beast/hash/impl)
install (
  FILES
    src/MUSO/beast/rfc2616.h
    src/MUSO/beast/type_name.h
    src/MUSO/beast/unit_test.h
    src/MUSO/beast/xor_shift_engine.h
  DESTINATION include/MUSO/beast)
install (
  FILES
    src/MUSO/beast/utility/Journal.h
    src/MUSO/beast/utility/PropertyStream.h
    src/MUSO/beast/utility/Zero.h
    src/MUSO/beast/utility/rngfill.h
  DESTINATION include/MUSO/beast/utility)
# WARNING!! -- horrible levelization ahead
# (these files should be isolated or moved...but
#  unfortunately unit_test.h above creates this dependency)
install (
  FILES
    src/beast/extras/beast/unit_test/amount.hpp
    src/beast/extras/beast/unit_test/dstream.hpp
    src/beast/extras/beast/unit_test/global_suites.hpp
    src/beast/extras/beast/unit_test/match.hpp
    src/beast/extras/beast/unit_test/recorder.hpp
    src/beast/extras/beast/unit_test/reporter.hpp
    src/beast/extras/beast/unit_test/results.hpp
    src/beast/extras/beast/unit_test/runner.hpp
    src/beast/extras/beast/unit_test/suite.hpp
    src/beast/extras/beast/unit_test/suite_info.hpp
    src/beast/extras/beast/unit_test/suite_list.hpp
    src/beast/extras/beast/unit_test/thread.hpp
  DESTINATION include/beast/unit_test)
install (
  FILES
    src/beast/extras/beast/unit_test/detail/const_container.hpp
  DESTINATION include/beast/unit_test/detail)

#[===================================================================[
   MUSO executable
#]===================================================================]

#[=========================================================[
   this one header is added as source just to keep older
   versions of cmake happy. cmake 3.10+ allows
   add_executable with no sources
#]=========================================================]
add_executable (MUSO src/MUSO/app/main/Application.h)
if (unity)
  set_target_properties(MUSO PROPERTIES UNITY_BUILD ON)
endif ()
target_sources (MUSO PRIVATE
  #[===============================[
     main sources:
       subdir: app
  #]===============================]
  src/MUSO/app/consensus/RCLConsensus.cpp
  src/MUSO/app/consensus/RCLCxPeerPos.cpp
  src/MUSO/app/consensus/RCLValidations.cpp
  src/MUSO/app/ledger/AcceptedLedger.cpp
  src/MUSO/app/ledger/AcceptedLedgerTx.cpp
  src/MUSO/app/ledger/AccountStateSF.cpp
  src/MUSO/app/ledger/BookListeners.cpp
  src/MUSO/app/ledger/ConsensusTransSetSF.cpp
  src/MUSO/app/ledger/Ledger.cpp
  src/MUSO/app/ledger/LedgerHistory.cpp
  src/MUSO/app/ledger/OrderBookDB.cpp
  src/MUSO/app/ledger/TransactionStateSF.cpp
  src/MUSO/app/ledger/impl/BuildLedger.cpp
  src/MUSO/app/ledger/impl/InboundLedger.cpp
  src/MUSO/app/ledger/impl/InboundLedgers.cpp
  src/MUSO/app/ledger/impl/InboundTransactions.cpp
  src/MUSO/app/ledger/impl/LedgerCleaner.cpp
  src/MUSO/app/ledger/impl/LedgerDeltaAcquire.cpp
  src/MUSO/app/ledger/impl/LedgerMaster.cpp
  src/MUSO/app/ledger/impl/LedgerReplay.cpp
  src/MUSO/app/ledger/impl/LedgerReplayer.cpp
  src/MUSO/app/ledger/impl/LedgerReplayMsgHandler.cpp
  src/MUSO/app/ledger/impl/LedgerReplayTask.cpp
  src/MUSO/app/ledger/impl/LedgerToJson.cpp
  src/MUSO/app/ledger/impl/LocalTxs.cpp
  src/MUSO/app/ledger/impl/OpenLedger.cpp
  src/MUSO/app/ledger/impl/SkipListAcquire.cpp
  src/MUSO/app/ledger/impl/TimeoutCounter.cpp
  src/MUSO/app/ledger/impl/TransactionAcquire.cpp
  src/MUSO/app/ledger/impl/TransactionMaster.cpp
  src/MUSO/app/main/Application.cpp
  src/MUSO/app/main/BasicApp.cpp
  src/MUSO/app/main/CollectorManager.cpp
  src/MUSO/app/main/GRPCServer.cpp
  src/MUSO/app/main/LoadManager.cpp
  src/MUSO/app/main/Main.cpp
  src/MUSO/app/main/NodeIdentity.cpp
  src/MUSO/app/main/NodeStoreScheduler.cpp
  src/MUSO/app/reporting/DBHelpers.cpp
  src/MUSO/app/reporting/ReportingETL.cpp
  src/MUSO/app/reporting/ETLSource.cpp
  src/MUSO/app/reporting/P2pProxy.cpp
  src/MUSO/app/misc/CanonicalTXSet.cpp
  src/MUSO/app/misc/FeeVoteImpl.cpp
  src/MUSO/app/misc/HashRouter.cpp
  src/MUSO/app/misc/NegativeUNLVote.cpp
  src/MUSO/app/misc/NetworkOPs.cpp
  src/MUSO/app/misc/SHAMapStoreImp.cpp
  src/MUSO/app/misc/detail/impl/WorkSSL.cpp
  src/MUSO/app/misc/impl/AccountTxPaging.cpp
  src/MUSO/app/misc/impl/AmendmentTable.cpp
  src/MUSO/app/misc/impl/LoadFeeTrack.cpp
  src/MUSO/app/misc/impl/Manifest.cpp
  src/MUSO/app/misc/impl/Transaction.cpp
  src/MUSO/app/misc/impl/TxQ.cpp
  src/MUSO/app/misc/impl/ValidatorKeys.cpp
  src/MUSO/app/misc/impl/ValidatorList.cpp
  src/MUSO/app/misc/impl/ValidatorSite.cpp
  src/MUSO/app/paths/AccountCurrencies.cpp
  src/MUSO/app/paths/Credit.cpp
  src/MUSO/app/paths/Flow.cpp
  src/MUSO/app/paths/PathRequest.cpp
  src/MUSO/app/paths/PathRequests.cpp
  src/MUSO/app/paths/Pathfinder.cpp
  src/MUSO/app/paths/MUSOCalc.cpp
  src/MUSO/app/paths/MUSOLineCache.cpp
  src/MUSO/app/paths/MUSOState.cpp
  src/MUSO/app/paths/impl/BookStep.cpp
  src/MUSO/app/paths/impl/DirectStep.cpp
  src/MUSO/app/paths/impl/PaySteps.cpp
  src/MUSO/app/paths/impl/MUSOEndpointStep.cpp
  src/MUSO/app/tx/impl/ApplyContext.cpp
  src/MUSO/app/tx/impl/BookTip.cpp
  src/MUSO/app/tx/impl/CancelCheck.cpp
  src/MUSO/app/tx/impl/CancelOffer.cpp
  src/MUSO/app/tx/impl/CashCheck.cpp
  src/MUSO/app/tx/impl/Change.cpp
  src/MUSO/app/tx/impl/CreateCheck.cpp
  src/MUSO/app/tx/impl/CreateOffer.cpp
  src/MUSO/app/tx/impl/CreateTicket.cpp
  src/MUSO/app/tx/impl/DeleteAccount.cpp
  src/MUSO/app/tx/impl/DepositPreauth.cpp
  src/MUSO/app/tx/impl/Escrow.cpp
  src/MUSO/app/tx/impl/InvariantCheck.cpp
  src/MUSO/app/tx/impl/OfferStream.cpp
  src/MUSO/app/tx/impl/PayChan.cpp
  src/MUSO/app/tx/impl/Payment.cpp
  src/MUSO/app/tx/impl/SetAccount.cpp
  src/MUSO/app/tx/impl/SetRegularKey.cpp
  src/MUSO/app/tx/impl/SetSignerList.cpp
  src/MUSO/app/tx/impl/SetTrust.cpp
  src/MUSO/app/tx/impl/SignerEntries.cpp
  src/MUSO/app/tx/impl/Taker.cpp
  src/MUSO/app/tx/impl/Transactor.cpp
  src/MUSO/app/tx/impl/apply.cpp
  src/MUSO/app/tx/impl/applySteps.cpp
  #[===============================[
     main sources:
       subdir: basics (partial)
  #]===============================]
  src/MUSO/basics/impl/Archive.cpp
  src/MUSO/basics/impl/BasicConfig.cpp
  src/MUSO/basics/impl/PerfLogImp.cpp
  src/MUSO/basics/impl/ResolverAsio.cpp
  src/MUSO/basics/impl/UptimeClock.cpp
  src/MUSO/basics/impl/make_SSLContext.cpp
  src/MUSO/basics/impl/mulDiv.cpp
  #[===============================[
     main sources:
       subdir: conditions
  #]===============================]
  src/MUSO/conditions/impl/Condition.cpp
  src/MUSO/conditions/impl/Fulfillment.cpp
  src/MUSO/conditions/impl/error.cpp
  #[===============================[
     main sources:
       subdir: core
  #]===============================]
  src/MUSO/core/impl/Config.cpp
  src/MUSO/core/impl/DatabaseCon.cpp
  src/MUSO/core/impl/Job.cpp
  src/MUSO/core/impl/JobQueue.cpp
  src/MUSO/core/impl/LoadEvent.cpp
  src/MUSO/core/impl/LoadMonitor.cpp
  src/MUSO/core/impl/SNTPClock.cpp
  src/MUSO/core/impl/SociDB.cpp
  src/MUSO/core/impl/Stoppable.cpp
  src/MUSO/core/impl/TimeKeeper.cpp
  src/MUSO/core/impl/Workers.cpp
  src/MUSO/core/Pg.cpp
  #[===============================[
     main sources:
       subdir: consensus
  #]===============================]
  src/MUSO/consensus/Consensus.cpp
  #[===============================[
     main sources:
       subdir: ledger
  #]===============================]
  src/MUSO/ledger/impl/ApplyStateTable.cpp
  src/MUSO/ledger/impl/ApplyView.cpp
  src/MUSO/ledger/impl/ApplyViewBase.cpp
  src/MUSO/ledger/impl/ApplyViewImpl.cpp
  src/MUSO/ledger/impl/BookDirs.cpp
  src/MUSO/ledger/impl/CachedSLEs.cpp
  src/MUSO/ledger/impl/CachedView.cpp
  src/MUSO/ledger/impl/CashDiff.cpp
  src/MUSO/ledger/impl/Directory.cpp
  src/MUSO/ledger/impl/OpenView.cpp
  src/MUSO/ledger/impl/PaymentSandbox.cpp
  src/MUSO/ledger/impl/RawStateTable.cpp
  src/MUSO/ledger/impl/ReadView.cpp
  src/MUSO/ledger/impl/TxMeta.cpp
  src/MUSO/ledger/impl/View.cpp
  #[===============================[
     main sources:
       subdir: net
  #]===============================]
  src/MUSO/net/impl/DatabaseDownloader.cpp
  src/MUSO/net/impl/HTTPClient.cpp
  src/MUSO/net/impl/HTTPDownloader.cpp
  src/MUSO/net/impl/HTTPStream.cpp
  src/MUSO/net/impl/InfoSub.cpp
  src/MUSO/net/impl/RPCCall.cpp
  src/MUSO/net/impl/RPCErr.cpp
  src/MUSO/net/impl/RPCSub.cpp
  src/MUSO/net/impl/RegisterSSLCerts.cpp
  #[===============================[
     main sources:
       subdir: nodestore
  #]===============================]
  src/MUSO/nodestore/backend/CassandraFactory.cpp
  src/MUSO/nodestore/backend/MemoryFactory.cpp
  src/MUSO/nodestore/backend/NuDBFactory.cpp
  src/MUSO/nodestore/backend/NullFactory.cpp
  src/MUSO/nodestore/backend/RocksDBFactory.cpp
  src/MUSO/nodestore/impl/BatchWriter.cpp
  src/MUSO/nodestore/impl/Database.cpp
  src/MUSO/nodestore/impl/DatabaseNodeImp.cpp
  src/MUSO/nodestore/impl/DatabaseRotatingImp.cpp
  src/MUSO/nodestore/impl/DatabaseShardImp.cpp
  src/MUSO/nodestore/impl/DecodedBlob.cpp
  src/MUSO/nodestore/impl/DummyScheduler.cpp
  src/MUSO/nodestore/impl/EncodedBlob.cpp
  src/MUSO/nodestore/impl/ManagerImp.cpp
  src/MUSO/nodestore/impl/NodeObject.cpp
  src/MUSO/nodestore/impl/Shard.cpp
  src/MUSO/nodestore/impl/TaskQueue.cpp
  #[===============================[
     main sources:
       subdir: overlay
  #]===============================]
  src/MUSO/overlay/impl/Cluster.cpp
  src/MUSO/overlay/impl/ConnectAttempt.cpp
  src/MUSO/overlay/impl/Handshake.cpp
  src/MUSO/overlay/impl/Message.cpp
  src/MUSO/overlay/impl/OverlayImpl.cpp
  src/MUSO/overlay/impl/PeerImp.cpp
  src/MUSO/overlay/impl/PeerReservationTable.cpp
  src/MUSO/overlay/impl/PeerSet.cpp
  src/MUSO/overlay/impl/ProtocolVersion.cpp
  src/MUSO/overlay/impl/TrafficCount.cpp
  #[===============================[
     main sources:
       subdir: peerfinder
  #]===============================]
  src/MUSO/peerfinder/impl/Bootcache.cpp
  src/MUSO/peerfinder/impl/Endpoint.cpp
  src/MUSO/peerfinder/impl/PeerfinderConfig.cpp
  src/MUSO/peerfinder/impl/PeerfinderManager.cpp
  src/MUSO/peerfinder/impl/SlotImp.cpp
  src/MUSO/peerfinder/impl/SourceStrings.cpp
  #[===============================[
     main sources:
       subdir: resource
  #]===============================]
  src/MUSO/resource/impl/Charge.cpp
  src/MUSO/resource/impl/Consumer.cpp
  src/MUSO/resource/impl/Fees.cpp
  src/MUSO/resource/impl/ResourceManager.cpp
  #[===============================[
     main sources:
       subdir: rpc
  #]===============================]
  src/MUSO/rpc/handlers/AccountChannels.cpp
  src/MUSO/rpc/handlers/AccountCurrenciesHandler.cpp
  src/MUSO/rpc/handlers/AccountInfo.cpp
  src/MUSO/rpc/handlers/AccountLines.cpp
  src/MUSO/rpc/handlers/AccountObjects.cpp
  src/MUSO/rpc/handlers/AccountOffers.cpp
  src/MUSO/rpc/handlers/AccountTx.cpp
  src/MUSO/rpc/handlers/AccountTxOld.cpp
  src/MUSO/rpc/handlers/BlackList.cpp
  src/MUSO/rpc/handlers/BookOffers.cpp
  src/MUSO/rpc/handlers/CanDelete.cpp
  src/MUSO/rpc/handlers/Connect.cpp
  src/MUSO/rpc/handlers/ConsensusInfo.cpp
  src/MUSO/rpc/handlers/CrawlShards.cpp
  src/MUSO/rpc/handlers/DepositAuthorized.cpp
  src/MUSO/rpc/handlers/DownloadShard.cpp
  src/MUSO/rpc/handlers/Feature1.cpp
  src/MUSO/rpc/handlers/Fee1.cpp
  src/MUSO/rpc/handlers/FetchInfo.cpp
  src/MUSO/rpc/handlers/GatewayBalances.cpp
  src/MUSO/rpc/handlers/GetCounts.cpp
  src/MUSO/rpc/handlers/LedgerAccept.cpp
  src/MUSO/rpc/handlers/LedgerCleanerHandler.cpp
  src/MUSO/rpc/handlers/LedgerClosed.cpp
  src/MUSO/rpc/handlers/LedgerCurrent.cpp
  src/MUSO/rpc/handlers/LedgerData.cpp
  src/MUSO/rpc/handlers/LedgerDiff.cpp
  src/MUSO/rpc/handlers/LedgerEntry.cpp
  src/MUSO/rpc/handlers/LedgerHandler.cpp
  src/MUSO/rpc/handlers/LedgerHeader.cpp
  src/MUSO/rpc/handlers/LedgerRequest.cpp
  src/MUSO/rpc/handlers/LogLevel.cpp
  src/MUSO/rpc/handlers/LogRotate.cpp
  src/MUSO/rpc/handlers/Manifest.cpp
  src/MUSO/rpc/handlers/NoMUSOCheck.cpp
  src/MUSO/rpc/handlers/OwnerInfo.cpp
  src/MUSO/rpc/handlers/PathFind.cpp
  src/MUSO/rpc/handlers/PayChanClaim.cpp
  src/MUSO/rpc/handlers/Peers.cpp
  src/MUSO/rpc/handlers/Ping.cpp
  src/MUSO/rpc/handlers/Print.cpp
  src/MUSO/rpc/handlers/Random.cpp
  src/MUSO/rpc/handlers/Reservations.cpp
  src/MUSO/rpc/handlers/MUSOPathFind.cpp
  src/MUSO/rpc/handlers/ServerInfo.cpp
  src/MUSO/rpc/handlers/ServerState.cpp
  src/MUSO/rpc/handlers/SignFor.cpp
  src/MUSO/rpc/handlers/SignHandler.cpp
  src/MUSO/rpc/handlers/Stop.cpp
  src/MUSO/rpc/handlers/Submit.cpp
  src/MUSO/rpc/handlers/SubmitMultiSigned.cpp
  src/MUSO/rpc/handlers/Subscribe.cpp
  src/MUSO/rpc/handlers/TransactionEntry.cpp
  src/MUSO/rpc/handlers/Tx.cpp
  src/MUSO/rpc/handlers/TxHistory.cpp
  src/MUSO/rpc/handlers/UnlList.cpp
  src/MUSO/rpc/handlers/Unsubscribe.cpp
  src/MUSO/rpc/handlers/ValidationCreate.cpp
  src/MUSO/rpc/handlers/ValidatorInfo.cpp
  src/MUSO/rpc/handlers/ValidatorListSites.cpp
  src/MUSO/rpc/handlers/Validators.cpp
  src/MUSO/rpc/handlers/WalletPropose.cpp
  src/MUSO/rpc/impl/DeliveredAmount.cpp
  src/MUSO/rpc/impl/Handler.cpp
  src/MUSO/rpc/impl/GRPCHelpers.cpp
  src/MUSO/rpc/impl/LegacyPathFind.cpp
  src/MUSO/rpc/impl/RPCHandler.cpp
  src/MUSO/rpc/impl/RPCHelpers.cpp
  src/MUSO/rpc/impl/Role.cpp
  src/MUSO/rpc/impl/ServerHandlerImp.cpp
  src/MUSO/rpc/impl/ShardArchiveHandler.cpp
  src/MUSO/rpc/impl/ShardVerificationScheduler.cpp
  src/MUSO/rpc/impl/Status.cpp
  src/MUSO/rpc/impl/TransactionSign.cpp

  #[===============================[
     main sources:
       subdir: server
  #]===============================]
  src/MUSO/server/impl/JSONRPCUtil.cpp
  src/MUSO/server/impl/Port.cpp
  #[===============================[
     main sources:
       subdir: shamap
  #]===============================]
  src/MUSO/shamap/impl/NodeFamily.cpp
  src/MUSO/shamap/impl/SHAMap.cpp
  src/MUSO/shamap/impl/SHAMapDelta.cpp
  src/MUSO/shamap/impl/SHAMapInnerNode.cpp
  src/MUSO/shamap/impl/SHAMapItem.cpp
  src/MUSO/shamap/impl/SHAMapLeafNode.cpp
  src/MUSO/shamap/impl/SHAMapNodeID.cpp
  src/MUSO/shamap/impl/SHAMapSync.cpp
  src/MUSO/shamap/impl/SHAMapTreeNode.cpp
  src/MUSO/shamap/impl/ShardFamily.cpp
  #[===============================[
     test sources:
       subdir: app
  #]===============================]
  src/test/app/AccountDelete_test.cpp
  src/test/app/AccountTxPaging_test.cpp
  src/test/app/AmendmentTable_test.cpp
  src/test/app/Check_test.cpp
  src/test/app/CrossingLimits_test.cpp
  src/test/app/DeliverMin_test.cpp
  src/test/app/DepositAuth_test.cpp
  src/test/app/Discrepancy_test.cpp
  src/test/app/DNS_test.cpp
  src/test/app/Escrow_test.cpp
  src/test/app/FeeVote_test.cpp
  src/test/app/Flow_test.cpp
  src/test/app/Freeze_test.cpp
  src/test/app/HashRouter_test.cpp
  src/test/app/LedgerHistory_test.cpp
  src/test/app/LedgerLoad_test.cpp
  src/test/app/LedgerReplay_test.cpp
  src/test/app/LoadFeeTrack_test.cpp
  src/test/app/Manifest_test.cpp
  src/test/app/MultiSign_test.cpp
  src/test/app/OfferStream_test.cpp
  src/test/app/Offer_test.cpp
  src/test/app/OversizeMeta_test.cpp
  src/test/app/Path_test.cpp
  src/test/app/PayChan_test.cpp
  src/test/app/PayStrand_test.cpp
  src/test/app/PseudoTx_test.cpp
  src/test/app/RCLCensorshipDetector_test.cpp
  src/test/app/RCLValidations_test.cpp
  src/test/app/Regression_test.cpp
  src/test/app/SHAMapStore_test.cpp
  src/test/app/SetAuth_test.cpp
  src/test/app/SetRegularKey_test.cpp
  src/test/app/SetTrust_test.cpp
  src/test/app/Taker_test.cpp
  src/test/app/TheoreticalQuality_test.cpp
  src/test/app/Ticket_test.cpp
  src/test/app/Transaction_ordering_test.cpp
  src/test/app/TrustAndBalance_test.cpp
  src/test/app/TxQ_test.cpp
  src/test/app/ValidatorKeys_test.cpp
  src/test/app/ValidatorList_test.cpp
  src/test/app/ValidatorSite_test.cpp
  src/test/app/tx/apply_test.cpp
  #[===============================[
     test sources:
       subdir: basics
  #]===============================]
  src/test/basics/Buffer_test.cpp
  src/test/basics/DetectCrash_test.cpp
  src/test/basics/FileUtilities_test.cpp
  src/test/basics/IOUAmount_test.cpp
  src/test/basics/KeyCache_test.cpp
  src/test/basics/PerfLog_test.cpp
  src/test/basics/RangeSet_test.cpp
  src/test/basics/Slice_test.cpp
  src/test/basics/StringUtilities_test.cpp
  src/test/basics/TaggedCache_test.cpp
  src/test/basics/MUSOAmount_test.cpp
  src/test/basics/base64_test.cpp
  src/test/basics/base_uint_test.cpp
  src/test/basics/contract_test.cpp
  src/test/basics/FeeUnits_test.cpp
  src/test/basics/hardened_hash_test.cpp
  src/test/basics/mulDiv_test.cpp
  src/test/basics/tagged_integer_test.cpp
  #[===============================[
     test sources:
       subdir: beast
  #]===============================]
  src/test/beast/IPEndpoint_test.cpp
  src/test/beast/LexicalCast_test.cpp
  src/test/beast/SemanticVersion_test.cpp
  src/test/beast/aged_associative_container_test.cpp
  src/test/beast/beast_CurrentThreadName_test.cpp
  src/test/beast/beast_Journal_test.cpp
  src/test/beast/beast_PropertyStream_test.cpp
  src/test/beast/beast_Zero_test.cpp
  src/test/beast/beast_abstract_clock_test.cpp
  src/test/beast/beast_basic_seconds_clock_test.cpp
  src/test/beast/beast_io_latency_probe_test.cpp
  src/test/beast/define_print.cpp
  #[===============================[
     test sources:
       subdir: conditions
  #]===============================]
  src/test/conditions/PreimageSha256_test.cpp
  #[===============================[
     test sources:
       subdir: consensus
  #]===============================]
  src/test/consensus/ByzantineFailureSim_test.cpp
  src/test/consensus/Consensus_test.cpp
  src/test/consensus/DistributedValidatorsSim_test.cpp
  src/test/consensus/LedgerTiming_test.cpp
  src/test/consensus/LedgerTrie_test.cpp
  src/test/consensus/NegativeUNL_test.cpp
  src/test/consensus/ScaleFreeSim_test.cpp
  src/test/consensus/Validations_test.cpp
  #[===============================[
     test sources:
       subdir: core
  #]===============================]
  src/test/core/ClosureCounter_test.cpp
  src/test/core/Config_test.cpp
  src/test/core/Coroutine_test.cpp
  src/test/core/CryptoPRNG_test.cpp
  src/test/core/JobQueue_test.cpp
  src/test/core/SociDB_test.cpp
  src/test/core/Stoppable_test.cpp
  src/test/core/Workers_test.cpp
  #[===============================[
     test sources:
       subdir: crypto
  #]===============================]
  src/test/crypto/Openssl_test.cpp
  #[===============================[
     test sources:
       subdir: csf
  #]===============================]
  src/test/csf/BasicNetwork_test.cpp
  src/test/csf/Digraph_test.cpp
  src/test/csf/Histogram_test.cpp
  src/test/csf/Scheduler_test.cpp
  src/test/csf/impl/Sim.cpp
  src/test/csf/impl/ledgers.cpp
  #[===============================[
     test sources:
       subdir: json
  #]===============================]
  src/test/json/Object_test.cpp
  src/test/json/Output_test.cpp
  src/test/json/Writer_test.cpp
  src/test/json/json_value_test.cpp
  #[===============================[
     test sources:
       subdir: jtx
  #]===============================]
  src/test/jtx/Env_test.cpp
  src/test/jtx/WSClient_test.cpp
  src/test/jtx/impl/Account.cpp
  src/test/jtx/impl/Env.cpp
  src/test/jtx/impl/JSONRPCClient.cpp
  src/test/jtx/impl/ManualTimeKeeper.cpp
  src/test/jtx/impl/WSClient.cpp
  src/test/jtx/impl/acctdelete.cpp
  src/test/jtx/impl/account_txn_id.cpp
  src/test/jtx/impl/amount.cpp
  src/test/jtx/impl/balance.cpp
  src/test/jtx/impl/check.cpp
  src/test/jtx/impl/delivermin.cpp
  src/test/jtx/impl/deposit.cpp
  src/test/jtx/impl/envconfig.cpp
  src/test/jtx/impl/fee.cpp
  src/test/jtx/impl/flags.cpp
  src/test/jtx/impl/invoice_id.cpp
  src/test/jtx/impl/jtx_json.cpp
  src/test/jtx/impl/last_ledger_sequence.cpp
  src/test/jtx/impl/memo.cpp
  src/test/jtx/impl/multisign.cpp
  src/test/jtx/impl/offer.cpp
  src/test/jtx/impl/owners.cpp
  src/test/jtx/impl/paths.cpp
  src/test/jtx/impl/pay.cpp
  src/test/jtx/impl/quality2.cpp
  src/test/jtx/impl/rate.cpp
  src/test/jtx/impl/regkey.cpp
  src/test/jtx/impl/sendmax.cpp
  src/test/jtx/impl/seq.cpp
  src/test/jtx/impl/sig.cpp
  src/test/jtx/impl/tag.cpp
  src/test/jtx/impl/ticket.cpp
  src/test/jtx/impl/trust.cpp
  src/test/jtx/impl/txflags.cpp
  src/test/jtx/impl/utility.cpp

  #[===============================[
     test sources:
       subdir: ledger
  #]===============================]
  src/test/ledger/BookDirs_test.cpp
  src/test/ledger/CashDiff_test.cpp
  src/test/ledger/Directory_test.cpp
  src/test/ledger/Invariants_test.cpp
  src/test/ledger/PaymentSandbox_test.cpp
  src/test/ledger/PendingSaves_test.cpp
  src/test/ledger/SkipList_test.cpp
  src/test/ledger/View_test.cpp
  #[===============================[
     test sources:
       subdir: net
  #]===============================]
  src/test/net/DatabaseDownloader_test.cpp
  #[===============================[
     test sources:
       subdir: nodestore
  #]===============================]
  src/test/nodestore/Backend_test.cpp
  src/test/nodestore/Basics_test.cpp
  src/test/nodestore/DatabaseShard_test.cpp
  src/test/nodestore/Database_test.cpp
  src/test/nodestore/Timing_test.cpp
  src/test/nodestore/import_test.cpp
  src/test/nodestore/varint_test.cpp
  #[===============================[
     test sources:
       subdir: overlay
  #]===============================]
  src/test/overlay/ProtocolVersion_test.cpp
  src/test/overlay/cluster_test.cpp
  src/test/overlay/short_read_test.cpp
  src/test/overlay/compression_test.cpp
  src/test/overlay/reduce_relay_test.cpp
  src/test/overlay/handshake_test.cpp
  #[===============================[
     test sources:
       subdir: peerfinder
  #]===============================]
  src/test/peerfinder/Livecache_test.cpp
  src/test/peerfinder/PeerFinder_test.cpp
  #[===============================[
     test sources:
       subdir: protocol
  #]===============================]
  src/test/protocol/BuildInfo_test.cpp
  src/test/protocol/InnerObjectFormats_test.cpp
  src/test/protocol/Issue_test.cpp
  src/test/protocol/KnownFormatToGRPC_test.cpp
  src/test/protocol/PublicKey_test.cpp
  src/test/protocol/Quality_test.cpp
  src/test/protocol/STAccount_test.cpp
  src/test/protocol/STAmount_test.cpp
  src/test/protocol/STObject_test.cpp
  src/test/protocol/STTx_test.cpp
  src/test/protocol/STValidation_test.cpp
  src/test/protocol/SecretKey_test.cpp
  src/test/protocol/Seed_test.cpp
  src/test/protocol/SeqProxy_test.cpp
  src/test/protocol/TER_test.cpp
  src/test/protocol/types_test.cpp
  #[===============================[
     test sources:
       subdir: resource
  #]===============================]
  src/test/resource/Logic_test.cpp
  #[===============================[
     test sources:
       subdir: rpc
  #]===============================]
  src/test/rpc/AccountCurrencies_test.cpp
  src/test/rpc/AccountInfo_test.cpp
  src/test/rpc/AccountLinesRPC_test.cpp
  src/test/rpc/AccountObjects_test.cpp
  src/test/rpc/AccountOffers_test.cpp
  src/test/rpc/AccountSet_test.cpp
  src/test/rpc/AccountTx_test.cpp
  src/test/rpc/AmendmentBlocked_test.cpp
  src/test/rpc/Book_test.cpp
  src/test/rpc/DepositAuthorized_test.cpp
  src/test/rpc/DeliveredAmount_test.cpp
  src/test/rpc/Feature_test.cpp
  src/test/rpc/Fee_test.cpp
  src/test/rpc/GatewayBalances_test.cpp
  src/test/rpc/GetCounts_test.cpp
  src/test/rpc/JSONRPC_test.cpp
  src/test/rpc/KeyGeneration_test.cpp
  src/test/rpc/LedgerClosed_test.cpp
  src/test/rpc/LedgerData_test.cpp
  src/test/rpc/LedgerRPC_test.cpp
  src/test/rpc/LedgerRequestRPC_test.cpp
  src/test/rpc/ManifestRPC_test.cpp
  src/test/rpc/NoMUSOCheck_test.cpp
  src/test/rpc/NoMUSO_test.cpp
  src/test/rpc/OwnerInfo_test.cpp
  src/test/rpc/Peers_test.cpp
  src/test/rpc/ReportingETL_test.cpp
  src/test/rpc/Roles_test.cpp
  src/test/rpc/RPCCall_test.cpp
  src/test/rpc/RPCOverload_test.cpp
  src/test/rpc/RobustTransaction_test.cpp
  src/test/rpc/ServerInfo_test.cpp
  src/test/rpc/ShardArchiveHandler_test.cpp
  src/test/rpc/Status_test.cpp
  src/test/rpc/Submit_test.cpp
  src/test/rpc/Subscribe_test.cpp
  src/test/rpc/Transaction_test.cpp
  src/test/rpc/TransactionEntry_test.cpp
  src/test/rpc/TransactionHistory_test.cpp
  src/test/rpc/Tx_test.cpp
  src/test/rpc/ValidatorInfo_test.cpp
  src/test/rpc/ValidatorRPC_test.cpp
  src/test/rpc/Version_test.cpp
  #[===============================[
     test sources:
       subdir: server
  #]===============================]
  src/test/server/ServerStatus_test.cpp
  src/test/server/Server_test.cpp
  #[===============================[
     test sources:
       subdir: shamap
  #]===============================]
  src/test/shamap/FetchPack_test.cpp
  src/test/shamap/SHAMapSync_test.cpp
  src/test/shamap/SHAMap_test.cpp
  #[===============================[
     test sources:
       subdir: unit_test
  #]===============================]
  src/test/unit_test/multi_runner.cpp)
target_link_libraries (MUSO
  MUSO::boost
  MUSO::opts
  MUSO::libs
  MUSO::MUSOl_core
  )
exclude_if_included (MUSO)
# define a macro for tests that might need to
# be exluded or run differently in CI environment
if (is_ci)
  target_compile_definitions(MUSO PRIVATE MUSO_RUNNING_IN_CI)
endif ()

if (reporting)
    target_compile_definitions(MUSO PRIVATE MUSO_REPORTING)
endif ()

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.16)
  # any files that don't play well with unity should be added here
  set_source_files_properties(
    # these two seem to produce conflicts in beast teardown template methods
    src/test/rpc/ValidatorRPC_test.cpp
    src/test/rpc/ShardArchiveHandler_test.cpp
    PROPERTIES SKIP_UNITY_BUILD_INCLUSION TRUE)
endif ()
