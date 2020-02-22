#include <stdio.h>
#include "../mm.c"

void test_PACK() {
    FILE* fp = fopen("./test_file/test_PACK.txt", "r");
    int size, alloc, ans;
    while(fscanf(fp, "%d %d %d", &size, &alloc, &ans) == 3) {
        if(PACK(size, alloc) == ans)
            printf("SUCCESS: PACK(%d, %d) == %d \n", size, alloc, ans);
        else 
            printf("FAIL: PACK(%d, %d) == %d not %d\n", size, alloc, ans, PACK(size, alloc));
    }
}

void test_GET_SIZE_AND_ALLOC() {
    FILE* fp = fopen("./test_file/test_GET_SIZE_AND_ALLOC.txt", "r");
    int size, alloc;
    void* head = malloc(sizeof(int));
    while(fscanf(fp, "%d %d", &size, &alloc) == 2) {
        *(int*) head = PACK(size, alloc);
        if(GET_SIZE(head) == size)
            printf("SUCCESS: GET_SIZE(%d) == %d \n", size, size);
        else 
            printf("FAIL: GET_SIZE(%d) == %d not %d\n", size, size, GET_SIZE(head));
        if(GET_ALLOC(head) == alloc)
            printf("SUCCESS: GET_ALLOC(%d) == %d \n", alloc, alloc);
        else 
            printf("FAIL: GET_ALLOC(%d) == %d not %d\n", alloc, alloc, GET_ALLOC(head));
    }
    free(head);
}

void test_HEAD_TRAIL_CONT() {
    FILE* fp = fopen("./test_file/test_HEAD_TRAIL_CONT.txt", "r");
    int size;
    void* head;
    void* trail;
    void* cont;
    int h, t, c;
    while(fscanf(fp, "%d", &size) == 1) {
        head = malloc(8*size + 8);
        *(int*) head = PACK(size, 0);
        h = (int) head;
        t = h + 4 + 8*size;
        c = h + 4;
        if(((int) HEAD2TRAIL(head)) == t)
            printf("SUCCESS: HEAD2TRAIL(%p) == %p\n", head, t);
        else
            printf("FAIL: HEAD2TRAIL(%p) == %p not %p\n", head, t, HEAD2TRAIL(head));
        
        trail = (int*) t;
        *(int*) trail = PACK(size, 0);
        if(((int) TRAIL2HEAD(trail)) == h)
            printf("SUCCESS: TRAIL2HEAD(%p) == %p\n", trail, h);
        else
            printf("FAIL: TRAIL2HEAD(%p) == %p not %p\n", trail, h, TRAIL2HEAD(trail));
        
        if(((int) HEAD2CONT(head)) == c)
            printf("SUCCESS: HEAD2CONT(%p) == %p\n", head, c);
        else
            printf("FAIL: HEAD2CONT(%p) == %p not %p\n", head, c, HEAD2CONT(head));
        
        cont = (void*) c;
        if(((int) CONT2HEAD(cont)) == h)
            printf("SUCCESS: CONT2HEAD(%p) == %p\n", c, head);
        else
            printf("FAIL: CONT2HEAD(%p) == %p not %p\n", c, head, CONT2HEAD(cont));
        

        free(head);
    }
}

void test_PRE_NEXT_HEAD_TRAIL() {
    FILE* fp = fopen("./test_file/test_PRE_NEXT_HEAD_TRAIL.txt", "r");
    int size1;
    int size2;
    void* head1; 
    void* head2; 
    void* trail1; 
    void* trail2;
    while(fscanf(fp, "%d %d", &size1, &size2) == 2) {
        //PROB:block1 and block2 are not continuous
        head1 = malloc(size1*8+2);
        trail1 = HEAD2TRAIL(head1);
        head2 = (void*) ((int) trail1 + 4);
        trail2 = HEAD2TRAIL(head2);
        head1 = PACK(size1, 0);
        trail1 = PACK(size1, 0);
        head2 = PACK(size2, 0);
        trail2 = PACK(size2, 0);
        if(PRE_HEAD(head2) == head1)
            printf("SUCCESS: PRE_HEAD(%p) == %p", head2, head1);
        else
            printf("FAIL: PRE_HEAD(%p) == %p not %p", head2, head1, PRE_HEAD(head2));
    }
}

int main() {
    //test_PACK();
    //test_GET_SIZE_AND_ALLOC();
    //test_HEAD_TRAIL_CONT();
    test_PRE_NEXT_HEAD_TRAIL();
    return 0;
}