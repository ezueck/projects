/* 
 * EE312 Simple CRM Project
 *
 * Eduardo Zueck Garces
 * EZ2959
 */

#include <stdio.h>
#include <assert.h>
#include "MyString.h"
#include "Invent.h"

int checkAvailability(String*, unsigned int, String*);
void updateData(int, String, unsigned int);
void printSales(int, const char[], int);
void initConstString();
void destConstString();

#define MAX_CUSTOMERS 1000
Customer customers[MAX_CUSTOMERS];
int num_customers = 0;
unsigned int inv_bottles = 0;
unsigned int inv_diapers = 0;
unsigned int inv_rattles = 0;
int somewhere_in_mem = 0;
String bottles;
String rattles;
String diapers;

/* you'll probably need several more global variables */
/* and of course, you have a few functions to write */


/* clear the inventory and reset the customer database to empty */
void reset(void) {
	//We have to free the heap from all of our customers and their data structs
	for (int k = 0; k < num_customers; k += 1) {
		StringDestroy(&customers[k].name); //Deallocation
		customers[k].bottles = 0; //Zero sales for customer k
		customers[k].diapers = 0;
		customers[k].rattles = 0;
	}

	//Zero our global variables 
	num_customers = 0;
	inv_bottles = 0;
	inv_diapers = 0; 
	inv_rattles = 0;
}

void processSummarize() {
	//Print elements of inventory and customers 
	printf("There are %d Bottles, %d Diapers and %d Rattles in inventory\n", inv_bottles, inv_diapers, inv_rattles);
	printf("we have had a total of %d different customers\n", num_customers);

	//Finding fun info 
	int count;
	int high_bot = 0; //Find the customer with more bottle sales 
	for (count = 0; count < num_customers; count += 1) {
		if (customers[count].bottles>=customers[high_bot].bottles) {
			high_bot = count;
		}
	}
	int high_dia = 0; //Find the customer with more diaper sales 
	for (count = 0; count < num_customers; count += 1) {
		if (customers[count].diapers>=customers[high_dia].diapers) {
			high_dia = count;
		}
	}
	int high_rat = 0; //Find the cutomer with more rattles sales 
	for (count = 0; count < num_customers; count += 1) {
		if (customers[count].rattles>=customers[high_rat].rattles) {
			high_rat = count;
		}
	}
	//We print the big spenders 
	printSales(high_bot, "Bottles", customers[high_bot].bottles);
	printSales(high_dia, "Diapers", customers[high_dia].diapers);
	printSales(high_rat, "Rattles", customers[high_rat].rattles);

}

void processPurchase() {
	//Variables to obtain our data (customer, item to buy, and number of items)
	String customer;
	String item;
	int *p = &somewhere_in_mem;
	unsigned int number;

	//Initialize string variables


	//Read our data from the text file 
	readString(&customer);
	readString(&item);
	readNum(p);
	number = *p;

	//Checking for availability
	if (!checkAvailability(&item, number, &customer)) {
		StringDestroy(&item);
		StringDestroy(&customer);

		return; //If not available return
	}
	
	//Update our customer database
	int i = 0;
	int found = 0;
	//Find an existing customer
	for (i = 0; i < num_customers && found==0; i += 1) { //Check for an existing customer 
		if (StringIsEqualTo(&customers[i].name, &customer)) {
			updateData(i, item, number); //Update data for existing customer 
			found = 1;
		}
	}
	//If no existing customer exists, create him and update its data
	if (found ==0) { 
		num_customers += 1;							//Update total of customers 
		customers[i].name = StringDup(&customer);	//Set his name 
		updateData(i, item, number);				//Update data for new customer 
	}

	//Destroy those pesky Strings
	StringDestroy(&item);
	StringDestroy(&customer);

}

/* This function processes new additions to our inventory
 * It obtains the kind of item and it's number from the text file 
 * It then adds the new inventory to its kind of item
*/
void processInventory() {
	//Variables to obtain our data
	String item;
	int* p = &somewhere_in_mem;
	unsigned int number;

	//Read our data from the text
	readString(&item);
	readNum(p);
	number = *p;

	//Initialize String variables
	initConstString();

	//Add to each kind of item using our constant strings 
	if (StringIsEqualTo(&item, &bottles)) {
		inv_bottles = inv_bottles + number;
	}
	else if (StringIsEqualTo(&item, &diapers)) {
		inv_diapers = inv_diapers + number;
	}
	else if (StringIsEqualTo(&item, &rattles)) {
		inv_rattles = inv_rattles + number;
	}

	//Destroy our Strings
	StringDestroy(&item);
	destConstString();
}

/* To make my life easier, I initialize constant strings for our items
 * This makes the use of StringIsEqualTo a lot easier :) 
 */
void initConstString() {
	//We initialize Strings for our items
	bottles = StringCreate("Bottles");
	rattles = StringCreate("Rattles");
	diapers = StringCreate("Diapers");
}

/*Strings need to be destroyed. F memory leaks right?*/
void destConstString() {
	StringDestroy(&bottles);
	StringDestroy(&rattles);
	StringDestroy(&diapers);
}

/* A function to standardize the printing of the error message*/
void printError(String* customer, String* item, unsigned int number) {
	printf("Sorry ");
	StringPrint(customer);
	printf(", we only have %d ", number);
	StringPrint(item);
	printf("\n");
}

/* This function checks for the availability of an item in the inventory
 * If enough are available for the requested number, we sell them, reducing our inventory
 * We return 0 if no sale was available 
 */
int checkAvailability(String* item, unsigned int number, String* customer) {
	//We run IFs for each kind of items, if there are enough we sell them 
	//If there aren't enough of them, we print the error message
	
	//Initialize some String variables 
	initConstString();
	
	//Run our comparisons
	if (StringIsEqualTo(item, &bottles)) { //If they want to buy bottles
		destConstString(); //Destroy constant strings of items 
		if (inv_bottles >= number) { 
			inv_bottles = inv_bottles - number; //Enough are available, sell them then
			return 1;
		}
		else {
			printError(customer, item, inv_bottles); //Not enough are available, don't sell them then and print error 
			return 0;
		}
	}
	else if (StringIsEqualTo(item, &diapers)) { //If they want to buy diapers 
		destConstString(); //Destroy constant strings of items 
		if (inv_diapers >= number) {
			inv_diapers = inv_diapers - number; //Enough are available, sell them then
			return 1;
		}
		else {
			printError(customer, item, inv_diapers); //Not enough are available, don't sell them then and print error 
			return 0;
		}
	}
	else if (StringIsEqualTo(item, &rattles)) { //If they want to buy rattles
		destConstString(); //Destroy constant strings of items 
		if (inv_rattles >= number) {
			inv_rattles = inv_rattles - number; //Enough are available, sell them then
			return 1;
		}
		else {
			printError(customer, item, inv_rattles); //Not enough are available, don't sell them then and print error 
			return 0; 
		}
	}
	destConstString();
	return 0;
}
/* This funtion updates the customer data for purchases
 * It checks which kind of purchase they did and then adds the amount 
*/
void updateData(int cus_num, String item, unsigned int number) {
	//Initialize our String variables 
	initConstString();

	//We update our customer struct, depending on the item and customer it is 
	if (StringIsEqualTo(&item, &bottles)) { //Update customer data for bottles
		customers[cus_num].bottles += number;
	}
	else if (StringIsEqualTo(&item, &diapers)) { //Update customer data for diapers 
		customers[cus_num].diapers += number;
	}
	else if (StringIsEqualTo(&item, &rattles)) { //Update customer data  for rattles
		customers[cus_num].rattles += number;
	}

	//Destroy our String variables
	destConstString();
}

/* This function prints our customer stats 
   It works for any item, to avoid writing it a lot of times 
   It prints the special case with zero sales 
 */
void printSales(int customer, const char item[], int sales) {
	if (sales == 0) {
		printf("no one has purchased any %s\n", item );
		return;
	}
	else {
		StringPrint(&customers[customer].name);
		printf(" has purchased the most %s (%d)\n", item, sales);
		return;
	}
}