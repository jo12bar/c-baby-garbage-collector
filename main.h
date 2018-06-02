#ifndef __MAIN_H
#define __MAIN_H

/** Maximum size of the VM stack. */
#define STACK_MAX 256

/** How many Objects to allocate before running GC. */
#define INITIAL_GC_THRESHOLD 8

void assert(int condition, const char* message);

/*********|
| OBJECTS |
|*********/

/** Our two types of collectable objects. */
typedef enum {
	OBJ_INT,
	OBJ_PAIR
} ObjectType;

/**
 * Define what an object is; in this case as a tagged union.
 * (either an OBJ_INT or an OBJ_PAIR)
 */
typedef struct sObject {
	unsigned char marked;
	ObjectType type;

	/** The next Object in the list of all Objects. */
	struct sObject* next;

	union {
		/** OBJ_INT */
		int value;

		/** OBJ_PAIR */
		struct {
			struct sObject* head;
			struct sObject* tail;
		};
	};
} Object;

/****|
| VM |
|****/

/** A simple virtual machine for managing out-of-scope variables. */
typedef struct {
	Object* stack[STACK_MAX];
	int stackSize;

	/** The first Object in the list of all Objects. */
	Object* firstObject;

	/** Total number of currently allocated Objects. */
	int numObjects;

	/** Number of objects required to trigger a garbage collection. */
	int maxObjects;
} VM;

VM* newVM();
void freeVM(VM* vm);

void push(VM* vm, Object* obj);
Object* pop(VM* vm);

Object* newObject(VM* vm, ObjectType type);
void pushInt(VM* vm, int val);
Object* pushPair(VM* vm);

/********************|
| GARBAGE COLLECTION |
|********************/

void mark(Object* object);
void markAll(VM* vm);
void sweep(VM* vm);
void gc(VM* vm);

#endif
