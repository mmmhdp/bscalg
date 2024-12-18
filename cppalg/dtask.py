import os
import shutil


def delete_problem(problem_name):
    # Check if the src directory exists
    src_dir = 'src'
    if not os.path.exists(src_dir):
        print(f"Error: {src_dir} directory does not exist.")
        return

    # Check if the problem directory exists and delete it
    problem_dir = os.path.join(src_dir, problem_name)
    if os.path.exists(problem_dir):
        shutil.rmtree(problem_dir)
        print(f"Problem directory '{problem_name}' deleted.")
    else:
        print(f"Error: Directory for '{problem_name}' does not exist.")
        return

    # Update CMakeLists.txt
    cmake_file = 'CMakeLists.txt'
    if not os.path.exists(cmake_file):
        print("Error: CMakeLists.txt does not exist.")
        return

    # Read and rewrite CMakeLists.txt excluding the problem's add_executable and target_compile_options lines
    with open(cmake_file, 'r') as f:
        lines = f.readlines()

    with open(cmake_file, 'w') as f:
        skip_next_line = False
        skip_link_line = False
        for line in lines:
            if f'add_executable({problem_name}' in line:
                # Skip this line and the next (target_compile_options and target_link_libraries)
                skip_next_line = True
                skip_link_line = True
                print(f"Removing 'add_executable' for {problem_name}...")
            elif skip_next_line:
                skip_next_line = False  # Skip the target_compile_options line
                print(f"Removing 'target_compile_options' for {
                      problem_name}...")
            elif skip_link_line:
                skip_link_line = False  # Skip the target_link_libraries line
                print(f"Removing 'target_link_libraries' for {
                      problem_name}...")
            else:
                f.write(line)

    print(f"CMakeLists.txt updated, removed '{problem_name}' target.")

    # Remove the executable from the build directory
    build_dir = 'build'
    executable_path = os.path.join(build_dir, problem_name)
    if os.path.exists(executable_path):
        os.remove(executable_path)
        print(f"Executable '{problem_name}' deleted from 'build' directory.")
    else:
        print(f"Executable for '{
              problem_name}' not found in 'build' directory.")


# Example usage
if __name__ == '__main__':
    problem_name = input("Enter the problem name to delete: ")
    delete_problem(problem_name)
