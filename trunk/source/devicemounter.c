#include <fat.h>
#include <malloc.h>
#include <unistd.h>
#include <ogc/mutex.h>
#include <ogc/system.h>
#include <sdcard/wiisd_io.h>
#include <ntfs.h>   //has to be after usbstorage.h so our usbstorage.h is loaded and not the libogc one
#include <sdcard/gcsd.h>

#include "devicemounter.h"
#include "libdisk/fst.h"
#include "libdisk/iso.h"
#include "libdisk/di2.h"

//these are the only stable and speed is good
#define CACHE 8
#define SECTORS 64

static ntfs_md *ntfs_mounts = NULL;
static int ntfs_mountCount = 0;

int NTFS_Mount()
{
    ntfs_mountCount = ntfsMountAll(&ntfs_mounts, NTFS_SHOW_HIDDEN_FILES | NTFS_RECOVER);

    return ntfs_mountCount;
}

void NTFS_UnMount()
{
    int i = 0;

    if (ntfs_mounts) {
        for (i = 0; i < ntfs_mountCount; i++)
            ntfsUnmount(ntfs_mounts[i].name, true);
        free(ntfs_mounts);
        ntfs_mounts = NULL;
    }
    ntfs_mountCount = 0;
}

int NTFS_GetMountCount()
{
    return ntfs_mountCount;
}

const char *NTFS_GetMountName(int mountIndex)
{
    if(mountIndex < 0 || mountIndex > ntfs_mountCount)
        return NULL;

    return ntfs_mounts[mountIndex].name;
}

int USBDevice_Init()
{
	//closing all open Files write back the cache and then shutdown em!
	fatUnmount("usb:/");

    if (fatMount("usb", &__io_usbstorage, 0, CACHE, SECTORS))
    {
        return 1;
	}

	return -1;
}

void USBDevice_deInit()
{
	//closing all open Files write back the cache and then shutdown em!
	fatUnmount("usb:/");
	__io_usbstorage.shutdown();
}

bool USBDevice_Inserted()
{
	return __io_usbstorage.isInserted();
}

bool SDCard_Inserted()
{
    return __io_wiisd.isInserted();
}

int SDCard_Init()
{
	//closing all open Files write back the cache and then shutdown em!
	fatUnmount("sd:/");
	//right now mounts first FAT-partition
	if (fatMount("sd", &__io_wiisd, 0, CACHE, SECTORS))
		return 1;
	return -1;
}

void SDCard_deInit()
{
	//closing all open Files write back the cache and then shutdown em!
	fatUnmount("sd:/");
	__io_wiisd.shutdown();
}

int SDGeckoA_Init()
{
	//closing all open Files write back the cache and then shutdown em!
	fatUnmount("gca:/");
	//right now mounts first FAT-partition
	if (fatMount("gca", &__io_gcsda, 0, CACHE, SECTORS))
		return 1;

	return -1;
}

bool SDGeckoA_Inserted()
{
    return __io_gcsda.isInserted();
}

void SDGeckoA_deInit()
{
	//closing all open Files write back the cache and then shutdown em!
	fatUnmount("gca:/");
	__io_gcsda.shutdown();
}

int SDGeckoB_Init()
{
	//closing all open Files write back the cache and then shutdown em!
	fatUnmount("gcb:/");
	//right now mounts first FAT-partition
	if (fatMount("gcb", &__io_gcsdb, 0, CACHE, SECTORS))
		return 1;

	return -1;
}

bool SDGeckoB_Inserted()
{
    return __io_gcsdb.isInserted();
}

void SDGeckoB_deInit()
{
	//closing all open Files write back the cache and then shutdown em!
	fatUnmount("gcb:/");
	__io_gcsdb.shutdown();
}

int DiskDrive_Init(bool have_dvdx)
{
	return DI2_Init(have_dvdx); //Init DVD Driver
}

void DiskDrive_deInit()
{
    FST_Unmount();
    ISO9660_Unmount();
    DI2_Close(); //Deinit DVD Driver
}

bool Disk_Inserted()
{
    uint32_t cover = 0;
    DI2_GetCoverRegister(&cover);

    if(cover & DVD_COVER_DISC_INSERTED)
        return true;

	return false;
}

bool DiskDrive_Mount()
{
    if(DI2_IsMotorRunning())
        return true;

    bool devicemounted = false;

    DiskDrive_UnMount();

    DI2_Mount();
	time_t timer1, timer2;
	timer1 = time(0);

	while(DI2_GetStatus() & DVD_INIT)
	{
		timer2 = time(0);
		if(timer2-timer1 > 15)
            return false;

		usleep(5000);
	}

    devicemounted = ISO9660_Mount();

    if(!devicemounted)
        devicemounted = FST_Mount();

    return devicemounted;
}

void DiskDrive_UnMount()
{
    FST_Unmount();
    ISO9660_Unmount();
}
