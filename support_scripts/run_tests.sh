#!/bin/bash

# ----- target directories paths -----
readonly TESTS_BUILD_DIR="build-test-suite"
readonly TEST_PASSING_DIR="testCases/passingTests"
readonly TEST_FAILING_DIR="testCases/failingTests"

# ----- script setup -----
COUNTER_TOTAL_TESTS=0
mkdir -p "$TESTS_BUILD_DIR"

# ----- running passing tests -----
echo "---------------------------------------------"
echo "Running Passing Tests"
echo "---------------------------------------------"

readonly PASSING_TEST_FILES=$(find "$TEST_PASSING_DIR" -type f -name "*.zyx")
COUNTER_PASSED_TESTS=0

for file in $PASSING_TEST_FILES; do
    [[ -f "$file" ]] || { printf '%s is not a file, skipping\n' "$file"; continue; }

    ((COUNTER_TOTAL_TESTS++))

    test_case_name=$(basename "$file" .zyx)
    test_build_dir="$TESTS_BUILD_DIR/passingTests/$test_case_name"
    test_case_full_path="$test_build_dir/${test_case_name}.zyx"

    mkdir -p "$test_build_dir"
    cp "$file" "$test_build_dir"

    ./support_scripts/run_compiler_for_tests.sh "$test_case_full_path" > "$test_build_dir/${test_case_name}.log" 2>&1
	
    exec_path="$test_build_dir/${test_case_name}.zyx_exec"

    if [[ -f "$exec_path" ]]; then
        output_file="$test_build_dir/${test_case_name}.out"
        "$exec_path" > "$output_file" 2>&1

        actual_output="$(cat "$output_file")"
        expected_output=$(cat "$TEST_PASSING_DIR/${test_case_name}.out")

        if [[ "$expected_output" == "$actual_output" ]]; then
            echo "[OK]  $COUNTER_TOTAL_TESTS: $test_case_name"
            ((COUNTER_PASSED_TESTS++))
        else
            echo "[ERR] $COUNTER_TOTAL_TESTS: $test_case_name =>  Output of program execution does not match the expected output."
        fi

    else
        echo "[ERR] $COUNTER_TOTAL_TESTS: $test_case_name =>  Executable file with name ${test_case_name}_exec not found in $test_build_dir directory." 
    fi
done


echo "---------------------------------------------"
echo "Running Failing Tests"
echo "---------------------------------------------"

readonly FAILING_TEST_FILES=$(find "$TEST_FAILING_DIR" -type f -name "*.zyx")

for file in $FAILING_TEST_FILES; do
    [[ -f "$file" ]] || { printf '%s is not a file, skipping\n' "$file"; continue; }
    ((COUNTER_TOTAL_TESTS++))

    test_case_name=$(basename "$file" .zyx)
    test_build_dir="$TESTS_BUILD_DIR/failingTests/$test_case_name"
    test_case_full_path="$test_build_dir/${test_case_name}.zyx"

    mkdir -p "$test_build_dir"
    cp "$file" "$test_build_dir"

    ./support_scripts/run_compiler_for_tests.sh "$test_case_full_path" > "$test_build_dir/${test_case_name}.log" 2>&1

    if find "$test_build_dir" -maxdepth 1 -type f -name '*_exec' | grep -q .; then
        echo "[ERR] $COUNTER_TOTAL_TESTS: $test_case_name => Exec file has been generated for failing test.."
    else
        echo "[OK]  $COUNTER_TOTAL_TESTS: $test_case_name"
        ((COUNTER_PASSED_TESTS++))
    fi
done


echo "---------------------------------------------"
echo "Test Summary"
echo "---------------------------------------------"
echo "Total tests: $COUNTER_TOTAL_TESTS"
echo "Tests passed: $COUNTER_PASSED_TESTS"
echo "Tests failed: $(($COUNTER_TOTAL_TESTS - $COUNTER_PASSED_TESTS))"
echo "---------------------------------------------"
