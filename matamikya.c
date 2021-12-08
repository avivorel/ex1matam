#include "amount_set.h"
#include "matamikya.h"
#include "matamikya_print.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct  Matamikya_t {
    AmountSet inventory;
    MtmCopyData copyData;
    MtmFreeData freeData;
    MtmGetProductPrice prodPrice;
};
struct MtmProductData {
    const unsigned int id;
    const char *name;
    const double amount_per_type;
    const MatamikyaAmountType amountType;
};
int inventoryCompareFunc(ASElement element1 , ASElement element2);

Matamikya matamikyaCreate()
{
    Matamikya warehouse = malloc(sizeof(*warehouse));
    if(warehouse==NULL) return NULL;
    AmountSet inventory= asCreate(warehouse->copyData, warehouse->freeData, inventoryCompareFunc);
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
