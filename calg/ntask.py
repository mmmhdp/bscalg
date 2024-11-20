#!/usr/bin/env python3

import subprocess
from pathlib import Path


def main():
    # Prompt for user input
    src = input("Enter source directory (default: src): ") or "src"
    build = input("Enter build directory (default: build): ") or "build"
    sem = input("Enter seminar prefix (default: sem_): ") or "sem_"
    nsem = input("Enter seminar number (default: 0): ") or "0"
    root = input("Enter root directory (default: .): ") or "."
    name = input("Enter problem name (default: a): ") or "a"
    langext = input("Enter language extension (default: .c): ") or ".c"
    makefile = input("Enter Makefile name (default: Makefile): ") or "Makefile"

    root_path = Path(root)
    sem_rel_name = sem + nsem
    path_to_code = root_path / src / sem_rel_name
    path_to_build = root_path / build / sem_rel_name

    # Create source and build directories
    path_to_code.mkdir(parents=True, exist_ok=True)
    path_to_build.mkdir(parents=True, exist_ok=True)

    # Create the main source file
    filename_with_ext = name + langext
    path_to_file = path_to_code / filename_with_ext

    if not path_to_file.is_file():
        with path_to_file.open("w", encoding="utf-8") as fp:
            fp.write("int main(){}\n")

    # Update the Makefile
    path_to_makefile = root_path / makefile
    filename_for_make = sem_rel_name + "/" + name

    # Check if the Makefile already exists
    if path_to_makefile.exists() and path_to_makefile.is_file():
        with path_to_makefile.open("r", encoding="utf-8") as fp:
            lines = fp.readlines()

        # Check if the target already exists to avoid duplicates
        if any(f"curr := {filename_for_make}" in line for line in lines):
            print(
                f"Target '{filename_for_make}' already exists in the Makefile.")
            return

        with path_to_makefile.open("a", encoding="utf-8") as fp:
            fp.write(f"\n# {filename_for_make}\n")
            fp.write(f"curr := {filename_for_make}\n")
            fp.write("CC = gcc \n")
            fp.write(
                "CFLAGS = -g -lc -lm -Wall -Werror -O0 -fsanitize=address -static-libasan -Werror=declaration-after-statement\n")
            fp.write(f"{filename_for_make}:\n")
            fp.write(
                f"\n$(curr): src/$(curr).c \n\t$(CC) $(CFLAGS) src/$(curr).c -o build/$(curr)\n")
            fp.write(f"\n# end of {filename_for_make}\n")

    else:
        # Create a new Makefile
        with path_to_makefile.open("w", encoding="utf-8") as fp:
            fp.write(f"\n# {filename_for_make}\n")
            fp.write(f"curr := {filename_for_make}\n")
            fp.write("CC = gcc \n")
            fp.write(
                "CFLAGS = -g -lc -lm -Wall -Werror -O0 -fsanitize=address -static-libasan -Werror=declaration-after-statement\n")
            fp.write(f"{filename_for_make}:\n")
            fp.write(
                f"\n$(curr): src/$(curr).c \n\t$(CC) $(CFLAGS) src/$(curr).c -o build/$(curr)\n")
            fp.write(f"\n# end of {filename_for_make}\n")

    # Run make
    subprocess.run(["make"])


if __name__ == "__main__":
    main()
