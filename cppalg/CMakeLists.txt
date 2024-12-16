cmake_minimum_required(VERSION 3.28)




# add_executable(Two_Sum src/Two_Sum/Two_Sum.cpp src/Two_Sum/Two_Sum.h)
# target_compile_options(Two_Sum PRIVATE -Wall -Wextra -Wpedantic -g -O0)
# target_link_libraries(Two_Sum gtest_main gmock_main)

# add_executable(Valid_Parentheses src/Valid_Parentheses/Valid_Parentheses.cpp src/Valid_Parentheses/Valid_Parentheses.h)
# target_compile_options(Valid_Parentheses PRIVATE -Wall -Wextra -Wpedantic -g -O0)
# target_link_libraries(Valid_Parentheses gtest_main gmock_main)

add_executable(Merge_Two_Sorted_Lists src/Merge_Two_Sorted_Lists/Merge_Two_Sorted_Lists.cpp src/Merge_Two_Sorted_Lists/Merge_Two_Sorted_Lists.h)
target_compile_options(Merge_Two_Sorted_Lists PRIVATE -Wall -Wextra -Wpedantic -g -O0)
target_link_libraries(Merge_Two_Sorted_Lists gtest_main gmock_main)
