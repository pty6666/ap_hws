#include <aphw1.h>

#include <cmath>
#include <fstream>
#include <iostream>

void show(const Matrix& a) {
  for (const auto& row : a) {
    for (const auto& element : row) {
      std::cout << element << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

Matrix getData(const char* filename) {
  std::ifstream file;
  file.open(filename);
  Matrix matrix;
  matrix.resize(10);
  for (size_t i = 0; i < 10; i++) {
    matrix[i].resize(5);
    matrix[i][0] = 1;
    double x, y;
    file >> x;
    file.get();
    file >> y;
    matrix[i][1] = x;
    matrix[i][2] = sin(x);
    matrix[i][3] = sqrt(x);
    matrix[i][4] = y;
  }
  return matrix;
}

Matrix getX(const Matrix& data) {
  Matrix X;
  X.resize(data.size());
  for (size_t i = 0; i < data.size(); i++) {
    for (size_t j = 0; j + 1 < data[i].size(); j++) {
      X[i].push_back(data[i][j]);
    }
  }
  return X;
}

Matrix gety(const Matrix& data) {
  Matrix y;
  y.resize(data.size());
  for (size_t i = 0; i < data.size(); i++) {
    y[i].push_back(data[i].back());
  }
  return y;
}

// x* y X y* z
Matrix multiply(const Matrix& a, const Matrix& b) {
  Matrix ans;
  ans.resize(a.size());
  for (size_t i = 0; i < a.size(); i++) {
    ans[i].resize(b[0].size());
    for (size_t j = 0; j < b[0].size(); j++) {
      for (size_t k = 0; k < a[0].size(); k++) {
        ans[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  return ans;
}

Matrix transpose(const Matrix& a) {
  Matrix ans;
  ans.resize(a[0].size());
  for (size_t i = 0; i < ans.size(); i++) {
    for (size_t j = 0; j < a.size(); j++) {
      ans[i].push_back(a[j][i]);
    }
  }
  return ans;
}

// double det(Matrix& a) {

// }

Matrix inv(const Matrix& a) {
  Matrix ans = a;
  for (size_t i = 0; i < a.size(); i++) {
    for (size_t j = 0; j < a.size(); j++) {
      if (i == j) {
        ans[i].push_back(1);
      } else {
        ans[i].push_back(0);
      }
    }
  }
  // show(ans);
  for (size_t i = 0; i < ans.size(); i++) {
    for (size_t j = i; j < ans.size(); j++) {
      if (ans[j][i] != 0) {
        std::swap(ans[i], ans[j]);
        break;
      }
    }
    // show(ans);
    double mul = 1 / ans[i][i];
    for (size_t j = 0; j < ans[i].size(); j++) {
      ans[i][j] *= mul;
    }
    // show(ans);
    for (size_t j = 0; j < ans.size(); j++) {
      if(j == i) continue;
      mul = ans[j][i];
      for (size_t k = 0; k < ans[j].size(); k++) {
        ans[j][k] -= ans[i][k] * mul;
      }
    }
    // show(ans);
  }
  // show(ans);
  for(size_t i = 0; i < ans.size();i++) {
    for(size_t j = 0; j < ans.size();j ++) {
      ans[i][j] = ans[i][j + ans.size()];
      ans[i].resize(ans.size());
    }
  }
  return ans;
}

Matrix solve(const char* filename) {
  auto data = getData(filename);
  auto X = getX(data);
  auto y = gety(data);
  return multiply(multiply(inv(multiply(transpose(X), X)), transpose(X)), y);
}