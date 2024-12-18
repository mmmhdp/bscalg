#!/usr/bin/env python3

import os
from pathlib import Path


def main():
    src = input("Enter source directory (default: src): ") or "src"
    build = input("Enter build directory (default: build): ") or "build"
    seminar = input("Enter seminar prefix (default: sem_): ") or "sem_"
    nseminar = input("Enter seminar number (default: 0): ") or "0"
    root = input("Enter root directory (default: .): ") or "."
    problem_name = input(
        "Enter the problem name to delete (default: a): ") or "a"

    root_path = Path(root)
    seminar_rel_name = seminar + nseminar
    path_to_code = root_path / src / seminar_rel_name / problem_name
    path_to_bin = root_path / build / seminar_rel_name / problem_name

    if path_to_code.exists():
        os.remove(path_to_code)
        print(f"Deleted source: {path_to_code}")
    else:
        print(f"Source does not exist: {path_to_code}")

    if path_to_bin.exists():
        os.remove(path_to_bin)
        print(f"Deleted bin: {path_to_bin}")
    else:
        print(f"Bin does not exist: {path_to_bin}")


if __name__ == "__main__":
    main()
