#include "matrix.h"
#include <cassert>

Matrix::Matrix(int nRows, int nColumns) : rows{nRows}, columns{nColumns} {
    assert(rows >= 1);
    assert(columns >= 1);
    matrix = new double*[rows];
    for (int row = 0; row<rows; row++){
        matrix[row] = new double[columns];
        for (int col = 0; col<columns; col++) {
            matrix[row][col] = 0.0;
        }
    }
}

Matrix::Matrix(int nRows): Matrix(nRows, nRows) {
    for (int row = 0; row<rows; row++) {
        matrix[row][row] = 1.0;
    }
}

Matrix::~Matrix() {
    for (int row = 0; row<rows; row++){
        delete[] matrix[row];
        matrix[row] = nullptr;
    }
    delete[] matrix;
    matrix = nullptr;

    rows = 0;
    columns = 0;
}

double Matrix::get(int row, int col) const{
    return matrix[row][col];
}

void Matrix::set(int row, int col, double value) {
    matrix[row][col] = value;
}

double *Matrix::operator[](int row){ // Valgfri oppgave
    assert(row >= 0 && row < rows);
    return matrix[row];
}

std::ostream &operator<<(std::ostream &os, const Matrix &rhs) {
    for (int row = 0; row<rhs.getRows(); row++){
        for (int col = 0; col<rhs.getColumns(); col++) {
            os << rhs.get(row,col) << '\t';
        }
        os << '\n';
    }
    return os;
}

Matrix::Matrix(const Matrix & rhs) : Matrix(rhs.getRows(), rhs.getColumns()) {
    for (int row = 0; row<rows; row++){
        for (int col = 0; col<columns; col++) {
            matrix[row][col] = rhs.get(row,col);
        }
    }
}




Matrix& Matrix::operator=(Matrix rhs) {
    std::swap(this->rows, rhs.rows);
    std::swap(this->columns, rhs.columns);
    std::swap(this->matrix, rhs.matrix);

    return *this;
}

Matrix& Matrix::operator+=(const Matrix & rhs) {
    assert(this->getRows() == rhs.getRows() && this->getColumns() == rhs.getColumns());
    for (int row = 0; row<this->rows; row++){
        for (int col = 0; col<this->columns; col++) {
            this->matrix[row][col] = this->matrix[row][col] + rhs.matrix[row][col];
        }
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix  rhs) const {
    assert(this->getRows() == rhs.getRows() && this->getColumns() == rhs.getColumns());
    return Matrix{ *this } += rhs;
}


Matrix& Matrix::operator-=(const Matrix & rhs){ 
    assert(this->getRows() == rhs.getRows() && this->getColumns() == rhs.getColumns());
    for (int row = 0; row<this->rows; row++){
        for (int col = 0; col<this->columns; col++) {
            this->matrix[row][col] = this->matrix[row][col] - rhs.matrix[row][col];
        }
    }
    return *this;
}

Matrix Matrix::operator-(const Matrix & rhs) const { 
    assert(this->getRows() == rhs.getRows() && this->getColumns() == rhs.getColumns());
    return Matrix{ *this } -= rhs;
}

Matrix Matrix::operator*(const Matrix & rhs) const { 
    assert(this->getColumns() == rhs.getRows());
    Matrix result{this->getRows(),rhs.getColumns()};
        for (int row = 0; row < this->getRows(); row++){
            for (int col = 0; col < rhs.getColumns(); col++){
                for (int i = 0; i< this->getColumns(); i++){
                    result[row][col] += this->get(row, i) * rhs.get(i,col);
                }
            }
        }
    
    // else if (this->getRows() == rhs.getRows()){
    //     Matrix result{this->getRows(),rhs.getColumns()};
    //     for (int row = 0; row < this->getRows(); row++){
    //         for (int col = 0; col < rhs.getColumns(); col++){
    //             for (int i = 0; i< this->getColumns(); i++){
    //                 result[row][col] += this->get(row, i) * rhs.get(i,col);
    //             }
    //         }
    //     }
    // }

    return result;
}

Matrix& Matrix::operator*=(const Matrix & rhs) { 
    return *this = *this * rhs;
}

Matrix Matrix::operator*(double rhs) const{ 
    for (int row = 0; row<this->rows; row++){
        for (int col = 0; col<this->columns; col++) {
            this->matrix[row][col] = this->matrix[row][col] * rhs;
        }
    }
    return *this;
}

Matrix operator*(double lhs, const Matrix & rhs){ 
    return rhs * lhs;
}

Matrix& Matrix::operator*=(double rhs){ 
    return *this = *this * rhs;
}

Matrix Matrix::operator-() const{ 
    return *this * -1.0;
}

bool Matrix::operator==(const Matrix & rhs) const{
    for (int row = 0; row<this->rows; row++){
        for (int col = 0; col<this->columns; col++) {
            if (this->matrix[row][col] != rhs.get(row,col)){
                return false;
            }
        }
    }
    return true;
}

Matrix Matrix::Transpose(){
    Matrix T_matrix(columns, rows);
    for (int row = 0; row < this->rows; row++){
        for (int col = 0; col< this->columns; col++) {
            T_matrix[col][row] = this->get(row,col);
        }
    }
    
    return T_matrix;
}