#pragma once
#include <vector>
#include <algorithm>
using Matrix = std::vector<std::vector<double>>;

Matrix multiply(const Matrix& a, const Matrix& b);

Matrix transpose(const Matrix& a);

double det(const Matrix& a);

Matrix inv(const Matrix& a);

void show(const Matrix& a);

Matrix getData(const char* filename);

Matrix getX(const Matrix&);

Matrix gety(const Matrix& data);

Matrix solve(const char* filename);