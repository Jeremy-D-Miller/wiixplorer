/***************************************************************************
 * Copyright (C) 2010
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
 * for WiiXplorer 2010
 ***************************************************************************/
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <gctypes.h>

const char * fmt(const char * format, ...)
{
    static char strChar[512];
    memset(strChar, 0, sizeof(strChar));
	char * tmp = NULL;

	va_list va;
	va_start(va, format);
	if((vasprintf(&tmp, format, va) >= 0) && tmp)
	{
        snprintf(strChar, sizeof(strChar), tmp);
		free(tmp);
		return (const char *) strChar;
	}
	va_end(va);

	return NULL;
}

const wchar_t * wfmt(const char * format, ...)
{
    static wchar_t strWChar[512];
    memset(strWChar, 0, sizeof(strWChar));

	if(!format)
        return (const wchar_t *) &strWChar;

	if(strcmp(format, "") == 0)
        return (const wchar_t *) &strWChar;

	char * tmp = NULL;

	va_list va;
	va_start(va, format);
	if((vasprintf(&tmp, format, va) >= 0) && tmp)
	{
        int	bt;
        int strlength = strlen(tmp);
        bt = mbstowcs(strWChar, tmp, (strlength < 512) ? strlength : 512 );
        free(tmp);

        if(bt > 0)
            return (const wchar_t *) &strWChar;
	}
	va_end(va);

	return NULL;
}

bool char2wchar_t(const char * strChar, wchar_t * dest)
{
    if(!strChar || !dest)
        return false;

    int	bt;
    bt = mbstowcs(dest, strChar, strlen(strChar));
    if (bt > 0) {
        dest[bt] = (wchar_t) '\0';
        return true;
    }

    return false;
}

int strtokcmp(const char * string, const char * compare, const char * separator)
{
    if(!string || !compare)
        return -1;

    char TokCopy[512];
    strcpy(TokCopy, compare);

    char * strTok = strtok(TokCopy, separator);

    while (strTok != NULL)
    {
        if (strcasecmp(string, strTok) == 0)
        {
            return 0;
        }
        strTok = strtok(NULL,separator);
    }

    return -1;
}
