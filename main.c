#include "./main.h"
#include <stdlib.h>
#include <stdio.h>

void assert(int condition, const char* message) {
	if (!condition) {
		printf("%s\n", message);
		exit(1);
	}
}

/**
 * We need to get some "preliminaries" out of the way first. We need stuff to
 * actually garbage-collect! Normally, this would be done through super-fancy
 * macro voodoo magic, or by implementing an entire language, but for
 * simplicity's sake we'll just implement two different types of objects - an
 * int and a pair.
 */

/** Our two types of collectable objects. */
// (see main.h)

/**
 * Define what an object is; in this case as a tagged union.
 * (either an OBJ_INT or an OBJ_PAIR)
 */
// (see main.h)

/**
 * Now, we implement a "virtual machine" of sorts. Really its only role is to
 * have a stack to store the variables that are not in scope. Therefore, it's
 * good for storing local variables and temporary variables needed in the
 * middle of an expression.
 */

/** A simple virtual machine for managing out-of-scope variables. */
// (see main.h)

/**
 * Creates and initializes a new VM.
 * @return A pointer to the new VM
 */
VM* newVM() {
	VM* vm = malloc(sizeof(VM));
	vm->stackSize = 0;
	vm->firstObject = NULL;
	vm->numObjects = 0;
	vm->maxObjects = INITIAL_GC_THRESHOLD;
	return vm;
}

/**
 * Push an Object to a VM's stack.
 * @param {VM*} vm Pointer to the VM
 * @param {Object*} obj Pointer to the Object to be pushed
 */
void push(VM* vm, Object* obj) {
	assert(vm->stackSize < STACK_MAX, "Stack overflow!");
	vm->stack[vm->stackSize++] = obj;
}

/**
 * Pop an Object from the VM's stack.
 * @param {VM*} vm Pointer to the VM
 * @return Pointer to the popped Object
 */
Object* pop(VM* vm) {
	assert(vm->stackSize > 0, "Stack underflow!");
	return vm->stack[--vm->stackSize];
}

/**
 * Instantiate a new Object.
 * @param {VM*} vm Pointer to a vm
 * @param {ObjectType} type The type of the object (either an OBJ_INT or
 *                          an OBJ_PAIR)
 * @return A pointer to the instantiated Object.
 */
Object* newObject(VM* vm, ObjectType type) {
	// Run GC if we're reaching max number of Objects.
	if (vm->numObjects == vm->maxObjects) gc(vm);

	Object* object = malloc(sizeof(Object));
	object->type = type;
	object->marked = 0;

	// Insert into the list of allocated Objects.
	object->next = vm->firstObject;
	vm->firstObject = object;
	
	vm->numObjects++;
	return object;
}

/**
 * Push a new int Object to the VM's stack.
 * @param {VM*} vm Pointer to a vm
 * @param {int} val The value of the int Object
 */
void pushInt(VM* vm, int val) {
	Object* obj = newObject(vm, OBJ_INT);
	obj->value = val;
	push(vm, obj);
}

/**
 * Instantiate and push a new pair Object to the VM's stack.
 * The pair's tail will be the top object on the stack, and it's head will be
 * the object on the stack just underneath that.
 * @param {VM*} vm A pointer to a vm
 * @return The newly instantiated Object.
 */
Object* pushPair(VM* vm) {
	Object* obj = newObject(vm, OBJ_PAIR);
	obj->tail = pop(vm);
	obj->head = pop(vm);

	push(vm, obj);
	return obj;
}

/**
 * Mark an Object as reachable.
 * @param {Object*} object Pointer to the Object
 */
void mark(Object* object) {
	// Check to see if the object is already marked. This allows us to avoid
	// infinite recursion loops.
	if (object->marked) return;

	object->marked = 1;

	if (object->type == OBJ_PAIR) {
		mark(object->head);
		mark(object->tail);
	}
}

/**
 * Mark all reachable Object's.
 * @param {VM*} vm Pointer to the vm
 */
void markAll(VM* vm) {
	for (int i = 0; i < vm->stackSize; i++) {
		mark(vm->stack[i]);
	}
}

/**
 * Sweep through the VM's stack and delete unmarked Objects.
 * @param {VM*} vm Pointer to the vm.
 */
void sweep(VM* vm) {
	Object** object = &vm->firstObject;

	while (*object) {
		if (!(*object)->marked) {
			// This object wasn't reached, so remove it from the
			// list and free it.
			Object* unreached = *object;

			*object = unreached->next;
			free(unreached);

			vm->numObjects--;
		}
		else {
			// This Object was reached, so unmark it (for the next GC) and move
			// on to the next.
			(*object)->marked = 0;
			object = &(*object)->next;
		}
	}
}

/**
 * Run the garbage collector.
 * @param {VM*} vm Pointer to the VM
 */
void gc(VM* vm) {
	int numObjects = vm->numObjects;

	markAll(vm);
	sweep(vm);

	vm->maxObjects = vm->numObjects * 2;
}

/**
 * Free the VM from memory.
 * @param {VM*} vm Pointer to the VM.
 */
void freeVM(VM* vm) {
	vm->stackSize = 0;
	gc(vm);
	free(vm);
}

int main(int argc, const char* argv[]) {
	return 0;
}
