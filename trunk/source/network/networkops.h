#ifndef _NETWORKOPS_H_
#define _NETWORKOPS_H_

void Initialize_Network(void);
bool IsNetworkInit(void);
bool IsNetworkInitiating(void);
bool ConnectSMBShare();
void CloseSMBShare();
char * GetNetworkIP(void);
bool ShutdownWC24();
s32 network_request(const char * request);
s32 network_read(u8 *buf, u32 len);
s32 download_request(const char * url);
void CloseConnection();
int CheckUpdate();

void HaltNetworkThread();
void ResumeNetworkThread();
void InitNetworkThread();
void ShutdownNetworkThread();

#endif