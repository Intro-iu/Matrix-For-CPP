#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <random>
#define Iter(Mat) for (int i = 0;i < Mat.row;i++) for (int j = 0;j < Mat.col;j++)

std::mt19937 random(time(0));
double RandomTheta(double l, double r){
    return l + (r - l) * random() / (INT_MAX * 2.0);
}

struct Matrix {
    int row, col;
    double * a;

    Matrix() {
        row = col = 0;
        a = nullptr;
    }

    Matrix(const double &num) {
        row = col = 1;
        a = new double[row * col];
        a[0] = num;
    }

    Matrix(const Matrix &Mat) {
        row = Mat.row;
        col = Mat.col; 
        a = new double[row * col];
        for(int i = 0;i < row*col;i++)  a[i] = Mat.a[i];
    }

    ~Matrix() {
        delete[] a;
    }

    Matrix& operator = (const Matrix &Mat) {
        if (this == &Mat)   return *this;

        delete[] a;

        row = Mat.row; col = Mat.col;
        a = new double[row * col];
        for (int i = 0;i < row * col;i++)
            a[i] = Mat.a[i];

        return *this;
    }

    double& operator () (const int &x, const int &y) {
        return a[x*col+y];
    }

    double operator () (const int &x, const int &y) const {
        return a[x*col+y];
    }
};

void disp(const Matrix &Mat) {
    for (int i = 0;i < Mat.row;i++) {
        for (int j = 0;j < Mat.col;j++) {
            std::cout << Mat(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

Matrix MatrixAssignment(int x, int y, double l, double r) {
    Matrix Mat;
    Mat.row = x; Mat.col = y;
    Mat.a = new double[x * y];

    for (int i = 0;i < x;i++)   for (int j = 0;j < y;j++)
        Mat(i, j) = RandomTheta(l, r);

    return Mat;
}

Matrix MatrixAssignment(int x, int y, double val = 0) {
    Matrix Mat;
    Mat.row = x; Mat.col = y;
    Mat.a = new double[x * y];

    for (int i = 0;i < x;i++)   for (int j = 0;j < y;j++)
        Mat(i, j) = val;

    return Mat;
}

Matrix MatrixAssignment(int x, int y, const Matrix &TargetMatrix) {
    Matrix Mat;
    Mat.row = x; Mat.col = y;
    Mat.a = new double[x * y];

    if (TargetMatrix.row == 1 && TargetMatrix.col == 1) std::cout << "Wrong!" << std::endl;

    if (TargetMatrix.row == 1 && TargetMatrix.col == y)
        for (int i = 0;i < x;i++)
            for (int j = 0;j < y;j++)
                Mat(i, j) = TargetMatrix(0, j);

    else if (TargetMatrix.col == 1 && TargetMatrix.row == x)
        for (int i = 0;i < x;i++)
            for (int j = 0;j < y;j++)
                Mat(i, j) = TargetMatrix(i, 0); 

    return Mat;
}

Matrix MatrixAssignment(int x, int y, std::initializer_list<double> val) {
    Matrix Mat;
    Mat.row = x; Mat.col = y;
    Mat.a = new double[x * y];
    
    int cnt = -1;
    for (auto itm : val) {
        int i = ++cnt / x;
        int j = cnt % x;
        Mat(i, j) = itm;
    }
    return Mat;
}

/*--------------------------------------------------------*/

Matrix operator * (const Matrix &Mat_x, const Matrix &Mat_y) {
    if (Mat_x.col != Mat_y.row)  std::cout << "Wrong multiply" << std::endl;
    
    Matrix Mat = MatrixAssignment(Mat_x.row, Mat_y.col);
    for (int i = 0;i < Mat.row;i++) for (int j = 0;j < Mat.col;j++) for (int k = 0;k < Mat_x.col;k++)
        Mat(i, j) += Mat_x(i, k) * Mat_y(k, j);

    return Mat;
}

void BroadCast(Matrix &Mat_x, Matrix &Mat_y) {
    if (Mat_x.row == Mat_y.row && Mat_x.col == Mat_y.col)   return;

    if (Mat_x.row == 1 && Mat_x.col == Mat_y.col)   Mat_x = MatrixAssignment(Mat_y.row, 1, 1) * Mat_x;
    if (Mat_x.col == 1 && Mat_x.row == Mat_y.row)   Mat_x = Mat_x * MatrixAssignment(1, Mat_y.col, 1);
    if (Mat_y.row == 1 && Mat_y.col == Mat_x.col)   Mat_y = MatrixAssignment(Mat_x.row, 1, 1) * Mat_y;
    if (Mat_y.col == 1 && Mat_y.row == Mat_x.row)   Mat_y = Mat_y * MatrixAssignment(1, Mat_x.col, 1);

    if (Mat_x.row == 1 && Mat_x.col == 1)   Mat_x = MatrixAssignment(Mat_y.row, Mat_y.col, Mat_x(0, 0));   
    if (Mat_y.row == 1 && Mat_y.col == 1)   Mat_y = MatrixAssignment(Mat_x.row, Mat_x.col, Mat_y(0, 0));   
}

Matrix operator + (const Matrix &Mat_x, const Matrix &Mat_y) {
    Matrix Mat_a = Mat_x;   Matrix Mat_b = Mat_y;
    BroadCast(Mat_a, Mat_b);
    if (Mat_a.row != Mat_b.row || Mat_a.col != Mat_b.col)   std::cout << "Wrong plus" << std::endl;

    Matrix Mat = MatrixAssignment(Mat_a.row, Mat_b.col);
    for (int i = 0;i < Mat.row * Mat.col;i++) 
        Mat.a[i] = Mat_a.a[i] + Mat_b.a[i];

    return Mat;
}

Matrix operator - (const Matrix &TargetMatrix) {
    Matrix Mat = TargetMatrix;
    for (int i = 0;i < Mat.row;i++) for (int j = 0;j < Mat.col;j++)
        Mat(i, j) = - Mat(i, j);
    return Mat;
}

Matrix operator - (const Matrix &Mat_x, const Matrix &Mat_y) {
    return Mat_x + (-Mat_y);
}

Matrix operator % (const Matrix &Mat_x, const Matrix &Mat_y) {
    Matrix Mat_a = Mat_x;   Matrix Mat_b = Mat_y;
    BroadCast(Mat_a, Mat_b);
    if (Mat_a.row != Mat_b.row || Mat_a.col != Mat_b.col)  std::cout << "Wrong %" << std::endl;
    
    Matrix Mat = MatrixAssignment(Mat_a.row, Mat_b.col);
    for (int i = 0;i < Mat.row;i++) for (int j = 0;j < Mat.col;j++)
        Mat(i, j) = Mat_a(i, j) * Mat_b(i, j);

    return Mat;
}

Matrix operator / (const Matrix &Mat_x, const Matrix &Mat_y) {
    Matrix Mat_a = Mat_x;   Matrix Mat_b = Mat_y;
    BroadCast(Mat_a, Mat_b);
    if (Mat_a.row != Mat_b.row || Mat_a.col != Mat_b.col)  std::cout << "Wrong /" << std::endl;
    
    Matrix Mat = MatrixAssignment(Mat_a.row, Mat_b.col);
    for (int i = 0;i < Mat.row;i++) for (int j = 0;j < Mat.col;j++)
        Mat(i, j) = Mat_a(i, j) / Mat_b(i, j);

    return Mat;
}

/*--------------------------------------------------------*/

Matrix exp(const Matrix &x) {
    Matrix Mat = MatrixAssignment(x.row, x.col);
    Iter(x) Mat(i, j) = exp(x(i, j));
    return Mat;
}

Matrix tanh(const Matrix &x) {
    return (exp(x)-exp(-x)) / (exp(x)+exp(-x));
}

Matrix tanh_D(const Matrix &x)  {
    return 1 - (tanh(x) % tanh(x));
}

Matrix logistic(const Matrix &x) {
    return 1 / (1 + exp(-x));
}

/*--------------------------------------------------------*/

int size(const Matrix &Mat) {
    return Mat.row * Mat.col;
}

double sumAll(const Matrix &Mat) {
    double num = 0;
    Iter(Mat)   num += Mat(i, j);
    return num;
}

Matrix sum(const Matrix &Mat, const int &opt) {
    if (opt&1)  return Mat * MatrixAssignment(Mat.col, 1, 1);
    else return MatrixAssignment(1, Mat.row, 1) * Mat;
}

double ave(const Matrix &Mat) {
    return sumAll(Mat) / size(Mat);
}

double var(const Matrix &Mat) {
    double num = 0;
    double a = ave(Mat);
    double s = size(Mat);
    Iter(Mat)   num += (Mat(i, j) - a) / s;
    return num;
}

Matrix T(const Matrix &Mat) {
    Matrix Mat_T = MatrixAssignment(Mat.col, Mat.row);
    Iter(Mat_T) Mat_T(i, j) = Mat(j, i);
    return Mat_T;
}

/*--------------------------------------------------------*/


