#include <stdio.h>
#include <unistd.h>

int main() {
    for (int i = 0; i < 100000; i++) {
        // Wywołanie funkcji systemowej, aby przesunąć obliczenia do przestrzeni jądra
        getpid();
    }
    return 0;
}
