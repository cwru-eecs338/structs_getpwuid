#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

void the_basics(void);
void structs_and_memory(void);
void getpwuid_example(void);

int main() {

    the_basics();

    structs_and_memory();

    getpwuid_example();

    return EXIT_SUCCESS; // Just a constant in stdlib
}

struct person {
    char* name;
    int age;
};

void doesnt_work(struct person);
void this_works(struct person*);
void this_also_works(struct person*);

void the_basics() {
    
    // Initialize a struct:
    struct person alice = { "Alice", 25 };
    struct person bob = { "Bob", 24 };

    // Access 'members'
    char *format = "Person %d: %s (%d years old)\n";
    printf(format, 1, alice.name, alice.age);
    printf(format, 2, bob.name, bob.age);
    puts("");

    // It's Bob's birthday!
    // Are structs passed by reference or value?
    doesnt_work(bob);
    printf("Bob's age: %d\n", bob.age);

    // Explicitly pass by reference (pointer)
    struct person* pointer_to_bob = &bob;
    this_works(pointer_to_bob);
    printf("Bob's age: %d\n", bob.age);

    // Same functionality, but using
    // nicer syntax:
    this_also_works(&alice);
    printf("Alice's age: %d\n", alice.age);
}

/*
 * Three attempts at a function
 * to increment the age of a person.
 */

void doesnt_work(struct person p) {
    // Try to increment age:
    p.age++;
}

void this_works(struct person* p) {
    // Dereference pointer, then
    // access member:
    (*p).age++;
}

void this_also_works(struct person* p) {
    // A nicer syntax for accessing
    // member from a pointer:
    p->age++;
}

void structs_and_memory() {
    // Allocates on stack
    struct person alice = {"Alice", 26};

    // Allocates on heap
    size_t person_size = sizeof(struct person);
    printf("\nAllocating %d bytes...\n", (int) person_size);
    struct person *heap_alice = malloc(person_size);

    // Direct assignment of structs
    // copies the memory values
    // (it's important that we
    //  dereferenced 'heap_alice')
    *heap_alice = alice;
    printf("Person on heap: %s (age %d)\n",
        heap_alice->name, heap_alice->age);

    // Keep in mind that we've only copied
    // a pointer to the heap. The pointer
    // points to a string on the stack.
    // (i.e. heap_alice->name points to
    //  the stack)

    // Oops! Leaked memory by not calling
    // "free()" on "heap_alice"
    // Try using 'valgrind' to debug
    // and fix the leak (use the
    // --leak-check=full flag)
}

/*
 * Example using getpwuid()
 */

void getpwuid_example() {
    //
    // From the manual page for getpwuid:
    //
    //   The passwd structure is defined in <pwd.h> as follows:
    //
    //       struct passwd {
    //           char   *pw_name;       /* username */
    //           char   *pw_passwd;     /* user password */
    //           uid_t   pw_uid;        /* user ID */
    //           gid_t   pw_gid;        /* group ID */
    //           char   *pw_gecos;      /* real name */
    //           char   *pw_dir;        /* home directory */
    //           char   *pw_shell;      /* shell program */
    //       };
    //

    // Get 'effective' user id
    // (as opposed to 'real' user id)
    uid_t user_id = geteuid();

    // Get pointer to passwd struct
    struct passwd *pointer_to_passwd = getpwuid(user_id);

    // Use the -> syntax to access member of struct
    // from a pointer to the struct
    char *username = pointer_to_passwd->pw_name;

    printf("\nUsername: %s\n", username);
}
