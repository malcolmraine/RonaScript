#!/bin/bash

TEST_DIR=test/functional/$1
mkdir "$TEST_DIR"
touch "$TEST_DIR/source.rn"
touch "$TEST_DIR/expected_output.txt"