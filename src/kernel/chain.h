// Copyright (c) 2022 The Qogecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef QOGECOIN_KERNEL_CHAIN_H
#define QOGECOIN_KERNEL_CHAIN_H

class CBlock;
class CBlockIndex;
namespace interfaces {
struct BlockInfo;
} // namespace interfaces

namespace kernel {
//! Return data from block index.
interfaces::BlockInfo MakeBlockInfo(const CBlockIndex* block_index, const CBlock* data = nullptr);
} // namespace kernel

#endif // QOGECOIN_KERNEL_CHAIN_H
