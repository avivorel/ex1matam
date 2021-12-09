//
// Created by darkm on 09/12/2021.
//

#ifndef EX1MATAM_INVENTORY_H
#define EX1MATAM_INVENTORY_H

#endif //EX1MATAM_INVENTORY_H

#include <stdio.h>
#include <stdbool.h>
#include "amount_set.h"
#include "matamikya.h"



typedef struct Inventory_t *Inventory;

/**
 * createInventory: Allocates a new empty Inventory.
 *
 * @return
 *     NULL - if one of the parameters is NULL or allocations failed.
 *     A new Inventory in case of success.
 */
Inventory createInventory();


/**
* destroyInventory: Deallocates and destroys a given inventory.
 *
 * @param inventory - Target inventory to be deallocated. If inventory is NULL nothing will be done.
*/

void destroyInventory(Inventory inventory);

/**
 * asRegister: Add a new product into the set.
 *
 * The product is added with an initial amount of 0.
 * Iterator's value is undefined after this operation.
 *
 * @param inventory - The target inventory to which the ASElement is added.
 * @param product - The product to add.
 * @return
 *     AS_NULL_ARGUMENT - if a NULL argument was passed.
 *     AS_ITEM_ALREADY_EXISTS - if an equal element already exists in the inventory.
 *     AS_SUCCESS - if the element was added successfully.
 */
AmountSetResult registerInventory();

/**
* changeInventoryAmount: changes the amount of a certain product in the inventory.
 * Iterator's value is undefined after this operation.
 *
 * @param inventory - the target inventory where were going to change the product's amount.
 * @param id - the product's ID.
 * @param amount - the amount we want to add \ substract from the current amount.
 * @return
 *      AS_SUCCESS - amount changed successfully.
 *      AS_NULL_ARGUMENT - if a NULL argument was passed.
*/

AmountSetResult changeInventoryAmount(Inventory inventory, const unsigned int id, const double amount);

/**
 * removeProduct: removes a given product from the inventory.
 * Iterator's value is undefined after.
 *
 * @param inventory - the target inventory to delete from.
 * @param id - the products ID which you wish to delete
 * @return
 *      AS_SUCCESS - product deleted successfully.
 *      AS_NULL_ARGUMENT - a NULL argument was passed
 *      AS_DOES_NOT_EXIST - the product does not exist in the inventory.
 */

AmountSetResult removeProduct(Inventory inventory, const unsigned int id);