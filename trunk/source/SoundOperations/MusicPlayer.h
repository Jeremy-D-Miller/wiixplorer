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
#ifndef _MUSICPLAYER_H_
#define _MUSICPLAYER_H_

#include <vector>
#include <string>
#include "libwiigui/gui.h"
#include "PlayList.hpp"

enum
{
    ONCE = 0,
    LOOP,
    RANDOM_MUSIC,
    PLAYLIST_LOOP,
    MAX_LOOP_MODES
};

class MusicPlayer : public GuiWindow
{
    public:
		static MusicPlayer * Instance();
		static void DestroyInstance();

        bool Load(const char *path, bool silent = true);
        bool LoadStandard();
        bool ParsePath(const char * filepath) { return TitleList.ParsePath(filepath); };
        void Resume();
        bool Play();
        bool Play(int pos);
        bool PlayNext();
        bool PlayPrevious();
        bool PlayRandom();
        void Pause() { if(IsStopped()) return; MainSound->Pause(); Paused = true; };
        void Stop();
        void SetLoop(u8 mode) { LoopMode = mode; MainSound->SetLoop(LoopMode); };
        void SetVolume(int volume) { MainSound->SetVolume(volume); };
        void Show();
        void Hide();
        void AddEntrie(const char * filename) { TitleList.AddEntrie(filename); };
        void ClearList() { LoadStandard(); Stop(); TitleList.ClearList(); };
        bool IsStopped() { return Stopped; };
        void ResumeThread();
        void HaltThread();
        void Update(GuiTrigger * t);
    protected:
        MusicPlayer();
        ~MusicPlayer();
        void Setup();
		static void * UpdateBMG(void *arg);
        void UpdateState();
        void OnButtonClick(GuiElement *sender, int pointer, POINT p);

		lwp_t bgmthread;
		bool Paused;
		bool Stopped;
		bool DisplayGUI;
		bool ExitRequested;

		GuiSound * MainSound;
        u8 LoopMode;

		static MusicPlayer *instance;
        int currentPlaying;
        int CircleImageDelay;
        PlayList TitleList;

        GuiSound * btnSoundOver;
        GuiImageData * playerImgData;
        GuiImageData * navi_defaultImgData;
        GuiImageData * navi_upImgData;
        GuiImageData * navi_downImgData;
        GuiImageData * navi_leftImgData;
        GuiImageData * navi_rightImgData;

        GuiImage * BackgroundImg;
        GuiImage * CircleImg;

        GuiButton * BackButton;
        GuiButton * PlayBtn;
        GuiButton * StopBtn;
        GuiButton * NextBtn;
        GuiButton * PreviousBtn;

        std::string Title;
        GuiText * PlayTitle;

        SimpleGuiTrigger * trigA;
        GuiTrigger * trigB;
};

#endif
