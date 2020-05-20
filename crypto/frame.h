
#ifndef __OPTIMIZE__
 #ifndef frame_address
  #define frame_address(level_integer) __builtin_frame_address(level_integer)
  #define frame_address0               __builtin_frame_address(0)
  #define frame_address1               __builtin_frame_address(1)
  #define frame_address2               __builtin_frame_address(2)
  #define frame_address3               __builtin_frame_address(3)
  /*...*/
 #endif
 #ifndef return_address
  #define return_address(level_integer) __builtin_return_address(level_integer)
  #define return_address0 __builtin_return_address(0)
  #define return_address1 __builtin_return_address(1)
  #define return_address2 __builtin_return_address(2)
  #define return_address3 __builtin_return_address(3)
  /*...*/
 #endif
#else //__OPTIMIZE__
 #ifndef frame_address
  #define frame_address(level_integer) (NULL)
  #define frame_address0               (NULL)
  #define frame_address1               (NULL)
  #define frame_address2               (NULL)
  #define frame_address3               (NULL)
  /*...*/
 #endif
 #ifndef return_address
  #define return_address(level_integer)   (NULL)
  #define return_address0                 (NULL)
  #define return_address1                 (NULL)
  #define return_address2                 (NULL)
  #define return_address3                 (NULL)
  /*...*/
 #endif
#endif //__OPTIMIZE__

