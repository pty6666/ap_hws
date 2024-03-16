#include "aphw1.h"

#include <limits.h>

#include <iomanip>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"
namespace {
TEST(APHW1Test, Test1) {
  Matrix data{getData("data.txt")};
  show(data);
  EXPECT_EQ(1, data[0][0]);
  EXPECT_EQ(true, (103.4 < data[1][4]) && (103.6 > data[1][4]));
}

TEST(APHW1Test, Test2) {
  Matrix data{getData("data.txt")};
  Matrix X{getX(data)};
  EXPECT_EQ(true, (1.05 < data[1][3]) && (1.06 > data[1][3]));
}

TEST(APHW1Test, Test3) {
  Matrix data{getData("data.txt")};
  Matrix y{gety(data)};
  EXPECT_EQ(true, (103.4 < y[1][0]) && (103.6 > y[1][0]));
}

TEST(APHW1Test, Test4) {
    Matrix data = getData("data.txt");
    show(data);
    show(transpose(data));
}

TEST(APHW1Test, Test5) {
    Matrix temp1, temp2;
    temp1.resize(5);
    temp2.resize(5);
    for(size_t i = 0; i < temp1.size();i++) {
        for(size_t j = 0; j < temp1.size();j++){
            temp1[i].push_back(rand() % 100 / 10.0);
        }
    }
    show(multiply(inv(temp1), temp1));
}

TEST(APHW1Test, Test6)
{
    Matrix w{solve("data.txt")};
    EXPECT_EQ(true, (0.82 < w[1][0])&&(0.84 > w[1][0]));
}
}