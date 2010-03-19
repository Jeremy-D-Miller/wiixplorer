/****************************************************************************
 * Copyright (C) 2009
 * by LilouMaster
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
 *
 * for WiiXplorer 2009
 ***************************************************************************/

extern bool cfgWriteIni(CsString *FilePath, CsString *Section, CsString *Entries, CsString *Value);
extern bool cfgReadIni(CsString *FilePath, CsString *Section, CsString *Entries, CsString * Value);


class blocConfig 
{ 
private :
CsString  sFilePath; 
CsString  dFilePath; 
CsString  dSection ;
FILE     *dfile; 


public :

 bool cfgOpen(CsString *FilePath, CsString *Section) ;
 bool cfgWriteEntries( CsString *Entries,  CsString *Value) ;
 bool cfgClose(void);

}; // end of class 