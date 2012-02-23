/****************************************************************************
 * Copyright (C) 2009-2011 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "Application.h"
#include "Menus/Explorer.h"
#include "SoundOperations/MusicPlayer.h"
#include "Memory/Resources.h"
#include "Controls/Taskbar.h"
#include "network/networkops.h"
#include "FTPOperations/FTPServer.h"
#include "Prompts/ProgressWindow.h"
#include "Settings.h"
#include "VideoOperations/video.h"
#include "input.h"
#include "sys.h"

Application *Application::instance = NULL;
bool Application::exitApplication = false;

Application::Application()
	: GuiFrame(screenwidth, screenheight) // screenwidth and height are defined in Video.h
{
	DeleteQueue = NULL;
	LWP_MutexInit(&m_mutex, true);

	//!FTP Server thread
	if(Settings.FTPServer.AutoStart)
		FTPServer::Instance();

	//!Initialize network thread if selected
	if(Settings.AutoConnect)
	{
		InitNetworkThread();
		ResumeNetworkThread();
	}

	//! Setup the music player
	MusicPlayer::Instance()->SetVolume(Settings.MusicVolume);
	MusicPlayer::Instance()->SetLoop(Settings.BGMLoopMode);
	MusicPlayer::Instance()->SetAlignment(ALIGN_RIGHT | ALIGN_TOP);
	MusicPlayer::Instance()->SetPosition(30, 230);

	GXColor ImgColor[4];
	ImgColor[0] = RGBATOGXCOLOR(Settings.BackgroundUL);
	ImgColor[1] = RGBATOGXCOLOR(Settings.BackgroundUR);
	ImgColor[2] = RGBATOGXCOLOR(Settings.BackgroundBR);
	ImgColor[3] = RGBATOGXCOLOR(Settings.BackgroundBL);

	bgImg = new GuiImage(screenwidth, screenheight, &ImgColor[0]);

	//! Setup WiiMote Pointers
	standardPointer[0] = Resources::GetImageData("player1_point.png");
	standardPointer[1] = Resources::GetImageData("player2_point.png");
	standardPointer[2] = Resources::GetImageData("player3_point.png");
	standardPointer[3] = Resources::GetImageData("player4_point.png");

	for (int i = 0; i < 4; i++)
	{
		grabPointer[i] = NULL;
		pointer[i] = standardPointer[i];
	}
}

Application::~Application()
{
	// Fade out...
	for(int i = 0; i <= 255; i += 15)
	{
		Draw();
		GXColor fadeoutColor = (GXColor){0, 0, 0, i};
		Menu_DrawRectangle(0, 0, 100.0f, screenwidth, screenheight, &fadeoutColor, false, true);
		Menu_Render();
	}

	RemoveAll();
	delete bgImg;

	for (int i = 0; i < 4; i++)
	{
		if(grabPointer[i])
			Resources::Remove(grabPointer[i]);
		Resources::Remove(standardPointer[i]);
	}

	LWP_MutexDestroy(m_mutex);
}

void Application::quit()
{
	exitApplication = true;
}

void Application::hide()
{
	RemoveAll();
}

void Application::show()
{
	Append(bgImg);
	Append(MusicPlayer::Instance());
	Append(ProgressWindow::Instance());
	//! Append taskbar instance
	Append(Taskbar::Instance());
}

void Application::exec()
{
	while(!exitApplication)
	{
		updateEvents();

		if(DeleteQueue)
		{
			LWP_MutexLock(m_mutex);
			while(DeleteQueue)
			{
				ElementList *tmp = DeleteQueue->next;
				delete DeleteQueue->element;
				delete DeleteQueue;
				DeleteQueue = tmp;
			}
			LWP_MutexUnlock(m_mutex);
		}
	}

	ExitApp();
}

void Application::updateEvents()
{
	if(shutdown) {
		exitApplication = true;
		Sys_Shutdown();
	}
	else if(reset) {
		exitApplication = true;
		Sys_Reboot();
	}

	UpdatePads();
	Draw();

	for (int i = 3; i >= 0; i--)
	{
		if (userInput[i].wpad->ir.valid)
		{
			Menu_DrawImg(pointer[i]->GetImage(), pointer[i]->GetWidth(),
						 pointer[i]->GetHeight(), GX_TF_RGBA8,
						 userInput[i].wpad->ir.x-pointer[i]->GetWidth()/2,
						 userInput[i].wpad->ir.y-pointer[i]->GetHeight()/2, 100.0f,
						 userInput[i].wpad->ir.angle, 1, 1, 255, -100, screenwidth+100, -100, screenheight+100);
		}
	}

	Menu_Render();

	for (int i = 0; i < 4; i++)
	{
		if(!updateOnlyElement.empty())
			updateOnlyElement.back()->Update(&userInput[i]);
		else
			Update(&userInput[i]);
	}
}

void Application::SetGrabPointer(int i)
{
	if (i < 0 || i > 3)
		return;

	if(!grabPointer[i])
	{
		if(i == 0)
			grabPointer[i] = Resources::GetImageData("player1_grab.png");
		else if(i == 1)
			grabPointer[i] = Resources::GetImageData("player2_grab.png");
		else if(i == 2)
			grabPointer[i] = Resources::GetImageData("player3_grab.png");
		else
			grabPointer[i] = Resources::GetImageData("player4_grab.png");
	}

	pointer[i] = grabPointer[i];
}

void Application::ResetPointer(int i)
{
	if (i < 0 || i > 3)
		return;

	pointer[i] = standardPointer[i];

	if(grabPointer[i])
		Resources::Remove(grabPointer[i]);
	grabPointer[i] = NULL;
}

void Application::PushForDelete(GuiElement *e)
{
	if(!e)
		return;

	LWP_MutexLock(m_mutex);

	if(!DeleteQueue)
	{
		DeleteQueue = new ElementList;
		DeleteQueue->element = e;
		DeleteQueue->next = 0;
		LWP_MutexUnlock(m_mutex);
		return;
	}

	ElementList *list = DeleteQueue;

	while(list->next)
		list = list->next;

	list->next = new ElementList;
	list->next->element = e;
	list->next->next = 0;

	LWP_MutexUnlock(m_mutex);
}
