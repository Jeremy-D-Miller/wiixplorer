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
#ifndef SCROLLBAR_HPP_
#define SCROLLBAR_HPP_

#include "libwiigui/gui.h"

enum
{
    ICONMODE = 0,
    LISTMODE,
};

class Scrollbar : public GuiElement
{
    public:
        Scrollbar(int height, u8 mode = LISTMODE);
        ~Scrollbar();
        void SetPageSize(int size);
        void SetRowSize(int size);
        void SetSelectedItem(int pos);
        void SetSelectedIndex(int pos);
        void SetEntrieCount(int cnt);
        int GetSelectedItem() { return SelItem; };
        int GetSelectedIndex() { return SelInd; };
        bool ListChanged();
        void SetScrollSpeed(u16 speed) { ScrollSpeed = speed; };
        void SetMinWidth(int w);
        void SetMaxWidth(int w);
        void SetMinHeight(int h);
        void SetMaxHeight(int h);
        void Draw();
        void Update(GuiTrigger * t);
    protected:
        void SetScrollboxPosition();
        void OnUpButtonHold(GuiElement *sender, int pointer, POINT p);
        void OnDownButtonHold(GuiElement *sender, int pointer, POINT p);
        void OnBoxButtonHold(GuiElement *sender, int pointer, POINT p);

        u8 Mode;
        u32 ScrollState;
        u16 ScrollSpeed;

        int MinHeight;
        int MaxHeight;
        int SelItem;
        int SelInd;
        int RowSize;
        int PageSize;
        int EntrieCount;
        bool listchanged;

		GuiButton * arrowUpBtn;
		GuiButton * arrowDownBtn;
		GuiButton * scrollbarBoxBtn;
		GuiImage * scrollbarTopImg;
		GuiImage * scrollbarButtomImg;
		GuiImage * scrollbarTileImg;
		GuiImage * arrowDownImg;
		GuiImage * arrowDownOverImg;
		GuiImage * arrowUpImg;
		GuiImage * arrowUpOverImg;
		GuiImage * scrollbarBoxImg;
		GuiImage * scrollbarBoxOverImg;
		GuiImageData * scrollbarTop;
		GuiImageData * scrollbarButtom;
		GuiImageData * scrollbarTile;
		GuiImageData * arrowDown;
		GuiImageData * arrowDownOver;
		GuiImageData * arrowUp;
		GuiImageData * arrowUpOver;
		GuiImageData * scrollbarBox;
		GuiImageData * scrollbarBoxOver;
		GuiSound * btnSoundOver;
		GuiSound * btnSoundClick;
		GuiTrigger * trigHeldA;
};

#endif
