/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

// #include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
// team_t team = {
//     /* Team name */
//     "ateam",
//     /* First member's full name */
//     "Harry Bovik",
//     /* First member's email address */
//     "bovik@cs.cmu.edu",
//     /* Second member's full name (leave blank if none) */
//     "",
//     /* Second member's email address (leave blank if none) */
//     ""
// };

#define INIT_SIZE 256
#define DWORD 8
#define WORD 4

#define PACK(size, alloc) ((size << 3) | alloc)
#define GET_SIZE(head) ((*(int*) head) >> 3)
#define GET_ALLOC(head) ((*(int*) head) & 1)

#define HEAD2TRAIL(head) ((void*) (((int*) head) + GET_SIZE(head)*2 + 1))
#define TRAIL2HEAD(trail) ((void*) (((int*) trail) - GET_SIZE(trail)*2 - 1))
#define HEAD2CONT(head) ((void*) (((int*) head) + 1))
#define CONT2HEAD(cont) ((void*) (((int*) cont) - 1))

#define PRE_HEAD(head) (TRAIL2HEAD(((void*) (((int*) head) - 1))))
#define PRE_TRAIL(trail) ((void*) ((int*) (TRAIL2HEAD(trail)) - 2))
#define NEXT_HEAD(head) ((void*) (((int*) head) + GET_SIZE(head)*2 + 4))
#define NEXT_TRAIL(trail) (HEAD2TRAIL(NEXT_HEAD(TRAIL2HEAD(trail))))



#define ALIGN(size) ((size >> 3) + 1)


void *op_pointer;
void *ed_pointer;

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{   
    op_pointer = mem_sbrk((INIT_SIZE+2)*DWORD);
    if((*(int*) op_pointer) == -1)
        return -1;

    void* head = op_pointer;
    void* trail = HEAD2TRAIL(head);
    *(int*) head = PACK(0, 1);
    *(int*) trail = PACK(0, 1);

    head = NEXT_HEAD(head);
    trail = HEAD2TRAIL(head);
    *(int*) head = PACK(INIT_SIZE, 0);
    *(int*) trail = PACK(INIT_SIZE, 0);

    head = NEXT_HEAD(head);
    trail = HEAD2TRAIL(head);
    *(int*) head = PACK(0, 1);
    *(int*) trail = PACK(0, 1);

    ed_pointer = trail;

    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t dsize = ALIGN(size);
    void* head = op_pointer;
    void* trail = HEAD2TRAIL(head);
    while(GET_SIZE(head) < dsize || GET_ALLOC(head) == 1) {
        head = NEXT_HEAD(head);
        trail = HEAD2TRAIL(head);
        if(trail == ed_pointer)
            break;
    }
    // can divide the block
    if(HEAD2TRAIL(head) != ed_pointer) {
        size_t rsize = GET_SIZE(head) - dsize - 2;
        if(rsize < 4) {
            *(int*) head = PACK(GET_SIZE(head), 1);
            *(int*) trail = PACK(GET_SIZE(head), 1);
        }
        else {
            trail = HEAD2TRAIL(head);
            *(int*) head = PACK(dsize, 1);
            *(int*) trail = PACK(dsize, 1);
            head = NEXT_HEAD(head);
            trail = HEAD2TRAIL(head);
            *(int*) head = PACK(rsize, 0);
            *(int*) trail = PACK(rsize, 0);
        }
        return HEAD2CONT(head);
    }
    // cant divide the block, do brk
    else {
        void* p = mem_sbrk((dsize+2)*DWORD);
        if(*(int*) p == -1)
            return NULL;
        *(int*) head = PACK(dsize, 0);
        trail = HEAD2TRAIL(head);
        *(int*) trail = PACK(dsize, 0);
        p = head;

        head = NEXT_HEAD(head);
        *(int*) head = PACK(0, 1);
        trail = HEAD2TRAIL(head);
        *(int*) trail = PACK(0, 1);
        return HEAD2CONT(p);
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    ptr = CONT2HEAD(ptr);
    void* head = PRE_HEAD(ptr);
    while(head != op_pointer && GET_ALLOC(head) != 1)
        head = PRE_HEAD(head);
    head = NEXT_HEAD(head);

    void* trail = HEAD2TRAIL(ptr);
    
    trail = NEXT_TRAIL(trail);
    while(trail != ed_pointer && GET_ALLOC(trail) != 1)
        trail = NEXT_TRAIL(trail);
    trail = PRE_TRAIL(trail);

    size_t size = ((int*) head - (int*) trail - 1) / 2;
    *(int*) head = PACK(size, 0);
    *(int*) trail = PACK(size, 0);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    // void *oldptr = ptr;
    // void *newptr;
    // size_t copySize;
    
    // newptr = mm_malloc(size);
    // if (newptr == NULL)
    //   return NULL;
    // copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    // if (size < copySize)
    //   copySize = size;
    // memcpy(newptr, oldptr, copySize);
    // mm_free(oldptr);
    // return newptr;
}














