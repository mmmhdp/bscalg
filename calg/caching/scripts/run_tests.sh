#! /usr/bin/env bash

test_list () {
	./${test_bin_dir}/test_list
}

test_hshtbl () {
	./${test_bin_dir}/test_hshtbl
}

test_cache () {
	./${test_bin_dir}/test_cache
}

while getopts "alhcb:": flag; do 
	case ${flag} in 
		a)test_list
		  test_hshtbl
		  test_cache
		  ;;
		l) test_list;;
		h) test_hshtbl;;
		c) test_cache;;
		b) test_bin_dir=${OPTARG};;
		*) ;;
	esac
done
