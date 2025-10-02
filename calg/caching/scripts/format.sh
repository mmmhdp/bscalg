#! /usr/bin/env bash

while getopts s: flag
do 
	case ${flag} in 
		s) src=${OPTARG};;
	esac
done

find ${src} -name "*.c" -exec clang-format -style=gnu -i {} +
find ${src} -name "*.h" -exec clang-format -style=gnu -i {} +
