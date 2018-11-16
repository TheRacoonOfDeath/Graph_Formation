//
// Created by moedinger on 13.11.18.
//
#include <iostream>
#include <cstddef>
#include <array>

#ifndef MOORE_MATRIX_H
#define MOORE_MATRIX_H

template<class T, std::size_t ROW, std::size_t COL>
class Matrix {
public:
    Matrix() {
        for (auto &col : data) {
            std::fill(col.begin(), col.end(), 0);
        }
    }

    inline const auto min_side() const { return ROW > COL ? ROW : COL; }

    auto& operator[](std::size_t index) { return data[index]; }

    const std::array<T, COL> operator[](std::size_t index) const { return data[index]; }

    auto begin() { return data.begin(); }

    auto end() { return data.end(); }

    void print(std::string title) {
        std::cout << title << std::endl;
        for (auto &i : *this) {
            for (auto &j: i) {
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

private:
    std::array<std::array<T, COL>, ROW> data;
};


#endif //MOORE_MATRIX_H

