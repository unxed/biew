/**
 * @namespace   libbeye
 * @file        libbeye/sysdep/ia32/qnx/nls.c
 * @brief       This file contains implementation of cyrillic codepages support for 866 CP.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Andrew Golovnia
 * @since       2003
 * @note        Development, fixes and improvements
**/
#ifdef __QNX4__

#include "libbeye/libbeye.h"

void __FASTCALL__ __nls_OemToOsdep(unsigned char *buff,unsigned int len)
{
}

void __FASTCALL__ __nls_OemToFs(unsigned char *buff,unsigned len)
{
}

void __FASTCALL__ __nls_CmdlineToOem(unsigned char *buff,unsigned len)
{
}

#else
#include "libbeye/sysdep/generic/unix/nls.c"
#endif
