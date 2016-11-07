#include "gtest/gtest.h"
#include "../src/Matrix.hpp"
#include "../src/utility.hpp"

/*! I use the constructor to initialise the four 4 different matrix.
 * This matrix will be use to check if our function work as expected because 
 * the different values test are the value we expect for the corresponding
 *  matrix when we applied the different function.
 */
Matrix_Neo r({{0.259260,0.000,0.148148,1.000}});
Matrix ma_matrice_1(r, relativeMatrix);

Matrix_Neo a({{0.18421,0.000,0.105263, 0.710526}});
Matrix ma_matrice_2(a, absoluteMatrix);

Matrix_Neo lm({{-0.440569,MINUSINFINI,-1.247930,1.506960}});
Matrix ma_matrice_3(lm,logMatrix);

Matrix_Neo lcm({{-1.947529,MINUSINFINI,-2.754889,0.000}});
Matrix ma_matrice_4(lcm,logConstMatrix);

SimpleVector value_test_1({1.407408});
SimpleVector value_test_2({0.710526});
SimpleVector value_test_3({1.407408});
SimpleVector value_test_4({1.506960});			
/*!
 *@brief Function testing if calcul_sum() returns the expected value, using a relativematrix
 */
TEST (calcul_sum_Test, PositiveSum)
{
	ASSERT_EQ(value_test_1,ma_matrice_1.calcul_sum());
}
/*!
 *@brief Function testing if max_values() returns the max value of the line, using an absoluteMatrix
 */
TEST (max_values_Test, MaxValueabsoluteMatrix)
{
	ASSERT_EQ(value_test_2,ma_matrice_2.max_values());
}
/*!
 *@brief Function testing if sum_pow2logConstMatrix() returns the expected value, using a logConstMatrix
 */
TEST (sum_pow2logConstMatrix_Test, Positivepow )
{
	ASSERT_EQ(value_test_3,ma_matrice_4.sum_pow2logConstMatrix());
}
/*!
 *@brief Function testing if logMatrix_max_values() return the max value of the line, using a logMatrix
 */
TEST (logMatrix_max_values_Test, MaxValuelogMatrix)
{
	ASSERT_EQ(value_test_4,ma_matrice_3.logMatrix_max_values());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}