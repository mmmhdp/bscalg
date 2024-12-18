import os


def create_problem(problem_name):
    # Create the src directory if it doesn't exist
    src_dir = 'src'
    if not os.path.exists(src_dir):
        print(f"Creating {src_dir} directory...")
        os.mkdir(src_dir)

    # Create the problem directory inside src
    problem_dir = os.path.join(src_dir, problem_name)
    if not os.path.exists(problem_dir):
        os.mkdir(problem_dir)
    else:
        print(f"Directory for problem '{problem_name}' already exists.")
        return

    # Create the .cpp file with the "Hello world!" program
    cpp_file = os.path.join(problem_dir, f'{problem_name}.cpp')
    with open(cpp_file, 'w') as f:
        f.write('#include <iostream>\n\n'
                'int main() {\n'
                '    std::cout << "Hello, world!" << std::endl;\n'
                '    return 0;\n'
                '}')

    # Create the .h file
    h_file = os.path.join(problem_dir, f'{problem_name}.h')
    open(h_file, 'a').close()  # This creates an empty file

    # Create the .dat file
    input_file = os.path.join(problem_dir, f'{problem_name}.dat')
    open(input_file, 'a').close()  # This creates an empty file

    # Create the output .dat file
    output_file = os.path.join(problem_dir, f'{problem_name}.out.dat')
    open(output_file, 'a').close()  # This creates an empty file

    # Update CMakeLists.txt with the new format
    cmake_file = 'CMakeLists.txt'
    if not os.path.exists(cmake_file):
        print("Error: CMakeLists.txt does not exist.")
        return

    with open(cmake_file, 'a') as f:
        f.write(f'\nadd_executable({problem_name} src/{problem_name}/{
                problem_name}.cpp src/{problem_name}/{problem_name}.h)\n')
        f.write(f'target_compile_options({
                problem_name} PRIVATE -Wall -Wextra -Wpedantic -g -O0 \
                -Weffc++ -Wconversion -Wsign-conversion)\n')
        f.write(f'target_link_libraries({
                problem_name} gtest_main gmock_main)\n')

    print(f"Problem '{
          problem_name}' created successfully and CMakeLists.txt updated.")


# Example usage
if __name__ == '__main__':
    problem_name = input("Enter the problem name: ")
    create_problem(problem_name)
