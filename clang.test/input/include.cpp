#include "header1.h"
#include "header2.h"
#include "header1.h"

int main() { }

// This is for testing File::SkippedExtents
#ifdef __SKIPPEDEXTENT1_
int skipped1 = 0;
#endif

// This is for testing File::SkippedExtents
#ifdef __SKIPPEDEXTENT2_
int skipped2 = 0;
#endif
