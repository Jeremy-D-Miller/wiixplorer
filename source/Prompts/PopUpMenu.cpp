/***************************************************************************
 * Copyright (C) 2009
 * by Dimok
 *
 * Copyright (C) 2010
 * by dude
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
 * PopUpMenu.cpp
 *
 * for WiiXplorer 2010
 ***************************************************************************/

#include "PopUpMenu.h"
#include "Controls/MainWindow.h"

const int ButtonX = 20;
const u32 ButtonHeight = 32;
const u32 MaxVisible = 12;

PopUpMenu::PopUpMenu(int x, int y)
    :GuiWindow(0, 0)
{
	choice = -1;
	width = 0;
	height = 0;
	maxTxtWidth = 0;
	scrollIndex = 0;
	hasIcons = false;
	hasSubmenus = false;

	PopUpMenuClick = Resources::GetSound(button_click_pcm, button_click_pcm_size);

	PopUpMenuUpper = Resources::GetImageData(ClickMenuUpper_png, ClickMenuUpper_png_size);
	PopUpMenuMiddle = Resources::GetImageData(ClickMenuMiddle_png, ClickMenuMiddle_png_size);
	PopUpMenuLower = Resources::GetImageData(ClickMenuLower_png, ClickMenuLower_png_size);
	PopUpMenuSelect = Resources::GetImageData(menu_selection_png, menu_selection_png_size);
	PopUpMenuScrollUp = Resources::GetImageData(arrow_up_png, arrow_up_png_size);
	PopUpMenuScrollDown = Resources::GetImageData(arrow_down_png, arrow_down_png_size);
	PopUpMenuExpand = Resources::GetImageData(arrow_right_png, arrow_right_png_size);

	PopUpMenuUpperImg = new GuiImage(PopUpMenuUpper);
	PopUpMenuMiddleImg = new GuiImage(PopUpMenuMiddle);
	PopUpMenuLowerImg = new GuiImage(PopUpMenuLower);
	PopUpMenuScrollUpImg = new GuiImage(PopUpMenuScrollUp);
	PopUpMenuScrollDownImg = new GuiImage(PopUpMenuScrollDown);

	trigA = new SimpleGuiTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	trigB = new GuiTrigger();
	trigUp = new GuiTrigger();
	trigDown = new GuiTrigger();
	trigHome = new GuiTrigger();
	trigB->SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);
	trigUp->SetButtonOnlyTrigger(-1, WPAD_BUTTON_UP | WPAD_CLASSIC_BUTTON_UP, PAD_BUTTON_UP);
	trigDown->SetButtonOnlyTrigger(-1, WPAD_BUTTON_DOWN | WPAD_CLASSIC_BUTTON_DOWN, PAD_BUTTON_DOWN);
	trigHome->SetButtonOnlyTrigger(-1, WPAD_BUTTON_HOME | WPAD_CLASSIC_BUTTON_HOME, PAD_TRIGGER_L);

	NoBtn = new GuiButton(screenwidth, screenheight);
	NoBtn->SetPosition(-x, -y);
	NoBtn->SetTrigger(trigA);
	NoBtn->SetTrigger(trigB);
	NoBtn->Clicked.connect(this, &PopUpMenu::OnClick);

	HomeBtn = new GuiButton(0, 0);
	HomeBtn->SetTrigger(trigHome);
	HomeBtn->Clicked.connect(this, &PopUpMenu::OnClick);

	ScrollUpBtn = new GuiButton(16, 16);
	ScrollUpBtn->SetImage(PopUpMenuScrollUpImg);
	ScrollUpBtn->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	ScrollUpBtn->SetTrigger(trigA);
	ScrollUpBtn->SetVisible(false);
	ScrollUpBtn->SetState(STATE_DISABLED);
	ScrollUpBtn->SetTrigger(trigUp);
	ScrollUpBtn->Clicked.connect(this, &PopUpMenu::OnScrollUp);

	ScrollDownBtn = new GuiButton(16, 16);
	ScrollDownBtn->SetImage(PopUpMenuScrollDownImg);
	ScrollDownBtn->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	ScrollDownBtn->SetTrigger(trigA);
	ScrollDownBtn->SetVisible(false);
	ScrollDownBtn->SetState(STATE_DISABLED);
	ScrollDownBtn->SetTrigger(trigDown);
	ScrollDownBtn->Clicked.connect(this, &PopUpMenu::OnScrollDown);

	Append(PopUpMenuUpperImg);
	Append(PopUpMenuMiddleImg);
	Append(PopUpMenuLowerImg);
	Append(NoBtn);
	Append(HomeBtn);
	Append(ScrollUpBtn);
	Append(ScrollDownBtn);

	xpos = x;
	ypos = y;
}

PopUpMenu::~PopUpMenu()
{
	MainWindow::Instance()->HaltGui();
	if(parentElement)
		((GuiWindow *) parentElement)->Remove(this);

	RemoveAll();

	Resources::Remove(PopUpMenuMiddle);
	Resources::Remove(PopUpMenuUpper);
	Resources::Remove(PopUpMenuLower);
	Resources::Remove(PopUpMenuSelect);
	Resources::Remove(PopUpMenuScrollUp);
	Resources::Remove(PopUpMenuScrollDown);
	Resources::Remove(PopUpMenuExpand);
	Resources::Remove(PopUpMenuClick);

	for (u32 i = 0; i < Item.size(); i++)
	{
		delete Item[i].ButtonMenuSelect;
		delete Item[i].Button;
		delete Item[i].ButtonTxt;
		delete Item[i].ButtonTxtOver;

		if (Item[i].Icon)
		{
			Resources::Remove(Item[i].Icon);
			delete Item[i].IconImg;
		}

		if (Item[i].ExpandImg)
			delete Item[i].ExpandImg;
	}

	Item.clear();

	delete PopUpMenuMiddleImg;
	delete PopUpMenuUpperImg;
	delete PopUpMenuLowerImg;
	delete PopUpMenuScrollUpImg;
	delete PopUpMenuScrollDownImg;

	delete ScrollUpBtn;
	delete ScrollDownBtn;
	delete NoBtn;
	delete HomeBtn;

	delete trigA;
	delete trigB;
	delete trigUp;
	delete trigDown;
	delete trigHome;

	MainWindow::Instance()->ResumeGui();
}

void PopUpMenu::AddItem(const char *text, const u8 *icon, u32 icon_size, bool submenu)
{
	Items item;

	item.ButtonTxt = new GuiText(text, 24, (GXColor){0, 0, 0, 255});
	item.ButtonTxtOver = new GuiText(text, 24, (GXColor){28, 32, 190, 255});
	item.Button = new GuiButton(0, 0);
	item.ButtonMenuSelect = new GuiImage(PopUpMenuSelect);
	item.ButtonTxt->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	item.ButtonTxtOver->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	item.Button->SetLabel(item.ButtonTxt);
	item.Button->SetLabelOver(item.ButtonTxtOver);
	item.Button->SetSoundClick(PopUpMenuClick);
	item.Button->SetImageOver(item.ButtonMenuSelect);
	item.Button->SetTrigger(trigA);
	item.Button->SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	item.Button->Clicked.connect(this, &PopUpMenu::OnClick);

	Append(item.Button);

	if (icon)
	{
		item.Icon = Resources::GetImageData(icon, icon_size);

		item.IconImg = new GuiImage(item.Icon);
		item.IconImg->SetPosition(ButtonX, (Item.size()+1)*ButtonHeight-18);
		item.IconImg->SetScale(0.8);

		hasIcons = true;

		Append(item.IconImg);
	}
	else
	{
		item.Icon = NULL;
		item.IconImg = NULL;
	}

	if (submenu)
	{
		item.ExpandImg = new GuiImage(PopUpMenuExpand);

		hasSubmenus = true;

		Append(item.ExpandImg);
	}
	else
		item.ExpandImg = NULL;

	if (maxTxtWidth < item.ButtonTxt->GetTextWidth())
		maxTxtWidth = item.ButtonTxt->GetTextWidth();

	Item.push_back(item);
}

void PopUpMenu::Finish()
{
	u32 x = xpos;
	u32 y = ypos;
	u32 middleheight = ButtonHeight*Item.size();
	int leftmargin = (hasIcons ? ButtonX+40 : ButtonX);
	int rightmargin = (hasSubmenus ? 40 : 20);
	float scale = 1.0f;

	if (Item.size() > MaxVisible)
	{
		ScrollDownBtn->SetVisible(true);
		ScrollDownBtn->SetState(STATE_DEFAULT);

		middleheight = ButtonHeight*MaxVisible;

		for (u32 i = MaxVisible; i < Item.size(); i++)
		{
			Item[i].Button->SetVisible(false);
			Item[i].Button->SetState(STATE_DISABLED);

			if (Item[i].IconImg)
				Item[i].IconImg->SetVisible(false);

			if (Item[i].ExpandImg)
				Item[i].ExpandImg->SetVisible(false);
		}
	}

	width  = maxTxtWidth+leftmargin+rightmargin;
	height = PopUpMenuUpperImg->GetHeight()+middleheight+PopUpMenuLowerImg->GetHeight();

	PopUpMenuMiddleImg->SetTileVertical(middleheight/PopUpMenuMiddleImg->GetHeight());

	if (width > PopUpMenuMiddleImg->GetWidth())
		scale = 1.0f * width / PopUpMenuMiddleImg->GetWidth();

	PopUpMenuUpperImg->SetScaleX(scale);
	PopUpMenuMiddleImg->SetScaleX(scale);
	PopUpMenuLowerImg->SetScaleX(scale);

	u32 scaledX = (scale-1)*PopUpMenuMiddleImg->GetWidth()/2;

	PopUpMenuUpperImg->SetPosition(scaledX, 0);
	PopUpMenuMiddleImg->SetPosition(scaledX, PopUpMenuUpperImg->GetHeight());
	PopUpMenuLowerImg->SetPosition(scaledX, PopUpMenuUpperImg->GetHeight()+middleheight);

	ScrollUpBtn->SetPosition(width/2, 0);
	ScrollDownBtn->SetPosition(width/2, PopUpMenuLowerImg->GetTop());

	if (x + width + 15 > (u32)screenwidth)
		x = screenwidth - width - 15;

	if (y + height + 25 > (u32)screenheight)
		y = screenheight - height - 25;

	for (u32 i = 0; i < Item.size(); i++)
	{
		Item[i].Button->SetSize(maxTxtWidth, ButtonHeight);
		Item[i].Button->SetPosition(leftmargin, i*ButtonHeight+20);

		Item[i].ButtonMenuSelect->SetScaleX(scale);
		Item[i].ButtonMenuSelect->SetPosition(scaledX, -4);

		if (Item[i].ExpandImg)
			Item[i].ExpandImg->SetPosition(width-32, i*ButtonHeight+22);
	}

	SetPosition(x, y);
}

int PopUpMenu::GetChoice()
{
	return choice;
}

void PopUpMenu::OnClick(GuiElement *sender, int pointer, POINT p)
{
	sender->ResetState();
	//TODO add the functions instead of the link
	if (sender == NoBtn || sender == HomeBtn)
	{
		choice = -10;
		return;
	}

	for (u32 i = 0; i < Item.size(); i++)
	{
		if(sender == Item[i].Button)
		{
			choice = i;
			break;
		}
	}
}

void PopUpMenu::OnScrollUp(GuiElement *sender, int pointer, POINT p)
{
	sender->ResetState();

	Scroll(UP);
}

void PopUpMenu::OnScrollDown(GuiElement *sender, int pointer, POINT p)
{
	sender->ResetState();

	Scroll(DOWN);
}

void PopUpMenu::Scroll(int direction)
{
	choice = -1;
	int step = ButtonHeight;

	if (direction == UP)
	{
		if (scrollIndex < 1)
			return;

		scrollIndex--;
		ScrollDownBtn->SetVisible(true);
		ScrollDownBtn->SetState(STATE_DEFAULT);
	}
	else
	{
		if (scrollIndex >= Item.size()-MaxVisible)
			return;

		scrollIndex++;
		ScrollUpBtn->SetVisible(true);
		ScrollUpBtn->SetState(STATE_DEFAULT);
		step *= -1;
	}

	for (u32 i = 0; i < Item.size(); i++)
	{
		bool visible = (i >= scrollIndex && i < scrollIndex+MaxVisible);
		int state = visible ? STATE_DEFAULT : STATE_DISABLED;

		Item[i].Button->SetVisible(visible);
		Item[i].Button->SetPosition(Item[i].Button->GetLeft()-GetLeft(), Item[i].Button->GetTop()-GetTop()+step);
		Item[i].Button->SetState(state);

		if (Item[i].IconImg)
		{
			Item[i].IconImg->SetVisible(visible);
			Item[i].IconImg->SetPosition(Item[i].IconImg->GetLeft()-GetLeft(), Item[i].IconImg->GetTop()-GetTop()+step);
		}

		if (Item[i].ExpandImg)
		{
			Item[i].ExpandImg->SetVisible(visible);
			Item[i].ExpandImg->SetPosition(Item[i].ExpandImg->GetLeft()-GetLeft(), Item[i].ExpandImg->GetTop()-GetTop()+step);
		}
	}

	if (scrollIndex >= Item.size()-MaxVisible)
	{
		ScrollDownBtn->SetVisible(false);
		ScrollDownBtn->SetState(STATE_DISABLED);
	}

	if (scrollIndex < 1)
	{
		ScrollUpBtn->SetVisible(false);
		ScrollUpBtn->SetState(STATE_DISABLED);
	}
}
