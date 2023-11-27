#pragma once
#include <iostream>
#include <utility>

//An old matrix thing I made in Norway
class Matrix {
private:
    int rows;
    int columns;
    double** matrix;
public:
    Matrix(int nRows, int nColumns);
    explicit Matrix(int nRows);
    ~Matrix();
    
    double get(int row, int col) const;
    void set(int row, int col, double value);

    double *operator[](int row);                   

    int getRows() const {return rows;};
    int getColumns() const {return columns;};

    Matrix(const Matrix & rhs);

    Matrix& operator=(Matrix rhs);
    Matrix& operator+=(const Matrix & rhs);
    Matrix operator+(const Matrix  rhs) const;

    Matrix& operator-=(const Matrix & rhs);         
    Matrix operator-(const Matrix & rhs) const;     

    Matrix operator*(const Matrix & rhs) const;     
    Matrix& operator*=(const Matrix & rhs);         
    Matrix operator*(double rhs) const;             
    Matrix& operator*=(double rhs); 
    bool operator==(const Matrix & rhs) const;             
                
    Matrix operator-() const;    

    Matrix Transpose();                   
};

std::ostream &operator<<(std::ostream &os, const Matrix &rhs);


Matrix operator*(double lhs, const Matrix & rhs);   

