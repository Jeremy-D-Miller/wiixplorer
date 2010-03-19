/***************************************************************************
 * Copyright (C) 2009
 * by Dimok
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 * ZipFile.cpp
 *
 * for WiiXplorer 2009
 ***************************************************************************/
#ifndef _ZIPFILE_H_
#define _ZIPFILE_H_

#include "unzip/unzip.h"
#include "7ZipFile.h"

class ZipFile
{
    public:
		//!Constructor
        ZipFile(const char *filepath);
		//!Destructor
		~ZipFile();
		//!Check if it is a 7zip file
        bool Is7ZipFile (char *buffer);
		//!Get the archive file structure
        ArchiveFileStruct * GetFileStruct(int fileIndx);
		//!Extract a files from a zip file to a path
		int ExtractFile(int ind, const char *dest, bool withpath = false);
		//!Extract all files from a zip file to a directory
		int ExtractAll(const char *dest);
		//!Get the total amount of items inside the archive
        u32 GetItemCount() { return ItemCount; };

    private:
        bool LoadList();
        void ResetOffsets();

        char filename[MAXPATHLEN];
        ArchiveFileStruct CurArcFile;
        int curFileIndex;
        int ItemCount;
        unz_file_pos * unzPosition;
        unzFile File;
        unz_file_info cur_file_info;
};

#endif