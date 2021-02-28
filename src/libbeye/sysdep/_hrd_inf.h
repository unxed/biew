/**
 * @namespace   libbeye
 * @file        libbeye/sysdep/_hrd_inf.h
 * @brief       This file contains all hardware depended part of BEYE project.
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
#ifndef __HRD_INF_H
#define __HRD_INF_H 1

#ifdef __cplusplus
extern "C" {
#endif

                   /** Fills buffer with information about CPU in free form.
                     * @return                none
                     * @param buff            buffer to be filled
                     * @param cbBuff          size of buffer
                     * @param percents_callback pointer to the function that will be used to indicate execution progress
                    **/
extern void          __FillCPUInfo(char *buff,unsigned cbBuff,void (*percents_callback)(int));


#ifdef __cplusplus
}
#endif

#endif
