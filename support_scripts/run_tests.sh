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

readonly PASSING_TEST_CASES=$(find "$TEST_PASSING_DIR" -type d -depth 1)
COUNTER_PASSED_TESTS=0

for test_case_dir in $PASSING_TEST_CASES; do
    ((COUNTER_TOTAL_TESTS++))

    test_case_name=$(basename "$test_case_dir")
    test_build_dir="$TESTS_BUILD_DIR/passingTests/$test_case_name"
    mkdir -p "$test_build_dir"
    cp $test_case_dir/* $test_build_dir

    test_case_source_code="${test_build_dir}/${test_case_name}.zyx"
    ./support_scripts/run_compiler_for_tests.sh "$test_case_source_code" &> "$test_build_dir/${test_case_name}.cli_log"
	
    exec_file="$test_build_dir/${test_case_name}"
    if [[ -x $exec_file ]]; then
        output_file="$test_build_dir/${test_case_name}.act_out"
        "$exec_file" > "$output_file" 2>&1

        actual_output="$(cat "$output_file")"
        expected_output=$(cat "$test_build_dir/${test_case_name}.exp_out")

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

readonly FAILING_TEST_FILES=$(find "$TEST_FAILING_DIR" -type d -depth 1)

for test_case_dir in $FAILING_TEST_FILES; do
    ((COUNTER_TOTAL_TESTS++))

    test_case_name=$(basename "$test_case_dir")
    test_build_dir="$TESTS_BUILD_DIR/failingTests/$test_case_name"
    mkdir -p "$test_build_dir"
    cp $test_case_dir/* $test_build_dir

    test_case_source_code="$test_build_dir/${test_case_name}.zyx"
    ./support_scripts/run_compiler_for_tests.sh "$test_case_source_code" &> "$test_build_dir/${test_case_name}.cli_log"

    exec_file="$test_build_dir/${test_case_name}"
    if [[ -x $exec_file ]]; then
        echo "[ERR] $COUNTER_TOTAL_TESTS: $test_case_name => Executable has been generated for failing test."
    else
        expected_error=$(cat "$test_build_dir/${test_case_name}.exp_err")
        # Get last line in log and cut out the timestamp part
        actual_error=$(tail -1 "$test_build_dir/${test_case_name}.cli_log" | cut -c 29-)

        if [[ "$expected_error" == "$actual_error" ]]; then
            echo "[OK]  $COUNTER_TOTAL_TESTS: $test_case_name"
            ((COUNTER_PASSED_TESTS++))
        else
            echo "[ERR] $COUNTER_TOTAL_TESTS: $test_case_name => Actual error does not match the expected error."
        fi
    fi
done

echo "---------------------------------------------"
echo "Summary"
echo "---------------------------------------------"
echo "Total tests:  $COUNTER_TOTAL_TESTS"
echo "Tests passed: $COUNTER_PASSED_TESTS"
echo "Tests failed: $(($COUNTER_TOTAL_TESTS - $COUNTER_PASSED_TESTS))"
echo "---------------------------------------------"
