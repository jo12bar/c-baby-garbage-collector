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
