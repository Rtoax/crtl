
/**
 *  To use , you must compile like "gcc XXX.c -O2" (优化选项: -O,-O0,-O1,-O2,-O3)
 */
#ifndef object_size
 #define object_size(p_obj, type)    __builtin_object_size(p_obj, type)
 #define object_size0(p_obj)         __builtin_object_size(p_obj, 0)
 #define object_size1(p_obj)         __builtin_object_size(p_obj, 1)
 #define object_size2(p_obj)         __builtin_object_size(p_obj, 2)
 #define object_size3(p_obj)         __builtin_object_size(p_obj, 3)
#endif

