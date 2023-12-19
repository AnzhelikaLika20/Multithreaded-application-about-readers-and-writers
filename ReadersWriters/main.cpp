#include <iostream>
#include "multithreading_solution1.h" // Include the header file for the first multithreading solution
//#include "multithreading_solution2.h" // (Commented out) Include the header file for the second multithreading solution
//#include"multithreading_solution3.h" // (Commented out) Include the header file for the third multithreading solution

int main(int argv, char *argc[]) {
    // Declare variables
    int number_of_readers = 0;
    int number_of_writers = 0;
    int buf_size = 0;
    std::string output_file_path;
    std::string input_file_path;
    int instruction;
    std::ifstream fin; // File input stream

    // If command line arguments are provided
    if (argv > 1) {
        instruction = std::stoi(*(argc + 1)); // Convert the first argument to an integer
        switch (instruction) {
            case 1: // If the instruction is 1
                // Convert the next three arguments to integers and the fourth to a string
                number_of_readers = std::stoi(*(argc + 2));
                number_of_writers = std::stoi(*(argc + 3));
                buf_size = std::stoi(*(argc + 4));
                output_file_path = *(argc + 5);
                break;
            case 2: // If the instruction is 2
                input_file_path = *(argc + 2); // Set the input file path to the second argument
                fin.open(input_file_path); // Open the input file
                if(fin.is_open()) { // If the file is successfully opened
                    std::cout << "File " << input_file_path << " was opened for reading\n"; // Print a message
                    // Read the number of readers, the number of writers, the buffer size, and the output file path from the file
                    fin >> number_of_readers >> number_of_writers >> buf_size >> output_file_path;
                }
                break;
            default: // If the instruction is neither 1 nor 2
                std::cout << "Incorrect instruction. Enter 1 or 2.\n"; // Print an error message
                return 0; // Exit the program
        }
    } else { // If no command line arguments are provided
        std::cout << "Enter 1 - for reading parameters from the console, 2 - for using values from the config name (then you need to enter path to the config file)\n";
        std::cin >> instruction; // Read the instruction from the console
        switch (instruction) {
            case 1: // If the instruction is 1
                // Prompt the user to enter the number of readers, the number of writers, the buffer size, and the output file path
                std::cout << "Enter number of reader threads\n";
                std::cin >> number_of_readers;
                std::cout << "Enter number of writer threads\n";
                std::cin >> number_of_writers;
                std::cout << "Enter the size of buffer\n";
                std::cin >> buf_size;
                std::cout << "Enter the path of the output file\n";
                std::cin >> output_file_path;
                break;
            case 2: // If the instruction is 2
                std::cout << "Enter the path to the config file.\n";
                std::cin >> input_file_path; // Prompt the user to enter the input file path
                fin.open(input_file_path); // Open the input file
                if(fin.is_open()) { // If the file is successfully opened
                    std::cout << "File " << input_file_path << " was opened for reading\n"; // Print a message
                    // Read the number of readers, the number of writers, the buffer size, and the output file path from the file
                    fin >> number_of_readers >> number_of_writers >> buf_size >> output_file_path;
                }
                break;
            default: // If the instruction is neither 1 nor 2
                std::cout << "Incorrect instruction. Enter 1 or 2.\n"; // Print an error message
                return 0; // Exit the program
        }
    }

    // Start the program with the first multithreading solution
    start_program_mutex(number_of_readers,  number_of_writers, buf_size, output_file_path);

    // (Commented out) Start the program with the second multithreading solution
    //start_program_rwlock(number_of_readers,  number_of_writers, buf_size, output_file_path);

    // (Commented out) Start the program with the third multithreading solution
    //start_program_omp(number_of_readers,  number_of_writers, buf_size, output_file_path);

    return 0; // Exit the program
}