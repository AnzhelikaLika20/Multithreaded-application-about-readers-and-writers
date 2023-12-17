#include <iostream>
#include  "multithreading_solution1.h"
//#include "multithreading_solution2.h"
//#include"multithreading_solution3.h"

int main(int argv, char *argc[]) {
    int number_of_readers = 0;
    int number_of_writers = 0;
    int buf_size = 0;
    std::string output_file_path;
    std::string input_file_path;
    int instruction;
    std::ifstream fin;

    if (argv > 1) {
        instruction = std::stoi(*(argc + 1));
        switch (instruction) {
            case 1:
                number_of_readers = std::stoi(*(argc + 2));
                number_of_writers = std::stoi(*(argc + 3));
                buf_size = std::stoi(*(argc + 4));
                output_file_path = *(argc + 5);
                break;
            case 2:
                input_file_path = *(argc + 2);
                fin.open(input_file_path);
                if(fin.is_open()) {
                    std::cout << "File " << input_file_path << " was opened for reading\n";
                    fin >> number_of_readers >> number_of_writers >> buf_size >> output_file_path;
                }
                break;
            default:
                std::cout << "Incorrect instruction. Enter 1 or 2.\n";
                return 0;
        }
    } else {
        std::cout << "Enter 1 - for reading parameters from the console, 2 - for using values from the config name (then you need to enter path to the config file)\n";
        std::cin >> instruction;
        switch (instruction) {
            case 1:
                std::cout << "Enter number of reader threads\n";
                std::cin >> number_of_readers;
                std::cout << "Enter number of writer threads\n";
                std::cin >> number_of_writers;
                std::cout << "Enter the size of buffer\n";
                std::cin >> buf_size;
                std::cout << "Enter the path of the output file\n";
                std::cin >> output_file_path;
                break;
            case 2:
                std::cout << "Enter the path to the config file.\n";
                std::cin >> input_file_path;
                fin.open(input_file_path);
                if(fin.is_open()) {
                    std::cout << "File " << input_file_path << " was opened for reading\n";
                    fin >> number_of_readers >> number_of_writers >> buf_size >> output_file_path;
                }
                break;
            default:
                std::cout << "Incorrect instruction. Enter 1 or 2.\n";
                return 0;
        }
    }

    start_program_mutex(number_of_readers,  number_of_writers, buf_size, output_file_path);

    //start_program_rwlock(number_of_readers,  number_of_writers, buf_size, output_file_path);

    //start_program_omp(number_of_readers,  number_of_writers, buf_size, output_file_path);

    return 0;
}
