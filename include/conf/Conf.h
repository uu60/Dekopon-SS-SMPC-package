//
// Created by 杜建璋 on 2025/1/19.
//

#ifndef CONF_H
#define CONF_H
#include <thread>
#include "Consts.h"

class Conf {
public:
    // ---------------Settings for bmts---------------
    // If intermediate data produced in background
    constexpr static int BMT_METHOD = Consts::BMT_FIXED;
    // Bmt max num in queue (INVALID when BMT_BACKGROUND is false)
    constexpr static int MAX_BMTS = 10000;
    // Used times limit of one bmt (INVALID when BMT_BACKGROUND is false)
    constexpr static int BMT_USAGE_LIMIT = 1;
    // Blocking Bmt Queue (INVALID when background bmt is disabled)
    constexpr static int BMT_QUEUE_TYPE = Consts::CAS_QUEUE;

    // ---------------Settings for threads---------------
    // Enable multiple-thread computation in each single executor
    constexpr static bool INTRA_OPERATOR_PARALLELISM = true;
    // Sum of threads in a process
    inline static int LOCAL_THREADS = static_cast<int>(std::thread::hardware_concurrency() * 10);
    // Index of thread pool type (0 = ctpl, 1 = tbb)
    constexpr static int THREAD_POOL_TYPE = Consts::CTPL_POOL;
    // Separate thread groups for computation and networks
    constexpr static bool THREAD_POOL_SEPARATION = false;
    // Thread number of computing threads (INVALID when THREAD_SEPARATION is false)
    inline static int COMPUTING_THREADS = static_cast<int>(std::thread::hardware_concurrency());
    // Thread number of network threads (INVALID when THREAD_SEPARATION is false)
    inline static int NETWORK_THREADS = static_cast<int>(std::thread::hardware_concurrency());
    // Thread pool task queue separation
    constexpr static bool JOB_QUEUE_SEPARATION = false;
    // Sort in parallel
    constexpr static bool SORT_IN_PARALLEL = true;

    // ---------------Settings for networks---------------
    // Communication object index (0 = OpenMpi)
    constexpr static int COMM_TYPE = Consts::MPI;
    // Batch communicate or execute by elements
    constexpr static bool TASK_BATCHING = false;
    // Invalid if intra parallelism or batching is false
    constexpr static int BATCH_SIZE = 10;

    // ---------------Settings for benchmark---------------
    constexpr static bool CLASS_WISE_TIMING = true;
};



#endif //CONF_H
