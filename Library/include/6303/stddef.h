#ifndef __STDDEF_H

typedef int ptrdiff_t;
typedef char wchar_t;
typedef unsigned int size_t;

/* offsetof macro */
#define offsetof(type, member)  (size_t) (&((type*) 0)->member)

#endif
