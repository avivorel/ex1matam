#include <stdio.h>
#include <stdbool.h>
#include "amount_set.h"
#include "matamikya.h"
#include "inventory.h"
#include <stdlib.h>
#include "MtmProductData.h"

struct MtmProductData {
    const unsigned int id;
    const char *name;
    const double amount_per_type;
    const MatamikyaAmountType amountType;
};

MtmProductData createProductData(){
    MtmProductData data = malloc(sizeof(*data));

}

void destroyProductData(MtmProductData data){
    if(data == NULL) return;
    free(data);
}

AmountSetResult registerProductData(MtmProductData data, const unsigned int id){
   if(data == NULL || (void *) id == NULL) return AS_NULL_ARGUMENT;

}