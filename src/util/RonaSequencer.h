/*****************************************************************************
* File: RonaSequencer.h
* Description:
* Author: Malcolm Hall
* Date: 5/16/22
* Version: 1
*
******************************************************************************/

#ifndef RONASCRIPT_RONASEQUENCER_H
#define RONASCRIPT_RONASEQUENCER_H

#include <string>
#include <utility>
#include <algorithm>

template <typename T, typename E>
class RonaSequencer {
public:
    static const int DEFAULT_BUFFER_SIZE = 3;

    RonaSequencer();
    ~RonaSequencer() = default;
    T Peek(int n = 1) const;
    T Current() const;
    T Lookback(int n = 1) const;
    void Expect(E item);
    void Expect(std::vector<E> items);
    void AdvanceBuffer(int n);
    virtual void LoadNextItem();
    virtual bool CheckExpected();
    virtual std::string ItemToString(T item) = 0;

    /*************************************************************************/
    virtual bool CheckCurrent(E item) {
        Expect(item);
        return CheckExpected();
    }

    /*************************************************************************/
    virtual bool CheckCurrent(std::vector<E> items) {
        Expect(items);
        return CheckExpected();
    }

    [[nodiscard]] virtual bool EndOfSequence() const;

    /*************************************************************************/
    void SetCaptureUnexpected(bool flag) { _capture_unexpected = flag; }

    /*************************************************************************/
    [[nodiscard]] bool GetCaptureUnexpected() const { return _capture_unexpected; }

    /*************************************************************************/
    void SetItemTypeName(std::string item_type_name) {
        _item_type_name = std::move(item_type_name);
    }

    /*************************************************************************/
    [[nodiscard]] std::string GetItemTypeName() const { return _item_type_name; }

    /*************************************************************************/
    void SetBufferSize(size_t size) {
        _buffer.reserve(size);
        _buffer_size = size;
    }

    /*************************************************************************/
    [[nodiscard]] long GetBufferSize() const { return _buffer_size; }

    /*************************************************************************/
    void ClearUnexpectedItems() { _unexpected_captures.clear(); }

    /*************************************************************************/
    [[nodiscard]] size_t GetDataSize() const { return _data.size(); }

    /*************************************************************************/
    void SetWindowIdxCurrent(int i) { _window_idx_current = i; }

    /*************************************************************************/
    [[nodiscard]] int GetWindowIdxCurrent() const { return _window_idx_current; }

    /*************************************************************************/
    [[nodiscard]] size_t GetDataIdx() const { return _data_idx; }

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
    void LoadData(std::vector<T> data) {
        _data.insert(_data.begin(), data.begin(), data.end());
    }

    virtual E GetCurrentAsExpectedType() = 0;

protected:
    std::vector<T> _buffer;  // Window buffer
    int _window_idx_current = 1;
    std::vector<T> _data;  // All the data
    size_t _data_idx = 0;
    std::vector<E> _expected_items;
    std::vector<T> _unexpected_captures;
    std::string _item_type_name;
    bool _capture_unexpected = false;
    size_t _buffer_size = DEFAULT_BUFFER_SIZE;

private:
    /*************************************************************************/
    [[nodiscard]] int ClampIndex(int idx) const {
        return std::max((long long)0,
                        (long long)std::min((long long)_buffer.size(), (long long)idx));
    }
};

/*****************************************************************************/
template <class T, typename E>
RonaSequencer<T, E>::RonaSequencer() {
    SetBufferSize(DEFAULT_BUFFER_SIZE);
}

/*****************************************************************************/
template <class T, typename E>
T RonaSequencer<T, E>::Peek(int n) const {
    return _buffer[ClampIndex(GetWindowIdxCurrent() + n)];
}

/*****************************************************************************/
template <class T, typename E>
T RonaSequencer<T, E>::Current() const {
    return _buffer[ClampIndex(GetWindowIdxCurrent())];
}

/*****************************************************************************/
template <class T, typename E>
T RonaSequencer<T, E>::Lookback(int n) const {
    return _buffer[ClampIndex(GetWindowIdxCurrent() - n)];
}

/*****************************************************************************/
template <class T, typename E>
void RonaSequencer<T, E>::Expect(E item) {
    _expected_items.push_back(item);
}

/*****************************************************************************/
template <class T, typename E>
void RonaSequencer<T, E>::Expect(std::vector<E> items) {
    _expected_items.insert(std::end(_expected_items), std::begin(items),
                           std::end(items));
}

/*****************************************************************************/
template <class T, typename E>
void RonaSequencer<T, E>::AdvanceBuffer(int n) {
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
bool RonaSequencer<T, E>::CheckExpected() {
    if (_expected_items.empty()) {
        return true;
    }

    bool item_is_expected =
        std::find(_expected_items.begin(), _expected_items.end(),
                  GetCurrentAsExpectedType()) != _expected_items.end();

    if (item_is_expected) {
        _expected_items.clear();
    }

    return item_is_expected;
}

/*****************************************************************************/
template <class T, typename E>
bool RonaSequencer<T, E>::EndOfSequence() const {
    return GetDataIdx() == GetDataSize();
}

/*****************************************************************************/
template <class T, typename E>
void RonaSequencer<T, E>::LoadNextItem() {
    if (!EndOfSequence()) {
        _buffer[GetBufferSize() - 1] = _data[_data_idx++];
    }
}

#endif  //RONASCRIPT_RONASEQUENCER_H