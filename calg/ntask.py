#!/usr/bin/env python3

from pathlib import Path


def main():
    src = input("Enter source directory (default: src): ") or "src"
    build = input("Enter build directory (default: build): ") or "build"
    sem = input("Enter seminar prefix (default: sem_): ") or "sem_"
    nsem = input("Enter seminar number (default: 0): ") or "0"
    root = input("Enter root directory (default: .): ") or "."
    name = input("Enter problem name (default: a): ") or "a"
    langext = input("Enter language extension (default: .c): ") or ".c"

    root_path = Path(root)
    sem_rel_name = sem + nsem
    path_to_code = root_path / src / sem_rel_name
    path_to_build = root_path / build / sem_rel_name

    path_to_code.mkdir(parents=True, exist_ok=True)
    path_to_build.mkdir(parents=True, exist_ok=True)

    filename_with_ext = name + langext
    path_to_file = path_to_code / filename_with_ext

    if not path_to_file.is_file():
        with path_to_file.open("w", encoding="utf-8") as fp:
            fp.write("int main(){}\n")


if __name__ == "__main__":
    main()
