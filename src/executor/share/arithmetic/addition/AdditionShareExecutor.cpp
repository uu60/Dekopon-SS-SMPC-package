//
// Created by 杜建璋 on 2024/7/13.
//

#include "executor/share/arithmetic/addition/AdditionShareExecutor.h"
#include "utils/Math.h"
#include "utils/Mpi.h"
#include <limits>

AdditionShareExecutor::AdditionShareExecutor(int64_t x, int64_t y) : AbstractIntegerShareExecutor(x, y) {}

void AdditionShareExecutor::compute() {
    bool bm = _benchmarkLevel == BenchmarkLevel::DETAILED;
    if (Mpi::isCalculator()) {
        int64_t zi = _xi + _yi;
        Mpi::sendTo(&zi, Mpi::TASK_PUBLISHER_RANK, _mpiTime, bm);
    } else {
        int64_t z0, z1;
        Mpi::recvFrom(&z0, 0, _mpiTime, bm);
        Mpi::recvFrom(&z1, 1, _mpiTime, bm);
        _result = z0 + z1;
    }
}

std::string AdditionShareExecutor::tag() const {
    return "[Addition Share]";
}

