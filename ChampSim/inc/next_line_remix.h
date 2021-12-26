/**
 * next_line_remix.h -- Header file containing the required classes, utility functions
 *                      and macros required by Golu's prefetcher
 *
 * ChampSim Lab - Part 2
 * Computer Architecture Winter School (2021)
 *
 * Solution Author:
 *     Debasish Das (debasish@cse.iitb.ac.in)
 *     M.Tech. CSE (2020-22)
 *     IIT Bombay
 */

#ifndef __NEXT_LINE_REMIX_H
#define __NEXT_LINE_REMIX_H

#include <deque>
#include "champsim.h"
using namespace std;


/* Convenient macros to extract out page ID and block ID from a given 64-bit address */
#define EXTRACT_PAGE_ID(addr)   ((addr) >> LOG2_PAGE_SIZE)              /* Extract the page ID */
#define EXTRACT_BLOCK_ID(addr)  (((addr) >> LOG2_BLOCK_SIZE) & 0x3f)    /* Extract the block ID within the page */

/* Minimum and maximum value of the block IDs */
#define BLOCK_ID_MIN    0
#define BLOCK_ID_MAX    ((PAGE_SIZE / BLOCK_SIZE) - 1)

/* --------------------------------------------------------------- */
/* The length of the buffer, i.e. 'K' as mentioned in the question */
/* Tweak this value accordingly */
#define BUFFER_LENGTH   5
/* --------------------------------------------------------------- */


/* Buffer class for storing the last K accesses */
class RequestHistoryBuffer {
    deque<int32_t> history;

public:

    RequestHistoryBuffer() {}   /* Empty constructor */


    /* size -- Returns the current size of the buffer */
    int32_t size() {
        return (int32_t)history.size();
    }

    /* append -- Method that takes a cache block # as input and pushes it to the back of the queue */
    void append(uint32_t blk_id)
    {
        /* If the buffer is already full, remove the oldest entry */
        /* Using while loop to be fully sure that the buffer size doesn't exceed the maximum size */
        while(history.size() >= BUFFER_LENGTH)
            history.pop_front();

        /* NOTE: Block ID's range is in [0, 63] */
        history.push_back((int32_t)blk_id);
    }


    /* reset -- Method that resets the corresponding buffer by removing all the elements */
    void reset()
    {
        /* As long as the buffer is not empty, keep popping (doesn't matter if popped from back or front) */
        while(!history.empty())
            history.pop_front();
    }


    /* is_full -- Returns TRUE if the buffer is full. FALSE otherwise */
    bool is_full() {
        return history.size() == BUFFER_LENGTH;
    }


    /* has_forward_movement -- Method that returns TRUE if atleast (K-1)/2 elements in the buffer
     *                         has a forward movement. Returns FALSE otherwise.
     *
     * NOTE: Ensure that this method is called only when the buffer is completely full to ensure
     *       correct results
     */
    bool has_forward_movement()
    {
        int n_forward = 0;
        for(int t=1; t<BUFFER_LENGTH; t++)
        {
            /* If the difference between block at time 't' and 't-1' is positive */
            /* then it is a forward movement */
            if(history[t] - history[t-1] >= 0)
                n_forward++;
        }

        /* Yep, at least half of the entries follow forward movement */
        if(n_forward >= (BUFFER_LENGTH - 1) >> 1)
            return true;

        /* Nope, there is a backward movement */
        return false;
    }
};


/* Utility function to prepare the prefethching address */
uint64_t prepare_prefetch_address(uint64_t page_id, uint32_t block_id)
{
    return (page_id << LOG2_PAGE_SIZE) + (block_id << LOG2_BLOCK_SIZE);
}


#endif /* __NEXT_LINE_REMIX_H */
