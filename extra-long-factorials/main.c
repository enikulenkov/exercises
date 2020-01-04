#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 128
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define COUNTOF(a) (sizeof(a)/sizeof((a)[0]))

typedef struct big_int_s {
    /*
     * Array of bytes in an unsigned big integer.
     * Every byte contains number '0..99'.
     * Least significant byte first.
     */
    unsigned char num[MAX_LEN];
    /* Bytes count in 'num' */
    size_t len;
} big_int_t;

void big_int_print(big_int_t *n)
{
    size_t i;

    /* Do not print leading zeroes for most significant byte. */
    printf("%u", n->num[n->len - 1]);

    for (i = n->len - 1; i > 0; i--) {
        printf("%02u", n->num[i-1]);
    }
}

void big_int_init_power(big_int_t *bint, unsigned int val, unsigned int pow)
{
    unsigned int remainder = 0;
    size_t i = pow;

    memset(bint, 0, sizeof(*bint));

    while (val > 0) {
        bint->num[i++] = (val % 100);
        val = val / 100;
    }
    bint->len = i;
}

void big_int_init(big_int_t *bint, unsigned int val)
{
    big_int_init_power(bint, val, 0);
}

void big_int_sum(big_int_t *n1, big_int_t *n2, big_int_t *res)
{
    size_t i;
    unsigned int remainder = 0;

    memset(res, 0, sizeof(*res));
    res->len = MAX(n1->len, n2->len);
    for (i = 0; i < res->len; i++) {
        unsigned int sum = n1->num[i] + n2->num[i];
        res->num[i] = sum % 100 + remainder;
        remainder = sum / 100;
    }
    if (remainder != 0) {
        res->num[i] = remainder;
        res->len++;
    }
}

void big_int_mul(big_int_t *n1, big_int_t *n2, big_int_t *res)
{
    size_t i, j;
    unsigned int prod;

    memset(res, 0, sizeof(*res));

    for (i = 0; i < n1->len; i++) {
        for (j = 0; j < n2->len; j++) {
            big_int_t add;
            big_int_t new_res;

            prod = n1->num[i] * n2->num[j];
            big_int_init_power(&add, prod, i + j);
            big_int_sum(&add, res, &new_res);
            memcpy(res, &new_res, sizeof(new_res));
        }
    }
}

char* readline();

void extraLongFactorials(int n) {
    big_int_t n1, res, tmp;
    int i;

    big_int_init(&res, 1);
    for (i = 2; i <= n; i++) {
        big_int_init(&n1, i);
        big_int_mul(&n1, &res, &tmp);
        memcpy(&res, &tmp, sizeof(tmp));
    }
    big_int_print(&res);
    printf("\n");
}

int main()
{
#if 1
    char* n_endptr;
    char* n_str = readline();
    int n = strtol(n_str, &n_endptr, 10);
#else
    char *n_str = "25";
    int n = 25;
    char* n_endptr = n_str+strlen(n_str);
#endif

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    extraLongFactorials(n);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) { break; }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') { break; }

        size_t new_length = alloc_length << 1;
        data = realloc(data, new_length);

        if (!data) { break; }

        alloc_length = new_length;
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
    }

    data = realloc(data, data_length);

    return data;
}