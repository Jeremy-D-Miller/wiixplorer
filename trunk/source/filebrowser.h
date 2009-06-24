/****************************************************************************
 * libwiigui Template
 * Tantric 2009
 *
 * filebrowser.h
 *
 * Generic file routines - reading, writing, browsing
 ****************************************************************************/

#ifndef _FILEBROWSER_H_
#define _FILEBROWSER_H_

#include <unistd.h>
#include <gccore.h>

#define MAXJOLIET 255
#define MAXDISPLAY MAXPATHLEN


enum {
    SD,
    USB,
    SMB
};

typedef struct
{
	char dir[MAXPATHLEN]; // directory path of browserList
	char rootdir[10]; // directory path of browserList
	int numEntries; // # of entries in browserList
	int selIndex; // currently selected index of browserList
	int pageIndex; // starting index of browserList page display
} BROWSERINFO;

typedef struct
{
	u64 offset; // DVD offset
	u32 length; // file length
	char isdir; // 0 - file, 1 - directory
	char filename[MAXJOLIET + 1]; // full filename
	char displayname[MAXDISPLAY + 1]; // name for browser display
} BROWSERENTRY;

typedef struct
{
	char filepath[MAXPATHLEN];
	char filename[MAXJOLIET + 1];
	u32  filesize;
	bool isdir;
} CLIPBOARD;

extern BROWSERINFO browser;
extern BROWSERENTRY * browserList;

int UpdateDirName(int method);
int FileSortCallback(const void *f1, const void *f2);
void ResetBrowser();
int BrowserChangeFolder();
int BrowseDevice(int device);

#endif
