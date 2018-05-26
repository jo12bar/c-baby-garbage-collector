/**
 * We need to get some "preliminaries" out of the way first. We need stuff to
 * actually garbage-collect! Normally, this would be done through super-fancy
 * macro voodoo magic, or by implementing an entire language, but for
 * simplicity's sake we'll just implement two different types of objects - an
 * int and a pair.
 */

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
	ObjectType type;

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

/**
 * Now, we implement a "virtual machine" of sorts. Really its only role is to
 * have a stack to store the variables that are not in scope. Therefore, it's
 * good for storing local variables and temporary variables needed in the
 * middle of an expression.
 */

/** Maximum size of the VM stack. */
#define STACK_MAX 256

/** A simple virtual machine for managing out-of-scope variables. */
typedef struct {
	Object* stack[STACK_MAX];
	int stackSize;
} VM;

/**
 * Creates and initializes a new VM.
 * @return A pointer to the new VM
 */
VM* newVM() {
	VM* vm = malloc(sizeof(VM));
	vm->stackSize = 0;
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

