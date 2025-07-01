#include <iostream>

#include "Utils_test.hpp"
#include "Geometry_test.hpp"
#include "Triangle_test.hpp"
#include "Graph_test.hpp"

#include <gtest/gtest.h>


int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}