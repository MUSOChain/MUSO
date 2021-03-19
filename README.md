# The MUSO

The [MUSO](https://MUSOl.org/) is a decentralized cryptographic ledger powered by a network of peer-to-peer servers. The MUSO uses a novel Byzantine Fault Tolerant consensus algorithm to settle and record transactions in a secure distributed database without a central operator.

## MUSO

[MUSO](https://MUSOl.org/MUSO.html)is a public, counterparty-free asset native to the MUSO, and is designed to bridge the many different currencies in use worldwide. MUSO is traded on the open-market and is available for anyone to access. MUSO uses MUSO to help build the Internet of Value, ushering in a world in which money moves as fast and efficiently as information does today.

## MUSO

The server software that powers the MUSO is called `MUSO` and is available in this repository under the permissive [ISC open-source license](LICENSE). The `MUSO` server is written primarily in C++ and runs on a variety of platforms.

### Build from Source

- [Linux](Builds/linux/README.md)
- [Mac](Builds/macos/README.md)
- [Windows](Builds/VisualStudio2017/README.md)

## Key Features of the MUSO

- **[Censorship-Resistant Transaction Processing][]:** No single party decides which transactions succeed or fail, and no one can "roll back" a transaction after it completes. As long as those who choose to participate in the network keep it healthy, they can settle transactions in seconds.
- **[Fast, Efficient Consensus Algorithm][]:** The MUSO's consensus algorithm settles transactions in 4 to 5 seconds, processing at a throughput of up to 1500 transactions per second. These properties put MUSO at least an order of magnitude ahead of other top digital assets.
- **[Finite MUSO Supply][]:** When the MUSO began, 100 billion MUSO were created, and no more MUSO will ever be created. The available supply of MUSO decreases slowly over time as small amounts are destroyed to pay transaction costs.
- **[Responsible Software Governance][]:** A team of full-time, world-class developers at MUSO maintain and continually improve the MUSO's underlying software with contributions from the open-source community. MUSO acts as a steward for the technology and an advocate for its interests, and builds constructive relationships with governments and financial institutions worldwide.
- **[Secure, Adaptable Cryptography][]:** The MUSO relies on industry standard digital signature systems like ECDSA (the same scheme used by Bitcoin) but also supports modern, efficient algorithms like Ed25519. The extensible nature of the MUSO's software makes it possible to add and disable algorithms as the state of the art in cryptography advances.
- **[Modern Features for Smart Contracts][]:** Features like Escrow, Checks, and Payment Channels support cutting-edge financial applications including the [Interledger Protocol](https://interledger.org/). This toolbox of advanced features comes with safety features like a process for amending the network and separate checks against invariant constraints.
- **[On-Ledger Decentralized Exchange][]:** In addition to all the features that make MUSO useful on its own, the MUSO also has a fully-functional accounting system for tracking and trading obligations denominated in any way users want, and an exchange built into the protocol. The MUSO can settle long, cross-currency payment paths and exchanges of multiple currencies in atomic transactions, bridging gaps of trust with MUSO.

[censorship-resistant transaction processing]: https://MUSOl.org/MUSO-ledger-overview.html#censorship-resistant-transaction-processing
[fast, efficient consensus algorithm]: https://MUSOl.org/MUSO-ledger-overview.html#fast-efficient-consensus-algorithm
[finite muso supply]: https://MUSOl.org/MUSO-ledger-overview.html#finite-MUSO-supply
[responsible software governance]: https://MUSOl.org/MUSO-ledger-overview.html#responsible-software-governance
[secure, adaptable cryptography]: https://MUSOl.org/MUSO-ledger-overview.html#secure-adaptable-cryptography
[modern features for smart contracts]: https://MUSOl.org/MUSO-ledger-overview.html#modern-features-for-smart-contracts
[on-ledger decentralized exchange]: https://MUSOl.org/MUSO-ledger-overview.html#on-ledger-decentralized-exchange

## Source Code

[![travis-ci.com: Build Status](https://travis-ci.com/MUSO/MUSO.svg?branch=develop)](https://travis-ci.com/MUSO/MUSO)
[![codecov.io: Code Coverage](https://codecov.io/gh/MUSO/MUSO/branch/develop/graph/badge.svg)](https://codecov.io/gh/MUSO/MUSO)

### Repository Contents

| Folder     | Contents                                       |
| :--------- | :--------------------------------------------- |
| `./bin`    | Scripts and data files for MUSO integrators.   |
| `./Builds` | Platform-specific guides for building `MUSO`.  |
| `./docs`   | Source documentation files and doxygen config. |
| `./cfg`    | Example configuration files.                   |
| `./src`    | Source code.                                   |

Some of the directories under `src` are external repositories included using
git-subtree. See those directories' README files for more details.

## See Also

- [MUSO Dev Portal](https://MUSOl.org/)
- [Setup and Installation](https://MUSOl.org/install-MUSO.html)
- [Source Documentation (Doxygen)](https://MUSO.github.io/MUSO)
