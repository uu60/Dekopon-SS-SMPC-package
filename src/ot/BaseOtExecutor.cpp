//
// Created by 杜建璋 on 2024/7/15.
//

#include "ot/BaseOtExecutor.h"

#include "comm/Comm.h"
#include "utils/Math.h"
#include "utils/Crypto.h"
#include "utils/Log.h"

BaseOtExecutor::BaseOtExecutor(int sender, int64_t m0, int64_t m1, int choice, int l, int16_t taskTag, int16_t msgTagOffset)
    : BaseOtExecutor(2048, sender, m0, m1, choice, l, taskTag, msgTagOffset) {
}

BaseOtExecutor::BaseOtExecutor(int bits, int sender, int64_t m0, int64_t m1, int choice, int l, int16_t taskTag,
                             int16_t msgTagOffset) : AbstractOtExecutor(sender, m0, m1, choice, l, taskTag, msgTagOffset) {
    _bits = bits;
}

BaseOtExecutor *BaseOtExecutor::execute() {
    if (Comm::isServer()) {
        // preparation
        generateAndShareRandoms();
        generateAndShareRsaKeys();

        // process
        process();
    }
    return this;
}

void BaseOtExecutor::generateAndShareRandoms() {
    // 11 for PKCS#1 v1.5 padding
    int len = (_bits >> 3) - 11;
    if (_isSender) {
        _rand0 = Math::randString(len);
        _rand1 = Math::randString(len);

        std::string comb = _rand0 + _rand1;
        Comm::serverSend(comb, buildTag(_currentMsgTag));
    } else {
        _randK = Math::randString(len);
        std::string comb;
        Comm::serverReceive(comb, buildTag(_currentMsgTag));
        _rand0 = comb.substr(0, len);
        _rand1 = comb.substr(len, len);
    }
}

void BaseOtExecutor::generateAndShareRsaKeys() {
    if (_isSender) {
        bool newKey = Crypto::generateRsaKeys(_bits);
        _pub = Crypto::_selfPubs[_bits];
        _pri = Crypto::_selfPris[_bits];
        if (newKey) {
            Comm::serverSend(_pub, buildTag(_currentMsgTag++));
        }
    } else {
        // receiver
        if (Crypto::_otherPubs.count(_bits) > 0) {
            _pub = Crypto::_otherPubs[_bits];
        } else {
            Comm::serverReceive(_pub, buildTag(_currentMsgTag++));
            Crypto::_otherPubs[_bits] = _pub;
        }
    }
}

void BaseOtExecutor::process() {
    if (!_isSender) {
        std::string ek = Crypto::rsaEncrypt(_randK, _pub);
        std::string sumStr = Math::add(ek, _choice == 0 ? _rand0 : _rand1);
        Comm::serverSend(sumStr, buildTag(_currentMsgTag));

        std::string m0, m1;
        Comm::serverReceive(m0, buildTag(static_cast<int16_t>(_currentMsgTag + 1)));
        Comm::serverReceive(m1, buildTag(static_cast<int16_t>(_currentMsgTag + 2)));

        _result = std::stoll(Math::minus(_choice == 0 ? m0 : m1, _randK));
    } else {
        std::string sumStr;
        Comm::serverReceive(sumStr, buildTag(_currentMsgTag));
        std::string k0 = Crypto::rsaDecrypt(
            Math::minus(sumStr, _rand0), _pri
        );
        std::string k1 = Crypto::rsaDecrypt(
            Math::minus(sumStr, _rand1), _pri
        );
        std::string m0 = Math::add(std::to_string(_m0), k0);
        std::string m1 = Math::add(std::to_string(_m1), k1);

        auto f0 = System::_threadPool.push([m0, this] (int _) {
            Comm::serverSend(m0, buildTag(static_cast<int16_t>(_currentMsgTag + 1)));
        });
        auto f1 = System::_threadPool.push([m1, this] (int _) {
            Comm::serverSend(m1, buildTag(static_cast<int16_t>(_currentMsgTag + 2)));
        });
        f0.wait();
        f1.wait();
    }
}

std::string BaseOtExecutor::className() const {
    return "RsaOtExecutor";
}

int16_t BaseOtExecutor::needMsgTags() {
    return 6;
}
