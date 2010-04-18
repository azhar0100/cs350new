#ifndef FILE_int_compare_h_INCLUDED
#define FILE_int_compare_h_INCLUDED


/* Integer comparison function for use with the stdlib's Qsort. */
inline int int_compare(const void *p1, const void *p2) {
        return ( *(int*)p1 - *(int*)p2 );
}


#endif /*FILE_int_compare_h_INCLUDED*/
