
// Created by Aviv on 11/29/2021.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "amount_set_str.h"
#include "avivtest.h"
#include "avivtestutilities.h"

/**
 * testRegister will check varrious end-cases and will try to break the program
 * the test will include but not limited to: NULL cases, Wrong arguments, misuse and more..
 */
bool testCreate() {
    //initial creation
    AmountSet set = asCreate();
    ASSERT_TEST(set != NULL);
    asDestroy(set);
    return true;
}
bool testDestroy(){
    AmountSet set = asCreate();
    asDestroy(set);
    AmountSet set2 = asCreate();
    asDestroy(set2);
    ASSERT_TEST(set2!=NULL);
    return true;
}
bool testCopy(){
    AmountSet set = asCreate();
    AmountSet setcopy = asCopy(set);
    ASSERT_TEST(setcopy != NULL);
    asDestroy(setcopy);
    asRegister(set,"Element1");
    asChangeAmount(set,"Element1",31.3);
    setcopy = asCopy(set);
    ASSERT_TEST(setcopy != NULL);
    while(1){
        double set1amount = 0;
        double set2amount = 0;
        asGetAmount(set,"Element1",&set1amount);
        asGetAmount(setcopy,"Element1",&set2amount);
        ASSERT_TEST(set1amount == set2amount);
        break;
    }
    asDestroy(set);
    ASSERT_TEST(setcopy != NULL);
    asDestroy(setcopy);
    return true;
}
bool testGetSize(){
    AmountSet set = asCreate();
    ASSERT_TEST(set!=NULL);
    ASSERT_TEST(asGetSize(set) == 0);
    asRegister(set,"first");
    ASSERT_TEST(asGetSize(set) == 1);
    asDestroy(set);
    return true ;
}
bool testContains(){
    AmountSet set = asCreate();
    ASSERT_TEST(set!= NULL);
    asRegister(set,"First");
    ASSERT_TEST(asContains(set,NULL) == false);
    ASSERT_TEST(asContains(set,"First") == true);
    ASSERT_TEST(asContains(set,"first") == false);
    ASSERT_TEST(asContains(set,"") == false);
    ASSERT_TEST(asDelete(set,"First") == AS_SUCCESS);
    ASSERT_TEST(asContains(set,"First") == false);
    asDestroy(set);
    return true;
}
bool testGetAmount(){
    AmountSet set = asCreate();
    ASSERT_TEST(set != NULL);
    asRegister(set,"First");
    double amounttotest = 23.34;
    asChangeAmount(set,"First",23.33);
    asChangeAmount(set,"First",23.35);
    asChangeAmount(set,"First",amounttotest);
    double amount;
    ASSERT_TEST(asGetAmount(set,"First",&amount) == AS_SUCCESS);
    asDelete(set,"First");
    ASSERT_TEST(asGetAmount(set,"First",&amount) == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(asGetAmount(set,NULL,&amount) == AS_NULL_ARGUMENT);
    asDestroy(set);
    return true;
}
bool testRegister(){
    AmountSet set = asCreate();
    ASSERT_TEST(asRegister(set,"")== AS_SUCCESS);
    asDestroy(set);
    return true;
}
bool testChangeAmount(){
    AmountSet set = asCreate();
    ASSERT_TEST(set!=NULL);
    ASSERT_TEST(asRegister(set,"test1") == AS_SUCCESS);
    ASSERT_TEST(asChangeAmount(set,"test1",15.5)==AS_SUCCESS);
    ASSERT_TEST(asChangeAmount(set,"test1",16.5)==AS_SUCCESS);
    ASSERT_TEST(asChangeAmount(set,"test1",0.00)==AS_SUCCESS);
    ASSERT_TEST(asChangeAmount(set,"test1",0)==AS_SUCCESS);
    ASSERT_TEST(asChangeAmount(set,"test1",-100)==AS_INSUFFICIENT_AMOUNT);
    ASSERT_TEST(asChangeAmount(set,"test1",15.5)==AS_SUCCESS);
    ASSERT_TEST(asChangeAmount(set,"test1",-3)==AS_SUCCESS);
    asDestroy(set);
    return 1;
}
bool testDelete(){
    AmountSet set = asCreate();
    ASSERT_TEST(set != NULL);
    ASSERT_TEST(asRegister(set,"test1") == AS_SUCCESS);
    ASSERT_TEST(asDelete(set,"test1") == AS_SUCCESS);
    ASSERT_TEST(asDelete(set,"test1") == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(asDelete(set,"test12") == AS_ITEM_DOES_NOT_EXIST);
    ASSERT_TEST(asDelete(set,"") == AS_ITEM_DOES_NOT_EXIST);
    asDestroy(set);
    return true ;
}
bool testClear(){
    AmountSet set = asCreate();
    ASSERT_TEST(set != NULL);
    ASSERT_TEST(asRegister(set,"test1") == AS_SUCCESS);
    ASSERT_TEST(asRegister(set,"test2") == AS_SUCCESS);
    ASSERT_TEST(asRegister(set,"test3") == AS_SUCCESS);
    ASSERT_TEST(asRegister(set,"test4") == AS_SUCCESS);
    ASSERT_TEST(asClear(set) == AS_SUCCESS);
    ASSERT_TEST(asRegister(set,"test1")== AS_SUCCESS);
    ASSERT_TEST(asRegister(set,"test2") == AS_SUCCESS);
    ASSERT_TEST(asRegister(set,"test4") == AS_SUCCESS);
    ASSERT_TEST(asRegister(set,"test3") == AS_SUCCESS);
    asDestroy(set);
    ASSERT_TEST(asClear(NULL) == AS_NULL_ARGUMENT);
    return true ;
}
bool testGetFirst(){
    AmountSet set = asCreate();
    char* first="first";
            char* second ="second";
            char* third = "third";
    ASSERT_TEST(set != NULL);
    ASSERT_TEST(asRegister(set,"first")==AS_SUCCESS);
    ASSERT_TEST(asRegister(set,"second")==AS_SUCCESS);
    ASSERT_TEST(asRegister(set,"third")==AS_SUCCESS);
    ASSERT_TEST(strcmp (asGetFirst(set),first)==0);
    ASSERT_TEST(asDelete(set,"first") == AS_SUCCESS);
    ASSERT_TEST(strcmp (asGetFirst(set),second)==0);
    ASSERT_TEST(asDelete(set,"second") == AS_SUCCESS);
    ASSERT_TEST(strcmp (asGetFirst(set),third)==0);

    asDestroy(set);
    return true ;
}
int main(){
    testCreate();
    testDestroy();
   testCopy();
    testContains();
    testGetAmount();
    testGetSize();
    testRegister();
    testChangeAmount();
    testDelete();
    testClear();
    testGetFirst();
    return 0;
}
