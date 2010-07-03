/****************************************************************************
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
 * WiiXplorer 2010
 ***************************************************************************/
#include "Task.hpp"
#include "menu.h"

Task::Task(const char * title)
    : GuiButton(100, 44)
{
    Parameter = 0;
    Callback = NULL;
    ButtonImg = NULL;
    ButtonIcon = NULL;
    Title = new GuiText(title, 20, (GXColor) {0, 0, 0, 255});
    Title->SetMaxWidth(80, DOTTED);
    Title->SetAlignment(ALIGN_LEFT, ALIGN_MIDDLE);

	trigA = new SimpleGuiTrigger(-1, WiiControls.ClickButton | ClassicControls.ClickButton << 16, GCControls.ClickButton);

    SetLabel(Title);
    SetImage(ButtonImg);
    SetTrigger(trigA);
    SetEffectGrow();
    Clicked.connect(this, &Task::OnButtonClick);
}

Task::~Task()
{
    delete Title;
    delete trigA;
    if(ButtonImg)
        delete ButtonImg;
    if(ButtonIcon)
        delete ButtonIcon;
}

void Task::Execute(int param)
{
    if(Callback)
        Callback->Execute(param);
}

void Task::SetIcon(GuiImageData * img)
{
    if(ButtonIcon)
        delete ButtonIcon;

    ButtonIcon = new GuiImage(img);
	GuiButton::SetIcon(ButtonIcon);
}

void Task::OnButtonClick(GuiElement * sender, int channel, POINT point)
{
    Execute(Parameter);
    sender->ResetState();
}