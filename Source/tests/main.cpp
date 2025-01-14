#include <gtest/gtest.h>


struct a
{
	int a = 3;
	int b = 4;
};


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}