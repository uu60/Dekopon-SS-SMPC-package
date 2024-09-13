//
// Created by 杜建璋 on 2024/7/27.
//

#include "executor/share/arithmetic/multiplication/RsaOtMultiplicationShareExecutor.h"
#include "utils/Mpi.h"
#include "executor/bmt/RsaOtTripleGenerator.h"

RsaOtMultiplicationShareExecutor::RsaOtMultiplicationShareExecutor(int64_t x, int64_t y, int l): AbstractMultiplicationShareExecutor(x, y, l) {}

RsaOtMultiplicationShareExecutor::RsaOtMultiplicationShareExecutor(int64_t x, int64_t y, int l, bool dummy)
        : AbstractMultiplicationShareExecutor(x, y, l, dummy) {}

void RsaOtMultiplicationShareExecutor::obtainMultiplicationTriple() {
    RsaOtTripleGenerator e(_l);
    e.benchmark(_benchmarkLevel)->logBenchmark(false)->execute(false);
    if (_benchmarkLevel == BenchmarkLevel::DETAILED) {
        _mpiTime += e.mpiTime();
    }
    _ai = e.ai();
    _bi = e.bi();
    _ci = e.ci();

    if (_benchmarkLevel == BenchmarkLevel::DETAILED && _isLogBenchmark) {
        Log::i(tag() + " OT RSA keys generation time: " + std::to_string(e.otRsaGenerationTime()) + " ms.");
        Log::i(tag() + " OT RSA encryption time: " + std::to_string(e.otRsaEncryptionTime()) + " ms.");
        Log::i(tag() + " OT RSA decryption time: " + std::to_string(e.otRsaDecryptionTime()) + " ms.");
        Log::i(tag() + " OT MPI transmission and synchronization time: " + std::to_string(e.otMpiTime()) + " ms.");
        Log::i(tag() + " OT total computation time: " + std::to_string(e.otEntireComputationTime()) + " ms.");
    }
}

std::string RsaOtMultiplicationShareExecutor::tag() const {
    return "[RSA OT Multiplication Share]";
}
