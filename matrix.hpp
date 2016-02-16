//
//  matrix.hpp
//  MatrixTest
//
//  Created by Anthony Super on 2/6/16.
//  Copyright © 2016 Noided Media. All rights reserved.
//

#ifndef matrix_hpp
#define matrix_hpp

#include <stdio.h>
#include <initializer_list>
#include <memory>
#include <stdlib.h>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <sstream>


namespace NM {
    class Matrix;
    
    template<typename T>
    Matrix operator*(T o, Matrix);

    class Matrix {
    public:
        Matrix(std::initializer_list<std::initializer_list<double>>);
        Matrix(const Matrix&);
        Matrix(int rows, int columns);
        Matrix(Matrix&&);
        class BadMultiplication : std::runtime_error {
        public:
            explicit BadMultiplication(const std::string& msg) : std::runtime_error(msg)
            {}
        };
    
        class BadAddition : std::runtime_error {
        public:
            explicit BadAddition(const std::string &msg) : std::runtime_error(msg)
            {}
        };
        
        class NonSquareMatrix : std::runtime_error {
        public:
            explicit NonSquareMatrix() :
            std::runtime_error("Matrix is not square")
            {}
        };
        
        inline double getElement(int row, int column){
            return data[row * rows + column];
        }
        void performDecomp();
        bool isSquare();
       
        double determinant();
        
        const int rows;
        const int columns;
        
        //--- OPERATORS
        explicit operator std::string();
        Matrix& operator=(const Matrix&) = delete;
        Matrix operator*(Matrix&);
        Matrix operator+(Matrix&);
        bool operator!=(Matrix&);
        bool operator==(Matrix&);
    protected:
        std::unique_ptr<Matrix> decomp_l;
        std::unique_ptr<Matrix> decomp_u;
        template<typename T>
        friend Matrix operator*(T, NM::Matrix);
        std::unique_ptr<double[]> data;
        inline void setElement(int row, int columns, double value){
            data[row * rows + columns] = value;
        }
    };
    
    template<typename T>
    Matrix operator*(T o, Matrix m){
        static_assert(std::is_arithmetic<T>::value,
                      "Can only multiply matricies by scalars or other matricies");
        Matrix r(m.rows, m.columns);
        for(int i = 0; i < m.rows; i++){
            for(int k = 0; k < m.columns; k++){
                r.setElement(i, k, o * m.getElement(i, k));
            }
        }
        return r;
    }
    
    template<typename T>
    Matrix operator*(Matrix m, T o){
        return o * m;
    }
}


#endif /* matrix_hpp */
