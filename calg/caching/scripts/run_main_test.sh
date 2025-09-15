#! /usr/bin/env bash

while getopts s: flag
do 
	case ${flag} in 
		s) datfdir=${OPTARG};;
	esac
done

for dat_file in ${datfdir}/*.dat; do
	./build/main < ${dat_file}
done
