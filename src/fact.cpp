#include <iostream>
#include <limits>

using namespace std;

namespace
{
/** Maximum value to calculate factorial */
const int32_t FACT_MAX_INPUT = 180;
}

/**
 * Calculates the factorial
 * @param number Number to calcukate factorial for
 * @return Factorial of the number as double
 *         Depending on the input the result can be
 *              NaN if number is less than zero
 *              Inf if result is too big
 */
double fact (int32_t number)
{
    if (number < 0) {
        return numeric_limits<double>::quiet_NaN();
    }
    // avoid calculation if number is too large
    if (number > FACT_MAX_INPUT) {
        return numeric_limits<double>::infinity();
    }
    double res = 1.0;
    for (int32_t i = 2; i <= number; ++i) {
        res *= i;
    }
    return res;
}

int main(int argc, char *argv[])
{
    try {
        if (argc != 2) {
            throw runtime_error("Usage: fact <number>");
        }
        char * end = nullptr;
        int32_t number = strtol(argv[1], &end, 10);
        if (end != nullptr && *end != '\0') {
            throw runtime_error("Error: can't parse argument");
        }

        cout.precision(numeric_limits<double>::max_digits10);
        cout << fact(number) << endl;
    } catch (const exception & ex) {
        cerr << ex.what() << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
