//
// Created by moedinger on 13.11.18.
//

#include <cstddef>
#include <array>

#ifndef MOORE_MATRIX_H
#define MOORE_MATRIX_H

template <class T, std::size_t ROW, std::size_t COL>
class Matrix {
public:
    Matrix() {
        for(auto& col : data) {
            std::fill(col.begin(), col.end(), 0);
        }
    }

    std::array<T, COL>& operator[](std::size_t index) { return data[index]; }
    const std::array<T, COL> operator[](std::size_t index) const { return data[index]; }
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }

private:
    std::array<std::array<T, COL>, ROW> data;
};


#endif //MOORE_MATRIX_H

