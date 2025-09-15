/**
 * Assignment 1 - The Wacky Store
 *
 * Please read the comments below carefully, they describe your task in detail.
 *
 * There are also additional notes and clarifications on Quercus.
 *
 * Any clarifications and corrections will be posted to Piazza so please keep an
 * eye on the forum!
 *
 * Unauthorized distribution or posting is strictly prohibited. You must seek
 * approval from course staff before uploading and sharing with others.
 */

// No additional imports are allowed. You can make helper functions if you wish.
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 1024

typedef struct ItemNode ItemNode;
struct ItemNode {
    char name[MAX_NAME_LENGTH];
    int count;
    ItemNode* next;
};

typedef struct Customer Customer;
struct Customer {
    char name[MAX_NAME_LENGTH];
    ItemNode* cart;
};

typedef struct CheckoutLaneNode CheckoutLaneNode;
struct CheckoutLaneNode {
    Customer* customer;

    CheckoutLaneNode* front;
    CheckoutLaneNode* back;
};

typedef struct CheckoutLane CheckoutLane;
struct CheckoutLane {
    CheckoutLaneNode* first;
    CheckoutLaneNode* last;
};

/**
 * Function: new_item_node
 * -----------------------
 * Allocate a new ItemNode. Allocation must be done manually (with malloc or
 * calloc). Initialize all variables using the arguments provided. Assume that
 * count will always be greater than 0.
 */

ItemNode* new_item_node(char* name, int count) {
    ItemNode *p = NULL;
    p = (ItemNode*)calloc(1, sizeof(ItemNode));
    if (p == NULL) exit(1);
    strcpy(p->name, name);
    p->count = count;
    p->next = NULL;
    return p;
}

/**
 * Function: new_customer
 * ----------------------
 * Allocate a new Customer. Allocation must be done manually (with malloc or
 * calloc). Initialize all variables using the arguments provided.
 */
Customer* new_customer(char* name) {
    Customer *p = NULL;
    p = (Customer*)calloc(1, sizeof(Customer));
    if (p == NULL){
        exit(1);
    }
    strcpy(p->name, name);
    p->cart = NULL;
    return p;
}

/**
 * Function: free_customer
 * -----------------------
 * Release all memory associated with a Customer back to the system. This
 * includes any items they may have had in their cart.
 */
void free_customer(Customer* customer) {
    if (customer != NULL){
        if(customer->cart != NULL){
            ItemNode *p = NULL;
            ItemNode *q = NULL;
            p = customer->cart;
            while(p!= NULL){
                q = p->next;
                free(p);
                p = q;
            }
        }
        free(customer);
    }
}

/**
 * Function: open_new_checkout_line
 * --------------------------------
 * Allocate a new empty checkout lane. Allocation must be done manually (with
 * malloc or calloc).
 */
CheckoutLane* open_new_checkout_line() {
    CheckoutLane *p = NULL;
    p = (CheckoutLane*)calloc(1, sizeof(CheckoutLane));
    if (p == NULL){
        exit(1);
    }
    p->first = NULL;
    p->last = NULL;
    
    return p;
}

/**
 * Function: new_checkout_node
 * ---------------------------
 * Allocate a new CheckoutLaneNode. Allocation must be done manually (with
 * malloc or calloc). Initialize all variables using the arguments provided. Do
 * not allocate a new customer; instead copy the existing reference over.
 */
CheckoutLaneNode* new_checkout_node(Customer* customer) {
    CheckoutLaneNode *p = NULL;
    p = (CheckoutLaneNode*)calloc(1, sizeof(CheckoutLaneNode));
    if (p == NULL){
        exit(1);
    }
    p->customer = customer;
    p->front = NULL;
    p->back = NULL;

    return p;
}

/**
 * Function: add_item_to_cart
 * --------------------------
 * Add an item to a customer's cart, given its name and amount.
 *
 * If the given amount is 0 or less, do nothing.
 *
 * IMPORTANT: The items in a customer's cart should always be arranged in
 * lexicographically smallest order based on the item names. Consider the use of
 * the ASCII strcmp() function from <string.h>.
 *
 * No two ItemNodes in a customer's cart can have the same name.
 * If the customer already has an ItemNode with the same item name in their
 * cart, increase the node's count by the given amount instead.
 */

void add_item_to_cart(Customer* customer, char* item_name, int amount) {
    if (customer == NULL || amount <= 0) return;
    ItemNode *new_item = new_item_node(item_name, amount);
    if (customer->cart == NULL){
        customer->cart = new_item;
        return;
    }
    
    ItemNode *p = NULL;
    ItemNode *q = NULL;
    p = customer->cart;

    while(p!=NULL){
        if(strcmp(p->name, item_name) == 0){
            p->count += amount;
            free(new_item);
            break;
        }
        else if(strcmp(p->name, item_name) > 0){
            if(q == NULL){
                customer->cart = new_item;
                new_item->next = p;
            }
            else{
                q->next = new_item;
                new_item->next = p;
            }
            break;
        }
        else if(p->next == NULL){
            p->next = new_item;
            break;
        }
        q = p;
        p = p->next;
    }
}

/**
 * Function: remove_item_from_cart
 * -------------------------------
 * Attempt to reduce the quantity of an item in a customer's cart, given its
 * name and amount.
 *
 * If no ItemNode in the customer's cart match the given item name, or the
 * amount given is <= 0, do nothing.
 *
 * If the quantity is reduced to a value less than or equal to 0, remove the
 * ItemNode from the customer's cart. This means you will need to do memory
 * cleanup as well.
 */

void remove_item_from_cart(Customer* customer, char* item_name, int amount) {
    if(customer == NULL || customer->cart == NULL || amount <= 0) return;
    ItemNode *p = NULL;
    ItemNode *q = NULL;

    p = customer->cart;
    while(p != NULL){
        if(strcmp(p->name, item_name) == 0){
            p->count -= amount;
            if(p->count <= 0){
                if(q == NULL){
                    customer->cart = p->next;
                    free(p);
                }
                else{
                    q->next = p->next;
                    free(p);
                }
            }
            break;
        }

        q = p;
        p = p->next;
    }
}

/**
 * Function: total_number_of_items
 * -------------------------------
 * Count the total number of items in a customer's cart by summing all ItemNodes
 * and their associated quantities.
 */
int total_number_of_items(Customer* customer) {
    ItemNode *p = NULL;
    int counter = 0;
    if (customer->cart == NULL) return counter;
    p = customer->cart;

    while(p != NULL){
        counter += p->count;
        p = p->next;
    }
    return counter;
}

/**
 * Function: queue
 * ---------------
 * A customer has finished shopping and wishes to checkout. Add the given
 * customer to the end of the given checkout lane.
 */
void queue(Customer* customer, CheckoutLane* lane) {
    if (lane != NULL && customer != NULL){
        CheckoutLaneNode *new_node = new_checkout_node(customer);
        if (lane->first == NULL) {
            lane->first = new_node;
            lane->last = new_node;
        } else {
            lane->last->back = new_node;
            new_node->front = lane->last;
            lane->last = new_node;
        }
    }
}

/**
 * Function: process
 * -----------------
 * Serve the customer at the head of the checkout lane and return the
 * total_number_of_items() the customer had in their cart.
 *
 * The customer leaves the store after being processed. Therefore, you must also
 * free any memory associated with them.
 *
 * If this function is called on an empty lane, return 0.
 */
int process(CheckoutLane* lane) {
    if ((lane == NULL) || (lane->first == NULL)) return 0;

    int amount = 0;
    Customer *customer = lane->first->customer;
    amount = total_number_of_items(customer);
    free_customer(customer);
    
    if(lane->first->customer == lane->last->customer){
        free(lane->first);
        lane->first = NULL;
        lane->last = NULL;
    }
    else{
        CheckoutLaneNode *q = NULL;
        q = lane->first->back;
        lane->first->back->front = NULL;
        lane->first->back = NULL;
        free(lane->first);
        lane->first = q;
    }
    return amount;
}


/**
 * Function: total_number_of_customers
 * -----------------------------------
 * Return the number of customers in a given lane,
 * Return 0 if given lane is empty.
*/

int total_number_of_customers(CheckoutLane* lane){
    int counter = 0;
    if (lane == NULL || lane->first == NULL) return counter;
    if (lane->first->customer == lane->last->customer) return counter+1;

    CheckoutLaneNode *p = NULL;
    p = lane->first;

    while(p != NULL){
        counter++;
        p = p->back;
    }
    return counter;
}



/**
 * Function: balance_lanes
 * -----------------------
 * Move a single customer from the end of the most busy checkout lane to the end
 * of the least busy checkout lane.
 *
 * Busyness is defined as the total number of customers in a checkout lane.
 *
 * If multiple lanes have the same busyness, select the lane that comes first in
 * the CheckoutLane* array.
 *
 * If the difference between the MAX and MIN checkout lanes is <= 1, do nothing.
 *
 * If there are less than 2 lanes, do nothing.
 *
 * Return true if and only if a customer was moved; otherwise false.
 */
bool balance_lanes(CheckoutLane* lanes[], int number_of_lanes) {
    if(number_of_lanes < 2) return false;

    int most_busy = -1;
    int least_busy = -1;
    CheckoutLane *most_busy_lane = NULL;
    CheckoutLane *least_busy_lane = NULL;
    for (int i = 0; i < number_of_lanes; i++){
        int busyness = total_number_of_customers(lanes[i]);
        if (most_busy == -1 || busyness > most_busy){
            most_busy = busyness;
            most_busy_lane = lanes[i];
        }
        if(least_busy == -1 || busyness < least_busy){
            least_busy = busyness;
            least_busy_lane = lanes[i];
        }
    }
    if(abs(least_busy - most_busy) <= 1) return false;
    CheckoutLaneNode* p = NULL;
    CheckoutLaneNode* q = NULL;
    p = most_busy_lane->last;
    q = p->front;
    q->back = NULL;
    p->front = NULL;
    p->back = NULL;
    most_busy_lane->last = q;
    queue(p->customer, least_busy_lane);
    free(p);
    return true;
}

/**
 * Function: process_all_lanes
 * ---------------------------
 * Given an array of CheckoutLane*, process() each CheckoutLane a single time
 * and return the the sum of the result.
 */
int process_all_lanes(CheckoutLane* lanes[], int number_of_lanes) {
    if(number_of_lanes == 0) return 0;

    int counter = 0;
    for(int i = 0; i < number_of_lanes; i++){
        int processed_amount = process(lanes[i]);
        counter += processed_amount;
    }

    return counter;
}

/**
 * Function: close_store
 * ---------------------
 * It's closing time. Given an array of CheckoutLane*, free all memory
 * associated with them. Any customers still left in the queue is kicked out and
 * also freed from memory.
 */
void close_store(CheckoutLane* lanes[], int number_of_lanes) {
    if(number_of_lanes != 0){

        for (int i = 0; i < number_of_lanes; i++){
            int amount = total_number_of_customers(lanes[i]);
            if (amount >= 1){
                for(int j = 0; j < amount; j++){
                    process(lanes[i]);
                }
            }
            free(lanes[i]);
        }
    }
}
