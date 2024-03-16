#include "hw1.h"

#include <iomanip>
#include <iostream>
#include <random>

namespace algebra {

Matrix zeros(size_t n, size_t m) { return Matrix(n, std::vector<double>(m)); }

Matrix ones(size_t n, size_t m) {
  Matrix ans(n, std::vector<double>(m));
  for (auto& row : ans) {
    std::fill(row.begin(), row.end(), 1);
  }
  return ans;
}

Matrix random(size_t n, size_t m, double min, double max) {
  if (min > max) {
    throw std::logic_error{"min must <= max"};
  }
  std::mt19937 gen{std::random_device{}()};
  std::uniform_real_distribution<double> rnd(min, max);
  Matrix ans(n, std::vector<double>(m));
  for (auto& row : ans) {
    for (auto& ele : row) {
      ele = rnd(gen);
    }
  }
  return ans;
}

void show(const Matrix& matrix) {
  for (const auto& row : matrix) {
    for (const auto& element : row) {
      std::cout << std::setw(3) << element << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

Matrix multiply(const Matrix& matrix, double c) {
  auto ans = matrix;
  for (auto& row : ans) {
    for (auto& ele : row) {
      ele *= c;
    }
  }
  return ans;
}

Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
  if (matrix1.empty()) {
    return Matrix{};
  }
  if (matrix1[0].size() != matrix2.size()) {
    throw std::logic_error{
        "colunm num of matrix1 must equals to row num of matrix2"};
  }
  Matrix ans;
  ans.resize(matrix1.size());
  for (size_t i = 0; i < matrix1.size(); i++) {
    ans[i].resize(matrix2[0].size());
    for (size_t j = 0; j < matrix2[0].size(); j++) {
      for (size_t k = 0; k < matrix1[0].size(); k++) {
        ans[i][j] += matrix1[i][k] * matrix2[k][j];
      }
    }
  }
  return ans;
}

Matrix sum(const Matrix& matrix1, double c) {
  auto ans = matrix1;
  for (auto& row : ans) {
    for (auto& ele : row) {
      ele += c;
    }
  }
  return ans;
}

Matrix sum(const Matrix& matrix1, const Matrix& matrix2) {
  if (matrix1.size() != matrix2.size()) {
    throw std::logic_error{"matrix1 and matrix2 must have same size"};
  }
  if (matrix1.empty()) {
    return Matrix{};
  }
  if (matrix1[0].size() != matrix2[0].size()) {
    throw std::logic_error{"matrix1 and matrix2 must have same size"};
  }
  auto ans = matrix1;
  for (size_t i = 0; i < ans.size(); i++) {
    for (size_t j = 0; j < ans[i].size(); j++) {
      ans[i][j] += matrix2[i][j];
    }
  }
  return ans;
}

Matrix transpose(const Matrix& matrix) {
  Matrix ans;
  if (matrix.empty()) {
    return Matrix{};
  }
  ans.resize(matrix[0].size());
  for (size_t i = 0; i < ans.size(); i++) {
    for (size_t j = 0; j < matrix.size(); j++) {
      ans[i].push_back(matrix[j][i]);
    }
  }
  return ans;
}

Matrix minor(const Matrix& matrix, size_t n, size_t m) {
  Matrix ans;
  ans.resize(matrix.size() - 1);
  for (size_t i = 0; i < matrix.size(); i++) {
    for (size_t j = 0; j < matrix[i].size(); j++) {
      if (i == n || j == m) continue;
      if (i < n)
        ans[i].push_back(matrix[i][j]);
      else
        ans[i - 1].push_back(matrix[i][j]);
    }
  }
  return ans;
}

double determinant(const Matrix& matrix) {
  if (matrix.empty()) {
    return 1;
  }
  if (matrix.size() != matrix[0].size()) {
    throw std::logic_error{""};
  }
  if (matrix.size() == 1) {
    return matrix[0][0];
  }
  double ans = 0;
  int flag = 1;
  for (size_t i = 0; i < matrix.size(); i++) {
    ans += flag * matrix[0][i] * determinant(minor(matrix, 0, i));
    flag = -flag;
  }
  return ans;
}

Matrix inverse(const Matrix& matrix) {
  if (matrix.empty()) {
    return Matrix{};
  }
  if (matrix.size() != matrix[0].size()) {
    throw std::logic_error{""};
  }
  Matrix ans = matrix;
  for (size_t i = 0; i < matrix.size(); i++) {
    for (size_t j = 0; j < matrix.size(); j++) {
      if (i == j) {
        ans[i].push_back(1);
      } else {
        ans[i].push_back(0);
      }
    }
  }
  for (size_t i = 0; i < ans.size(); i++) {
    for (size_t j = i; j < ans.size(); j++) {
      if (ans[j][i] != 0) {
        std::swap(ans[i], ans[j]);
        break;
      }
    }
    if (ans[i][i] == 0) {
      throw std::logic_error{""};
    }
    // show(ans);
    double mul = 1 / ans[i][i];
    for (size_t j = 0; j < ans[i].size(); j++) {
      ans[i][j] *= mul;
    }
    // show(ans);
    for (size_t j = 0; j < ans.size(); j++) {
      if (j == i) continue;
      mul = ans[j][i];
      for (size_t k = 0; k < ans[j].size(); k++) {
        ans[j][k] -= ans[i][k] * mul;
      }
    }
    // show(ans);
  }
  // show(ans);
  for (size_t i = 0; i < ans.size(); i++) {
    for (size_t j = 0; j < ans.size(); j++) {
      ans[i][j] = ans[i][j + ans.size()];
      ans[i].resize(ans.size());
    }
  }
  return ans;
}

Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis) {
  if (axis == 0) {
    if (matrix1.empty()) {
      return matrix2;
    }
    if (matrix2.empty()) {
      return matrix1;
    }
    if (matrix1[0].size() != matrix2[0].size()) {
      throw std::logic_error{""};
    }
    auto ans = matrix1;
    for (const auto& row : matrix2) {
      ans.push_back(row);
    }
    return ans;
  } else {
    if (matrix1.size() != matrix2.size()) {
      throw std::logic_error{""};
    }
    auto ans = matrix1;
    for (size_t i = 0; i < ans.size(); i++) {
      for (auto ele : matrix2[i]) {
        ans[i].push_back(ele);
      }
    }
    return ans;
  }
}

Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2) {
  if (std::max(r1, r2) >= matrix.size()) {
    throw std::logic_error{""};
  }
  auto ans = matrix;
  std::swap(ans[r1], ans[r2]);
  return ans;
}

Matrix ero_multiply(const Matrix& matrix, size_t r, double c) {
  if (r >= matrix.size()) {
    throw std::logic_error{""};
  }
  auto ans = matrix;
  for (auto& ele : ans[r]) {
    ele *= c;
  }
  return ans;
}

Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2) {
  if (std::max(r1, r2) >= matrix.size()) {
    throw std::logic_error{""};
  }
  auto ans = matrix;
  for (size_t i = 0; i < ans[r1].size(); i++) {
    ans[r2][i] += ans[r1][i] * c;;
  }
  return ans;
}

Matrix upper_triangular(const Matrix& matrix) {
  if (matrix.empty()) {
    return Matrix{};
  }
  if (matrix.size() != matrix[0].size()) {
    throw std::logic_error{""};
  }
  auto ans = matrix;
  for (size_t i = 0; i < ans.size(); i++) {
    for(size_t j = i;j<ans.size();j++) {
      if (ans[j][i] != 0) {
        std::swap(ans[i], ans[j]);
        break;
      }
    }
    if (ans[i][i] == 0) {
      throw std::logic_error{""};
    }
    // double mul = 1 / ans[i][i];
    // for (size_t j = 0; j < ans[i].size(); j++) {
    //   ans[i][j] *= mul;
    // }
    show(ans);
    for (size_t j = i + 1; j < ans.size(); j++) {
      double mul = ans[j][i] / ans[i][i];
      for (size_t k = 0; k < ans[j].size(); k++) {
        ans[j][k] -= ans[i][k] * mul;
      }
    }
    // show(ans);
  }
  return ans;
}

}  // namespace algebra