#include <stdint.h>
#include <stdio.h>

/* Initialised global */
static char accumulator_ready = 0;

/* Uninitialised global */
static int32_t accumulator_result;

void reduce32_recursive(const int32_t* arr, size_t count, void (*reduce_func)(int32_t*, const int32_t)) {
    /* Initialised static local */
    static size_t loop_count = 0;
    /* Uninitialised static local */
    static int32_t accumulator;

    if (loop_count == 0) {
        accumulator = 0;
        accumulator_ready = 0;
        accumulator_result = 0xffffffff;
    }

    // second literal prevents accumulator from running if argument < 0
    if (loop_count == count || (count >> (sizeof(count) - 1))) {
        loop_count = 0; // reset counter state for next top-level call
        accumulator_ready = 1;
        accumulator_result = accumulator;
        return;
    }

    reduce_func(&accumulator, *arr);

    loop_count++;   // forget this and you get segfault
    reduce32_recursive(arr + 1, count, reduce_func);
}

void simple_reduction_function(int32_t* accumulator, const int32_t element) {
    *accumulator += element;
}

int main(int argc, char** argv) {
    const char* goodbye_message = "Welcome to this Simple C Program! where the output is %d";
    const int32_t data[] = { 1, 2, 3, 4, 5, 6 };

    reduce32_recursive(data, sizeof(data) / sizeof(data[0]), simple_reduction_function);

    printf(goodbye_message, accumulator_result);

    return 0;
}