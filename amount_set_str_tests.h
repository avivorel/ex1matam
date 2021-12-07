#ifndef AMOUNT_SET_STR_TESTS_H_
#define AMOUNT_SET_STR_TESTS_H_

#include "test_utilities.h"
#include "amount_set_str.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

bool testAsCreateDestroy();
bool testAsAddAndContainsAndSize();
bool testChangeAndGetAmount();
bool testAsDeleteAndClear();
bool allAroundTest();
bool testNoMemory();



#endif /*AMOUNT_SET_STR_TESTS_H_*/
