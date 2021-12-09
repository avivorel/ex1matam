#include <stdio.h>
#include <stdbool.h>
#include "amount_set.h"
#include "matamikya.h"
#include "inventory.h"
#include <stdlib.h>
#include "MtmProductData.h"

struct Inventory_t {
    AmountSet header;
    AmountSet current_element;
} ;
typedef struct Product_t{
    unsigned int id;
    char* name;
    double amount;
    MatamikyaAmountType amountType;
    MtmGetProductPrice productPrice;
} *Product;

Inventory createInventory(){
    Inventory inventory = malloc(sizeof(*inventory));
    return inventory;
}

void destroyInventory(Inventory inventory){
    if(inventory == NULL) return;
    asDestroy(inventory->header);
}

AmountSetResult registerInventory(Inventory inventory, const unsigned int id, const char* name, const double amount, MatamikyaAmountType amountType, MtmGetProductPrice productPrice){
    if(inventory == NULL || (void *) id == NULL || name == NULL)
        return AS_NULL_ARGUMENT;
    AmountSet header = inventory->header;
    Product product = malloc(sizeof(*product));
    if(product!=NULL){
        product->id = id;
    }
    if(!asContains(header,(ASElement)product)){
        product->name = name;
        product->amount = amount;
        product->amountType = amountType;
        product->productPrice = productPrice;
        return AS_SUCCESS;
    }
    else{
        return AS_ITEM_ALREADY_EXISTS;
    }
}
