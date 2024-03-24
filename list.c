#include "list.h"
#include <stdlib.h>
#include <stdio.h>

//static allocation, with initialization function to initialize the stack
static List headsInList[LIST_MAX_NUM_HEADS];
static Node nodesInList[LIST_MAX_NUM_NODES];
static int freelistheadnodes[LIST_MAX_NUM_HEADS];
static int topListHeads = -1;
static int freeNodesInList[LIST_MAX_NUM_NODES];
static int topNodes  = -1; 
static bool setInitialize = false;
static void initialize() {
   topListHeads=-1;
    topNodes=-1;
    for (int i = LIST_MAX_NUM_HEADS - 1; i >= 0; --i) {
        //0:9, 
        freelistheadnodes[++topListHeads] = i;
    }
    for (int i = LIST_MAX_NUM_NODES - 1; i >= 0; --i) {
        freeNodesInList[++topNodes] = i;
    }
    setInitialize=true;
}

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create(){
    if(setInitialize==false){
        initialize();
    }
    if(topListHeads == -1) 
    {
        printf("Maximum number of lists reached. Cannot create more lists.\n");
        return NULL;
    }
    int index = freelistheadnodes[topListHeads--];
    if (index < 0 || index >= LIST_MAX_NUM_HEADS) {
        printf("Invalid index for list creation.\n");
        return NULL;
    }
    headsInList[index].statusCondition = LIST_OOB_START;
    headsInList[index].size = 0;
    headsInList[index].current = NULL;
    headsInList[index].head = NULL;
    headsInList[index].tail = NULL;
    return &headsInList[index];
}

// create a new node with value void* item
Node* Node_create(void* pItem) {
    if (topNodes == -1) {
        return NULL;
    }
    int index = freeNodesInList[topNodes--];
    nodesInList[index].next = NULL;
    nodesInList[index].prev = NULL;
    nodesInList[index].item=pItem;
    return &nodesInList[index];
}

// Returns the number of items in pList.
int List_count(List* pList){
    return pList->size;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList){
      if (pList == NULL || pList->head == NULL) {
        pList->current=NULL;
        return NULL;
    } else {
        pList->current=pList->head;
        return pList->current->item;
    }
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
    if(pList==NULL || pList->tail==NULL){
        pList->current=NULL;
        return NULL;
    }
    else{
        pList->current=pList->tail;
        return pList->current->item;
    }
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
    if(pList == NULL) {
        pList->statusCondition = LIST_OOB_END;
        return NULL;
    }
    else if(pList->current==NULL){
        pList->statusCondition = LIST_OOB_END;
        pList->current = NULL;
        return NULL;
    }
    else if(pList->current->next == NULL){
        pList->statusCondition = LIST_OOB_END;
        pList->current = NULL;
        return NULL;
    }
    else{
        pList->current = pList->current->next;
        printf("Returning %p from List_next\n", pList->current->item);
        return pList->current->item;
    }
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
   if(pList == NULL){
        pList->current = NULL;
        pList->statusCondition = LIST_OOB_START;
        return NULL;
    }
    if(pList->current==NULL|| pList->current->prev==NULL){
        pList->current = NULL;
        pList->statusCondition = LIST_OOB_START;
        return NULL;
    }
    else{
        pList->current = pList->current->prev;
        printf("Returning %p from List_prev\n", pList->current->item);
        return pList->current->item;
    }
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList){
    if(pList==NULL){
        return NULL;
    }
    else if(pList->current==NULL){
        printf("Null item or Null current, so returning NULL");
        return NULL;
    }
    else{
        return pList->current->item;
    }
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_after(List* pList, void* pItem){
     Node* newNode = Node_create(pItem);
    
    // base case, no more nodes
    if(newNode == NULL || pList==NULL){
         return LIST_FAIL;
    }

    // if list empty add new node
    if(pList->head == NULL){
        pList->head = newNode;
        pList->current = newNode;
        pList->tail = newNode;
    } else if(pList->current != NULL) { 
        if(pList->current->next != NULL){
            newNode->prev = pList->current;
            newNode->next = pList->current->next;
            pList->current->next->prev = newNode;
            pList->current->next = newNode;
            pList->current = newNode;
        }else{
            newNode->prev = pList->current;
            newNode->next = pList->current->next;
            pList->tail = newNode; 
            pList->current->next = newNode;
            pList->current = newNode;
        }
    }
    else if(pList->current==NULL){
        if(pList->statusCondition == LIST_OOB_END)
    {
        pList->tail->next = newNode;
        newNode->prev = pList->tail;
        pList->tail = newNode;
        pList->current = pList->tail;
    }

    else if(pList->statusCondition == LIST_OOB_START)
    { 
        pList->head->prev = newNode;
        newNode->next = pList->head;
        pList->head = newNode;
        pList->current = pList->head;
    } 
    }
    
    pList->size++;
    return LIST_SUCCESS;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_before(List* pList, void* pItem){
    Node* newNode = Node_create(pItem);
    
    if(newNode == NULL || pList==NULL) 
    {
        return LIST_FAIL;
    }
    if(pList->head ==NULL){
        pList->head = newNode;
        pList->current = newNode;
        pList->tail = newNode;
    } else if(pList->current != NULL) { 
        if(pList->current->prev != NULL){
            newNode->next = pList->current;
            newNode->prev = pList->current->prev;
            pList->current->prev->next = newNode;
            pList->current->prev = newNode;
            pList->current = newNode;
        }else{
            newNode->next = pList->current;
            newNode->prev = pList->current->prev;
            pList->head = newNode; 
            pList->current->prev = newNode;
            pList->current = newNode; 
        }
    }
    else if(pList->current==NULL){
        if(pList->statusCondition == LIST_OOB_END)
    {
        pList->tail->next = newNode;
        newNode->prev = pList->tail;
        pList->tail = newNode;
        pList->current = pList->tail;
    }

    else if(pList->statusCondition == LIST_OOB_START)
    { 
        pList->head->prev = newNode;
        newNode->next = pList->head;
        pList->head = newNode;
        pList->current = pList->head;
    } 
    }
    pList->size++;
    return LIST_SUCCESS;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem){
    Node* newNode = Node_create(pItem);
    bool badCondition=false;
    if(LIST_MAX_NUM_NODES<=pList->size)
    {
        badCondition=true;
    }
    if(pList==NULL || newNode==NULL || badCondition){
        return LIST_FAIL;
    }
    newNode->next = NULL;
    if(pList->tail==NULL){
        newNode->prev=NULL;
        pList->head=newNode;
        pList->tail=newNode;
    }
    else{
        pList->tail->next=newNode;
        newNode->prev=pList->tail;
        pList->tail=newNode;
    }
    pList->current=newNode;
    pList->size++;
    return LIST_SUCCESS;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem){
   Node* newNode = Node_create(pItem);
    if(pList==NULL || newNode==NULL){
        return LIST_FAIL;
    }
    newNode->prev=NULL;
    if(pList->head==NULL){
        pList->head=newNode;
        pList->tail=newNode;
        newNode->next=NULL;
    }
    else{
        pList->head->prev=newNode;
        newNode->next=pList->head;
        pList->head=newNode;
    }
    pList->current=newNode;
    pList->size++;
    return 0;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){
    if (pList==NULL|| pList->current == NULL || pList->size==0) {
        return NULL; 
    }
    Node* nodeToRemove = pList->current;
    void* removeData = nodeToRemove->item;

    if (nodeToRemove->prev != NULL) {
        nodeToRemove->prev->next = nodeToRemove->next;
    } else if(nodeToRemove==pList->head) {
        if(pList->head->next==NULL){
            pList->statusCondition=LIST_OOB_START;
        }
        pList->head = nodeToRemove->next;
        if (pList->head != NULL) {
            pList->head->prev = NULL;
            pList->current=pList->head;
        }
        else{
            pList->current=NULL;
        }
    }

    if (nodeToRemove->next != NULL) {
        nodeToRemove->next->prev = nodeToRemove->prev;
        pList->current = nodeToRemove->next;
    } else if(nodeToRemove->next==NULL && nodeToRemove==pList->tail) {
        //scenario: current item your removing is tail
        if(nodeToRemove->next==NULL){
            pList->statusCondition=LIST_OOB_END;
        }
        pList->tail = nodeToRemove->prev;
        if(pList->tail!=NULL){
            pList->tail->next=NULL;
            pList->current=pList->tail;
        }
        pList->current=NULL;
    }
     if (topNodes >= LIST_MAX_NUM_NODES - 1){
        pList->size--;
        return removeData;
    } 

    else{
        nodeToRemove->next=NULL;
        nodeToRemove->prev=NULL;
        nodeToRemove->item=NULL;
        freeNodesInList[++topNodes] = nodeToRemove - nodesInList;
        pList->size--;
        return removeData;
    }
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
    // base edge cases
    if (pList == NULL || pList->tail == NULL || pList->current==NULL) {
        return NULL;
    }
    Node *removeNode=pList->tail;
    void *removeData=removeNode->item;
    // CASE 1: Last item is head
    if(pList->head==removeNode){
        pList->tail=NULL;
        pList->head=NULL;
        pList->current=NULL;
    }
    // If last item is tail
    else if(removeNode->prev!=NULL){
        pList->tail=removeNode->prev;
        pList->tail->next=NULL;
        pList->current=pList->tail;
    }

    if (topNodes >= LIST_MAX_NUM_NODES - 1){
        pList->size--;
        return removeData;
    } 
    
    removeNode->next=NULL;
    removeNode->prev=NULL;
    removeNode->item=NULL;
    freeNodesInList[++topNodes] = removeNode - nodesInList;
    pList->size--;
    return removeData;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2){
    if (pList1 == NULL || pList2 == NULL) {
        return;
    }
    if (pList2->head == NULL) {
        if (topListHeads >= LIST_MAX_NUM_HEADS-1)
        {
            return;
        } 
        else 
        {
            pList2->head = NULL;
            pList2->tail = NULL;
            pList2->current = NULL;
            pList2->size = 0;
            freelistheadnodes[++topListHeads] = pList2 - headsInList;
            return;
        }
    }
    if (pList1->size == 0) {
        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
        pList1->size = pList2->size;
        pList1->current = NULL;
        pList1->statusCondition = LIST_OOB_START;
        if (topListHeads >= LIST_MAX_NUM_HEADS-1)
        {
            return;
        } 
        else 
        {
            pList2->head = NULL;
            pList2->tail = NULL;
            pList2->current = NULL;
            pList2->size = 0;
            freelistheadnodes[++topListHeads] = pList2 - headsInList;
            return;
        }
        return;
    }
    pList1->tail->next = pList2->head;
    pList2->head->prev = pList1->tail;
    pList1->tail = pList2->tail;
    pList1->size += pList2->size;
    if (topListHeads < LIST_MAX_NUM_HEADS - 1) {
        pList2->head = NULL;
        pList2->tail = NULL;
        pList2->current = NULL;
        freelistheadnodes[++topListHeads] = pList2 - headsInList;
    }
    pList2->size=0;
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
void List_free(List* pList, FREE_FN pItemFreeFn){
     if (pList == NULL) {
        return;
    }
    //you need to still keep track of your head, so make a temp you can iterate over llist with
    //also you need to free pTemp everytime so keep track of pTemp->next with another temp variable pNext
    Node* pTemp = pList->head;
    Node* pNext;

    while (pTemp != NULL) {
        pNext = pTemp->next;

        if (pItemFreeFn != NULL) {
            // Call the provided item freeing function
            (*pItemFreeFn)(pTemp->item);
            pList->size--;
        }
        if (pTemp->prev != NULL) {
            pTemp->prev->next = pTemp->next;
        } else {
            pList->head = pTemp->next;
        }

        if (pTemp->next != NULL) {
            pTemp->next->prev = pTemp->prev;
        } else {
            pList->tail = pTemp->prev;
        }
        // Free the node itself
        pTemp->next=NULL;
        pTemp->prev=NULL;
        pTemp->item=NULL;
        freeNodesInList[++topNodes] = pTemp - nodesInList;
        pTemp = pNext;
    }
    pList->size=0;
    pList->head = NULL;
    pList->tail = NULL;
    pList->current = NULL;
    freelistheadnodes[++topListHeads] = pList - headsInList;
    return;
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    bool currcondition=false;
    bool oobCondition=false;
    if (pList == NULL){
        return NULL;
    }
    if(pComparator==NULL)
    {
        return NULL;
    }
    if(pList->current==NULL){
        currcondition=true;
    }
    if (currcondition && pList->statusCondition == LIST_OOB_START) 
    {
        pList->current = pList->head;
    }
    
    Node* nodeCurr= pList->current;
    while (nodeCurr != NULL) {
        if (pComparator(nodeCurr->item, pComparisonArg) == 1) {
            pList->current = nodeCurr;
            oobCondition=true;
            return nodeCurr->item;
        }
        nodeCurr = nodeCurr->next;
    }
        if(!oobCondition)
        {
            pList->statusCondition = LIST_OOB_END;
            pList->current = NULL;
        }
    return NULL;
}
