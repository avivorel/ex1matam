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

AmountSetResult changeInventoryAmount(Inventory inventory, const unsigned int id, const double amount){
    if(inventory == NULL) return AS_NULL_ARGUMENT;
    if(amount == 0) return AS_SUCCESS;
    Product tmp_product = malloc(sizeof(*tmp_product));
    if(tmp_product!= NULL){
        tmp_product->id = id;
    }
    if(asContains(inventory->header,(ASElement)tmp_product)){
        asChangeAmount(inventory->header,(ASElement)tmp_product,amount);
        return AS_SUCCESS;
    }
    return AS_ITEM_DOES_NOT_EXIST;
}

