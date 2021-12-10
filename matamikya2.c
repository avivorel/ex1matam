#include "amount_set.h"
#include "matamikya.h"
#include "matamikya_print.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "MtmProductData.h"

struct  Matamikya_t {
    AmountSet inventory;
    MtmCopyData copyData;
    MtmFreeData freeData;
    MtmGetProductPrice prodPrice;
};
int (*inventoryCompareFunc)(ASElement element1 , ASElement element2);
Matamikya matamikyaCreate()
{
    Matamikya warehouse = malloc(sizeof(*warehouse));
    if(warehouse==NULL) return NULL;
    AmountSet inventory= asCreate(warehouse->copyData, warehouse->freeData,inventoryCompareFunc);
    if(inventory==NULL)
    {
        free(warehouse);
        return NULL;
    }
    warehouse->inventory=inventory;
    MtmProductData data=malloc(sizeof(*data));
    asRegister(warehouse->inventory,(ASElement)data);
    return warehouse;
};

void matamikyaDestroy(Matamikya matamikya){
    if(matamikya == NULL) return;
    // need to add the actual free func
}

