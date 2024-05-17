#include "projects/automated_warehouse/aw_thread.h"

//
// You need to read carefully thread/synch.h and thread/synch.c
// 
// In the code, a fucntion named "sema_down" implements blocking thread and 
// makes list of blocking thread
// 
// And a function named "sema_up" implements unblocing thread using blocking list
//
// You must implement blocking list using "blocking_threads" in this code.
// Then you can also implement unblocking thread.
//


struct list blocked_threads;

// Initialize the list of blocked threads
void init_blocked_threads() {
    list_init(&blocked_threads);
}

/**
 * A function unblocking all blocked threads in "blocked_threads" 
 * It must be called by robot threads
 */
void block_thread(){// You must implement this

    enum intr_level old_level;// Disable interrupts to ensure atomicity
    old_level = intr_disable ();
    list_push_back(&blocked_threads, &thread_current()->elem);// Add the current thread to the list of blocked threads
    thread_block ();// Block the current thread
    intr_set_level (old_level);// Restore interrupts
    
}

/**
 * A function unblocking all blocked threads in "blocked_threads" 
 * It must be called by central control thread
 */
void unblock_threads(){// you must implement this

    enum intr_level old_level;
    old_level = intr_disable();

    // Iterate through the list of blocked threads and unblock each thread
    while (!list_empty(&blocked_threads)) {
        struct list_elem* e = list_pop_front(&blocked_threads);
        struct thread* t = list_entry(e, struct thread, elem);
        thread_unblock(t);
    }
    intr_set_level(old_level);
}
