#!/usr/bin/env bash

while getopts s: flag; do 
	case ${flag} in 
		s) datfdir=${OPTARG};;
	esac
done

for dat_file in "${datfdir}"/*.dat; do
    base_name="${dat_file%.dat}"
    ans_file="${base_name}.ans"

    echo "Running test: $(basename "$dat_file")"

    output=$(./build/main < "$dat_file")

    if diff -q <(echo "$output") "$ans_file" > /dev/null; then
        echo "Test passed: $(basename "$dat_file")"
    else
        echo "Test failed: $(basename "$dat_file")"
        echo "Differences:"
        diff -y <(echo "$output") "$ans_file"
#        exit 1
    fi

    echo
done

echo "Tests passed successfully!"

