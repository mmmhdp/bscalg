#!/usr/bin/env python3

import shutil
from pathlib import Path


def main():
    # Prompt for user input
    src = input("Enter source directory (default: src): ") or "src"
    build = input("Enter build directory (default: build): ") or "build"
    seminar = input("Enter seminar prefix (default: sem_): ") or "sem_"
    nseminar = input("Enter seminar number (default: 0): ") or "0"
    root = input("Enter root directory (default: .): ") or "."
    makefile = input("Enter Makefile name (default: Makefile): ") or "Makefile"
    problem_name = input(
        "Enter the problem name to delete (default: a): ") or "a"

    root_path = Path(root)
    seminar_rel_name = seminar + nseminar
    full_problem_name = seminar_rel_name + "/" + problem_name
    path_to_code = root_path / src / seminar_rel_name
    path_to_build = root_path / build / seminar_rel_name
    path_to_makefile = root_path / makefile

    # Delete the source and build directories
    if path_to_code.exists() and path_to_code.is_dir():
        shutil.rmtree(path_to_code)
        print(f"Deleted source directory: {path_to_code}")
    else:
        print(f"Source directory does not exist: {path_to_code}")

    if path_to_build.exists() and path_to_build.is_dir():
        shutil.rmtree(path_to_build)
        print(f"Deleted build directory: {path_to_build}")
    else:
        print(f"Build directory does not exist: {path_to_build}")

    # Update the Makefile
    if path_to_makefile.exists() and path_to_makefile.is_file():
        with path_to_makefile.open("r") as f:
            lines = f.readlines()

        with path_to_makefile.open("w") as f:
            in_block = False
            for line in lines:
                # Detect start of the block by matching the problem name
                if line.strip() == f"# {full_problem_name}":
                    in_block = True
                    print(f"Removing target rule block for '{
                          full_problem_name}' from the Makefile.")
                    continue  # Skip this line

                # Detect end of the block
                if line.strip() == f"# end of {full_problem_name}":
                    in_block = False
                    print(f"End of block for '{full_problem_name}'.")
                    continue  # Skip this line

                # Skip lines within the block
                if in_block:
                    continue

                # Write lines outside of the block
                f.write(line)

        print(f"Makefile updated.")


if __name__ == "__main__":
    main()
