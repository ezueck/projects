/*
 * Project4.c 
 *
 * Eduardo Zueck Garces
 * EID: ez2959
 * Discussion time: 1pm
 * 
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Set.h"

bool found = false;
int divisor = 0;
/*
 * Several of the required functions have been written already
 * You may use these functions as-is or you may modify them in any way. 
 * Please NOTE, you are responsible for ensuring that the functions work correctly in 
 * your project solution. Just because the function works correctly in my solution does 
 * NOT mean that the function will work correctly in yours. 
 */

/*
 * Design NOTES:
 *
 * The design provided in this starter kit assumes
 * (1) empty sets will be represented with length == 0 and elements == nullptr (i.e., address 0)
 * (2) amortized doubling is not used, and capacity is ignored/unused. Functions should assume that 
 * the amount of storage available in the elements[] array is equal to length
 */



/* done for you already */
void destroySet(Set* self) {
	free(self->elements);
}

/* done for you already */
void createEmptySet(Set* self) {
	self->len = 0;
	self->elements = 0;
}

/* done for you already */
void createSingletonSet(Set* self, int x) {
	self->elements = (int*) malloc(sizeof(int));
	self->elements[0] = x;
	self->len = 1;
}

/* done for you already */
void createCopySet(Set* self, const Set* other) {
	self->elements = (int*) malloc(other->len * sizeof(int));
	for (int k = 0; k < other->len; k += 1) {
		self->elements[k] = other->elements[k];
	}
	self->len = other->len;	
}

/* This function copies a segment of a set into a new set 
 * I was originally goign to use this for isMember but that would turn it into O(n)
*/
void copySegmentSet(Set*self, int i, int j, Set*result) {
	result->elements = (int*)malloc(sizeof(int)*(j - i));
	for (int count = 0; count < (j - i); count+=1) {
		result->elements[count] = self->elements[count + i];
	}
	result->len = j - i;
}
/* done for you already */
void assignSet(Set* self, const Set* other) {
	if (self == other) { return; }
	
	destroySet(self);
	createCopySet(self, other);
}

/* simple function that returns the absolute value of an int */
int absolute(int x) {
	if (x < 0) { return -x; }
	else { return x; }
}

/* return true if x is an element of self
 * this is O(logn)
*/
bool isMemberSet(const Set* self, int x) {

	//We intialize our boundaries
	int length = self->len;
	int upper = length- 1;
	int lower = 0;

	//While our boundaries are checking from low to high
	while (upper >= lower) {
		int mid = lower + (upper - lower) / 2; //Our mid checkpoint 
		if (self->elements[mid] == x) { //If it is our element return true
			return true;
		}
		else if (self->elements[mid] < x) { //If not, check what's above our current mid 
			lower = mid + 1;
		}
		else { //If not check what's below are currend mit 
			upper = mid - 1;
		}
	}
	return false;

}


/*
 * add x as a new member to this set. 
 * If x is already a member, then self should not be changed
 * Be sure to restore the design invariant property that elemnts[] remains sorted
 * (yes, you can assume it is sorted when the function is called, that's what an invariant is all about)
 * this is O(n) as it runs through all the elements 
 */
void insertSet(Set* self, int x) {
	//Deal with the case of an empty set
	if (self->len == 0) {
		createSingletonSet(self, x);
		return;
	}

	//We first check if x is part of the set  
	if (isMemberSet(self, x)) {
		return;
	}

	//If it is not, we add it!
	int new_len = self->len + 1; //New length
	int* newdata = (int*) malloc(new_len*sizeof(int)); //New block of data
	int k = 0; //Counter for original set 
	int added = 0; //Check if our x has been added or not 
	for (int i = 0; i < new_len ; i += 1) {
		//If x is smaller than the first element  
		if (k == 0 && x < self->elements[k] && added ==0) {
			newdata[i] = x;
			added = 1;
		}
		//If we are done with new original elements it must be the last element of our new Set
		else if (k == self->len && added ==0) {
			newdata[i] = x;
			added = 1;
		}
		//If k is not zero, we also need to check the next element
		else if (x > self->elements[k - 1] && x < self->elements[k] && added == 0 && k>0) {
			newdata[i] = x;
			added = 1;
		}
		else {
			newdata[i] = self->elements[k];
			k += 1;
		}
	}

	destroySet(self);
	//Update our struct
	self->elements = newdata;
	self->len = new_len;
}


/*
 * don't forget: it is OK to try to remove an element
 * that is NOT in the set.  
 * If 'x' is not in the set 'self', then
 * removeSet should do nothing (it's not an error)
 * Otherwise, ('x' IS in the set), remove x. Be sure to update self->length
 * It is not necessary (nor recommended) to call malloc -- if removing an element means the 
 * array on the heap is "too big", that's almost certainly OK, and reallocating a smaller array 
 * is almost definitely NOT worth the trouble
 * definitely O(n)
 */
void removeSet(Set* self, int x) {
	if (!isMemberSet(self, x)) { return; } //If it's not in the set, don't try anything. Added for efficiency 
	int k = 0; //The variable to see which element we are updating 
	for (int i = 0; k < self->len; i += 1) {
		//If the element is x, we jump it 
		if (self->elements[i] == x) {
			self->len -= 1;
		}
		//If not, just add it to the set;
		else {
			self->elements[k] = self->elements[i];
			k++;
		}
	}

}

/* done for you already */
void displaySet(const Set* self) {
	int k;
	
	printf("{");

	if (self->len == 0) { 
		printf("}"); 
	}
	else {
		for (k = 0; k < self->len; k += 1) {
			if (k < self->len - 1) {
				printf("%d,", self->elements[k]);
			} else {
				printf("%d}", self->elements[k]);
			}
		}
	}
}

/* return true if self and other have exactly the same elements
 * definitely o(n)
 */
bool isEqualToSet(const Set* self, const Set* other) {
	//Corner case, if they don't have the same number of elements
	if (self->len!= other->len) {
		return false;
	}
	
	//Corner case, where both have 0 elements
	if (self->len == 0 && other->len == 0) {
		return true;
	}
	//Checing if they have equal elements
	for (int i = 0; i < self->len; i += 1) {
		if (self->elements[i] != other->elements[i]) {
			return false;
		}
	}
	return true;
}

/* return true if every element of self is also an element of other 
 * this is O(n)
 * Originally used isMemberOf, but it makes it slower. 
*/

bool isSubsetOf(const Set* self, const Set* other) {
	//Variables to check elements.
	int i = 0;
	int k = 0;

	//Some easy cases 
	if (self->len == 0) { return true; } //Set zero is part of everything
	if (self->len > other->len) { return false; } //If it is bigger, it can't be a subset 
	if (self->elements[0] < other->elements[0]) { return false; } //By invariant it can't be a subset 
	
	//Check all elements 
	for (int i = 0; i < self->len; i += 1) {
		while (true) { //We run a loop 
			if (self->elements[i] == other->elements[k]) break; //We found a match, so we jump to next element in self 
			if (other->elements[k] > self->elements[i]) return false; //If it is bigger, we won't ever find it so return false
			k++;
			if (k == other->len) return false; //We ran out of elements. Sorry
		}
	}

	return true; //There was a mtch for everything, so return true!
}

/* done for you */
bool isEmptySet(const Set* self) {
	return self->len == 0;
}

/* remove all elements from self that are not also elements of other
 * This is definitely O(n) as we can discard those log(n)s fo the isMemberOf
 */
void intersectFromSet(Set* self, const Set* other) {

	//We first deal with the scenario of an empty other (we return an empty)
	if (isEmptySet(other)) {
		destroySet(self);
		createEmptySet(self);
		return;
	}
	//We make a set to fill in our data and make life easier
	Set result; 
	result.elements = (int*)malloc(self->len*sizeof(int));
	result.len = 0;
	int debug = 0;

	//Check if all elements of self are in other, if not they don't get added
	for (int i = 0; i < self->len; i += 1) {
		if (isMemberSet(other, self->elements[i])) {
			result.elements[result.len] = self->elements[i];
			result.len += 1;
		}
	}
	destroySet(self);
	self->elements = result.elements;
	self->len = result.len;
}

/* remove all elements from self that are also elements of other
 * Checked to be O(n) n being thesum of the two lengths
*/
void subtractFromSet(Set* self, const Set* other) {
	//Corner case of empty other 
	if (isEmptySet(other)) {
		return;
	}

	//Create a new result set 
	Set result;
	result.elements = (int*)malloc(self->len*sizeof(int));
	result.len = 0;

	//Variables to check elements and adding them 
	int i = 0;
	int k = 0; 
	int add = 0;

	//Run until we finish either of the sets
	for (i; i < self->len && k<other->len; i += 1) {
		if (self->elements[i] < other->elements[k]) { //If element in self is smaller we can add it 
			result.elements[add] = self->elements[i];
			add++;
			result.len += 1;
		}
		else if (self->elements[i] == other->elements[k]) { //If both elements are equal, don't add and increase k
			k++;
		}
		else if (self->elements[i]>other->elements[k]) { //If self[i] is bigger than other[k] add to k so we can see if equal
			k++;
			i--;
		}
	}

	//We ran out of elements in other, so the rest of self must be added 
	while (i < self->len) {
		result.elements[add] = self->elements[i];
		add++;
		result.len += 1;
		i++;
	}

	//Free our old data and update self 
	destroySet(self);
	self->elements = result.elements;
	self->len = result.len;
}

/* add all elements of other to self (obviously, without creating duplicate elements)
 * Previous implemenetation just tried to add them all. Not very efficient!
 * It is now definitely O(n), don't believe the tests LIES 
 */
void unionInSet(Set* self, const Set* other) {
	//Some corner cases
	if (isEmptySet(other)) { //If other is empty, don't add anything 
		return;
	}
	else if (isEmptySet(self)) { //If slef is empty, just return other 
		assignSet(self, other);
		return;
	}

	//Make a new set to add to our data
	Set result;
	result.elements = (int*)malloc((self->len+other->len)*sizeof(int));
	result.len = 0;

	//Variables to search through the sets 
	int i = 0;
	int k = 0;
	int add = 0;

	//Until we are done with self 
	for (i; i < self->len && k<other->len; i+=1) {
		if (self->elements[i] < other->elements[k]) { //If element in self is smaller we add it
			result.elements[add] = self->elements[i];
			result.len += 1;
			add++;
		}
		else if (self->elements[i] == other->elements[k]) { //If they are equal, we only add it once
			result.elements[add] = self->elements[i];
			result.len += 1;
			add++;
			k++; 
		}
		else if (self->elements[i] > other->elements[k]) { //If element in self is bigger, we add elements from other 
			result.elements[add] = other->elements[k];
			result.len += 1;
			add++;
			k++;
			i--;
		}
	}

	//Sanity check before adding rest of self
	if (self->elements[i] == result.elements[add - 1]) {
		i++;
	}

	//Add remaining elements of self 
	while (i < self->len) {
		result.elements[add] = self->elements[i];
		result.len += 1;
		add++;
		i++;
	}

	//Sanity check before adding rest of other 
	if (other->elements[k] == result.elements[add - 1]) {
		k++;
	}

	//Add remaining elements of other 
	while (k < other->len) {
		result.elements[add] = other->elements[k];
		result.len += 1;
		add++;
		k++;
	}

	//Free our old data and update self 
	destroySet(self);
	self->elements = result.elements;
	self->len = result.len;

}