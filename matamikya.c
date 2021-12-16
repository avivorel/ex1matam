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
    double total_sold;
}
*ProductData;

typedef struct Order_t{
    unsigned int order_id;
    AmountSet order_data;
}*Order;


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
static ASElement getProductFromAmountSet (AmountSet Inventory , const unsigned int Id)
{
    ASElement found = (asGetFirst(Inventory));
    while( (*(ProductData)found).Id != Id && found != NULL )
    {
        found = asGetNext(Inventory)  ;
    }
    return found ;
}

/* End for static function for matamikya */

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
    product->total_sold=0;
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
    new_element->total_sold = element -> total_sold;
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

/* Function for Adt Order */
Order  copyOrder(Order order)
{
    if (order == NULL) return NULL;
    Order new_order = malloc(sizeof(*new_order));
    if (new_order == NULL) return NULL;
    new_order->order_data = asCopy(order->order_data);
    if (new_order->order_data == NULL) {
        free(order);
        return NULL;
    }
    new_order->order_id = order->order_id;
    return new_order;
}

void freeOrder(Order order) {
    if (order == NULL) return;
    if (order->order_data == NULL) {
        free(order);
        return;
    }
    asDestroy(order->order_data);
    free(order);
}

int compareOrders(Order order1, Order order2) {
    assert(order1 != NULL && order2 != NULL);
    unsigned int difference = order1->order_id - order2->order_id;
    if (difference > 0) return POSITIVE;
    if (difference < 0) return NEGATIVE;
    return EQUAL;
}

/*End For Order ADT Function */


/* Function for using the AmountSet ADT - ASElement points to ProductData ADT */

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

/* Function for using the Set Adt - SetElement points to Order ADT */

SetElement setCopyForOrderElementData(SetElement element) {
    if(element==NULL)return NULL;
    SetElement copy_element = copyOrder((Order) element);
    return copy_element;
}
void setDestroyForOrder (SetElement order)
{
    if(order==NULL) return ;
    freeOrder(order);
}
int setOrdersCompare (SetElement order1 , SetElement order2 )
{
    int result = compareOrders((Order) order1, (Order) order2);
    return result ;
}

/* End of function required to create a set */

Matamikya matamikyaCreate()
{

    Matamikya matamikya = malloc(sizeof(*matamikya))   ;
    if(matamikya==NULL)
    return NULL ;
    matamikya->warehouse = asCreate(copyProductDataToASElement, freeProductDataToASElement,compareProductDataToASElement);
    if(matamikya->warehouse == NULL ){
        free(matamikya);
        return NULL ;
    }
    matamikya->orders= setCreate(setCopyForOrderElementData,setDestroyForOrder,setOrdersCompare);
    if( matamikya->orders)
    {
        asDestroy(matamikya->warehouse);
        free(matamikya);
        return NULL ;
    }

        return matamikya;
}
void matamikyaDestroy(Matamikya matamikya)
{
    if(matamikya->warehouse != NULL) asDestroy(matamikya->warehouse);
    if(matamikya->orders != NULL) setDestroy(matamikya->orders);
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
        ASElement found = getProductFromAmountSet(matamikya->warehouse, id);
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
    ASElement found = getProductFromAmountSet(matamikya->warehouse, id);
    if (found == NULL) return MATAMIKYA_PRODUCT_NOT_EXIST;
    MatamikyaAmountType product_type = (*(ProductData)found).amount_type;
    if(!checkAmountType(amount,product_type)) return MATAMIKYA_INVALID_AMOUNT;
   AmountSetResult changing_result = asChangeAmount(matamikya->warehouse,found,amount);
   if(changing_result==AS_NULL_ARGUMENT) return MATAMIKYA_NULL_ARGUMENT;
   if(changing_result==AS_ITEM_DOES_NOT_EXIST) return MATAMIKYA_PRODUCT_NOT_EXIST;
   if(changing_result==AS_INSUFFICIENT_AMOUNT) return MATAMIKYA_INSUFFICIENT_AMOUNT;
   return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmClearProduct(Matamikya matamikya, const unsigned int id) {
    if (matamikya == NULL || matamikya->warehouse == NULL) return MATAMIKYA_NULL_ARGUMENT;
    ASElement found = getProductFromAmountSet(matamikya->warehouse, id);
    if (found == NULL) return MATAMIKYA_PRODUCT_NOT_EXIST;
    AmountSetResult clear_result = asDelete(matamikya->warehouse, found);
    if (clear_result == AS_NULL_ARGUMENT) return MATAMIKYA_NULL_ARGUMENT;
    if (clear_result == AS_ITEM_DOES_NOT_EXIST) return MATAMIKYA_PRODUCT_NOT_EXIST;
    return MATAMIKYA_SUCCESS;
}

static unsigned int getLastOrderId(Set orders) {
    if (orders == NULL) return 0;
    SetElement current_order = setGetFirst(orders);
    if (current_order == NULL) return 1;
    unsigned int last_id = 0;
    while (current_order != NULL) {
        SetElement next_order = setGetNext(current_order);
        if (next_order == NULL) {
            last_id = ((Order) (current_order))->order_id;
            break;
        }
        current_order = setGetNext(current_order);
    }
    return last_id + 1;
}
static SetElement findOrder (Set orders_list , unsigned int id)
    {
        SetElement current_element = setGetFirst(orders_list);
        while( (*(Order)current_element).order_id != id && current_element!=NULL)
        {
          current_element= setGetNext(orders_list);
        }
        return current_element;
    }
unsigned int mtmCreateNewOrder(Matamikya matamikya)
    {
        if(matamikya==NULL) return 0;
        Set orders_list = matamikya->orders ; // Didnt check for NULL because of setAdd ;
        Order new_order = malloc(sizeof(*new_order));
        if(new_order==NULL) return 0;
        unsigned int new_id = getLastOrderId(orders_list);
        new_order->order_id=new_id;
        new_order->order_data= asCreate(copyProductDataToASElement,freeProductDataToASElement,compareProductDataToASElement);
            if(setAdd(orders_list,(SetElement)new_order)==SET_SUCCESS) {
                free(new_order);
                return new_id;
        }
            free(new_order) ;
            return 0 ;
    }
// Check the error they said

MatamikyaResult mtmChangeProductAmountInOrder(Matamikya matamikya, const unsigned int
orderId, const unsigned int productId, const double amount){
    if(matamikya == NULL) return MATAMIKYA_NULL_ARGUMENT;
    Set orders_list = matamikya->orders;
    AmountSet warehouse = matamikya->warehouse;
    if( !warehouse ) return MATAMIKYA_NULL_ARGUMENT;
    if( !orders_list) return MATAMIKYA_ORDER_NOT_EXIST;
    if (amount==0) return MATAMIKYA_SUCCESS;
   Order found_order = (Order) findOrder(orders_list, orderId);
    AmountSet found_order_data = found_order->order_data;
    ASElement product = getProductFromAmountSet(found_order_data, productId);
            if (product == NULL) { // This is the adding part
                if(amount>0) {
                    ASElement product_to_add = getProductFromAmountSet(warehouse, productId);
                    if (checkAmountType(amount , (*(ProductData)product_to_add).amount_type)) return MATAMIKYA_INVALID_AMOUNT ;
                    AmountSetResult register_result = asRegister(found_order_data, product_to_add);
                    assert(register_result != AS_NULL_ARGUMENT);
                    if (register_result == AS_OUT_OF_MEMORY)return MATAMIKYA_OUT_OF_MEMORY;
                    AmountSetResult change_amount_result = asChangeAmount(found_order_data, product_to_add, amount);
                    assert(change_amount_result == AS_SUCCESS);
                }
                else return MATAMIKYA_INSUFFICIENT_AMOUNT ;
            }

            if (checkAmountType(amount , (*(ProductData)product).amount_type)) return MATAMIKYA_INVALID_AMOUNT ;
            AmountSetResult change_amount_result = asChangeAmount(found_order_data, product, amount);
            double new_amount = 0;
            asGetAmount(found_order_data,product,&new_amount);
            if(change_amount_result==AS_INSUFFICIENT_AMOUNT || new_amount== 0) {
                asDelete(found_order_data,product);
                return MATAMIKYA_SUCCESS;
            }
            assert(change_amount_result==AS_SUCCESS);
            return MATAMIKYA_SUCCESS;
        }

MatamikyaResult mtmShipOrder(Matamikya matamikya, const unsigned int
orderId){
    if(matamikya == NULL) return MATAMIKYA_NULL_ARGUMENT;
    Set orders_list = matamikya->orders;
    if(orders_list == NULL) return MATAMIKYA_ORDER_NOT_EXIST;
    Order found_order =(Order) findOrder(orders_list, orderId);
    if(found_order == NULL) return MATAMIKYA_ORDER_NOT_EXIST;
    AmountSet warehouse = matamikya->warehouse;
    AmountSet found_order_data = found_order->order_data;
    ASElement current_item = asGetFirst(found_order_data);
    double amount_in_order = 0, amount_in_storage =0;
    while(current_item != NULL){
        ASElement product_in_warehouse = getProductFromAmountSet(warehouse,((ProductData)current_item)->Id);
        asGetAmount(warehouse,product_in_warehouse,&amount_in_storage);
        asGetAmount(found_order_data, current_item, &amount_in_order);
        if(amount_in_order > amount_in_storage)
            return MATAMIKYA_INSUFFICIENT_AMOUNT;
        current_item = asGetNext(found_order_data);
    }
    current_item = asGetFirst(found_order_data);
    while(current_item != NULL){
        ASElement product_in_warehouse = getProductFromAmountSet(warehouse,((ProductData)current_item)->Id);
        asGetAmount(warehouse,product_in_warehouse,&amount_in_storage);
        asGetAmount(found_order_data, current_item, &amount_in_order);
        ((ProductData)(product_in_warehouse))->total_sold += ((ProductData)product_in_warehouse)->getProductPriceFunc( (*(ProductData)product_in_warehouse).custom_data , amount_in_order);
        asChangeAmount(warehouse,product_in_warehouse,-amount_in_order);
        current_item = asGetNext(found_order_data);
    }
    asDestroy(found_order_data);
    setRemove(orders_list, found_order);
    return MATAMIKYA_SUCCESS;
}
MatamikyaResult mtmCancelOrder(Matamikya matamikya, const unsigned int
orderId){
    if(matamikya == NULL) return MATAMIKYA_NULL_ARGUMENT;
    Set orders = matamikya->orders;
    if(orders == NULL) return MATAMIKYA_ORDER_NOT_EXIST;
    SetElement found_order = findOrder(orders,orderId);
    if(found_order == NULL) return MATAMIKYA_ORDER_NOT_EXIST;
    asDestroy((*(Order)found_order).order_data);
    SetResult result = setRemove(orders,found_order);
    if(result != SET_SUCCESS) return MATAMIKYA_ORDER_NOT_EXIST;
    return MATAMIKYA_SUCCESS;
}
MatamikyaResult mtmPrintInventory(Matamikya matamikya, FILE *output)
{
   if(matamikya==NULL || output==NULL ) return MATAMIKYA_NULL_ARGUMENT;
   fprintf(output,"Inventory Status:\n");
   if(matamikya->warehouse==NULL) return MATAMIKYA_SUCCESS;
   AmountSet warehouse = matamikya->warehouse ;
    AS_FOREACH(ASElement , element , warehouse)
    {
        double amount=0;
        AmountSetResult result = asGetAmount(warehouse,element,&amount);
        assert(result==AS_SUCCESS);
        mtmPrintProductDetails((*(ProductData)element).Name , (*(ProductData)element).Id,amount,(*(ProductData)element).getProductPriceFunc((*(ProductData)element).custom_data , amount ), output );
    }
return MATAMIKYA_SUCCESS;
}
MatamikyaResult mtmPrintOrder(Matamikya matamikya, const unsigned int orderId, FILE *output)
{
    if(matamikya == NULL || output==NULL) return MATAMIKYA_NULL_ARGUMENT ;
    if(matamikya->orders == NULL ) return MATAMIKYA_ORDER_NOT_EXIST;
    Set orders = matamikya->orders ;
    SetElement order_to_print = findOrder(orders, orderId );
    if(order_to_print==NULL) return MATAMIKYA_ORDER_NOT_EXIST;
    mtmPrintOrderHeading( (*(Order)order_to_print).order_id,output);
    double total_order_price=0;
    AmountSet order_data_to_print = (*(Order)order_to_print).order_data ;
    AS_FOREACH(ASElement , element , order_data_to_print) {
        double amount = 0;
        AmountSetResult result = asGetAmount(order_data_to_print, element, &amount);
        assert (result == AS_SUCCESS) ;
            double total_product_price = (*(ProductData) element).getProductPriceFunc((*(ProductData) element).custom_data, amount);
            mtmPrintProductDetails((*(ProductData) element).Name, (*(ProductData) element).Id, amount, total_product_price/amount , output);
        total_order_price += total_product_price ;

    }
    mtmPrintOrderSummary(total_order_price, output);
    return MATAMIKYA_SUCCESS;
}

MatamikyaResult mtmPrintBestSelling(Matamikya matamikya, FILE *output){
    if(matamikya== NULL) return MATAMIKYA_NULL_ARGUMENT;
    if(matamikya->warehouse==NULL) {
        fprintf(output, "none\n");
        return MATAMIKYA_SUCCESS;
    }
        ASElement best_selling = asGetFirst(matamikya->warehouse) ;
        AS_FOREACH(ASElement , element , matamikya->warehouse)
        {
           if((*(ProductData)element).total_sold > (*(ProductData)best_selling).total_sold) best_selling=element ;
        }
        if((*(ProductData)best_selling).total_sold==0)
        {
            fprintf(output,"none\n");
            return MATAMIKYA_SUCCESS;
        }
    mtmPrintIncomeLine((*(ProductData)best_selling).Name,(*(ProductData)best_selling).Id,(*(ProductData)best_selling).total_sold, output );
    return MATAMIKYA_SUCCESS;
    }