//
// Created by Malcolm Hall on 3/17/23.
//

#ifndef RONASCRIPT_SRC_UTIL_PTRSTACK_H_
#define RONASCRIPT_SRC_UTIL_PTRSTACK_H_

#include <cstdlib>

template <typename T>
class PtrStack {
public:
    explicit PtrStack(size_t reserve = 10) {
        Resize(reserve);
    }

    ~PtrStack() {
        std::free(_data);
        _data = nullptr;
    }

    void Push(T item) {
        assert(item);
        if (_size == _max_size)
            Resize(2 * _max_size);

        if (_size == 0)
            *_stack_ptr = item;
        else
            *(++_stack_ptr) = item;
        _size++;
    }

    T Pop() {
        assert(_size > 0);
        assert(_stack_ptr);
        auto item = *(_stack_ptr--);
        assert(item);
        _size--;
        return item;
    }

    T Top() {
        assert(_size > 0);
        return *_stack_ptr;
    }

    size_t Size() {
        return _size;
    }

    void Clear() {
        _stack_ptr = _data;
        _size = 0;
    }

    void push_back(T item) {
        Push(item);
    }

    T pop_back() {
        return Pop();
    }

    T back() {
        return Top();
    }

    void Resize(size_t new_size) {
        _data = static_cast<T*>(std::realloc(_data, sizeof(T) * new_size));
        _max_size = new_size;
        _stack_ptr = _data + _size;
    }

private:
    T* _data = nullptr;
    size_t _size = 0;
    size_t _max_size = 0;
    T* _stack_ptr = nullptr;
};

#endif  //RONASCRIPT_SRC_UTIL_PTRSTACK_H_
