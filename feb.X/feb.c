#include <stdio.h>
#include <stdlib.h>

#define FEB_VERSION 2
#if FEB_VERSION == 1
#include "feb1.h"
#elif FEB_VERSION == 2
#include "feb2.h"
#endif

void main() {
#if FEB_VERSION == 1
    feb1_setup();
#elif FEB_VERSION == 2
    feb2_setup();
#endif
}

