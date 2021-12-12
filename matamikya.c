#include "amount_set.h"
#include "matamikya.h"
#include "set.h"
#include "matamikya_print.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#define POSITIVE 1
#define NEGATIVE -1
#define EQUAL 0
#define INTEGER_LIMIT 0.001
#define UPPER_HALF_INTEGER_LIMIT 0.501
#define LOWER_HALF_INTEGER_LIMIT 0.499



struct  Matamikya_t {
    AmountSet warehouse;
    Set orders ;
};

typedef struct ProductData_t {
    unsigned int Id ;
    char* Name ;
    MtmProductData  custom_data;
    MatamikyaAmountType amount_type ;
    MtmCopyData productCopyFunction;
    MtmFreeData productFreeFunction;
    MtmGetProductPrice getProductPriceFunc;
}
*ProductData;

/* Static function used in matamikya */

/* strCopy
 * copy a string return NULL if not success and the string either , using a dynamic allocation
 * */

static char* strCopy (const char* element) // used in createdProductData , copyProductData
{
    size_t len_of_new_element=(strlen(element))+1;
    char* element_copy= malloc(sizeof(*element_copy)*len_of_new_element);
   element_copy=strcpy(element_copy,element);
    return element_copy ;
}
/* checkValidName : 
 * Check if the name is valid by the criterion of first letter starts with 0-9 or a-z or A-Z and the string not empty  *
  return true if the name is valid and false either . */
static bool checkValidName (const char* name) // used in mtmNewProduct
{
    if (((name[0] >= 'a' && name[0] <='z' ) || (name[0] >= 'A' && name[0] <='Z' ) || (name[0] >= '0' && name[0] <='9' )) || !strcmp(name,"") ) return true;
    return false ;
}
/* checkAmountType
 * check if the amount is valid .
 * if its integer amount the amount can be between (int-1).99 to int.001
 * if its half integer amount the amount can be between (int-1).99 to int.001 or int.499 to int.501
 * */
static bool checkAmountType (const double amount ,const MatamikyaAmountType amountType )
{
    double amount_diff;
    double rounded_amount= round(amount);
    if(amount >= rounded_amount) amount_diff = amount - rounded_amount;
    else if(amount<rounded_amount) amount_diff=rounded_amount - amount;
    if (amountType == MATAMIKYA_INTEGER_AMOUNT && !(amount_diff <= INTEGER_LIMIT && amount_diff >= EQUAL))
        return false;
    if (amountType == MATAMIKYA_HALF_INTEGER_AMOUNT &&
        ( !(amount_diff <= INTEGER_LIMIT && amount_diff >= EQUAL) || (amount_diff >= LOWER_HALF_INTEGER_LIMIT && amount_diff<=UPPER_HALF_INTEGER_LIMIT)) )
        return false;
    return true ;
}
static ASElement getWarehouseProduct (Matamikya matamikya , const unsigned int Id)
{
    ASElement found = (asGetFirst(matamikya->warehouse));
    while( (*(ProductData)found).Id != Id && found != NULL )
    {
        asGetNext(found)  ;
    }
    return found ;
}

/* Functions for ADT Product Data */

ProductData createProductData (const unsigned int id,
                           const char *name,
                           const MatamikyaAmountType amountType,
                           const MtmProductData customData,
                           MtmCopyData copyData,
                           MtmFreeData freeData,
                           MtmGetProductPrice prodPrice)

{
    if(name==NULL || customData==NULL || copyData==NULL || freeData==NULL || prodPrice == NULL ) return NULL;
    ProductData product = malloc(sizeof(*product));
    if(product == NULL ) return NULL ;
    product->Id = id;
    product->Name= strCopy(name);
    if(product->Name == NULL )
    {
        free(product) ;
        return NULL ;
    }
    product->custom_data = copyData(customData) ;
    if(product->custom_data == NULL)
    {
        free(product->Name);
        free(product);
        return NULL ;
    }
    product ->amount_type = amountType ;
    product->productCopyFunction = copyData;
    product->productFreeFunction = freeData;
    product-> getProductPriceFunc = prodPrice ;
    return product;
}

ProductData copyProductData(ProductData element)
{
    if(element == NULL ) return NULL;
    ProductData new_element = malloc(sizeof(*new_element));
    if(new_element==NULL) return NULL;
    new_element -> Id = element->Id;
    new_element->Name = strCopy(element->Name);
    if(new_element->Name==NULL) {
        free(new_element);
        return NULL ;
    }
    new_element -> custom_data = (element->productCopyFunction(element->custom_data));
    if(new_element->custom_data == NULL) {
        free(new_element->Name);
        free(new_element);
        return NULL;
    }
    new_element -> amount_type = element->amount_type;
    new_element -> productCopyFunction = element->productCopyFunction;
    new_element -> productFreeFunction = element->productFreeFunction;
    new_element -> getProductPriceFunc = element->getProductPriceFunc;
    return new_element ;
}
void destroyProductData(ProductData element)
{
    if(element==NULL) return ;
    if(element->custom_data != NULL ) element->productFreeFunction((element->custom_data));
    if( element->Name != NULL ) free (element->Name);
    free(element);
}
int compareProductData(ProductData element1 , ProductData element2)
{
if(element1==NULL)
{
    if(element2==NULL) return EQUAL;
   if(element2->Name == NULL) return EQUAL;
   return NEGATIVE;
    }
    if(element2==NULL)
    {
        if(element1->Name == NULL ) return EQUAL;
        return POSITIVE;
    }
    unsigned int Id1= element1->Id;
    unsigned int Id2= element2->Id;
    if(Id1>Id2)  return POSITIVE;
    if(Id1<Id2)  return NEGATIVE;
    return EQUAL ;
}


/*End Of Functions for ADT Product Data */

/* Function for using the AmountSet ADT - ASElement points to ProductData */

ASElement copyProductDataToASElement (ASElement element)
{
    if(element==NULL) return NULL ;
    ProductData new_element = copyProductData((ProductData) element);
    if((ASElement)new_element == NULL ) return NULL ;
    return (ASElement)new_element ;
}
void freeProductDataToASElement (ASElement element)
{
    destroyProductData((ProductData) element);
}

int compareProductDataToASElement(ASElement element1 , ASElement element2)
{
int result = compareProductData((ProductData) element1, (ProductData) element2);
return result;
}

/* End of Function for using the AmountSet ADT - ASElement points to ProductData */


Matamikya matamikyaCreate()
{

    Matamikya matamikya = malloc(sizeof(*matamikya))   ;
    if(matamikya==NULL)
    return NULL ;
    AmountSet warehouse = asCreate(copyProductDataToASElement, freeProductDataToASElement,compareProductDataToASElement);
    if(warehouse == NULL ){
        free(matamikya);
        return NULL ;
    }
    matamikya->warehouse = warehouse;
    // Set orders Creation
    return matamikya;
}
void matamikyaDestroy(Matamikya matamikya)
{
    asDestroy(matamikya->warehouse);
    /* Destroy to orders */
    free(matamikya);
}

MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData , MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
    if(matamikya == NULL || matamikya->warehouse==NULL || name==NULL || customData==NULL || copyData==NULL || freeData==NULL || prodPrice == NULL  ) return MATAMIKYA_NULL_ARGUMENT;
    if ( !checkValidName(name) ) return MATAMIKYA_INVALID_NAME;
    if(!checkAmountType(amount,amountType)) return MATAMIKYA_INVALID_AMOUNT;
    ProductData new_data  ;
    if ( ( new_data = createProductData(id, name, amountType, customData, copyData, freeData, prodPrice) )== NULL ) return MATAMIKYA_OUT_OF_MEMORY;
    AmountSetResult registerResult = asRegister(matamikya->warehouse, (ASElement)new_data);
    if(registerResult==AS_OUT_OF_MEMORY)
    {
        destroyProductData(new_data);
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    if(registerResult==AS_ITEM_ALREADY_EXISTS )
    {
        destroyProductData(new_data);
        return MATAMIKYA_PRODUCT_ALREADY_EXIST;
    }
    if(registerResult==AS_SUCCESS)
    {
        destroyProductData(new_data);
        ASElement found = getWarehouseProduct(matamikya, id);
    AmountSetResult amount_result = asChangeAmount(matamikya->warehouse, found , amount);
    if(amount_result == AS_INSUFFICIENT_AMOUNT) {
        asDelete(matamikya->warehouse, found) ;
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    }

    if(amount_result == AS_NULL_ARGUMENT) {
        asDelete(matamikya->warehouse, found) ;
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    assert(amount_result==AS_ITEM_DOES_NOT_EXIST);
    }
    return MATAMIKYA_SUCCESS;
}
MatamikyaResult mtmChangeProductAmount(Matamikya matamikya, const unsigned int id, const double amount)
{
   if ( matamikya==NULL || matamikya->warehouse) return MATAMIKYA_NULL_ARGUMENT;
    ASElement found = getWarehouseProduct(matamikya,id);
    if (found == NULL) return MATAMIKYA_PRODUCT_NOT_EXIST;
    MatamikyaAmountType product_type = (*(ProductData)found).amount_type;
    if(!checkAmountType(amount,product_type)) return MATAMIKYA_INVALID_AMOUNT;
   AmountSetResult changing_result = asChangeAmount(matamikya->warehouse,found,amount);
   if(changing_result==AS_NULL_ARGUMENT) return MATAMIKYA_NULL_ARGUMENT;
   if(changing_result==AS_ITEM_DOES_NOT_EXIST) return MATAMIKYA_PRODUCT_NOT_EXIST;
   if(changing_result==AS_INSUFFICIENT_AMOUNT) return MATAMIKYA_INSUFFICIENT_AMOUNT;
   return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id)
{
    if(matamikya==NULL  || matamikya->warehouse==NULL) return MATAMIKYA_NULL_ARGUMENT;
    ASElement found = getWarehouseProduct(matamikya,id);
    if (found == NULL ) return MATAMIKYA_PRODUCT_NOT_EXIST;
    AmountSetResult clear_result = asDelete(matamikya->warehouse,found);
    if(clear_result == AS_NULL_ARGUMENT ) return MATAMIKYA_NULL_ARGUMENT;
    if(clear_result == AS_ITEM_DOES_NOT_EXIST ) return MATAMIKYA_PRODUCT_NOT_EXIST;
    return MATAMIKYA_SUCCESS;
}
typedef struct OrderElementData_t {
    unsigned int order_id;
    ProductData Element_Data;
}
*OrderElementData;

typedef struct Orders_t{
    Set orderSet;
    SetElement orders_list;
    AmountSet current_order; // idk why, but you wrote it in what you sent me
    SetElement CopySetElement;
} *Orders;

unsigned int getOrderId(Orders orders);

unsigned int mtmCreateNewOrder(Matamikya matamikya){
    if(matamikya == NULL) return MATAMIKYA_NULL_ARGUMENT;
    // No orders at all
    if(matamikya->orders == NULL){
        Orders orders = malloc(sizeof(*orders));
        if(orders == NULL) MATAMIKYA_OUT_OF_MEMORY;
        orders->orderSet = setCreate(copyProductDataToASElement,freeProductDataToASElement,compareProductDataToASElement);
        if(orders->orderSet == NULL)return MATAMIKYA_OUT_OF_MEMORY;
        Set orderset = orders->orderSet;
        AmountSet first_order = asCreate(copyProductDataToASElement,freeProductDataToASElement,compareProductDataToASElement);
        if(first_order == NULL){
            free(orders->orderSet);
            return MATAMIKYA_OUT_OF_MEMORY;
        }
        OrderElementData order_data = malloc(sizeof(*order_data));
        if(order_data == NULL){
            free(orders->orderSet);
            free(first_order);
            return MATAMIKYA_OUT_OF_MEMORY;
        }
        order_data->order_id = getOrderId(orders);
        order_data->Element_Data = NULL;
        asRegister(first_order,(ASElement)order_data);
        setAdd(orders->orderSet,(SetElement)first_order);
        return order_data->order_id;
        // Look for free func to add here
    }
    // If the order has orders already

}