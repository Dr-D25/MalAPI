#include <stdio.h>

int main() {
    while (1) {
        for (int i = 0; i < 1000000; i++) {
            double result = i * i * i;
        }
    }
    return 0;
}
