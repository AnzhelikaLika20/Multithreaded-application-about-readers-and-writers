#include <algorithm>
#include <iostream>
#include <pthread.h>
#include <random>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//the mutex for write
//the mutex for output
pthread_mutex_t mutex_write;
pthread_mutex_t mutex_output;

std::vector<int> *buffer = nullptr;

unsigned int seed = 101;

int get_random_int(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

void write_data(std::vector<int> *buffer, int number_of_writer, int data, int index) {

    int old_data = (*buffer)[index];

    pthread_mutex_lock(&mutex_output);
    printf("Writer %d: Change value in cell [%d] from %d to %d\n", number_of_writer, index, old_data, data);
    pthread_mutex_unlock(&mutex_output);

    (*buffer)[index] = data;
    std::sort(buffer->begin(), buffer->end());
}

void *writer(void *param) {
    int number_of_writer = *((int *) param);
    int data = get_random_int(1, 20);
    int index = get_random_int(0, buffer->size() - 1);

    pthread_mutex_lock(&mutex_write);
    write_data(buffer, number_of_writer, data, index);
    pthread_mutex_unlock(&mutex_write);

    return nullptr;
}

void read_data(std::vector<int> *buffer, int number_of_reader) {
    int index = get_random_int(0, buffer->size() - 1);
    int data = (*buffer)[index];
    pthread_mutex_lock(&mutex_output);
    printf("Reader %d: Read value = %d from cell [%d]\n", number_of_reader, data, index);
    pthread_mutex_unlock(&mutex_output);
}

void *reader(void *param) {
    int number_of_reader = *((int *) param);

    read_data(buffer, number_of_reader);

    return nullptr;
}


int main(int argv, char *argc[]) {
    int number_of_readers;
    int number_of_writers;
    int buf_size;
    std::string output_file_path;
    if (argv > 1) {
        number_of_readers = std::stoi(*(argc + 1));
        number_of_writers = std::stoi(*(argc + 2));
        buf_size = std::stoi(*(argc + 3));
        output_file_path = *(argc + 4);
    } else {
        std::cout << "Enter number of reader threads\n";
        std::cin >> number_of_readers;
        std::cout << "Enter number of writer threads\n";
        std::cin >> number_of_writers;
        std::cout << "Enter the size of buffer\n";
        std::cin >> buf_size;
        std::cout << "Enter the path of the output file\n";
        std::cin >> output_file_path;
    }

    srand(seed);

    buffer = new std::vector<int>(buf_size);
    for (int i = 0; i < buf_size; ++i) {
        (*buffer)[i] = get_random_int(-10, 10);
    }

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

    return 0;
}
