# What is it
A C++ matrix class for deep learning

# What its functions
1. Matrix Assignment
2. Matrix Cauculate
3. Matrix Transpose

# How to use it
## For  Call Variables Calling
    A(i, j); // Means its element's value in row i, col j
## For Creating & Assignment
    Matrix A = MatrixAssignment(const int &row, const int &col, double &num); // Create a [row*col] size matrix with each element values num
    A(i, j) = double &num; // Assign A(i, j)'s value of num
## For Outputing
    disp(A); // A should be a matrix, this funtion can output all A's elements
