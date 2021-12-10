#include "amount_set.h"
#include "matamikya.h"
#include "set.h"
#include "matamikya_print.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#define POSITIVE 1
#define NEGATIVE -1
#define EQUAL 0
#define LEGAL_REMMANT 0.001
#define HALF_AMOUNT 0.5


struct  Matamikya_t {
    AmountSet Warehouse;
    Set Orders ;
};

typedef struct ProductData_t {
    unsigned int Id ;
    char* Name ;
    MtmProductData  CustomData;
    MatamikyaAmountType AmountType ;
    MtmCopyData ProductCopy;
    MtmFreeData ProductFree;
    MtmGetProductPrice ProductGetPrice;
}
*ProductData;

typedef struct OrderProductData_t {
    unsigned int OrderId;
    AmountSet OrderData;
}
*OrderProductData;

static char* strCopy (const char* element)
{
    int len_of_new_element=(int)strlen(element)+1;
    char* element_copy= malloc(len_of_new_element);
    if(element_copy==NULL) return NULL;
    for(int i=0 ; i<len_of_new_element ; i++)
    {
        element_copy[i]=(element)[i];
    }
    return element_copy ;
}

ProductData createProduct (const unsigned int id,
                           const char *name,
                           const MatamikyaAmountType amountType,
                           const MtmProductData customData,
                           MtmCopyData copyData,
                           MtmFreeData freeData,
                           MtmGetProductPrice prodPrice)

{
    ProductData product = malloc(sizeof(*product));
    if(product == NULL ) return NULL ;
    product->Id = id;
    product -> Name = strCopy(name);
    if(product->Name == NULL )
    {
        free(product) ;
        return NULL ;
    }
    product->CustomData = copyData(customData) ;
    if(product->CustomData == NULL)
    {
        free(product->Name);
        free(product);
        return NULL ;
    }
    product ->AmountType = amountType ;
    product->ProductCopy = copyData;
    product->ProductFree = freeData;
    product-> ProductGetPrice = prodPrice ;
    return product;
}

ProductData CopyData(ProductData element)
{
    if(element == NULL ) return NULL;
    ProductData new_element = malloc(sizeof(*new_element));
    if(new_element==NULL) return NULL;
    new_element -> Id = (*(ProductData)element).Id;
    new_element->Name = strCopy((*(ProductData)element).Name);
    if(new_element->Name==NULL) {
        free(new_element);
        return NULL ;
    }
    new_element -> CustomData = ((*(ProductData)element).ProductCopy((*(ProductData)element).CustomData));
    if(new_element->CustomData==NULL) {
        free(new_element->Name);
        free(new_element);
        return NULL;
    }
    new_element -> AmountType = (*element).AmountType;
    new_element -> ProductCopy = (*element).ProductCopy;
    new_element -> ProductFree = (*element).ProductFree;
    new_element -> ProductGetPrice = (*element).ProductGetPrice;
    return new_element ;
}
ProductData DestroyData(ProductData element)
{
    if(element==NULL) return NULL ;
    if(element->CustomData != NULL ) element->ProductFree((element->CustomData));
    if( element->Name != NULL ) free (element->Name);
    free(element);
}

ASElement copyProductData (ASElement element)
{
    if(element==NULL) return NULL ;
    ProductData new_element = CopyData((ProductData)element);
    if((ASElement)new_element == NULL ) return NULL ;
    return (ASElement)new_element ;
}
void freeProductData (ASElement element)
{
    DestroyData((ProductData)element);
}

int ComapreProducts(ASElement element1 , ASElement element2)
{
    if (element1==NULL && element2 != NULL ) return -1;
    else if( (element1!=NULL && element2 == NULL ) ) return 1;
    else if ( element1 == NULL && element2 == NULL ) return 0 ;
    unsigned int Id1=(*(ProductData)element1).Id;
    unsigned int Id2=(*(ProductData)element2).Id;
    if(Id1>Id2)  return POSITIVE;
    if(Id1<Id2)  return NEGATIVE;
    return EQUAL ;
}

/*
  ASElement copyProductData (ASElement element)
{
    if(element == NULL ) return NULL;
    ProductData new_element = malloc(sizeof(*new_element);
    if(new_element==NULL) return NULL;
    new_element -> Id = (*(ProductData)element).Id;
    new_element->Name = strCopy((*(ProductData)element).Name);
    if(new_element->Name==NULL) {
        free(new_element);
        return NULL ;
    }
    new_element -> CustomData = ((*(ProductData)element).ProductCopy((*(ProductData)element).CustomData));
    if(new_element->CustomData==NULL) {
        free(new_element->Name);
        free(new_element);
        return NULL;
    }
    new_element -> AmountType = (*(ProductData)element).AmountType;
    new_element -> ProductCopy = (*(ProductData)element).ProductCopy;
    new_element -> ProductFree = (*(ProductData)element).ProductFree;
    new_element -> ProductGetPrice = (*(ProductData)element).ProductGetPrice;
    return new_element ;
}
void freeProductData (ASElement element) {
    if (element == NULL) return;
    (*(ProductData) element).ProductFree((*(ProductData) element).CustomData);
    free((*(ProductData) element).Name);
    free(element);
}
*/

Matamikya matamikyaCreate()
{

    Matamikya matamikya = malloc(sizeof(*matamikya))   ;
    if(matamikya==NULL)
    return NULL ;
    matamikya->Warehouse= asCreate(copyProductData,freeProductData,ComapreProducts);
    if(matamikya->Warehouse==NULL)
    {
        free(matamikya);
        return NULL ;
    }
    // Set Orders Creation
    return matamikya;
}
void matamikyaDestroy(Matamikya matamikya)
{
    asDestroy(matamikya->Warehouse);
    /* Destory to Orders */
    free(matamikya);
}
static bool checkName (const char* name)
{
    if (((name[0] >= 'a' && name[0] <='z' ) || (name[0] >= 'A' && name[0] <='Z' ) || (name[0] >= '0' && name[0] <='9' )) || strcmp(name,"")==0 ) return true;
    return false ;
}
static bool checkAmount (const double amount ,const MatamikyaAmountType amountType )
{
    double rounded_amount= round(amount);
    double amount_diff = rounded_amount-amount;
    if(amount_diff < 0 ) amount_diff = (amount_diff)*NEGATIVE;
    if (amountType == MATAMIKYA_INTEGER_AMOUNT && !(amount_diff == LEGAL_REMMANT || amount_diff == EQUAL))
        return false;
    if (amountType == MATAMIKYA_HALF_INTEGER_AMOUNT &&
        !(amount_diff == LEGAL_REMMANT || amount_diff == EQUAL || amount_diff == LEGAL_REMMANT+HALF_AMOUNT || amount_diff==HALF_AMOUNT))
        return false;
    if(amount<0) return false;
    return true ;
}
static ASElement elementSearch (Matamikya matamikya , const unsigned int Id)
{
 ASElement found = asGetFirst(matamikya);
 while( (*(ProductData)found).Id != Id || found == NULL )
 {
     asGetNext(matamikya)  ;
 }
 return found ;
}
MatamikyaResult mtmNewProduct(Matamikya matamikya, const unsigned int id, const char *name,
                              const double amount, const MatamikyaAmountType amountType,
                              const MtmProductData customData , MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
    if(matamikya == NULL || name == NULL || customData == NULL || copyData == NULL || freeData == NULL || prodPrice == NULL ) return MATAMIKYA_NULL_ARGUMENT;
    if ( !checkName(name) ) return MATAMIKYA_INVALID_NAME;
    if(!checkAmount(amount,amountType)) return MATAMIKYA_INVALID_AMOUNT;
    ProductData newData = createProduct(id,name,amountType,customData,copyData,freeData,prodPrice);
    if(newData==NULL) return MATAMIKYA_OUT_OF_MEMORY;
    AmountSetResult registerResult = asRegister(matamikya->Warehouse,(ASElement)newData);
    if(registerResult==AS_OUT_OF_MEMORY)
    {
        DestroyData(newData);
        return MATAMIKYA_OUT_OF_MEMORY;
    }
    if(registerResult==AS_ITEM_ALREADY_EXISTS ) {

        return MATAMIKYA_PRODUCT_ALREADY_EXIST;
    }
    if(registerResult==AS_SUCCESS)
    {
        ASElement found = elementSearch(matamikya , id);
    AmountSetResult amount_result = asChangeAmount(matamikya->Warehouse, found ,amount);
    if(amount_result == AS_SUCCESS) {
        DestroyData(newData);
        return MATAMIKYA_SUCCESS;
    }
    else {
          DestroyData(newData);
        asDelete(matamikya->Warehouse, found) ;
        return MATAMIKYA_INSUFFICIENT_AMOUNT;
    }
    }


}
