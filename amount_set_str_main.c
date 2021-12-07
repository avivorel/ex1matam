#include "amount_set_str_tests.h"
#include "test_utilities.h"

/*If you would like to run an OUT_OF_MEMORY check, uncomment the #define
 * warning! the OUT_OF_MEMORY check contains an (almost) infinite loop.
 * therefore, the test might take long time to run.
 */

//#define WITH_NO_MEMORY_CHECK


int main() {
    printf("\033[0;36m");
    printf("Testing each function by it's own right now...\n\n");
    RUN_TEST(testAsCreateDestroy);
    RUN_TEST(testAsAddAndContainsAndSize);
    RUN_TEST(testChangeAndGetAmount);
    RUN_TEST(testAsDeleteAndClear);
    printf("\033[0;36m");
    printf("\n\nNow, let's run the all-around test:\n\n");
    RUN_TEST(allAroundTest);
    #ifdef WITH_NO_MEMORY_CHECK
    printf("\033[0;36m");
    printf("\n\nand an OUT-OF-MEMORY case test:\n");
    printf("\033[0;35m");
    printf("(don't panic, it can take some time till done)\n\n");
    RUN_TEST(testNoMemory);
    #endif
    printf("\n");
    printf("\033[0;34m");
    printf("************************** that's all folks! :-) ***************************\n");
    printf("\033[0;36m");
    printf("****************************************************************************\n");
    printf("\033[0;32m");
    printf("**************** if all tests shows [OK] - TEST SUCCEED :-) ****************\n");
    printf("\033[0;31m");
    printf("******** otherwise - go check the assertion failures and fix them! *********\n");
    printf("\033[0;36m");
    printf("****************************************************************************\n");
    printf("\033[0;34m");
    printf("*********************** test created by Noam Marco *************************\n");
    printf("\033[0;37m");
    return 0;
}
