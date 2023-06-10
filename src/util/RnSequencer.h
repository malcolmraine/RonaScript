/*****************************************************************************
* File: RnSequencer.h
* Description:
* Author: Malcolm Hall
* Date: 5/16/22
* Version: 1
*
* MIT License
*
* Copyright (c) 2021 Malcolm Hall
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
******************************************************************************/

#pragma once

#include <algorithm>
#include <string>
#include <utility>

template <typename T, typename E>
class RnSequencer {
public:
    static const int DEFAULT_BUFFER_SIZE = 3;

    RnSequencer();
    virtual ~RnSequencer() {
        delete[] _data;
        _data = nullptr;
    }

    T Peek(size_t n = 1) const;
    T Current() const;
    T Lookback(size_t n = 1) const;
    void Expect(E item);
    void Expect(std::vector<E> items);
    void AdvanceBuffer(size_t n);
    virtual void LoadNextItem();
    virtual bool CheckExpected();
    virtual std::string ItemToString(T item) = 0;

    [[nodiscard]] virtual bool EndOfSequence() const;

    /*************************************************************************/
    void SetCaptureUnexpected(bool flag) {
        _capture_unexpected = flag;
    }

    /*************************************************************************/
    [[nodiscard]] bool GetCaptureUnexpected() const {
        return _capture_unexpected;
    }

    /*************************************************************************/
    void SetItemTypeName(std::string item_type_name) {
        _item_type_name = std::move(item_type_name);
    }

    /*************************************************************************/
    [[nodiscard]] std::string GetItemTypeName() const {
        return _item_type_name;
    }

    /*************************************************************************/
    void SetBufferSize(size_t size) {
        _buffer.reserve(size);
        _buffer_size = size;
    }

    /*************************************************************************/
    [[nodiscard]] long GetBufferSize() const {
        return _buffer_size;
    }

    /*************************************************************************/
    [[nodiscard]] size_t GetDataSize() const {
        return _data_size;
    }

    /*************************************************************************/
    virtual void RunAdvanceBufferSideEffects() {}

    /*************************************************************************/
    virtual void HandleUnexpectedItem() {
        throw std::runtime_error("Unexpected item " + ItemToString(Current()));
    }

    /*************************************************************************/
    void FillBuffer(T item) {
        for (int i = 0; i < GetBufferSize(); ++i) {
            _buffer.push_back(item);
        }
    }

    /*************************************************************************/
    virtual void SetFromPtr(const T* data, size_t size) {
        delete[] _data;
        _data_size = size;
        _data = new T[_data_size];
        std::memcpy(_data, data, _data_size * sizeof(T));
    }

    /*************************************************************************/
    virtual T* GetData(bool release = false) {
        auto data = _data;
        if (release)
            _data = nullptr;
        return data;
    }

    virtual E GetCurrentAsExpectedType() = 0;

protected:
    std::vector<T> _buffer;  // Window buffer
    int _window_idx_current = 1;
    T* _data = nullptr;
    size_t _data_idx = 0;
    size_t _data_size = 0;
    std::vector<E> _expected_items;
    std::vector<T> _unexpected_captures;
    std::string _item_type_name;
    bool _capture_unexpected = false;
    size_t _buffer_size = DEFAULT_BUFFER_SIZE;

private:
    /*************************************************************************/
    [[nodiscard]] int ClampIndex(size_t idx) const {
        return std::max((size_t)0, std::min(_buffer.size(), idx));
    }
};

/*****************************************************************************/
template <class T, typename E>
RnSequencer<T, E>::RnSequencer() {
    SetBufferSize(DEFAULT_BUFFER_SIZE);
}

/*****************************************************************************/
template <class T, typename E>
T RnSequencer<T, E>::Peek(size_t n) const {
    return _buffer[ClampIndex(_window_idx_current + n)];
}

/*****************************************************************************/
template <class T, typename E>
T RnSequencer<T, E>::Current() const {
    return _buffer[ClampIndex(_window_idx_current)];
}

/*****************************************************************************/
template <class T, typename E>
T RnSequencer<T, E>::Lookback(size_t n) const {
    return _buffer[ClampIndex(_window_idx_current - n)];
}

/*****************************************************************************/
template <class T, typename E>
void RnSequencer<T, E>::Expect(E item) {
    _expected_items.push_back(item);
}

/*****************************************************************************/
template <class T, typename E>
void RnSequencer<T, E>::Expect(std::vector<E> items) {
    _expected_items.insert(std::end(_expected_items), std::begin(items),
                           std::end(items));
}

/*****************************************************************************/
template <class T, typename E>
void RnSequencer<T, E>::AdvanceBuffer(size_t n) {
    for (int i = 0; i < n; ++i) {
        for (size_t j = 0; j < _buffer.size() - 1; ++j) {
            _buffer[j] = _buffer[j + 1];
        }

        if (!CheckExpected()) {
            if (_capture_unexpected) {
                _unexpected_captures.push_back(Current());
            } else {
                HandleUnexpectedItem();
            }
        }
        LoadNextItem();
        RunAdvanceBufferSideEffects();
    }
}

/*****************************************************************************/
template <class T, typename E>
bool RnSequencer<T, E>::CheckExpected() {
    if (_expected_items.empty()) {
        return true;
    }

    bool item_is_expected =
        std::find(_expected_items.begin(), _expected_items.end(),
                  GetCurrentAsExpectedType()) != _expected_items.end();
    _expected_items.clear();

    return item_is_expected;
}

/*****************************************************************************/
template <class T, typename E>
bool RnSequencer<T, E>::EndOfSequence() const {
    return _data_idx > _data_size;
}

/*****************************************************************************/
template <class T, typename E>
void RnSequencer<T, E>::LoadNextItem() {
    if (_data_idx <= _data_size) {
        _buffer[GetBufferSize() - 1] = _data[_data_idx++];
    }
}
