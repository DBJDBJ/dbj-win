// https://godbolt.org/z/6ETE5Yqx4
#include <stdio.h>
#include <stdlib.h>

// -------------------------------------------------------
// (c) dbj at dbj dot org
// https://dbj.org/licesense_dbj
//
// "universal" variadic macro for when
// all the arguments are of the same type
//
// cb_ is a callback
// void callbacktype (size_t size_, T arr[static size_])
//
#define vmacro(cb_,T,...) \
cb_( \
sizeof((T[]){__VA_ARGS__}) / sizeof(T)  ,\
(T[]){__VA_ARGS__} \
)

// -------------------------------------------------------
#ifdef DBJ_VAR_MACRO_TESTING

static inline void funcwords(size_t size_ , char * p[static size_])
{
    for (size_t k = 0; k < size_; ++k) {
        printf("%s\n", p[k]);
    }
}

static inline void funcint(size_t size_ , int p[size_])
{
    for (size_t k = 0; k < size_; ++k) {
        printf("%d\n", p[k]);
    }
}

static inline void funcfree(size_t size_ , void * p[size_])
{
    for (size_t k = 0; k < size_; ++k) {
        printf("Going to free: %p\n", p[k]);
        free(p[k]);
    }
}

// -------------------------------------------------------
int main(void)
{
    vmacro(funcwords, char *, "abc", "def", "ghi");
    vmacro(funcint,int,1,2,3);

    void * p1 =  malloc(1);
    void * p2 =  malloc(1);
    void * p3 =  malloc(1);

    vmacro(funcfree,void *,p1,p2,p3);
    return 0;
}
#endif // DBJ_VAR_MACRO_TESTING
