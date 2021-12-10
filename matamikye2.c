#include "amount_set.h"
#include "matamikya.h"
#include "set.h"
#include "matamikya_print.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


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
    ProductData ElementData;
}
*OrderProductData;

ASElement copyProductData (ASElement element)
{
    if(element == NULL ) return NULL;
    ProductData new_element = malloc(sizeof(*new_element));
    if(new_element==NULL) return NULL ;
    *new_element = *(ProductData)element;
    return new_element ;
}
void freeProductData (ASElement element) {
    free(element);
}

int ComapreProducts(ASElement element1 , ASElement element2)
{
    ASElement copy_element1 = copyProductData(element1);
    ASElement copy_element2 =  copyProductData(element2);
    if(copy_element1==NULL && copy_element2!=NULL)
    {
        free(copy_element2);
        return -1;
    }
    if(copy_element1 != NULL && copy_element2 == NULL)
    {
        free(copy_element1);
        return 1;
    }
    if(copy_element1==NULL && copy_element2==NULL)
    {
        return 0;
    }
    unsigned  int result = ((*(ProductData)copy_element1).Id) - ((*(ProductData)copy_element2).Id);
    free(copy_element1);
    free(copy_element2);
    return (int)result;
}