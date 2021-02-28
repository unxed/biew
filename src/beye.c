/**
 * @namespace   beye
 * @file        beye.c
 * @brief       This file contains entry point of BEYE project.
 * @version     -
 * @remark      this source file is part of Binary EYE project (BEYE).
 *              The Binary EYE (BEYE) is copyright (C) 1995 Nickols_K.
 *              All rights reserved. This software is redistributable under the
 *              licence given in the file "Licence.en" ("Licence.ru" in russian
 *              translation) distributed in the BEYE archive.
 * @note        Requires POSIX compatible development system
 *
 * @author      Nickols_K
 * @since       1995
 * @note        Development, fixes and improvements
 * @author      Kostya Nosov <k-nosov@yandex.ru>
 * @date        27.11.2000
 * @note        Changing technology recognition of new-exe files
**/
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <limits.h>
#include <sys/stat.h>
#ifdef HAVE_SYS_RESOURCE
#include <sys/resource.h>
#endif
#include <stdlib.h>
#include <errno.h>

#include "bconsole.h"
#include "colorset.h"
#include "bmfile.h"
#include "bin_util.h"
#include "codeguid.h"
#include "editor.h"
#include "tstrings.h"
#include "reg_form.h"
#include "beyeutil.h"
#include "search.h"
#include "setup.h"
#include "libbeye/file_ini.h"
#include "libbeye/kbd_code.h"
#include "libbeye/libbeye.h"
#include "libbeye/pmalloc.h"


unsigned ArgCount;
char **  ArgVector;
unsigned ListFileCount;
static char **ListFile;
static char *LastOpenFileName;
__filesize_t LastOffset;
static tBool UseIniFile=True;
char beye_help_name[FILENAME_MAX+1] = "";
char beye_skin_name[FILENAME_MAX+1] = "";
char beye_syntax_name[FILENAME_MAX+1] = "";
char beye_codepage[256] = "CP866";
extern char last_skin_error[];
char beye_scheme_name[256] = "Built-in";
static char beye_ini_ver[32];
unsigned long beye_vioIniFlags = 0L;
unsigned long beye_twinIniFlags = 0L;
unsigned long beye_kbdFlags = 0L;
tBool iniSettingsAnywhere = False;
tBool fioUseMMF = False;
tBool iniPreserveTime = False;
tBool iniUseExtProgs = False;
__filesize_t headshift = 0L;
char *ini_name;

TWindow * MainWnd = 0,*HelpWnd = 0,*TitleWnd = 0,*ErrorWnd = 0;

#define SHORT_PATH_LEN __TVIO_MAXSCREENWIDTH-54

char shortname[SHORT_PATH_LEN + 1];

extern REGISTRY_BIN binTable;
extern REGISTRY_BIN rmTable;
extern REGISTRY_BIN movTable;
extern REGISTRY_BIN mp3Table;
extern REGISTRY_BIN mpegTable;
extern REGISTRY_BIN jpegTable;
extern REGISTRY_BIN wavTable;
extern REGISTRY_BIN aviTable;
extern REGISTRY_BIN asfTable;
extern REGISTRY_BIN bmpTable;
extern REGISTRY_BIN neTable;
extern REGISTRY_BIN peTable;
extern REGISTRY_BIN leTable;
extern REGISTRY_BIN lxTable;
extern REGISTRY_BIN nlm386Table;
extern REGISTRY_BIN elf386Table;
extern REGISTRY_BIN jvmTable;
extern REGISTRY_BIN coff386Table;
extern REGISTRY_BIN archTable;
extern REGISTRY_BIN aoutTable;
extern REGISTRY_BIN OldPharLapTable;
extern REGISTRY_BIN PharLapTable;
extern REGISTRY_BIN rdoffTable;
extern REGISTRY_BIN rdoff2Table;
extern REGISTRY_BIN sisTable;
extern REGISTRY_BIN sisxTable;
extern REGISTRY_BIN lmfTable;
extern REGISTRY_BIN mzTable;
extern REGISTRY_BIN dossysTable;

static REGISTRY_BIN *mainBinTable[] =
{
  &neTable,
  &peTable,
  &leTable,
  &lxTable,
  &nlm386Table,
  &elf386Table,
  &jvmTable,
  &coff386Table,
  &archTable,
  &aoutTable,
  &OldPharLapTable,
  &PharLapTable,
  &rdoffTable,
  &rdoff2Table,
  &lmfTable,
  &mzTable,
  &dossysTable,
  &sisTable,
  &sisxTable,
  &aviTable,
  &asfTable,
  &bmpTable,
  &mpegTable,
  &jpegTable,
  &wavTable,
  &movTable,
  &rmTable,
  &mp3Table,
  &binTable
};

REGISTRY_BIN *detectedFormat = 0;


extern REGISTRY_MODE binMode;
extern REGISTRY_MODE textMode;
extern REGISTRY_MODE hexMode;
extern REGISTRY_MODE disMode;

static REGISTRY_MODE *mainModeTable[] =
{
  &textMode,
  &binMode,
  &hexMode,
  &disMode
};

REGISTRY_MODE *activeMode;
static size_t LastMode = sizeof(mainModeTable)/sizeof(REGISTRY_BIN *)+10;

static unsigned defMainModeSel = 0;

tBool SelectMode( void )
{
  const char *modeName[sizeof(mainModeTable)/sizeof(REGISTRY_MODE *)];
  size_t i,nModes;
  int retval;

  nModes = sizeof(mainModeTable)/sizeof(REGISTRY_MODE *);
  for(i = 0;i < nModes;i++) modeName[i] = mainModeTable[i]->name;
  retval = SelBoxA(modeName,nModes," Select translation mode: ",defMainModeSel);
  if(retval != -1)
  {
    if(activeMode->term) activeMode->term();
    activeMode = mainModeTable[retval];
    if(activeMode->init) activeMode->init();
    defMainModeSel = retval;
    return True;
  }
  return False;
}

static void __NEAR__ __FASTCALL__ init_modes( hIniProfile *ini )
{
  if(activeMode->init) activeMode->init();
  if(activeMode->read_ini) activeMode->read_ini(ini);
}

static void __NEAR__ __FASTCALL__ term_modes( void )
{
  if(activeMode->term) activeMode->term();
}

static void __NEAR__ __FASTCALL__ __init_beye( void )
{
   LastOpenFileName = PMalloc(4096);
   ListFile = PMalloc((ArgCount-1)*sizeof(char *));
   if((!LastOpenFileName) || (!ListFile))
   {
     printm("BEYE initialization failed! Out of memory!");
     exit(EXIT_FAILURE);
   }
}

static void __NEAR__ __FASTCALL__ __term_beye( void )
{
   PFREE(LastOpenFileName);
   PFREE(ListFile);
}

void QuickSelectMode( void )
{
  unsigned nModes;
  nModes = sizeof(mainModeTable)/sizeof(REGISTRY_MODE *);
  if(defMainModeSel < nModes - 1) defMainModeSel++;
  else                            defMainModeSel = 0;
  if(activeMode->term) activeMode->term();
  activeMode = mainModeTable[defMainModeSel];
  if(activeMode->init) activeMode->init();
}

static void __NEAR__ __FASTCALL__ MakeShortName( void )
{
  unsigned l;
  unsigned slen = twGetClientWidth(TitleWnd)-54;
  l = strlen(ArgVector[1]);
  if(l <= slen) strcpy(shortname,ArgVector[1]);
  else
  {
    strncpy(shortname,ArgVector[1],slen/2 - 3);
    shortname[slen/2-4] = 0;
    strcat(shortname,"...");
    strcat(shortname,&ArgVector[1][l - slen/2]);
  }
  __nls_CmdlineToOem((unsigned char *)shortname,strlen(shortname));
}

__filesize_t IsNewExe()
{
  __filesize_t ret;
  char id[2];
  bmReadBufferEx(id,sizeof(id),0,BM_SEEK_SET);
#if 0
   /*
      It is well documented technology, but it correctly working
      only with normal stubs, i.e. when New EXE header is located at
      offset > 0x40. However, in PC world exists files with wrong
      stubs, which are normal for Host OS. Hence beye must recognize
      them as normal New EXE files, despite the fact that DOS can
      not execute ones.
      Fixed by Kostya Nosov <k-nosov@yandex.ru>.
   */
   if(!( id[0] == 'M' && id[1] == 'Z' &&
        bmReadWordEx(0x18,BM_SEEK_SET) >= 0x40 &&
        (ret=bmReadDWordEx(0x3C,BM_SEEK_SET)) > 0x40L)) ret = 0;
#endif
   if(!( id[0] == 'M' && id[1] == 'Z' &&
        (ret=bmReadDWordEx(0x3C,BM_SEEK_SET)) > 0x02L)) ret = 0;
   return (__filesize_t)ret;
}

static void __NEAR__ __FASTCALL__ AutoDetectMode( void )
{
  int i,n;
  n = sizeof(mainModeTable) / sizeof(REGISTRY_MODE *);
  for(i = 0;i < n;i++)
  {
    if(mainModeTable[i]->detect())
    {
      defMainModeSel = i;
      break;
    }
  }
  activeMode = mainModeTable[i];
  BMSeek(0,BM_SEEK_SET);
}

struct tagbeyeArg
{
  const char key[4];
  const char *prompt;
}beyeArg[] =
{
  { "-a", "autodetect mode (default)" },
  { "-b", "view file in binary mode" },
  { "-d", "view file in disassembler mode" },
  { "-h", "view file in hexadecimal mode" },
  { "-t", "view file in text mode" },
  { "-s", "change size of file to NNN bytes (create, if file does not exist)" },
  { "-i", "ignore .ini file (create new)" },
  { "-?", "display this screen" }
};

static int __NEAR__ __FASTCALL__ queryKey(char *arg)
{
  int ret = -1;
  size_t i;
  for(i = 0;i < sizeof(beyeArg)/sizeof(struct tagbeyeArg);i++)
  {
    if(strcmp(arg,beyeArg[i].key) == 0) { ret = i; break; }
  }
  return ret;
}

static unsigned int  beye_mode     = UINT_MAX;
static __filesize_t  new_file_size = FILESIZE_MAX;

static void __NEAR__ __FASTCALL__ ParseCmdLine( void )
{
  unsigned i;
  ListFileCount = 0;
  for(i = 1;i < ArgCount;i++)
  {
     int beye_key;
     beye_key = queryKey(ArgVector[i]);
     switch(beye_key)
     {
       case 0: beye_mode = UINT_MAX; break;
       case 1: beye_mode = 1; break;
       case 2: beye_mode = 3; break;
       case 3: beye_mode = 2; break;
       case 4: beye_mode = 0; break;
       case 5:
#if (__WORDSIZE >= 32) && !defined(__QNX4__)
		new_file_size = strtoull(ArgVector[++i],NULL,10);
#else
		new_file_size = strtoul(ArgVector[++i],NULL,10);
#endif
		break;
       case 6: UseIniFile = False; break;
       case 7: ListFileCount = 0; return;
       default: ListFile[ListFileCount++] = ArgVector[i];
     }
  }
  if(ListFileCount) ArgVector[1] = ListFile[0];
}

static tBool __NEAR__ __FASTCALL__ LoadInfo( void )
{
   MakeShortName();
   if(new_file_size != FILESIZE_MAX)
   {
       bhandle_t handle;
       if(__IsFileExists(ArgVector[1]) == False) handle = __OsCreate(ArgVector[1]);
       else
       {
         handle = __OsOpen(ArgVector[1],FO_READWRITE | SO_DENYNONE);
         if(handle == NULL_HANDLE) handle = __OsOpen(ArgVector[1],FO_READWRITE | SO_COMPAT);
       }
       if(handle != NULL_HANDLE)
       {
           __OsChSize(handle,new_file_size);
           __OsClose(handle);
       }
       else
       {
          errnoMessageBox(OPEN_FAIL,NULL,errno);
          return False;
       }
   }
   if(BMOpen(ArgVector[1]) != 0) return False;
   if(beye_mode != UINT_MAX)
   {
     defMainModeSel = beye_mode;
     activeMode = mainModeTable[defMainModeSel];
   }
   else
   {
     if(LastMode >= sizeof(mainModeTable)/sizeof(REGISTRY_MODE *) || !isValidIniArgs()) AutoDetectMode();
     else
     {
       defMainModeSel = LastMode;
       activeMode = mainModeTable[defMainModeSel];
     }
   }
 return True;
}

static void __NEAR__ __FASTCALL__ __detectBinFmt( void )
{
 unsigned i;
 if(!bmGetFLength())
 {
   detectedFormat = &binTable;
   return;
 }
 for(i = 0;i < sizeof(mainBinTable)/sizeof(REGISTRY_BIN *);i++)
 {
   if(mainBinTable[i]->check_format())
   {
     detectedFormat = mainBinTable[i];
     if(detectedFormat->init) detectedFormat->init();
     break;
   }
 }
 /* Special case: mz initialization */
 mzTable.check_format();
}

void PaintTitle( void )
{
 twUseWin(TitleWnd);
 twFreezeWin(TitleWnd);
 twGotoXY(1,1);
 twClrEOL();
 twPrintF("File : %s",shortname);
 twGotoXY(twGetClientWidth(TitleWnd)-43,1);
 twPrintF("Size : %8llu bytes",BMGetFLength());
 twRefreshWin(TitleWnd);
}

static void MyAtExit( void )
{
  if(MainWnd) CloseWnd(MainWnd);
  if(HelpWnd) CloseWnd(HelpWnd);
  if(TitleWnd) CloseWnd(TitleWnd);
  if(ErrorWnd) CloseWnd(ErrorWnd);
  termBConsole();
  __term_beye();
  __term_sys();
}

tBool isValidIniArgs( void )
{
  return iniSettingsAnywhere ? True :
         ArgVector[1] ?
	 strcmp(ArgVector[1],LastOpenFileName) == 0 ?
	 beye_mode != UINT_MAX && beye_mode != LastMode ?
	 False : True : False : False;
}

static hIniProfile * __NEAR__ __FASTCALL__ load_ini_info( void )
{
  char tmp[20], buf[20];
  hIniProfile *ini;
  ini_name = getenv("BEYE_INI");
  if(!ini_name) ini_name = __get_ini_name("beye");
  ini = UseIniFile ? iniOpenFile(ini_name,NULL) : NULL;
  beyeReadProfileString(ini,"Beye","Setup","HelpName","",beye_help_name,sizeof(beye_help_name));
  beyeReadProfileString(ini,"Beye","Setup","SkinName","",beye_skin_name,sizeof(beye_skin_name));
  beyeReadProfileString(ini,"Beye","Setup","SyntaxName","",beye_syntax_name,sizeof(beye_syntax_name));
  beyeReadProfileString(ini,"Beye","Search","String","",(char *)search_buff,sizeof(search_buff));
  search_len = strlen((char *)search_buff);
  beyeReadProfileString(ini,"Beye","Search","Case","off",tmp,sizeof(tmp));
  beyeSearchFlg = stricmp(tmp,"on") == 0 ? SF_CASESENS : SF_NONE;
  beyeReadProfileString(ini,"Beye","Search","Word","off",tmp,sizeof(tmp));
  if(stricmp(tmp,"on") == 0) beyeSearchFlg |= SF_WORDONLY;
  beyeReadProfileString(ini,"Beye","Search","Backward","off",tmp,sizeof(tmp));
  if(stricmp(tmp,"on") == 0) beyeSearchFlg |= SF_REVERSE;
  beyeReadProfileString(ini,"Beye","Search","Template","off",tmp,sizeof(tmp));
  if(stricmp(tmp,"on") == 0) beyeSearchFlg |= SF_WILDCARDS;
  beyeReadProfileString(ini,"Beye","Search","UsePlugin","off",tmp,sizeof(tmp));
  if(stricmp(tmp,"on") == 0) beyeSearchFlg |= SF_PLUGINS;
  beyeReadProfileString(ini,"Beye","Search","AsHex","off",tmp,sizeof(tmp));
  if(stricmp(tmp,"on") == 0) beyeSearchFlg |= SF_ASHEX;
  beyeReadProfileString(ini,"Beye","Browser","LastOpen","",LastOpenFileName,4096);
  sprintf(buf,"%u",LastMode); /* [dBorca] so that src and dst won't overlap for strncpy */
  beyeReadProfileString(ini,"Beye","Browser","LastMode",buf,tmp,sizeof(tmp));
  LastMode = (size_t)strtoul(tmp,NULL,10);
  beyeReadProfileString(ini,"Beye","Browser","Offset","0",tmp,sizeof(tmp));
#if (__WORDSIZE >= 32) && !defined(__QNX4__)
  LastOffset = atoll(tmp);
#else
  LastOffset = atol(tmp); /** by watcom */
#endif
  beyeReadProfileString(ini,"Beye","Setup","Version","",beye_ini_ver,sizeof(beye_ini_ver));
  beyeReadProfileString(ini,"Beye","Setup","DirectConsole","yes",tmp,sizeof(tmp));
  if(stricmp(tmp,"yes") == 0) beye_vioIniFlags = __TVIO_FLG_DIRECT_CONSOLE_ACCESS;
  beyeReadProfileString(ini,"Beye","Setup","ForceMono","no",tmp,sizeof(tmp));
  if(stricmp(tmp,"yes") == 0) beye_twinIniFlags = TWIF_FORCEMONO;
  beyeReadProfileString(ini,"Beye","Setup","Force7Bit","no",tmp,sizeof(tmp));
  if(stricmp(tmp,"yes") == 0) beye_vioIniFlags |= __TVIO_FLG_USE_7BIT;
  beyeReadProfileString(ini,"Beye","Setup","MouseSens","yes",tmp,sizeof(tmp));
  if(stricmp(tmp,"yes") == 0) beye_kbdFlags = KBD_NONSTOP_ON_MOUSE_PRESS;
  beyeReadProfileString(ini,"Beye","Setup","IniSettingsAnywhere","no",tmp,sizeof(tmp));
  if(stricmp(tmp,"yes") == 0) iniSettingsAnywhere = True;
  beyeReadProfileString(ini,"Beye","Setup","FioUseMMF","no",tmp,sizeof(tmp));
  if(stricmp(tmp,"yes") == 0) fioUseMMF = True;
  if(!__mmfIsWorkable()) fioUseMMF = False;
  beyeReadProfileString(ini,"Beye","Setup","PreserveTimeStamp","no",tmp,sizeof(tmp));
  if(stricmp(tmp,"yes") == 0) iniPreserveTime = True;
  beyeReadProfileString(ini,"Beye","Setup","UseExternalProgs","no",tmp,sizeof(tmp));
  if(stricmp(tmp,"yes") == 0) iniUseExtProgs = True;
  beyeReadProfileString(ini,"Beye","Setup","Codepage","CP866",beye_codepage,sizeof(beye_codepage));
  return ini;
}

static void __NEAR__ __FASTCALL__ save_ini_info( void )
{
  char tmp[20];
  hIniProfile *ini;
  search_buff[search_len] = 0;
  ini = iniOpenFile(ini_name,NULL);
  beyeWriteProfileString(ini,"Beye","Setup","HelpName",beye_help_name);
  beyeWriteProfileString(ini,"Beye","Setup","SkinName",beye_skin_name);
  beyeWriteProfileString(ini,"Beye","Setup","SyntaxName",beye_syntax_name);
  beyeWriteProfileString(ini,"Beye","Setup","Version",BEYE_VERSION);
  beyeWriteProfileString(ini,"Beye","Search","String",(char *)search_buff);
  beyeWriteProfileString(ini,"Beye","Search","Case",beyeSearchFlg & SF_CASESENS ? "on" : "off");
  beyeWriteProfileString(ini,"Beye","Search","Word",beyeSearchFlg & SF_WORDONLY ? "on" : "off");
  beyeWriteProfileString(ini,"Beye","Search","Backward",beyeSearchFlg & SF_REVERSE ? "on" : "off");
  beyeWriteProfileString(ini,"Beye","Search","Template",beyeSearchFlg & SF_WILDCARDS ? "on" : "off");
  beyeWriteProfileString(ini,"Beye","Search","UsePlugin",beyeSearchFlg & SF_PLUGINS ? "on" : "off");
  beyeWriteProfileString(ini,"Beye","Search","AsHex",beyeSearchFlg & SF_ASHEX ? "on" : "off");
  beyeWriteProfileString(ini,"Beye","Browser","LastOpen",ArgVector[1]);
  sprintf(tmp,"%u",defMainModeSel);
  beyeWriteProfileString(ini,"Beye","Browser","LastMode",tmp);
#if (__WORDSIZE >= 32) && !defined(__QNX4__)
  sprintf(tmp,"%llu",LastOffset);
#else
  sprintf(tmp,"%lu",LastOffset);
#endif
  beyeWriteProfileString(ini,"Beye","Browser","Offset",tmp);
  strcpy(tmp,beye_vioIniFlags & __TVIO_FLG_DIRECT_CONSOLE_ACCESS ? "yes" : "no");
  beyeWriteProfileString(ini,"Beye","Setup","DirectConsole",tmp);
  strcpy(tmp,beye_twinIniFlags & TWIF_FORCEMONO ? "yes" : "no");
  beyeWriteProfileString(ini,"Beye","Setup","ForceMono",tmp);
  strcpy(tmp,(beye_vioIniFlags & __TVIO_FLG_USE_7BIT) == __TVIO_FLG_USE_7BIT ? "yes" : "no");
  beyeWriteProfileString(ini,"Beye","Setup","Force7Bit",tmp);
  strcpy(tmp,beye_kbdFlags & KBD_NONSTOP_ON_MOUSE_PRESS ? "yes" : "no");
  beyeWriteProfileString(ini,"Beye","Setup","MouseSens",tmp);
  strcpy(tmp,iniSettingsAnywhere ? "yes" : "no");
  beyeWriteProfileString(ini,"Beye","Setup","IniSettingsAnywhere",tmp);
  strcpy(tmp,fioUseMMF ? "yes" : "no");
  beyeWriteProfileString(ini,"Beye","Setup","FioUseMMF",tmp);
  strcpy(tmp,iniPreserveTime ? "yes" : "no");
  beyeWriteProfileString(ini,"Beye","Setup","PreserveTimeStamp",tmp);
  strcpy(tmp,iniUseExtProgs ? "yes" : "no");
  beyeWriteProfileString(ini,"Beye","Setup","UseExternalProgs",tmp);
  beyeWriteProfileString(ini,"Beye","Setup","Codepage",beye_codepage);
  if(activeMode->save_ini) activeMode->save_ini(ini);
  udnTerm(ini);
  iniCloseFile(ini);
}

static FTime ftim;
static tBool ftim_ok = False;

static void __FASTCALL__ ShowUsage(void) {
    unsigned evt,i,nln,h,y;
    TWindow *win;
    nln = sizeof(beyeArg)/sizeof(struct tagbeyeArg);
    h = nln+4;
    y = tvioHeight/2-h/2;
    win = WindowOpen(2,y,tvioWidth-1,y+h,TWS_NONE | TWS_NLSOEM);
    if(!win) goto done;
    twSetTitleAttr(win,BEYE_VER_MSG,TW_TMODE_CENTER,error_cset.border);
    twCentredWin(win,NULL);
    twSetColorAttr(error_cset.main);
    twSetFrameAttr(win,TW_DOUBLE_FRAME,error_cset.border);
    twSetFooterAttr(win," Press [ ESC ] to quit ",TW_TMODE_RIGHT,error_cset.border);
    twClearWin();
    twGotoXY(1,1);
    twPutS(" Usage: beye [OPTIONS] file...");
    for(i = 0;i < nln;i++)
    {
	twGotoXY(1,4+i);
	twPrintF("  %s     %s\n",beyeArg[i].key,beyeArg[i].prompt);
    }
    twShowWin(win);
    do {
	evt = GetEvent(NULL,NULL,ErrorWnd);
    }while(!(evt == KE_ESCAPE || evt == KE_F(10) || evt == KE_ENTER));
    twDestroyWin(win);
    done:
    termBConsole();
}

int main( int argc, char *argv[] )
{
 hIniProfile *ini;
 tBool skin_err;
 int retval;
#ifndef NDEBUG
#ifdef RLIMIT_CORE
  {
    /* on many systems default coresize is 0.
       Enable any coresize here. */
    struct rlimit rl;
    getrlimit(RLIMIT_CORE,&rl);
    rl.rlim_cur = rl.rlim_max;
    setrlimit(RLIMIT_CORE,&rl);
  }
#endif
#endif
 ArgCount = argc;
 ArgVector = argv;
 __init_sys();
 __init_beye();
 ini = load_ini_info();
 skin_err = csetReadIniFile(beye_skin_name);
 initBConsole(beye_vioIniFlags,beye_twinIniFlags);
 if(ArgCount < 2) goto show_usage;
 ParseCmdLine();
 if(!ListFileCount)
 {
   /** print usage message */
    size_t i;
    show_usage:
    ShowUsage();
    printm("\n"BEYE_VER_MSG"\n");
    printm(" Usage: beye [OPTIONS] file...\n\n");
    for(i = 0;i < sizeof(beyeArg)/sizeof(struct tagbeyeArg);i++)
    {
      printm("  %s\t%s\n",beyeArg[i].key,beyeArg[i].prompt);
    }
    printm("\n");
    return EXIT_FAILURE;
 }
 udnInit(ini);
 ErrorWnd = WindowOpen(1,1,50,16,TWS_NONE | TWS_NLSOEM);
 if(ErrorWnd) twSetTitleAttr(ErrorWnd," Error ",TW_TMODE_CENTER,error_cset.border);
 else { printm("fatal error: can't create window"); return EXIT_FAILURE; }
 twCentredWin(ErrorWnd,NULL);
 twSetColorAttr(error_cset.main);
 twSetFrameAttr(ErrorWnd,TW_DOUBLE_FRAME,error_cset.border);
 HelpWnd = WindowOpen(1,tvioHeight,tvioWidth,tvioHeight,TWS_NLSOEM);
 twSetColorAttr(prompt_cset.digit);
 twClearWin();
 twShowWin(HelpWnd);
 if(strcmp(beye_ini_ver,BEYE_VERSION) != 0) Setup();
 TitleWnd = WindowOpen(1,1,tvioWidth,1,TWS_NONE);
 twSetColorAttr(title_cset.main);
 twClearWin();
 twShowWin(TitleWnd);
 activeMode = mainModeTable[1];
 atexit(MyAtExit);
 retval = EXIT_SUCCESS;
 if(skin_err)
 {
   char sout[256];
   sprintf(sout,"Error in skin file detected: '%s'",last_skin_error);
   ErrMessageBox(sout,NULL);
 }
 /* We must do it before opening a file because of some RTL has bug
    when are trying to open already open file with no sharing access */
 ftim_ok = __OsGetFTime(ArgVector[1],&ftim);
 if(!LoadInfo())
 {
   if(ini) iniCloseFile(ini);
   retval = EXIT_FAILURE;
   goto Bye;
 }
 __detectBinFmt();
 init_modes(ini);
 init_addons();
 init_sysinfo();
 if(ini) iniCloseFile(ini);
 MainWnd = WindowOpen(1,2,tvioWidth,tvioHeight-1,TWS_NONE);
 twSetColorAttr(browser_cset.main);
 twClearWin();
 PaintTitle();
 if(!isValidIniArgs() || LastOffset > BMGetFLength()) LastOffset = 0;
 twShowWin(MainWnd);
 MainLoop();
 LastOffset = BMGetCurrFilePos();
 save_ini_info();
 term_sysinfo();
 term_addons();
 term_modes();
 if(detectedFormat->destroy) detectedFormat->destroy();
 BMClose();
 if(iniPreserveTime && ftim_ok) __OsSetFTime(ArgVector[1],&ftim);
 Bye:
 return retval;
}

tBool NewSource( void )
{
  int i;
  tBool ret;
  unsigned j,freq;
  static int prev_file;
  char ** nlsListFile;
  nlsListFile = (char **)PMalloc(ListFileCount*sizeof(char *));
  if(nlsListFile)
  {
    for(j = 0;j < ListFileCount;j++)
    {
      nlsListFile[j] = PMalloc(strlen(ListFile[j])+1);
      if(!nlsListFile[j]) break;
    }
  }
  else { MemOutBox("Initializing List of File\n"); return 0; }
  for(freq = 0;freq < j;freq++)
  {
    unsigned ls;
    ls = strlen(ListFile[freq]);
    memcpy(nlsListFile[freq],ListFile[freq],ls+1);
    __nls_CmdlineToOem((unsigned char *)nlsListFile[freq],ls);
  }
  i = SelBoxA((const char **)nlsListFile,j," Select new file: ",prev_file);
  ret = 0;
  for(freq = 0;freq < j;freq++) PFree(nlsListFile[freq]);
  PFree(nlsListFile);
  if(i != -1)
  {
    if(iniPreserveTime && ftim_ok) __OsSetFTime(ArgVector[1],&ftim);
    BMClose();
    ftim_ok = __OsGetFTime(ListFile[i],&ftim);
    if(BMOpen(ListFile[i]) == 0)
    {
      ArgVector[1] = ListFile[i];
      if(detectedFormat->destroy) detectedFormat->destroy();
      if(activeMode->term) activeMode->term();
      MakeShortName();
      __detectBinFmt();
      if(activeMode->init) activeMode->init();
      ret = True;
    }
    else
    {
       if(BMOpen(ArgVector[1]) != 0)
       {
         exit(EXIT_FAILURE);
       }
       if(detectedFormat->destroy) detectedFormat->destroy();
       if(activeMode->term) activeMode->term();
       MakeShortName();
       __detectBinFmt();
       if(activeMode->init) activeMode->init();
       ret = False;
    }
  }
  return ret;
}

unsigned __FASTCALL__ beyeReadProfileString(hIniProfile *ini,
                               const char *section,
                               const char *subsection,
                               const char *_item,
                               const char *def_value,
                               char *buffer,
                               unsigned cbBuffer)
{
  return UseIniFile ? iniReadProfileString(ini,section,subsection,
                                           _item,def_value,buffer,cbBuffer)
                    : 1;
}

tBool __FASTCALL__ beyeWriteProfileString(hIniProfile *ini,
                                          const char *section,
                                          const char *subsection,
                                          const char *item,
                                          const char *value)
{
  return iniWriteProfileString(ini,section,subsection,item,value);
}
