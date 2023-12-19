#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define EXPONENT_BIT 8
#define SIGNIFICANT_BIT (32 - 1 - EXPONENT_BIT)

typedef struct {
    uint32_t significant : SIGNIFICANT_BIT;
    uint32_t exponent : EXPONENT_BIT;
    uint32_t sign : 1;
} CustomFloat;

union RawFloat {
    float a;
    CustomFloat b;
};

float custom_to_float(CustomFloat cf) {
    int32_t exp_bias = (1 << (EXPONENT_BIT - 1)) - 1;
    float significand = 1.0 + (float)(cf.significant) / (1 << SIGNIFICANT_BIT);
    float exponent = powf(2.0, (float)((int32_t)cf.exponent - exp_bias));
    return (cf.sign ? -1.0 : 1.0) * significand * exponent;
}

CustomFloat custom_float_add(CustomFloat cf1, CustomFloat cf2) {
    bool not_same_sign = cf1.sign != cf2.sign;
    // Ensure cf1 has a greater or equal exponent than cf2
    if (cf1.exponent < cf2.exponent) {
        CustomFloat tmp = cf1;
        cf1 = cf2;
        cf2 = tmp;
    }

    // Add the implicit leading 1 for normalized numbers
    uint32_t cf1_significant = cf1.significant | (1 << SIGNIFICANT_BIT);
    uint32_t cf2_significant = cf2.significant | (1 << SIGNIFICANT_BIT);

    // Align the significands
    uint32_t align_shift = cf1.exponent - cf2.exponent;
    cf2_significant >>= align_shift;

    // Add significands
    uint32_t result_significant;
    if (not_same_sign) {
        if (cf1_significant < cf2_significant) {
            result_significant = cf2_significant - cf1_significant;
            cf1.sign = cf2.sign;
        } else {
            result_significant = cf1_significant - cf2_significant;
        }
        if (result_significant == 0) {
            cf1.significant = 0;
            cf1.exponent = 0;
            return cf1;
        }
    } else {
        result_significant = cf2_significant + cf1_significant;
    }

    if (!not_same_sign && result_significant & (1 << (SIGNIFICANT_BIT + 1))) {
        result_significant >>= 1;
        cf1.exponent++;
        if (cf1.exponent >= (1u << EXPONENT_BIT)) {
            cf1.exponent = (1u << EXPONENT_BIT) - 1;
            cf1.significant = 0;  // infinity or max value
            return cf1;
        }
    }

    while (not_same_sign && !(result_significant & (1 << (SIGNIFICANT_BIT)))) {
        result_significant <<= 1;
        cf1.exponent--;
        if (cf1.exponent == 0) {
            break;
        }
    }

    // Set the new significand, removing the implicit leading 1
    cf1.significant = result_significant & ~(1 << SIGNIFICANT_BIT);

    return cf1;
}

int main() {
    union RawFloat rf1;
    union RawFloat rf2;
    scanf("%f %f", &rf1.a, &rf2.a);

    printf("a=%f; b=%f\n", custom_to_float(rf1.b), custom_to_float(rf2.b));
    printf("Add %f\n", custom_to_float(custom_float_add(rf1.b, rf2.b)));
}
