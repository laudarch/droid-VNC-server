/*
droid vnc server - Android VNC server
Copyright (C) 2011 Jose Pereira <onaips@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <dlfcn.h>

#include "gralloc.h"
#include "common.h"

void *gralloc_lib = NULL;


close_fn_type close_gralloc = NULL;
readfb_fn_type readfb_gralloc = NULL;
getscreenformat_fn_type getscreenformat_gralloc = NULL;

int initGralloc(void)
{
  L("--Loading gralloc native lib--\n");

  gralloc_lib = dlopen("/data/libdvnc_gralloc_sdk14.so", RTLD_NOW);
  if(gralloc_lib == NULL) {
    L("Couldnt load library! Error string: %s\n",dlerror());
    return -1;
  }

  init_fn_type init_gralloc = dlsym(gralloc_lib,"init_gralloc");
  if(init_gralloc == NULL) {
    L("Couldn't load init_gralloc! Error string: %s\n",dlerror());
    return -1;
  }

  close_gralloc = dlsym(gralloc_lib,"close_gralloc");
  if(close_gralloc == NULL) {
    L("Couldn't load close_gralloc! Error string: %s\n",dlerror());
    return -1;
  }

  readfb_gralloc = dlsym(gralloc_lib,"readfb_gralloc");
  if(readfb_gralloc == NULL) {
    L("Couldn't load readfb_gralloc! Error string: %s\n",dlerror());
    return -1;
  }

  getscreenformat_gralloc = dlsym(gralloc_lib,"getscreenformat_gralloc");
  if(getscreenformat_gralloc == NULL) {
    L("Couldn't load get_screenformat! Error string: %s\n",dlerror());
    return -1;
  }

  int ret = init_gralloc();
  if (ret == -1) {
  L("Gralloc method not supported by this device!\n");
  return -1;
  }

  screenformat = getScreenFormatGralloc();
  if ( screenformat.width <= 0 ) {
    L("Error: I have received a bad screen size from gralloc.\n");
    return -1;
  }

  return 0;
}

screenFormat getScreenFormatGralloc(void)
{
  screenFormat f;
  if (getscreenformat_gralloc)
     f = getscreenformat_gralloc();
  return f;
}

void closeGralloc(void)
{
  if (close_gralloc)
    close_gralloc();
  if (gralloc_lib)
    dlclose(gralloc_lib);
}

unsigned char *readBufferGralloc(void)
{
  if (readfb_gralloc)
    return readfb_gralloc();
  return NULL;
}
