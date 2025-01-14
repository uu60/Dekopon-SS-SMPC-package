//
// Created by 杜建璋 on 2024/12/29.
//

#ifndef BOOLLESSTHANEXECUTOR_H
#define BOOLLESSTHANEXECUTOR_H
#include "./BoolExecutor.h"

class BoolLessExecutor : public BoolExecutor {
public:
    // reverse x and y to obey less than logic
    BoolLessExecutor(int64_t x, int64_t y, int l, int16_t taskTag, int16_t msgTagOffset, int clientRank) : BoolExecutor(y, x, l, taskTag, msgTagOffset, clientRank) {};

    BoolLessExecutor *execute() override;

    BoolLessExecutor *reconstruct(int clientRank) override;

    std::string className() const override;

    static int needsMsgTags(int clientRank);

private:
    int64_t shiftGreater(int64_t in, int r) const;
};



#endif //BOOLLESSTHANEXECUTOR_H
