#!/bin/bash

TEST_DIR=test/functional/$1

# https://stackoverflow.com/questions/31968793/how-to-capitalize-first-letter-of-every-word-using-sed-in-osx
TEST_TITLE=$(echo "$1" | tr "_" " ")
TEST_TITLE=$(echo "$TEST_TITLE" | awk '{for (i=1;i<=NF;i++) $i=toupper(substr($i,1,1)) substr($i,2)} 1')

mkdir "$TEST_DIR"
echo "// $TEST_TITLE" > "$TEST_DIR/source.rn"
touch "$TEST_DIR/expected_output.txt"
file_contents=$(<test/functional/manifest.json.template)
file_contents="${file_contents//##TEST_NAME##/"$1"}"
file_contents="${file_contents//##TEST_TITLE##/"$TEST_TITLE"}"
echo "$file_contents" > "$TEST_DIR/manifest.json"