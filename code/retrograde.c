#include "retrograde.h"

uint32_t u0 = 42;
uint32_t a = 1103515245;
uint32_t c = 12345;
uint32_t m = 1<<31;

uint32_t u(int n){
  uint32_t res = u0;
  for(int i = 0; i < n; i++){
    res = (a * res + c) % m;
  }
  return res;
}

uint32_t v7(int n) {
  uint32_t u_n = u(n);

  // Calculate (3432 * u_n) / m avoiding overflow
  // Since m = 2^31, we can simplify the division by right shifting by 31 bits
  uint64_t product = (uint64_t)3432 * u_n;
  uint32_t result = product >> 31;

  return result;
}

unsigned int coefficient_binomial(int n, int k) {
    if (k > n - k) {
        k = n - k;
    }
    unsigned int result = 1;
    for (int i = 0; i < k; ++i) {
        result *= (n - i);
        result /= (i + 1);
    }
    return result;
}

unsigned int encoder(int* p){
  unsigned int res = 0;
  int c_k = p[0];
  for(int i = 1; i < 12; i++){
    res += coefficient_binomial(c_k, i);
    c_k = p[i] + 1 + c_k;
  }
  return res;
}

void test(){
  printf("a) %u, %u\n", u(1)%37, v7(1)%37);
  printf("%u oui oui \n", u(1000));
  printf("b) %u, %u \n", u(1000)%37, v7(1000)%37);
  printf("c) %u\n", u(1000000)%37);
  return;
}
