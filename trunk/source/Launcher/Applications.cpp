

#include <mxml.h>

#include "Applications.h"
#include "DirList.h"
#include "FileOperations/filebrowser.h"
#include "BootHomebrew/BootHomebrew.h"
#include "Controls/Clipboard.h"
#include "Controls/Taskbar.h"
#include "menu.h"

extern bool boothomebrew;

Applications::Applications()
{
	Search();
}

Applications::~Applications()
{
}

void Applications::Launch(Application app)
{
	if (LoadHomebrew(app.path) < 0)
		return;

	ItemStruct Item;
	memset(&Item, 0, sizeof(ItemStruct));
	snprintf(Item.itempath, sizeof(Item.itempath), "%s", app.path);
	Clipboard::Instance()->AddItem(&Item);
	boothomebrew = true;
	Taskbar::Instance()->SetMenu(MENU_EXIT);
}

bool Applications::GetNameFromXML(char *xml, char *name)
{
	mxml_node_t *tree;
	mxml_node_t *data;

	bool ret = false;

	FILE *fp = fopen(xml, "rb");
	if (fp)
	{
		tree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK);

		fclose(fp);
		if (tree)
		{
			data = mxmlFindElement(tree, tree, "name", NULL, NULL, MXML_DESCEND);
			if (data)
			{
				char *ptr = name;

				while (data->child != NULL && ptr < name+255)
				{
					if (data->child->value.text.whitespace)
						*ptr++ = ' ';

					strcpy(ptr, data->child->value.text.string);
					ptr += strlen(data->child->value.text.string);
					data->child = data->child->next;
				}
				*ptr = 0;

				mxmlDelete(data);
				ret = true;
			}

			mxmlDelete(tree);
		}
	}

	return ret;
}

void Applications::Search()
{
	char apppath[256], hbpath[256], hbname[256], metaname[256];
	char devices[][6] = {"sd", "usb", "ntfs0", "ntfs1", "ntfs2", "ntfs3", "ntfs4"};
	char apps[] = ":/apps/";

	FileBrowser browser;

	for (u32 i = 0; i < 7; i++)
	{
		sprintf(apppath, "%s%s", devices[i], apps);

		int entries = browser.BrowsePath(apppath);

		if (entries > 1)
		{
			for (int j = 1; j < entries; j++)
			{
				sprintf(hbpath, "%s%s", apppath, browser.GetItemFilename(j));

				DirList binary(hbpath, ".dol,.elf");
				if (binary.GetFilecount() > 0)
				{
					DirList meta(hbpath, ".xml");
					if (meta.GetFileIndex("meta.xml") >= 0)
					{
						sprintf(metaname, "%s/meta.xml", hbpath);

						if (!GetNameFromXML(metaname, hbname))
						{
							strcpy(hbname, browser.GetItemFilename(j));
						}
					}
					else
					{
						strcpy(hbname, browser.GetItemFilename(j));
					}

					Application app;
					sprintf(app.path, "%s/%s", binary.GetFilepath(0), binary.GetFilename(0));
					strcpy(app.name, hbname);

					applications.push_back(app);
				}
			}
		}
	}
}