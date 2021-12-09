#include <stdio.h>
#include <stdbool.h>
#include "amount_set.h"
#include "matamikya.h"
#include "inventory.h"
#include <stdlib.h>

struct Inventory_t {
    AmountSet header;
    AmountSet current_element;
} ;

Inventory createInventory(){
    Inventory inventory = malloc(sizeof(*inventory));
    return inventory;
}

void destroyInventory(Inventory inventory){
    if(inventory == NULL || inventory->header == NULL) return;
    asDestroy(inventory->header);
}

AmountSetResult registerInventory(Inventory inventory, MtmProductData product){
    if(inventory == NULL || product == NULL) return AS_NULL_ARGUMENT;
    if(asContains(inventory->header,product) == false){
        asRegister(inventory->header, product);
        return AS_SUCCESS;
    }
    else {
        return AS_ITEM_ALREADY_EXISTS;
    }
}

AmountSetResult changeInventoryAmount(Inventory inventory, const unsigned int id, void *const amount){
    if(inventory == NULL || (void *) id == NULL || amount == NULL) return AS_NULL_ARGUMENT;
    MtmProductData product = malloc(sizeof(*product));
    product
    while(inventory->header != NULL){
        if(inventory->header->)
    }
}