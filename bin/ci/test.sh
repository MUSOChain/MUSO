#!/usr/bin/env bash

set -o xtrace
set -o errexit

# Set to 'true' to run the known "manual" tests in MUSO.
MANUAL_TESTS=${MANUAL_TESTS:-false}
# The maximum number of concurrent tests.
CONCURRENT_TESTS=${CONCURRENT_TESTS:-$(nproc)}
# The path to MUSO.
MUSO=${MUSO:-build/MUSO}
# Additional arguments to MUSO.
MUSO_ARGS=${MUSO_ARGS:-}

function join_by { local IFS="$1"; shift; echo "$*"; }

declare -a manual_tests=(
  'beast.chrono.abstract_clock'
  'beast.unit_test.print'
  'MUSO.NodeStore.Timing'
  'MUSO.app.Flow_manual'
  'MUSO.app.NoMUSOCheckLimits'
  'MUSO.app.PayStrandAllPairs'
  'MUSO.consensus.ByzantineFailureSim'
  'MUSO.consensus.DistributedValidators'
  'MUSO.consensus.ScaleFreeSim'
  'MUSO.tx.CrossingLimits'
  'MUSO.tx.FindOversizeCross'
  'MUSO.tx.Offer_manual'
  'MUSO.tx.OversizeMeta'
  'MUSO.tx.PlumpBook'
)

if [[ ${MANUAL_TESTS} == 'true' ]]; then
  MUSO_ARGS+=" --unittest=$(join_by , "${manual_tests[@]}")"
else
  MUSO_ARGS+=" --unittest --quiet --unittest-log"
fi
MUSO_ARGS+=" --unittest-jobs ${CONCURRENT_TESTS}"

${MUSO} ${MUSO_ARGS}
