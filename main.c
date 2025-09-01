/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ROOTS 100

typedef struct {
    int id;
    int base;
    char value[50];
    double root_value;
} Root;

int convertToDecimal(const char *value, int base) {
    int result = 0;
    while (*value) {
        char c = *value;
        int digit;
        if (c >= '0' && c <= '9')
            digit = c - '0';
        else
            digit = (c - 'A') + 10;  // (not needed here but safe)

        result = result * base + digit;
        value++;
    }
    return result;
}

int main() {
    FILE *fp = fopen("input.json", "r");
    if (!fp) {
        printf("Error: Could not open input.json\n");
        return 1;
    }

    char buffer[4096];
    fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    int n = 0, k = 0;
    Root roots[MAX_ROOTS];
    int rootCount = 0;

    // -------- Parse n and k --------
    char *ptr = strstr(buffer, "\"n\"");
    if (ptr) sscanf(ptr, "\"n\" : %d", &n);

    ptr = strstr(buffer, "\"k\"");
    if (ptr) sscanf(ptr, "\"k\" : %d", &k);

    // -------- Parse roots --------
    ptr = buffer;
    while ((ptr = strstr(ptr, "\"base\""))) {
        Root r;
        // get id from the block start
        char *id_start = ptr;
        while (id_start > buffer && *id_start != '{') id_start--;
        sscanf(id_start, "\"%d\"", &r.id);

        // parse base
        sscanf(ptr, "\"base\" : \"%d\"", &r.base);

        // parse value
        char *val_ptr = strstr(ptr, "\"value\"");
        sscanf(val_ptr, "\"value\" : \"%49[^\"]\"", r.value);

        // convert value to decimal
        r.root_value = convertToDecimal(r.value, r.base);

        roots[rootCount++] = r;
        ptr = val_ptr + 1;
    }

    // -------- Show parsed data --------
    printf("n = %d, k = %d\n", n, k);
    printf("Parsed roots:\n");
    for (int i = 0; i < rootCount; i++) {
        printf("Root id=%d, base=%d, value=%s -> decimal=%g\n",
               roots[i].id, roots[i].base, roots[i].value, roots[i].root_value);
    }

    // -------- Polynomial coefficient calculation --------
    // We need only k roots (if more are given, we pick first k valid ones).
    if (rootCount < k) {
        printf("Error: Not enough valid roots!\n");
        return 1;
    }

    double coeffs[MAX_ROOTS+1];  // polynomial coefficients
    for (int i = 0; i <= k; i++) coeffs[i] = 0;
    coeffs[0] = 1;  // leading coefficient for x^k

    for (int i = 0; i < k; i++) {
        double r = -roots[i].root_value;
        for (int j = i+1; j > 0; j--) {
            coeffs[j] = coeffs[j] + r * coeffs[j-1];
        }
    }

    printf("\nPolynomial coefficients (degree %d):\n", k);
    for (int i = 0; i <= k; i++) {
        printf("a[%d] = %g\n", i, coeffs[i]);
    }

    return 0;
}
