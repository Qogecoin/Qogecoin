// Copyright (c) 2022 The Qogecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef QOGECOIN_TEST_FUZZ_MEMPOOL_UTILS_H
#define QOGECOIN_TEST_FUZZ_MEMPOOL_UTILS_H

#include <validation.h>

class DummyChainState final : public CChainState
{
public:
    void SetMempool(CTxMemPool* mempool)
    {
        m_mempool = mempool;
    }
};

#endif // QOGECOIN_TEST_FUZZ_MEMPOOL_UTILS_H
