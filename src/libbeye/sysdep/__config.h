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

#if defined(__i386__)
#include "libbeye/sysdep/ia32/__config.h"
#elif defined(__x86_64__)
#include "libbeye/sysdep/x86_64/__config.h"
#else
#include "libbeye/sysdep/generic/__config.h"
#endif

#endif
