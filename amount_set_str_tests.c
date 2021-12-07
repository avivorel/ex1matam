#include "amount_set_str_tests.h"
#include "test_utilities.h"
/*If you would like to run an OUT_OF_MEMORY check, uncomment the #define
 * warning! the OUT_OF_MEMORY check contains an (almost) infinite loop.
 * therefore, the test might take long time to run.
 */

//#define WITH_NO_MEMORY_CHECK

#define VERY_BIG_NEGATIVE -13232321

//random string generatoramount_set_str.c amount_set_str_main.c amount_set_str_tests.c
static void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}



bool testAsCreateDestroy() {
    AmountSet set = asCreate();
    ASSERT_TEST(set != NULL);
    ASSERT_TEST(asGetSize(set) == 0);
    ASSERT_TEST(asGetFirst(set) == NULL);

    asDestroy(set);
    return true;
}

bool testAsAddAndContainsAndSize() {
    AmountSet rand_set = asCreate();
    for (int key = 1 ; key < 1000 ; ++key)
    {
        char str[] = { [41] = '\1' };
        rand_str(str , sizeof str - 1);
        assert(str[41] == '\0');
        ASSERT_TEST(asRegister(rand_set,(const char* )str) == AS_SUCCESS) ;
        ASSERT_TEST(asContains(rand_set,str)==true) ;
        ASSERT_TEST(asGetSize(rand_set) == key);
    }
    char* previous = asGetFirst(rand_set);
    char* current = asGetNext(rand_set);
    while (current != NULL) {
        ASSERT_TEST(strcmp(previous,current)<0);
        previous = current;
        current = asGetNext(rand_set);
    }
    asDestroy(rand_set);
    AmountSet set = asCreate();
    ASSERT_TEST(asRegister(NULL, "chair") == AS_NULL_ARGUMENT);
    ASSERT_TEST(asRegister(set, NULL) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asRegister(set, "chair")==AS_SUCCESS);
    ASSERT_TEST(asRegister(set, "chair") == AS_ITEM_ALREADY_EXISTS);
    ASSERT_TEST(asRegister(set, "Chair") == AS_SUCCESS);
    char* chair = "CHAIR";
    ASSERT_TEST(asRegister(set, chair) == AS_SUCCESS);
    asRegister(set, "closet");
    char* desk = "desk";
    asRegister(set, desk);
    asRegister(set, "toilet seat #4328");
    ASSERT_TEST(asContains(set, desk) == true);
    ASSERT_TEST(asContains(set, "clOset") == false);
    ASSERT_TEST(asContains(set, "toilet seat #4329") == false);
    ASSERT_TEST(asContains(set, "toilet seat #4328") == true);
    ASSERT_TEST(asContains(set, "Gaming Chair") == false);
    ASSERT_TEST(asContains(NULL, "desk") == false);
    ASSERT_TEST(asContains(set, NULL) == false);
    desk="DESK";
    ASSERT_TEST(asContains(set, "DESK")==false);
    ASSERT_TEST(asContains(set, "desk")==true);
    chair="harta-barta";
    ASSERT_TEST(asContains(set,chair)==false);
    ASSERT_TEST(strcmp(asGetFirst(set),"CHAIR")==0);
    asDestroy(set);
    return true;
}

bool testChangeAndGetAmount() {
    AmountSet set = asCreate();
    asRegister(set, "chair");
    asRegister(set, "closet");
    char *desk = "desk";
    asRegister(set, desk);
    asRegister(set, "toilet seat #4328");
    double amount = rand();
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 0);
    amount = rand();
    ASSERT_TEST(asGetAmount(set, "closet", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 0);
    amount = rand();
    ASSERT_TEST(asGetAmount(set, desk, &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 0);
    amount = rand();
    ASSERT_TEST(asGetAmount(set, "toilet seat #4328", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 0);
    amount = rand();
    ASSERT_TEST(asGetAmount(NULL, "toilet seat #4328", &amount) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, NULL, &amount) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, "toilet seat #4328", NULL) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, "toilet seat #4329", NULL) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, "toilet Seat #4329", &amount) == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(asChangeAmount(NULL, "toilet Seat #4328", 10) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asChangeAmount(set, NULL, 10) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asChangeAmount(set, "toilet seat #4329", 10) == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(asChangeAmount(set, "chair", 10) == AS_SUCCESS);
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 10);
    amount=rand();
    ASSERT_TEST(asChangeAmount(set, "chair", -100) == AS_INSUFFICIENT_AMOUNT);
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 10);
    ASSERT_TEST(asChangeAmount(set, "chair", -9) == AS_SUCCESS);
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 1);
    ASSERT_TEST(asChangeAmount(NULL, "chair", -100) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 1);
    ASSERT_TEST(asChangeAmount(set, NULL, -100) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetAmount(set, "chair", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 1);
    ASSERT_TEST(asChangeAmount(set, "GAMING CHAIR", -100) == AS_ITEM_DOES_NOT_EXIST);
    amount = rand();
    ASSERT_TEST(asChangeAmount(set, "desk", 2) == AS_SUCCESS);
    ASSERT_TEST(asGetAmount(set, desk, &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 2);
    amount = rand();
    ASSERT_TEST(asChangeAmount(set, desk, -2) == AS_SUCCESS);
    ASSERT_TEST(asContains(set, desk) == true);
    ASSERT_TEST(asGetAmount(set, "desk", &amount) == AS_SUCCESS);
    ASSERT_TEST(amount == 0);

    asDestroy(set);
    return true;
}

bool testAsDeleteAndClear() {
    AmountSet office = asCreate();
    AmountSet bedroom = asCreate();
    AmountSet toilet_and_shower = asCreate();
    asRegister(office, "chair");
    asRegister(bedroom, "closet");
    char *desk = "desk";
    asRegister(office, desk);
    asRegister(toilet_and_shower, "toilet seat #4328");
    asRegister(office, "pencil holder");
    asRegister(office, "mouse pad");
    asRegister(bedroom, "bed");
    asRegister(bedroom, "dresser");
    asRegister(bedroom, "night light");
    asRegister(toilet_and_shower, "13th century Roman sink");
    asRegister(toilet_and_shower, "shower head (round)");
    asRegister(toilet_and_shower, "toilet heater");
    asRegister(toilet_and_shower, "shower head (square)");
    asRegister(toilet_and_shower, "towel holder");
    int office_set_size = asGetSize(office);
    int toilet_and_shower_set_size = asGetSize(toilet_and_shower);
    asRegister(office, "towel holder");
    ASSERT_TEST(asDelete(bedroom, "towel holder") == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(asDelete(NULL, "towel holder") == AS_NULL_ARGUMENT);
    ASSERT_TEST(asDelete(office, NULL) == AS_NULL_ARGUMENT);
    ASSERT_TEST(asDelete(office, "towel holder") == AS_SUCCESS);
    ASSERT_TEST(asGetSize(office) == office_set_size);
    ASSERT_TEST(asContains(office, "towel holder") == false);
    ASSERT_TEST(asDelete(toilet_and_shower, "shower head (round)") == AS_SUCCESS);
    ASSERT_TEST(asGetSize(toilet_and_shower) == toilet_and_shower_set_size - 1);
    ASSERT_TEST(asContains(toilet_and_shower, "shower head (round)") == false);
    ASSERT_TEST(asClear(bedroom) == AS_SUCCESS);
    ASSERT_TEST(asContains(bedroom, "dresser") == false);
    ASSERT_TEST(asContains(bedroom, "night light") == false);
    ASSERT_TEST(asGetSize(bedroom) == 0);
    ASSERT_TEST(asGetFirst(bedroom) == NULL);
    ASSERT_TEST(asGetNext(bedroom) == NULL);
    ASSERT_TEST(strcmp(asGetFirst(office), "chair") == 0);
    ASSERT_TEST(asDelete(office, "chair") == AS_SUCCESS);
    ASSERT_TEST(strcmp(asGetFirst(office), "chair") > 0);
    ASSERT_TEST(strcmp(asGetFirst(office), "desk") == 0);
    ASSERT_TEST(asGetSize(office) == office_set_size - 1);

    asDestroy(office);
    asDestroy(bedroom);
    asDestroy(toilet_and_shower);
    return true;
}

bool allAroundTest() {
    AmountSet badroom = asCreate();
    AmountSet office = asCreate();
    asRegister(badroom, "king-size bed");
    asRegister(badroom, "queen-size bed");
    asRegister(badroom, "double bed");
    asRegister(badroom, "single bed");
    asRegister(badroom, "bunk beds");
    asRegister(badroom, "gallery bed");
    asRegister(badroom, "pillow #328");
    asRegister(badroom, "pillow #142");
    asRegister(badroom, "pillow #906");
    asRegister(badroom, "orthopedic pillow #086");
    asRegister(badroom, "orthopedic pillow #094");
    asRegister(badroom, "mattress 220X140");
    asRegister(badroom, "mattress 220X160");
    asRegister(badroom, "mattress 220X180");
    asRegister(badroom, "mattress 180X140");
    ASSERT_TEST(strcmp("bunk beds", asGetFirst(badroom))==0);
    ASSERT_TEST(strcmp("double bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("gallery bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("king-size bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 180X140", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 220X140", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 220X160", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 220X180", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("orthopedic pillow #086", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("orthopedic pillow #094", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("pillow #142", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("pillow #328", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("pillow #906", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("queen-size bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("single bed", asGetNext(badroom))==0);
    ASSERT_TEST(asRegister(badroom, "pillow #142")==AS_ITEM_ALREADY_EXISTS);
    ASSERT_TEST(asRegister(badroom, "king-size bed")==AS_ITEM_ALREADY_EXISTS);
    ASSERT_TEST(asRegister(badroom, NULL)==AS_NULL_ARGUMENT);
    char* null_string=NULL;
    ASSERT_TEST(asRegister(badroom, null_string)==AS_NULL_ARGUMENT);
    AmountSet null_set=NULL;
    ASSERT_TEST(asGetSize(null_set)==-1);
    ASSERT_TEST(asRegister(null_set, "regular bed")==AS_NULL_ARGUMENT);
    ASSERT_TEST(asClear(null_set)==AS_NULL_ARGUMENT);
    ASSERT_TEST(asGetFirst(null_set)==NULL);

    double current_amount=rand();
    char* current=asGetFirst(badroom);
    while (current != NULL) {
        asGetAmount(badroom, current, &current_amount);
        ASSERT_TEST(current_amount==0);
        current_amount=rand();
        current = asGetNext(badroom);
    }
    double add_amount=(double)rand();
    char* current_item=asGetFirst(badroom);
    asChangeAmount(badroom, current_item, add_amount);
    asGetAmount(badroom, current_item, &current_amount);
    ASSERT_TEST(current_amount==add_amount);
    for (int i=1; i< asGetSize(badroom); i++){
        add_amount=(double)rand();
        current_item=asGetNext(badroom);
        asChangeAmount(badroom, current_item, add_amount);
        asGetAmount(badroom, current_item, &current_amount);
        ASSERT_TEST(current_amount==add_amount);
    }
    AmountSet bedroom= asCopy(badroom);
    ASSERT_TEST(asGetSize(badroom) == asGetSize(bedroom));
    ASSERT_TEST(asRegister(bedroom, "pillow #142")==AS_ITEM_ALREADY_EXISTS);
    ASSERT_TEST(asRegister(bedroom, "king-size bed")==AS_ITEM_ALREADY_EXISTS);
    ASSERT_TEST(strcmp("bunk beds", asGetFirst(badroom))==0);
    ASSERT_TEST(strcmp("double bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("gallery bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("king-size bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 180X140", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 220X140", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 220X160", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("mattress 220X180", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("orthopedic pillow #086", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("orthopedic pillow #094", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("pillow #142", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("pillow #328", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("pillow #906", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("queen-size bed", asGetNext(badroom))==0);
    ASSERT_TEST(strcmp("single bed", asGetNext(badroom))==0);
    double badroom_curr_amount;
    double bedroom_curr_amount;
    char* badroom_current=asGetFirst(badroom);
    char* bedroom_current=asGetFirst(badroom);
    while (badroom_current != NULL && bedroom_current != NULL ) {
        asGetAmount(badroom, badroom_current, &badroom_curr_amount);
        asGetAmount(bedroom, bedroom_current, &bedroom_curr_amount);
        ASSERT_TEST(badroom_curr_amount==bedroom_curr_amount);
        badroom_current = asGetNext(badroom);
        bedroom_current = asGetNext(bedroom);
    }
    asDelete(badroom, "king-size bed");
    ASSERT_TEST(asGetSize(badroom) == asGetSize(bedroom)-1);
    asClear(badroom);
    ASSERT_TEST(asGetSize(badroom)==0);
    ASSERT_TEST(asGetFirst(badroom)==NULL);
    ASSERT_TEST(asContains(badroom, "king-size bed")==false);
    ASSERT_TEST(asChangeAmount(badroom, "queen-size bed", (double)VERY_BIG_NEGATIVE) == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(asChangeAmount(badroom, asGetNext(badroom), (double)VERY_BIG_NEGATIVE) == AS_NULL_ARGUMENT);
    asDestroy(badroom);
    asRegister(office, "WOODEN desk");
    asRegister(office, "PLASTIC desk");
    asRegister(office, "ALUMINUM desk");
    asRegister(office, "office chair with 5 wheels");
    asRegister(office, "office chair with 3 wheels");
    asRegister(office, "static office chair");
    asRegister(office, "Gaming chair - pro");
    asRegister(office, "Gaming chair - comfort");
    asRegister(office, "Gaming chair - kids");
    asRegister(office, "Gaming chair - expert pro super duper max with an espresso machine in right handle");
    asRegister(office, "laptop holder #3986542");
    asRegister(office, "debugging duck");
    add_amount=1;
    current_item=asGetFirst(office);
    for (int i=0; i< asGetSize(office); i++){
        asChangeAmount(office, current_item, add_amount);
        asGetAmount(office, current_item, &current_amount);
        if (i%2 == 0){
            ASSERT_TEST(current_amount==add_amount);
        }
        else {
            ASSERT_TEST(current_amount==0);
        }
        add_amount*=(-1);
        current_item=asGetNext(office);
    }
    current_item=asGetFirst(office);
    for (int i=0; i< asGetSize(office); i++){
        asGetAmount(office, current_item, &add_amount);
        asChangeAmount(office, current_item, -add_amount);
        asGetAmount(office, current_item, &current_amount);
        ASSERT_TEST(current_amount==0);
        current_item=asGetNext(office);
    }

    asDestroy(bedroom);
    asDestroy(office);

    return true;
}

bool testNoMemory() {
    AmountSet set = asCreate();
    char str[] = { [41] = '\1' };
    rand_str(str, sizeof str - 1);
    assert(str[41] == '\0');
    AmountSetResult register_res=AS_SUCCESS;
    while (register_res!=AS_OUT_OF_MEMORY){
        register_res=asRegister(set, str);
        rand_str(str, sizeof str - 1);
    }

    asDestroy(set);
    return true;
}

