#! /usr/bin/env bash

for file in ../build/tests/*; do
	./${file}
done
