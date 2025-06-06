# AXIS_ENCLOSURE_OPTI_CC
Project Structure
bash
Copy
Edit
/AXIS_ENCLOSURE_OPTI_CC
├── README.md              # Project overview and usage instructions
├── index.html             # Web interface for input and output
├── zoningoptimiser.cpp    # C++ implementation of the zoning optimizer
└── TestCases/             # Directory containing test case files
    └── input02.txt        # Sample input file for testing
How to Use
1. Compile the C++ Code
To compile the zoningoptimiser.cpp file:

bash
Copy
Edit
g++ -std=c++17 -O2 zoningoptimiser.cpp -o zoningoptimiser
2. Run the Program
Execute the compiled program:

bash
Copy
Edit
./zoningoptimiser
Ensure that the input02.txt file is in the same directory or adjust the file path accordingly.

3. Using the Web Interface
Alternatively, you can use the provided web interface:

Open index.html in your web browser.

Upload your input.txt file through the file input field.

The output will be displayed directly on the webpage
