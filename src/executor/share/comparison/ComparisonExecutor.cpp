//
// Created by 杜建璋 on 2024/9/2.
//

#include "executor/share/comparison/ComparisonExecutor.h"
#include "utils/Mpi.h"
#include "utils/Math.h"

ComparisonExecutor::ComparisonExecutor(int64_t x, int64_t y) {
    bool bm = _benchmarkLevel == BenchmarkLevel::DETAILED;
    if (!Mpi::isCalculator()) {
        int64_t x1 = Math::rand64();
        int64_t x0 = x - x1;
        int64_t y1 = Math::rand64();
        int64_t y0 = y - y1;
        Mpi::sendTo(&x0, 0, _mpiTime, bm);
        Mpi::sendTo(&y0, 0, _mpiTime, bm);
        Mpi::sendTo(&x1, 1, _mpiTime, bm);
        Mpi::sendTo(&y1, 1, _mpiTime, bm);
    }
}

void ComparisonExecutor::compute() {

}

