#include "wackystore.c"
#include <assert.h>
#include <time.h>

void print_customer(Customer* customer) {
    printf("Customer: %s\n", customer->name);
    printf("  Cart [%d]:\n", total_number_of_items(customer));

    ItemNode* head = customer->cart;
    while (head != NULL) {
        printf("    - %s x %d\n", head->name, head->count);
        head = head->next;
    }

    printf("\n");
}

void test_single_item_in_cart() {
    Customer* customer = new_customer("Charles");

    // First item in customer's cart should be some V-Bucks
    add_item_to_cart(customer, "V-Bucks", 2800);
    assert(strcmp(customer->cart->name, "V-Bucks") == 0);
    assert(customer->cart->count == 2800);

    // Customer wants some more V-Bucks.
    add_item_to_cart(customer, "V-Bucks", 10);
    assert(strcmp(customer->cart->name, "V-Bucks") == 0);
    assert(customer->cart->count == 2810);

    // Customer removes some V-Bucks.
    remove_item_from_cart(customer, "V-Bucks", 23);
    assert(strcmp(customer->cart->name, "V-Bucks") == 0);
    assert(customer->cart->count == 2787);

    // Customer removes all V-Bucks. Cart is empty.
    remove_item_from_cart(customer, "V-Bucks", 999999);
    assert(customer->cart == NULL);

    free_customer(customer);
}

void test_many_items_in_cart() {
    Customer* customer = new_customer("Charles");

    // Customer wants some more items.
    add_item_to_cart(customer, "bAnAnA", 63);
    add_item_to_cart(customer, "duRiAn", 55);
    add_item_to_cart(customer, "AppLe", 42);
    add_item_to_cart(customer, "CheRRy", 23);
    // There should be a total of 183 items.
    
    assert(total_number_of_items(customer) == 183);
    // Print out customer.
    print_customer(customer);
    // All items should be in ascending strcmp() sorted order.
    
    assert(strcmp(customer->cart->name, "AppLe") == 0);
    assert(strcmp(customer->cart->next->name, "CheRRy") == 0);
    assert(strcmp(customer->cart->next->next->name, "bAnAnA") == 0);
    assert(strcmp(customer->cart->next->next->next->name, "duRiAn") == 0);
    
    free_customer(customer);
    
}

void test_single_checkout_lane() {
    Customer* charles = new_customer("Charles");
    add_item_to_cart(charles, "RP", 10000);

    Customer* helen = new_customer("Helen");
    add_item_to_cart(helen, "Pikachu Plushie", 1);
    add_item_to_cart(helen, "Advil", 30);

    CheckoutLane* lane = open_new_checkout_line();
    queue(helen, lane);
    queue(charles, lane);

    // Helen is first in the queue, and bought 31 items.
    assert(process(lane) == 31);
    // Charles is next, with 10000 items.
    assert(process(lane) == 10000);

    // Queue should be empty now.
    assert(process(lane) == 0);
    assert(lane->first == NULL);
    assert(lane->last == NULL);

    // Close the store. There was only one lane open.
    close_store(&lane, 1);
}

void print_customers_in_lane(char lane_id[], CheckoutLane* lane) {
    printf("Lane %s:\n\t-> ", lane_id);

    CheckoutLaneNode* head = lane->first;
    while (head != NULL) {
        printf("%s ", head->customer->name);
        head = head->back;
    }

    printf("\n");
}


/**
 * This test case is available as a diagram on Quercus.
 * Check the diagram listed under balance_lanes() function signature.
 */
void test_multiple_checkout_lanes() {
    // Create all lanes and customers.
    CheckoutLane* lanes[] = {open_new_checkout_line(), open_new_checkout_line(),
                             open_new_checkout_line()};

    Customer* alex = new_customer("Alex");
    Customer* billy = new_customer("Billy");
    Customer* charles = new_customer("Charles");
    Customer* devin = new_customer("Devin");
    Customer* eric = new_customer("Eric");
    Customer* frank = new_customer("Frank");
    Customer* gary = new_customer("Gary");
    Customer* henry = new_customer("Henry");
    Customer* issac = new_customer("Issac");

    // Add customers to lanes, as shown on diagram.
    queue(alex, lanes[0]);

    queue(billy, lanes[1]);
    queue(charles, lanes[1]);
    queue(devin, lanes[1]);
    queue(eric, lanes[1]);

    queue(frank, lanes[2]);
    queue(gary, lanes[2]);
    queue(henry, lanes[2]);
    queue(issac, lanes[2]);

    // Show the lanes after each call to balance_lanes()
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", lanes[0]);
    print_customers_in_lane("Lane 2", lanes[1]);
    print_customers_in_lane("Lane 3", lanes[2]);
    printf("\n");

    balance_lanes(lanes, 3);

    printf("Showing Lanes after calling balance_lanes()...\n");
    print_customers_in_lane("Lane 1", lanes[0]);
    print_customers_in_lane("Lane 2", lanes[1]);
    print_customers_in_lane("Lane 3", lanes[2]);
    printf("\n");

    balance_lanes(lanes, 3);

    printf("Showing Lanes after calling balance_lanes()...\n");
    print_customers_in_lane("Lane 1", lanes[0]);
    print_customers_in_lane("Lane 2", lanes[1]);
    print_customers_in_lane("Lane 3", lanes[2]);
    printf("\n");

    // Serve no customers, and cleanup the store.
    close_store(lanes, 3);
}

int main(void) {
    time_t start_time;
    time(&start_time);
    test_single_item_in_cart(); // passed
    test_many_items_in_cart(); // passed
    test_single_checkout_lane(); // passed
    test_multiple_checkout_lanes(); // passed
    printf("Looks good. \nNext, commence execution of R Tests.\n");

    // R - Tests Begin
    printf("--------------------------------------------------------\n\n");
    
    // R1 - new_item_node() Edge Case
    ItemNode *s = new_item_node("", 1);
    //printf("Address: %p, Name: %s, Count: %d\n", s, s->name, s->count);
    if (strcmp(s->name, "") == 0 && s->count == 1) printf("R1 - new_item_node() With Empty String Passed.\n\n");

    // R2 - new_customer() Edge Case
    Customer *s1 = new_customer("");
    //printf("Address: %p, Name: %s\n", s1, s1->name);
    if (strcmp(s->name, "") == 0) printf("R2 - new_customer() With Empty String Passed.\n\n");

    // R3 - new_checkout_node() Edge Case
    CheckoutLaneNode *n = new_checkout_node(s1);
    //printf("Address of n: %p, Address of customer: %p, Name: %s\n", n, n->customer, n->customer->name);
    if (strcmp(s1->name, n->customer->name) == 0 && s1 == n->customer) printf("R3 - new_checkout_node() With Empty Customer Passed.\n\n");
    free(n);

    // R4 - free_customer() Edge Case
    s1->cart = s;
    free_customer(s1);
    //printf("Address: %p, Name: %s, Item Name: %s, Count: %d\n", s1, s1->name, s1->cart->name, s1->cart->count);
    if(strcmp(s1->name, "") == 0 && strcmp(s1->cart->name, "") == 0 && s1->cart->count == 1) printf("R4 - free_customer() Empty Case Passed.\n\n");

    // R5 - total_number_of_items() General Case
    Customer *s2 = new_customer("Rohith");
    add_item_to_cart(s2, "Oranges", 1);
    add_item_to_cart(s2, "Bananas", 300);
    add_item_to_cart(s2, "Lemons", 49);
    add_item_to_cart(s2, "Apples", 2);
    int total = total_number_of_items(s2);
    free_customer(s2);
    if (total == 300+1+49+2) printf("R5 - total_number_of_items() General Case Passed.\n\n");

    // R6 - total_number_of_items() Edge Case
    Customer *s3 = new_customer("");
    add_item_to_cart(s3, "", 1);
    add_item_to_cart(s3, "", 300);
    add_item_to_cart(s3, "", 490);
    add_item_to_cart(s3, "", 23232);
    int total2 = total_number_of_items(s3);
    if (total2 == 300+1+490+23232) printf("R6 - total_number_of_items() Edge Case Passed.\n\n");

    // R7 - add_item_to_cart() Negative Item Case
    Customer *s4 = new_customer("Bob");
    add_item_to_cart(s4, "Oranges", 1);
    add_item_to_cart(s4, "Bananas", 300);
    add_item_to_cart(s4, "Lemons", 49);
    add_item_to_cart(s4, "Apples", 2);
    add_item_to_cart(s4, "raw berry", -2);
    int total3 = total_number_of_items(s4);
    if (total3 == total) printf("R7 - add_item_to_cart() Negative Item Case Passed.\n\n");

    // R8 - add_item_to_cart() NULL Case
    add_item_to_cart(NULL, "apple", 1);
    printf("R8 - add_item_to_cart() NULL Case Passed.\n\n");

    // R9 - add_item_to_cart() Item Repetition Case
    if(strcmp(s3->cart->name, "") == 0 && s3->cart->next == NULL) printf("R9 - add_item_to_cart() Item Repetition Case Passed.\n\n");
    free_customer(s3);

    // R10 - remove_item_from_cart() Negative Case
    remove_item_from_cart(s4, "Oranges", -1);
    ItemNode* p = NULL;
    p = s4->cart;
    while(p->next != NULL){
        //printf("Name: %s, Count: %d.\n", p->name, p->count);
        p = p->next;
    }
    if (p->count == 1) printf("R10 - remove_item_from_cart() Negative Case Passed.\n\n");
    
    // R11 - remove_item_from_cart() NULL Case
    remove_item_from_cart(NULL, "Oranges", 12);
    printf("R11 - remove_item_from_cart() NULL Case\n\n");
    
    // R12 - remove_item_from_cart() Empty String Case
    remove_item_from_cart(s4, "", 12);
    int total5 = total_number_of_items(s4);
    if(total5 == total3) printf("R12 - remove_item_from_cart() Empty String Case Passed.\n\n");

    // R13 - remove_item_from_cart() Item Deletion Case
    remove_item_from_cart(s4, "Oranges", 1);
    remove_item_from_cart(s4, "Apples", 500);
    int total6 = total_number_of_items(s4);
    if(total6 == total3-3) printf("R13 - remove_item_from_cart() Item Deletion Case Passed.\n\n");
    
    free_customer(s4);
    Customer *s5 = new_customer("Robert Jr.");
    CheckoutLane *l5 = open_new_checkout_line();

    // R14 - queue() General Case
    queue(s5, l5);
    if (l5->first != NULL && l5->first == l5->last) printf("R14 - queue() General Case Passed.\n\n");
    
    // R15 - queue() NULL Lane Case
    queue(s5, NULL);
    printf("R15 - queue() NULL Lane Case Passed.\n\n");

    // R16 - queue() NULL Customer Case
    queue(NULL, l5);
    if(l5->first != NULL && l5->first == l5->last) printf("R16 - queue() NULL Customer Case Passed.\n\n");

    // R17 - queue() NULL NULL Case
    queue(NULL, l5);
    if(l5->first != NULL && l5->first == l5->last) printf("R17 - queue() NULL NULL Case Passed.\n\n");

    // R18 - queue() Multiple Customers Case
    Customer *s6 = new_customer("R");
    Customer *s7 = new_customer("A");
    Customer *s8 = new_customer("Z");
    queue(s6, l5);
    queue(s7, l5);
    queue(s8, l5);

    CheckoutLaneNode *pointer = NULL;
    pointer = l5->last;
    while(pointer != NULL){
        //printf("Name: %s\n", pointer->customer->name);
        pointer = pointer->front;
    }
    if (l5->first->customer == s5 && l5->last->customer == s8) printf("R18 - queue() Multiple Customers Case Passed.\n\n");

    // R19 - process() General Case
    process(l5);
    pointer = l5->last;
    while(pointer != NULL){
        //printf("Name: %s\n", pointer->customer->name);
        pointer = pointer->front;
    }
    if (l5->first->customer == s6 && l5->last->customer == s8) printf("R19 - process() General Case Passed.\n\n");
    
    // R20 - process() Direct NULL Case
    process(NULL);
    printf("R20 - process() Direct NULL Case.\n\n");

    // R21 - process() Indirect NULL Case
    CheckoutLane *l6 = open_new_checkout_line();
    process(l6);
    pointer = l6->last;
    while(pointer != NULL){
        printf("Name: %s\n", pointer->customer->name);
        pointer = pointer->front;
    }
    if (pointer == NULL) printf("R21 - process() Indirect Case.\n\n");

    // R22 - process() Single Customer Case
    Customer *s9 = new_customer("G");
    queue(s9, l6);
    process(l6);
    if (l6->first == NULL && l6->first == l6->last) printf("R22 - process() Single Customer Case Passed.\n\n");

    // R23 - balance_lanes() Empty Lane Case
    CheckoutLane *l7 = open_new_checkout_line();
    CheckoutLane *l8 = open_new_checkout_line();
    CheckoutLane *l9 = open_new_checkout_line();
    CheckoutLane *l10 = open_new_checkout_line();
    CheckoutLane *lanes[] = {l5, l6, l7, l8, l9, l10};
    /**
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", lanes[0]);
    print_customers_in_lane("Lane 2", lanes[1]);
    print_customers_in_lane("Lane 3", lanes[2]);
    print_customers_in_lane("Lane 4", lanes[3]);
    print_customers_in_lane("Lane 5", lanes[4]);
    print_customers_in_lane("Lane 6", lanes[5]);
    printf("\n");
    **/
    if (balance_lanes(lanes, 6)) printf("R23 - balance_lanes() Empty Lane Case Passed.\n\n");
    // R24 - balance_lanes() Multiple Lanes Case
    /**
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", lanes[0]);
    print_customers_in_lane("Lane 2", lanes[1]);
    print_customers_in_lane("Lane 3", lanes[2]);
    print_customers_in_lane("Lane 4", lanes[3]);
    print_customers_in_lane("Lane 5", lanes[4]);
    print_customers_in_lane("Lane 6", lanes[5]);
    printf("\n");
    **/

    if (balance_lanes(lanes, 6)) printf("R24 - balance_lanes() Multiple Lanes Case Passed.\n\n");

    // R25 - balance_lanes() Max - Min <= 1 Case
    /**
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", lanes[0]);
    print_customers_in_lane("Lane 2", lanes[1]);
    print_customers_in_lane("Lane 3", lanes[2]);
    print_customers_in_lane("Lane 4", lanes[3]);
    print_customers_in_lane("Lane 5", lanes[4]);
    print_customers_in_lane("Lane 6", lanes[5]);
    printf("\n");
    **/
    if (balance_lanes(lanes, 6) == false) printf("R25 - balance_lanes() Max - Min <= 1 Case Passed.\n\n");
    /**
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", lanes[0]);
    print_customers_in_lane("Lane 2", lanes[1]);
    print_customers_in_lane("Lane 3", lanes[2]);
    print_customers_in_lane("Lane 4", lanes[3]);
    print_customers_in_lane("Lane 5", lanes[4]);
    print_customers_in_lane("Lane 6", lanes[5]);
    printf("\n");
    **/

    // R26 - balance_lanes() <= 2 Total Lanes Case
    if(balance_lanes(lanes, 2) == false) printf("R26 - balance_lanes() <= 2 Total Lanes Case Passed.\n\n");

    // R27 - balance_lanes() All Empty Lanes Case
    CheckoutLane *l11 = open_new_checkout_line();
    CheckoutLane *l12 = open_new_checkout_line();
    CheckoutLane *l13 = open_new_checkout_line();

    CheckoutLane *EmptyLanes[] = {l11, l12, l13};
    if (balance_lanes(EmptyLanes, 3) == false) printf("R27 - balance_lanes() All Empty Lanes Case Passed.\n\n");
    /**
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", EmptyLanes[0]);
    print_customers_in_lane("Lane 2", EmptyLanes[1]);
    print_customers_in_lane("Lane 3", EmptyLanes[2]);
    printf("\n");
    **/

    // R28 - process_all_lanes() All Empty Lanes Case
    /**
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", EmptyLanes[0]);
    print_customers_in_lane("Lane 2", EmptyLanes[1]);
    print_customers_in_lane("Lane 3", EmptyLanes[2]);
    printf("\n");
    **/
    int total9 = process_all_lanes(EmptyLanes, 3);
    if(total9 == 0) printf("R28 - process_all_lanes() All Empty Lanes Case Passed.\n\n");
    /**
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", EmptyLanes[0]);
    print_customers_in_lane("Lane 2", EmptyLanes[1]);
    print_customers_in_lane("Lane 3", EmptyLanes[2]);
    printf("\n");
    **/

    // R29 - process_all_lanes() Empty Cart Case
    /**
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", lanes[0]);
    print_customers_in_lane("Lane 2", lanes[1]);
    print_customers_in_lane("Lane 3", lanes[2]);
    print_customers_in_lane("Lane 4", lanes[3]);
    print_customers_in_lane("Lane 5", lanes[4]);
    print_customers_in_lane("Lane 6", lanes[5]);
    printf("\n");
    **/
    int total10 = process_all_lanes(lanes, 6);
    if(total10 == 0) printf("R29 - process_all_lanes() Empty Cart Case Passed.\n\n");
    /**
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", lanes[0]);
    print_customers_in_lane("Lane 2", lanes[1]);
    print_customers_in_lane("Lane 3", lanes[2]);
    print_customers_in_lane("Lane 4", lanes[3]);
    print_customers_in_lane("Lane 5", lanes[4]);
    print_customers_in_lane("Lane 6", lanes[5]);
    printf("\n");
    **/

    // R30 - process_all_lanes() Non-Empty Cart Case
    Customer* c1 = new_customer("Robert");
    Customer* c2 = new_customer("Jack");
    Customer* c3 = new_customer("Peter");
    add_item_to_cart(c1, "Apples", 500);
    add_item_to_cart(c2, "Orange", 10);
    add_item_to_cart(c2, "Bananas", 1000);
    queue(c1, lanes[0]);
    queue(c2, lanes[5]);
    queue(c3, lanes[5]);

    /**
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", lanes[0]);
    print_customers_in_lane("Lane 2", lanes[1]);
    print_customers_in_lane("Lane 3", lanes[2]);
    print_customers_in_lane("Lane 4", lanes[3]);
    print_customers_in_lane("Lane 5", lanes[4]);
    print_customers_in_lane("Lane 6", lanes[5]);
    printf("\n");
    **/

    int total11 = process_all_lanes(lanes, 6);
    if(total11 == 1510) printf("R30 - process_all_lanes() Non-Empty Cart Case Passed.\n\n");
    
    /**
    printf("Showing Initial Lanes ...\n");
    print_customers_in_lane("Lane 1", lanes[0]);
    print_customers_in_lane("Lane 2", lanes[1]);
    print_customers_in_lane("Lane 3", lanes[2]);
    print_customers_in_lane("Lane 4", lanes[3]);
    print_customers_in_lane("Lane 5", lanes[4]);
    print_customers_in_lane("Lane 6", lanes[5]);
    printf("\n");
    **/
   
    // RXX - close_store() All Cases
    close_store(lanes, 6);
    close_store(EmptyLanes, 3);
    time_t end_time;
    time(&end_time);

    time_t elapsed_time = end_time - start_time;
    printf("Sorting the list took %ld seconds ...\n", elapsed_time);

    printf("RXX - close_store() All Cases Passed.\n\n");

    return 0;
}
