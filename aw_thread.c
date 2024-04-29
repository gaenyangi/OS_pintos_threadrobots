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
    struct list_elem* e;
    for (e = list_begin(&blocked_threads); e != list_end(&blocked_threads); e = list_next(e)) {
        thread_unblock(list_entry(list_pop_front(&blocked_threads), struct thread, elem));
    }
    intr_set_level(old_level);
}