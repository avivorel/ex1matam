#include <stdio.h>
#include <stdbool.h>
#include "amount_set.h"
#include <stdlib.h>
#include "set.h"
#include "Orders.h"

struct Orders_t {
    Set orders_set;
};
typedef struct OrderData_t
{
    unsigned int orderId;
    AmountSet orderData;

} *OrderData ;

typedef struct ItemData_t
{
    char* name;
    unsigned int productId;
    double amount ;
} *ItemData ;

/*Function to use generic Amount Set */

ASElement copyItemData (ASElement element)
{
    if(element == NULL ) return NULL;
    ItemData new_data = malloc(sizeof(*new_data));
    if(new_data==NULL) return NULL ;
    *new_data = *(ItemData)element;
    return new_data ;
}
void freeItemsData (ASElement element)
{
    free(element);
}
int ComapreItems(ASElement element1 , ASElement element2)
{
    ItemData copy_element1 = (ItemData) copyItemData(element1);
    ItemData copy_element2 = (ItemData)  copyItemData(element2);
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
    unsigned  int result = ((copy_element1->productId) - (copy_element2->productId));
free(copy_element1);
free(copy_element2);
return (int)result;
}

/* Function to use generic Set */

SetElement copyOrderData (SetElement element)
{
    if(element == NULL ) return NULL;
    OrderData new_data = malloc(sizeof(*new_data));
    if(new_data==NULL) return NULL ;
    *new_data = *(OrderData)element;
    return new_data ;
}
void freeOrderData (SetElement element)
{
    free(element);
}
int ComapreOrdersId(SetElement element1 , SetElement element2)
{
    OrderData copy_element1 =(OrderData) copyOrderData(element1);
    OrderData copy_element2 =(OrderData) copyOrderData(element2);
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
   int result =(int)( copy_element1->orderId - copy_element2->orderId );
    free(copy_element1);
    free(copy_element2);
return result;
}

Orders orderCreate()
{
    Orders order = malloc(sizeof(*order));
    if(order==NULL) return NULL ;
    order->orders_set = (Set) setCreate(copyOrderData,freeOrderData,ComapreOrdersId);
    if(order->orders_set == NULL) {
        free(order);
        return NULL;
    }
    OrderData data = (OrderData) order->orders_set ;
    data->orderData = (AmountSet) asCreate(copyItemData,freeItemsData,ComapreItems);
    if(data->orderData == NULL )
    {
        free (order->orders_set);
        free (order);
        return NULL ;
    }
    return order;
}
int main() {

    Orders ori = orderCreate() ;
    free(ori);
    return 1;
}


