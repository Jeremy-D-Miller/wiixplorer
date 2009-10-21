/****************************************************************************
 * languagefile updater
 * Created for USB Loader GX by giantpune
 * Modified by Dj_Skual for WiiXPlorer
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>

#include "UpdateLanguage.h"

#include "Language/gettext.h"
#include "menu.h"
#include "main.h"
#include "Settings.h"
#include "network/networkops.h"
#include "network/http.h"
#include "network/URL_List.h"
#include "Prompts/PromptWindows.h"
#include "Prompts/ProgressWindow.h"
#include "fileops.h"

extern bool actioncanceled;

/****************************************************************************
 * LanguageUpdater
 ***************************************************************************/
int updateAllLanguageFiles() {

    if (!IsNetworkInit()) {
        int net = NetworkInitPrompt();
		if (net == 0) {
			return 0;
		}
    }

    if (IsNetworkInit()) {
        const char URL[60] = "http://wiixplorer.googlecode.com/svn/trunk/Languages/";
        char fullURL[300];
        char filenumber[50];
		FILE *pfile;
		
        URL_List LinkList(URL);
        int listsize = LinkList.GetURLCount();

		CreateSubfolder(Settings.LangPath);
		
		StartProgress(tr("Updating Language Files:"), LANGUAGE);

        for (int i = 0; i < listsize; i++) {
			
            if(actioncanceled)
			{
				usleep(20000);
				StopProgress();
				WindowPrompt(tr("ERROR"), tr("Action cancelled."), tr("OK"));
				return 0;
			}

			snprintf(filenumber, sizeof(filenumber), tr("%d files available"), listsize-3);
			ShowProgress(i, listsize-1, filenumber);

            if (strcasecmp(".lang", strrchr(LinkList.GetURL(i), '.')) == 0) {

                snprintf(fullURL, sizeof(fullURL), "%s%s", URL, LinkList.GetURL(i));

                struct block file = downloadfile(fullURL);

                if (file.data && file.size) {
                    char filepath[300];

                    snprintf(filepath, sizeof(filepath), "%s%s", Settings.LangPath, LinkList.GetURL(i));
                    pfile = fopen(filepath, "wb");
                    fwrite(file.data, 1, file.size, pfile);
                    fclose(pfile);

                }

                free(file.data);
				
            }
        }
        StopProgress();
		WindowPrompt(tr("All Language files updated."), 0, tr("OK"));
	}
	return 1;
}	