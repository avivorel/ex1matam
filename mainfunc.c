#include "amount_set_str.h"
#include <stdlib.h>
#include <string.h>
#include "test_utilities.h"
#include <assert.h>
    int main() {
        AmountSet set = asCreate();
        assert(asRegister(set, "Adams") == AS_SUCCESS);
        assert(asRegister(set, "Bettys") == AS_SUCCESS);
        asRegister(set, "Bat");
        assert(asRegister(set, "Bat") == AS_ITEM_ALREADY_EXISTS );
        asRegister(set, "Ab");
        printf("The Sorted Set Elements Are :\n");
        AS_FOREACH(char*, i, set) {
            printf("%s ", i);
        }
        printf("\n");
        asDelete(set, "Ab");
        AS_FOREACH(char*, i, set) {
            printf("%s ", i);
        }
        printf("\n");
        asRegister(set, "Ab");
        asDelete(set, "Betty");
        AS_FOREACH(char*, i, set) {
            printf("%s ", i);
        }
        printf("\n");
        asRegister(set, "Betty");
        asDelete(set, "Adam");
        AS_FOREACH(char*, i, set) {
            printf("%s ", i);
        }
        printf("\n");
        asRegister(set, "Adam");
        AS_FOREACH(char*, i, set) {
            printf("%s ", i);
        }
        printf("\n");
        printf("The size of the set is : %d\n", asGetSize(set));
        printf("Betty is a element in the set ? %d\n", asContains(set, "Betty"));
        printf("Roee is a element in the set ? %d \n", asContains(set, "Roee"));
        AmountSet copy = asCopy(set);
        printf("The size of the copy is : %d\n", asGetSize(copy));
        double amount;
        if (asChangeAmount(set, "Betty", -1) == AS_INSUFFICIENT_AMOUNT) printf("Cannot decrease -1\n");
        if ((asGetAmount(set, "Betty", &amount) == AS_SUCCESS)) printf("Betty is in the set for %f times\n", amount);
        if (asChangeAmount(set, "Betty", 4) == AS_SUCCESS) printf("Operation Succeded\n");
        if ((asGetAmount(set, "Betty", &amount) == AS_SUCCESS)) printf("Betty is in the set for %f times\n", amount);
        if (asChangeAmount(set, "Betty", -2) == AS_SUCCESS) printf("Operation Succeded\n");
        if ((asGetAmount(set, "Betty", &amount) == AS_SUCCESS)) printf("Betty is in the set for %f times\n", amount);
        if (asChangeAmount(set, "Betty", -3) == AS_INSUFFICIENT_AMOUNT) printf("Cannot decrease -3\n");
        if ((asGetAmount(set, "Betty", &amount) == AS_SUCCESS)) printf("Betty is in the set for %f times\n", amount);
        if (asChangeAmount(copy, "Betty", 0) == AS_SUCCESS) printf("Operation Succeded\n");
        if ((asGetAmount(copy, "Betty", &amount) == AS_SUCCESS)) printf("Betty is in the set for %f times\n", amount);
        if (asChangeAmount(copy, "Roee", 0) == AS_ITEM_DOES_NOT_EXIST) printf("Roee is not in the set\n");
        if ((asGetAmount(copy, NULL, &amount) == AS_NULL_ARGUMENT)) printf("Dont send me NULL Arr !\n");
        if ((asGetAmount(NULL, "Dean", &amount) == AS_NULL_ARGUMENT)) printf("Dont send me NULL Arr !\n");
        if ((asGetAmount(set, "Dean", NULL) == AS_NULL_ARGUMENT)) printf("Dont send me NULL Arr !\n");
        if (asChangeAmount(set, NULL, -3) == AS_NULL_ARGUMENT) printf("Dont send me NULL arg\n");
        if (asChangeAmount(NULL, "DAni", -3) == AS_NULL_ARGUMENT) printf("Dont send me NULL arg\n");

        asDestroy(copy);
        asDestroy(set);
        printf("I succeded the compilation from the Clion");
    }
