#include <stdio.h>
#include <stdbool.h>
#include "amount_set.h"
#include "matamikya.h"


#ifndef EX1MATAM_MTMPRODUCTDATA_H
#define EX1MATAM_MTMPRODUCTDATA_H

#endif //EX1MATAM_MTMPRODUCTDATA_H

MtmProductData createProductData();

void destroyProductData(MtmProductData data);

AmountSetResult registerProductData(MtmProductData data, const unsigned int id);

AmountSetResult changeProductAmount(MtmProductData data , const unsigned int id, void *const amount);

AmountSetResult removeProductData(MtmProductData data, const unsigned int id);