// Copyright (c) 2016-2021 The Valdcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef VALDCOIN_WALLET_RPC_WALLET_H
#define VALDCOIN_WALLET_RPC_WALLET_H

#include <span.h>

class CRPCCommand;

namespace wallet {
Span<const CRPCCommand> GetWalletRPCCommands();
} // namespace wallet

#endif // VALDCOIN_WALLET_RPC_WALLET_H
