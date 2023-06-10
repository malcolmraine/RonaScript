/*****************************************************************************
* File: RnCompilerPhase.h
* Description:
* Author: Malcolm Hall
* Date: 6/10/23
* Version: 1
*
*****************************************************************************/

#pragma once

class RnAbstractCompilerPhase {
public:
    virtual void Run() = 0;
    virtual char* SerializeResult() = 0;
    virtual void Reset() = 0;
};


template <typename T, typename V>
class RnCompilerPhase : public RnAbstractCompilerPhase {
public:
    RnCompilerPhase() = default;
    virtual ~RnCompilerPhase() = default;

    virtual void SetInput(T input) {
        _input = input;
    }

    [[nodiscard]] virtual V GetResult() {
        return _result;
    }

    [[nodiscard]] virtual char * SerializeResult() override {
        return nullptr;
    }

    void Reset() override {

    }

protected:
    T _input;
    V _result;
};
