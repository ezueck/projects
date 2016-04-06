/*
 * Replace this comment with the standard EE312 file header!
 */

#include <assert.h>
#include <string.h>
#include "MemHeap.h"
#include "String.h"

/* use these two macros and the function functions if you want -- not required */
#define SIGNATURE (~0xdeadbeef)
#define STRING(s) (((String*)s)-1)

/* this simple function can be useful when you implement stage 4
 * you are not required to use the function, and you can implement stage 4 without it */
int32_t isOurs(char* s) {
	uint32_t* metadata = (uint32_t*)s;
    if (!isHeapVar(s)) { return 0; }
    if (*(metadata-3) == SIGNATURE) { return 1; }
    else { return 0; }
}

/* allocate a utstring on the heap, initialize the string correctly by copying
 * the characters from 'src' and return a pointer to the first character of actual string data
 * I don't like using structs. At all. So I'll be using simple pointer to allocate our strings 
 */
char* utstrdup(const char* src) {
	//Find size of string and allocate pace
	int size = 0;
	char* string; //Pointing to where our characters will live
	uint32_t* data; //Pointing to our metadata
	while (src[size] != 0) { size++; } //We find the size of source
	data = (uint32_t*) malloc(3 * sizeof(uint32_t) + size + 1); //Allocate space
	
	//Point to allocation and fill string
	string = (char*)(data + 3); 
	for (int i = 0; i <= size; i = i + 1) {
		string[i] = src[i];
	}
	//Fill in our metadata
	*data = SIGNATURE; 
	*(data + 1) = size;
	*(data + 2) = size;
	return string; //Return pointer to string
}

/* the parameter 'utstr' must be a utstring. Find the length of this string by accessing the meta-data
 * and return that length */
uint32_t utstrlen(const char* utstr) {
	//Get our metadata
	uint32_t length;
	uint32_t* metadata = (uint32_t*)utstr;

	//Crash if not UTString
	assert(*(metadata-3) == SIGNATURE);

	//Point to our length metadata and get it 
	metadata = metadata - 1; 
	length = *metadata;
	return length;
}

/* s must be a utstring. suffix can be an ordinary string (or a utstring)
 * append the characters from suffix to the string s. Do not allocate any additional storage, and
 * only append as many characters as will actually fit in s. Update the length meta-data for utstring s
 * and then return s */
char* utstrcat(char* s, const char* suffix) {
	//Get our metadata
	uint32_t* meta = (uint32_t*)s;
	uint32_t length_s = *(meta - 1);
	uint32_t capacity_s = *(meta - 2);
	uint32_t size_suffix = 0;
	uint32_t total_size = length_s;

	//Crash if not UTString
	assert(*(meta - 3) == SIGNATURE); 

	//Find the size of the suffix
	while (suffix[size_suffix] != 0) { size_suffix++; } //We find the size of our suffix

	//Add the suffix to our utstring, until we are finished or run out of space
	for (total_size; (total_size < capacity_s) && (total_size < (length_s+size_suffix)); total_size += 1) {
		s[total_size] = suffix[total_size-length_s];
	}
	s[total_size] = 0; //Terminate the string
	*(meta - 1) = total_size;
	return s;
}

/* 'dst' must be a utstring. 'src' can be an ordinary string (or a utstring)
 * overwrite the characters in dst with the characters from src. Do not overflow the capacity of dst
 * For example, if src has five characters and dst has capacity for 10, then copy all five characters
 * However, if src has ten characters and dst only has capacity for 8, then copy only the first 8
 * characters. Do not allocate any additional storage, do not change capacity. Update the length
 * meta-data for dst and then return dst */
char* utstrcpy(char* dst, const char* src) {
	//Get all of our metadata
	uint32_t* meta = (uint32_t*)dst;
	assert(*(meta - 3) == SIGNATURE); //Crash if not UTString
	uint32_t dst_cap = *(meta - 2);
	uint32_t length_src = 0;

	//Get the size of the source to copy 
	while (src[length_src] != 0) { length_src++; }

	//Copy our data to the destination
	int i = 0;
	for (i; (i < length_src) && (i<dst_cap); i = i + 1) { //Copy until we are either finished or out of space
		dst[i] = src[i];
	}
	dst[i] = 0; //Terminate the string 
	*(meta - 1) = i; //New size
	return dst;
}

/* self must be a utstring. deallocate the storage for this string
 * (i.e., locate the start of the chunk and call free to dispose of the chunk, note that the start of
 * the chunk will be 12 bytes before *self) */
void utstrfree(char* self) {
	uint32_t* meta = (uint32_t*)self; //Get our metadata pointer
	assert(*(meta - 3) == SIGNATURE); //Crash if not UTString
	//Deallocate whole utstring
	self = self - 12;
	free(self);
}

/* s must be a utstring.
 * ensure that s has capacity at least as large as 'new_capacity'
 * if s already has capacity equal to or larger than new_capacity, then return s
 * if s has less capacity than new_capacity, then allocate new storage with sufficient space to store
 * new_capacity characters (plus a terminating zero), copy the current characters from s into this
 * new storage. Update the meta-data to correctly describe new new utstring you've created, deallocate s
 * and then return a pointer to the first character in the newly allocated storage */
char* utstrrealloc(char* s, uint32_t new_capacity) {
	//We first get all of our relevant data
	uint32_t* metadata = (uint32_t*)s;
	assert(*(metadata - 3) == SIGNATURE); //Crash if not UTString
	uint32_t capacity = *(metadata - 2);
	uint32_t size = *(metadata -1);

	//Check if we need to find more capacity
	if (capacity >= new_capacity) {
		return s;
	}

	//Make a new allocation
	uint32_t* new_meta = (uint32_t*)malloc(3 * sizeof(uint32_t) + new_capacity + 1);
	char* new_alloc = (char*)(new_meta + 3);
	*new_meta = SIGNATURE; //Fill in our metadata
	*(new_meta + 1) = new_capacity;
	*(new_meta + 2) = size;
	
	//Copy our data
	utstrcpy(new_alloc, s);
	
	//We free the previous allocation
	utstrfree(s);

	return new_alloc;	
}



