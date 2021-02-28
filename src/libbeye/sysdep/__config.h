/**
 * @namespace   libbeye
 * @file        libbeye/sysdep/_sys_dep.h
 * @brief       This file contains configuration part of BEYE project.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Nickols_K
 * @since       2000
 * @note        Development, fixes and improvements
**/
#ifndef __CONFIG_H
#define __CONFIG_H 1

#ifdef __TSC__
   #include "libbeye/sysdep/ia16/__config.h"
#else
  #if defined(__WIN32__) && defined(_MSC_VER)
    #include "libbeye/sysdep/ia32/__config.h"
  #else
    #define __CONFIG <libbeye/sysdep/__MACHINE__/__config.h>
    #include __CONFIG
  #endif
#endif

#endif
