//LZ77 implementation of "A simple algorithm for computing the
//Lempel-Ziv factorization", DCC 2008

#include <iostream>
#include <cstdio>
#include <cstring>
#include "test.h"
#include "suffixArray.h"
using namespace std;

pair<pair<intT, intT>*, intT> compute(intT *A, intT n) {
  timer lzTm;
  lzTm.start();

  A[n] = 128; //let last one be the max one, so SA[n] = n, and LCP[n] = 0
  pair<intT *, intT *> SA_LCP = suffixArray(A, n + 1, false);

  intT *SA = SA_LCP.first;
  lzTm.reportNext("\tsuffix array time:");
  intT *LCP = GetLCP(A, n + 1, SA);
  lzTm.reportNext("\tlcp time:");
  SA[n] = -1;
  A[n] = '\0';

  intT *LPF = newA(intT, n);
  intT top = 0;
  intT *stack = newA(intT, n);
  stack[0] = 0;
  for (intT i = 1; i <= n; i++) { //compute LPF array
    while (top != -1 &&
           ((SA[i] < SA[stack[top]]) ||
            ((SA[i] > SA[stack[top]]) && (LCP[i] <= LCP[stack[top]])))) {
      intT stack_top = stack[top];
      if (SA[i] < SA[stack_top]) {
        LPF[SA[stack_top]] = max<intT>(LCP[i], LCP[stack_top]);
        LCP[i] = min(LCP[i], LCP[stack_top]);
      } else {
        LPF[SA[stack_top]] = LCP[stack_top];
      }
      top--;
    }
    if (i < n) stack[++top] = i;
  }

  free(stack); free(SA); free(LCP);

  //compute LZ array

  pair<intT, intT> *LZ = new pair<intT, intT>[n];

  LZ[0].first = 0; LZ[0].second = -1;
  intT j = 0;
  while (LZ[j].first < n) {
    LZ[j + 1].first = LZ[j].first + max<intT>(1, LPF[LZ[j].first]);
    LZ[j + 1].second = -1; //no prev_occ computed
    j++;
  }
  free(LPF);
  lzTm.reportNext("\tlpf && lz");
  return pair<pair<intT, intT>*, intT>(LZ, j);
}

int parallel_main(int argc, char *argv[]) {
  return test_main(argc, argv, (char *)"Seq LZ77 DCC 2008 (no prev_occ)", compute);
}

