#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <random>
#include <stdio.h>
#include <stdlib.h>

std::ofstream out;

pthread_mutex_t mutex_write;
pthread_mutex_t mutex_output;

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

    pthread_mutex_lock(&mutex_output);
    out << "Writer " << number_of_writer << ": Change value in cell [" << index << "] from " << old_data << " to " << data << "\n";
    printf("Writer %d: Change value in cell [%d] from %d to %d\n", number_of_writer, index, old_data, data);
    pthread_mutex_unlock(&mutex_output);

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

    pthread_mutex_lock(&mutex_write);
    write_data(number_of_writer, data, index);
    pthread_mutex_unlock(&mutex_write);

    return nullptr;
}

void read_data(int number_of_reader) {
    int index = get_random_int(0, buffer->size() - 1);
    int data = (*buffer)[index];

    pthread_mutex_lock(&mutex_output);
    out << "Reader " << number_of_reader << ": Read value = " << data << " from cell [" << index << "]\n";
    printf("Reader %d: Read value = %d from cell [%d]\n", number_of_reader, data, index);
    pthread_mutex_unlock(&mutex_output);
}

void *reader(void *param) {
    int number_of_reader = *((int *) param);

    read_data(number_of_reader);

    return nullptr;
}

void start_program_mutex(int number_of_readers,
                         int number_of_writers,
                         int buf_size,
                         std::string output_file_path) {
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

    pthread_mutex_init(&mutex_write, nullptr);

    pthread_mutex_init(&mutex_output, nullptr);

    pthread_t thread_writers[number_of_writers];
    int writers[number_of_writers];

    for (int i = 0; i < number_of_writers; i++) {
        writers[i] = i + 1;
        pthread_create(&thread_writers[i], nullptr, writer, (void *) &writers[i]);
    }

    pthread_t thread_readers[number_of_readers];
    int readers[number_of_readers];

    for (int i = 0; i < number_of_readers; i++) {
        readers[i] = i + 1;
        pthread_create(&thread_readers[i], nullptr, reader, (void *) &readers[i]);
    }

    for (auto &x: thread_writers)
        pthread_join(x, nullptr);

    for (auto &x: thread_readers)
        pthread_join(x, nullptr);

    out.close();
}