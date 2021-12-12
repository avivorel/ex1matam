
#include "amount_set_str.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
 The struct :
 element: field for the strings
 amount :field for the amount of the element in the string
 next : the next element in the list ( always the set points to the header and the header is the smallest in the list
 current element : for the user uses that he point to the current element in the set
 */
typedef struct SortedLinkListed_t {
    char *element;
    double amount;
    struct SortedLinkListed_t* next;
} *List;

struct AmountSet_t {
    List header;
    List current_element;
} ;
/* asCreate();
 function that create a set and fill him with null argument for the pointer and 0 for the amount
 malloc allocation checked in the function and returns NULL if the allocation failed
 return pointer to the struct */
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
List listCreate()
{
    List list_element = malloc(sizeof(*list_element));
    if(list_element==NULL) return NULL ;
    list_element->element=NULL;
    list_element->amount=0;
    list_element->next=NULL;
    return(list_element);
}
List listCopy(List list_element)
{
    if(list_element == NULL) return NULL ;
    List copy= malloc(sizeof(*copy));
    if(copy==NULL) return NULL;
    char* element_copy= strCopy(list_element->element);
    if(element_copy==NULL){
        free(copy);
        return NULL;
    }
    copy -> element = element_copy ;
    copy->amount=list_element->amount;
    copy->next=list_element->next;
    return copy;
}
/*
 listDestroy :
 gets the header of the list and destory him and all the structs next to him
 */
void listDelete(List list)
{
    if(list==NULL || list->element==NULL) return ;
    free(list->element);
    free(list);
}
void listDestroy (List list_header)
{
    if(list_header == NULL) return;
    while(list_header != NULL)
    {
        List header_to_delete = list_header;
        list_header = list_header->next;
        listDelete(header_to_delete);
    }
}

AmountSet asCreate()
{
    AmountSet set = malloc(sizeof(*set));
    if(set==NULL) {
        return NULL;
    }
    set->header=NULL;
    set->current_element = NULL;
    return set;
}

/* asDestroy(set)
 gets pointer to the header of the set and free all the structs that in the list
 */
void asDestroy(AmountSet set)
{
    if(set==NULL) return ;
    AmountSetResult clear_result = asClear(set) ;
    // assert(set->header==NULL );
    if(clear_result==AS_SUCCESS)
    {
        free(set);
    }
    assert(clear_result!=AS_NULL_ARGUMENT);
}
AmountSetResult asClear(AmountSet set)
{

    if(set==NULL) return AS_NULL_ARGUMENT;
    if(set->header==NULL){
        set->current_element = NULL;
        return AS_SUCCESS;
    }
    listDestroy(set->header);
    set->header=NULL;
    set->current_element=NULL;
    return AS_SUCCESS;
}
AmountSetResult asDelete(AmountSet set, const char* element)
{
    if(set==NULL || element==NULL)
    {
        if (set != NULL ) set->current_element = NULL;
        return AS_NULL_ARGUMENT;
    }
    if(!asContains(set,element)) {
        set->current_element=NULL;
        return AS_ITEM_DOES_NOT_EXIST;
    }

    List previous_to_target;
    List target=set->header;
    if(strcmp(target->element, element) == 0 )
    {
        set->header=target->next;
        listDelete(target);
        set->current_element=NULL;
        return AS_SUCCESS;
    }
    previous_to_target=target;
    target=target->next;
    while ( strcmp(target->element, element) != 0 )
    {
        previous_to_target = target;
        target = target-> next;
    }
    previous_to_target->next=target->next;
    listDelete(target);
    set->current_element=NULL;
    return AS_SUCCESS;
}
/* asCopy()
 step 1 :Check if the set we got is empty if this is the case returning a clear copy ;
 step 2 : if the set is not clear loop until we got the element the points to null in the original set's list
 and copy every element on it to the set's copy ;
 */
AmountSet asCopy(AmountSet set) {
    if (set == NULL) return NULL;
    AmountSet set_copy = malloc(sizeof(*set_copy));
    if (set_copy == NULL) return NULL;
    if (set->header == NULL) {
        set->current_element=NULL;
        set_copy->header = NULL;
        set_copy->current_element = NULL;
        return set_copy;
    }
    List header = set->header;
    List next_to_copy;
    List header_copy;
    set_copy->header = listCopy(set->header);
    header_copy = set_copy->header;
    header_copy->next=NULL;
    while (header->next != NULL) {
        next_to_copy = listCopy(header->next);
        if(next_to_copy==NULL)
        {
            listDestroy(set_copy->header);
            free(set_copy);
            set->current_element=NULL;
            return NULL;
        }
        next_to_copy->next= header_copy->next;
        header_copy->next = next_to_copy;
        header_copy = next_to_copy;
        header = header->next;
    }
    set_copy->current_element=NULL;
    set->current_element=NULL;
    return set_copy;
}
/*
 asGetFirst :
 checks if we got null set or we got a set but it is empty ;
 if its not the case put in the current element the header and print the element string ;
 */

char* asGetFirst(AmountSet set) {
    if (set == NULL || set->header == NULL ) return NULL;
    set->current_element = set->header;
    List header = set->header;
  return header-> element;
}
/* asGetNext : check if the set is null or the current element is undefiend
 checks if the next element is null ( we or in the end of the list )
 if it's not the case puts the next element in the current element and print his string ;
 */
char* asGetNext(AmountSet set)
{
    if(set==NULL || set->current_element==NULL) return NULL ;
    List the_next = set->current_element;
    if(the_next->next == NULL) return NULL ;
    the_next = the_next->next;
    set->current_element = the_next;
    return the_next->element;
}
/*
 asGetSize :
 they want the current_element not to change so we wil work on a copy and count his number of element than destroy it ;
 */
int asGetSize(AmountSet set)
{
    if(set==NULL) return -1;
    if(set->header==NULL) return 0;
    int counter=0;
    List save_of_current_element = set->current_element;
    AS_FOREACH(char*,i,set)
    {
        counter++;
    }
    set->current_element=save_of_current_element;
    return counter ;
}
/* asConatins :
 Like asGetSize working on a copy and iterating the sets element and compare to the current element to the element .
 */
bool asContains(AmountSet set, const char* element)
{
    if(set==NULL || element==NULL) return false;
    List save_of_current_element = set->current_element;
    AS_FOREACH(char*,i,set)
    {
        if(strcmp(i,element)==0)
        {
            set->current_element=save_of_current_element;
            return true ;
        }
    }
    return false;
}
AmountSetResult asGetAmount(AmountSet set, const char* element, double* outAmount)
{
    if(set==NULL || element==NULL || outAmount==NULL) return AS_NULL_ARGUMENT;
    List save_of_current_element = set->current_element;
    if(asContains(set,element))
    {
        for(char* i= asGetFirst(set); strcmp(i,element)!=0 ; i= asGetNext(set)) ;
        List found = set->current_element;
        *outAmount = found->amount ;
        set->current_element=save_of_current_element;
        return AS_SUCCESS;
    }
    return AS_ITEM_DOES_NOT_EXIST ;
}
AmountSetResult asRegister(AmountSet set, const char* element) {
    if (set == NULL || element == NULL) return AS_NULL_ARGUMENT;
    List header = set->header;
    if (header == NULL || (strcmp(header->element, element) > 0)) {
        List new_list_header = listCreate();
        if(new_list_header==NULL) return AS_OUT_OF_MEMORY;
       char* element_copy= strCopy(element);
       if(element_copy==NULL)
       {
           free(new_list_header);
           return AS_OUT_OF_MEMORY;
       }
        new_list_header -> element = element_copy ;
        new_list_header->next = header;
        set->header = new_list_header;
        return AS_SUCCESS;
    }
    if ((strcmp(header->element, element) == 0)) return AS_ITEM_ALREADY_EXISTS;

    List new_element = listCreate();
    if(new_element==NULL) return AS_OUT_OF_MEMORY;
    char* element_copy= strCopy(element);
    {
        if(element_copy==NULL)
        {
            free(new_element);
            return AS_OUT_OF_MEMORY;
        }
    }

    new_element -> element = element_copy ;
    int compare_solution;
    List previous_element;
    while ( header->next != NULL)
    {
        previous_element = header;
        header = header->next;
        compare_solution = strcmp(header->element, new_element->element);
        if (compare_solution == 0) {
            free(element_copy);
            free(new_element);
            return AS_ITEM_ALREADY_EXISTS;
        }
        if (compare_solution > 0) {
            previous_element->next = new_element;
            new_element->next = header;
            return AS_SUCCESS;
        }
    }
    header->next=new_element;
    new_element->next=NULL;
    return AS_SUCCESS;
}

AmountSetResult asChangeAmount(AmountSet set, const char* element, double amount) {
    if (set == NULL || element == NULL) return AS_NULL_ARGUMENT;
    if(!asContains(set,element)) return AS_ITEM_DOES_NOT_EXIST;
    List header = set->header;
    while( strcmp( header->element , element) != 0) {
        header = header->next;
    }
    double new_amount = amount + header -> amount ;
    if (new_amount < 0) return AS_INSUFFICIENT_AMOUNT;
    else {
        header -> amount = new_amount ;
        return AS_SUCCESS;
    }
}