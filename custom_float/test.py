import random
import subprocess

# Define test cases as tuples (input1, input2, expected_result)
test_cases = [
    (0.0, 0.0, 0.0),
    (1.0, 2.0, 3.0),
    (-1.0, 1.0, 0.0),
    (3.5, -2.5, 1.0),
    (-1.5, -1.5, -3.0),
    # Add more test cases as needed
]

# Path to the compiled C executable
executable_path = './custom_float.out'

# Function to run a test case using the executable
def run_test(input1, input2):
    process = subprocess.Popen(
        executable_path,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    # Send two numbers to the C++ program
    output, errors = process.communicate(f'{input1} {input2}')
    # Close the process
    process.stdin.close()

    # Check if there are any errors
    if errors:
        print(f"Error occurred: {errors}")
        return None

    # Parse the output result
    lines = output.strip().split('\n')
    last_line = lines[-1]
    result_str = last_line.split("Add ")[-1]
    try:
        result = float(result_str)
    except ValueError:
        print(f"Failed to convert '{result_str}' to float")
        return None

    return result

# Run all test cases
for i, (input1, input2, expected) in enumerate(test_cases):
    result = run_test(input1, input2)

    if result is None:
        print(f"Test {i}: Error during execution")
    elif abs(result - expected) < 1e-6:  # Use a tolerance for floating-point comparisons
        print(f"Test {i}: Passed")
    else:
        print(f"Test {i}: Failed")
        print(f"\tExpected {expected}, got {result}")
        print(f"\t a = {input1}, b = {input2}")

# Function to generate a random floating-point number
def generate_random_float():
    return random.uniform(-1e5, 1e5)  # Adjust the range according to your requirements

# Function to generate and run random test cases
def generate_and_run_tests(num_cases):
    for i in range(num_cases):
        # Generate two random inputs
        input1 = generate_random_float()
        input2 = generate_random_float()
        # Calculate the expected result using Python's addition
        expected = input1 + input2

        # Run the test
        result = run_test(input1, input2)

        if result is None:
            print(f"Random test {i}: Error during execution")
        elif abs(result - expected) < 1e-5 * abs(expected):  # Use a tolerance for floating-point comparisons
            print(f"Random test {i}: Passed")
        else:
            print(f"Random test {i}: Failed")
            print(f"\tExpected {expected}, got {result}")
            print(f"\ta = {input1}, b = {input2}")

generate_and_run_tests(200)
