//
//  matrix.cpp
//  MatrixTest
//
//  Created by Anthony Super on 2/6/16.
//  Copyright © 2016 Noided Media. All rights reserved.
//

#include "matrix.hpp"

#include <iostream>
namespace NM {
    
   
    
    Matrix::Matrix(int rows, int columns) : rows(rows), columns(columns),
    data{new double[rows * columns]()}
    {
        
    }
    
    
    Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list) :
    Matrix::Matrix((int) list.size(), list.size() ? (int) list.begin()->size() : 0 )
    {
        int r_count = 0;
        for(auto elem: list){
            auto c_count = 0;
            for(auto e: elem){
                setElement(r_count, c_count, e);
                c_count++;
                if(c_count > columns){
                    throw std::length_error("Invalid initializer list (column too long)");
                }
            }
            if(c_count + 1 < columns){
                throw std::length_error("Invalid initializer list (column too short)");
            }
            r_count++;
        }
    }
    
    Matrix::Matrix(const Matrix &other) : Matrix(other.rows, other.columns){
        memcpy((void*)data.get(), (void*)other.data.get(), rows * columns * sizeof(double));
        if(other.decomp_l && other.decomp_u){
            decomp_u = std::make_unique<Matrix>(*other.decomp_u);
            decomp_l = std::make_unique<Matrix>(*other.decomp_l);
        }
    }
    
    Matrix::Matrix(Matrix&& other) :
    rows(other.rows), columns(other.columns), data(std::move(other.data))
    {
        decomp_u = std::move(other.decomp_u);
        decomp_l = std::move(other.decomp_l);
    }
    
    void Matrix::performDecomp(){
        if(! isSquare()){
            throw NonSquareMatrix();
        }
        if(decomp_l && decomp_u){
            return;
        }
        decomp_l = std::make_unique<Matrix>(rows, columns);
        decomp_u = std::make_unique<Matrix>(*this);
        for(int r = 0; r < rows; r++){
            double init = decomp_u->getElement(r, r);
            for(int ru = r + 1; ru < rows; ru++){
                double head = decomp_u->getElement(ru, r);
                double mult = (head / init);
                decomp_l->setElement(ru, r, mult);
                for(int c = r; c < columns; c++){
                    double val = (-mult) * decomp_u->getElement(r, c);
                    val += decomp_u->getElement(ru, c);
                    decomp_u->setElement(ru, c, val);
                }
                decomp_u->setElement(ru, r, 0);
            }
         
        }
    }
   
    double Matrix::determinant() {
        if(! isSquare()){
            throw NonSquareMatrix();
        }
        if(rows == 2){
            return (data[0] * data[3]) - (data[2] * data[1]);
        }
        performDecomp();
        double mult = decomp_u->getElement(0, 0);
        for(int i = 1; i < columns; i++){
            mult *= decomp_u->getElement(i, i);
        }
        return mult;
    }
    
    bool Matrix::operator!=(Matrix &m){
        return ! (*(this) == m);
    }
    
    bool Matrix::operator==(Matrix &o){
        if(rows != o.rows || columns != o.columns){
            return false;
        }
        for(int i = 0; i < rows; ++i){
            for(int k = 0; k < columns; ++k){
                if(getElement(i, k) != o.getElement(i,k)){
                    return false;
                }
            }
        }
        return true;
    }
    
    
    bool Matrix::isSquare(){
        return rows == columns;
    }
    
    
    
    
    
    Matrix Matrix::operator+(Matrix &o){
        if(rows != o.rows || columns != o.columns){
            throw BadAddition("Rows or columns were not equal");
        }
        NM::Matrix m(rows, columns);
        for(int i = 0; i < rows; i++){
            for(int k = 0; k < rows; k++){
                m.setElement(i, k, getElement(i, k) + o.getElement(i, k));
            }
        }
        return m;
    }
    
    
    
    Matrix::operator std::string(){
        std::stringstream s;
        for(int i = 0; i < rows; i++){
            for(int k = 0; k < columns; k++){
                s << getElement(i, k) << '\t';
            }
            s << std::endl;
        }
        return s.str();
    }
    
    
    Matrix Matrix::operator*(NM::Matrix &other){
        if(columns != other.rows){
            throw BadMultiplication("Rows is not equal to columns");
        }
        Matrix m(rows, other.columns);
        for(int i = 0; i < m.rows; i++){
            for(int j = 0; j < m.columns; j++){
                double sum = 0;
                for(int k = 0; k < other.rows; k++){
                    sum += getElement(i, k)  * other.getElement(k, j);
                }
                m.setElement(i, j, sum);
            }
        }
        return m;
    }
}










