#include "omp.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <random>

std::ofstream out;

std::vector<int> *buffer = nullptr;

unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

int get_random_int(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

void write_data(int number_of_writer, int data, int index) {
    int old_data = (*buffer)[index];

#pragma omp critical(cout)
    {
        out << "Writer " << number_of_writer << ": Change value in cell [" << index << "] from " << old_data << " to " << data << "\n";
        printf("Writer %d: Change value in cell [%d] from %d to %d\n", number_of_writer, index, old_data, data);
    }

    (*buffer)[index] = data;
    std::sort(buffer->begin(), buffer->end());
}

void *writer(void *param) {
    int number_of_writer = *((int *) param);
    int left = get_random_int(-20, 20);
    int right = get_random_int(-20, 20);
    if (left > right) {
        std::swap(left, right);
    }
    int data = get_random_int(left, right);
    int index = get_random_int(0, buffer->size() - 1);

#pragma omp critical
    {
        write_data(number_of_writer, data, index);
    }

    return nullptr;
}

void read_data(int number_of_reader) {
    int index = get_random_int(0, buffer->size() - 1);
    int data = (*buffer)[index];

#pragma omp critical(cout)
    {
        out << "Reader " << number_of_reader << ": Read value = " << data << " from cell [" << index << "]\n";
        printf("Reader %d: Read value = %d from cell [%d]\n", number_of_reader, data, index);
    }
}

void *reader(void *param) {
    int number_of_reader = *((int *) param);

    read_data(number_of_reader);

    return nullptr;
}

void start_program_omp(int number_of_readers,
                       int number_of_writers,
                       int buf_size,
                       std::string &output_file_path) {
    out.open(output_file_path);
    if (out.is_open()) {
        std::cout << "The file " << output_file_path << " is successfully opened for writing" << std::endl;
    } else {
        std::cout << "There is incorrect path " << output_file_path << "\n";
    }

    srand(seed);

    buffer = new std::vector<int>(buf_size);
    for (int i = 0; i < buf_size; ++i) {

        int left = get_random_int(-20, 20);
        int right = get_random_int(-20, 20);
        if (left > right) {
            std::swap(left, right);
        }

        (*buffer)[i] = get_random_int(left, right);
    }

    std::sort(buffer->begin(), buffer->end());

#pragma omp parallel num_threads(number_of_readers + number_of_writers)
    {
        int thread_number = omp_get_thread_num();
        if (thread_number % 2 == 0) {
            writer(&thread_number);
        } else {
            reader(&thread_number);
        }
    }

    out.close();
}