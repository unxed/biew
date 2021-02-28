/**
 * @namespace   libbeye
 * @file        libbeye/kbd_code.h
 * @brief       This file includes OS depended keyboard codes.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Nickols_K
 * @since       1999
 * @note        Development, fixes and improvements
**/
#include "libbeye/libbeye.h"
#if __WORDSIZE == 16
 #if defined( __MSDOS__ ) || defined ( __OS2__ ) || defined ( __WINDOWS__ )
   #include "libbeye/sysdep/ia16/dos/kbd_code.h"
 #else
   #error Unknown operationg system for IA-16 architecture
 #endif
#else
  #if defined(__WIN32__) && defined(_MSC_VER)
    #define __OS_KEYBOARD <libbeye/sysdep/ia32/win32/kbd_code.h>
  #else
    #define __OS_KEYBOARD <libbeye/sysdep/__MACHINE__/__OS__/kbd_code.h>
  #endif
  #include __OS_KEYBOARD
#endif

