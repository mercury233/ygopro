#pragma once
#include <map>
#include <vector>
#include <string>
#include <common.h>
#include <memory>
#include <boost/asio.hpp>
#include <thread>
#include <atomic>
#include <mutex>
#include "deck_con.h"
#include "duelclient.h"
#include "PlatformString.h"

class Connector;

enum class RegisterResult : byte;
enum class LoginFailedReason : byte;


#define LOBBY_DEFAULT_PORT 7910

#if __ANDROID__
inline void* memcpy_s(void* dest,const int destlen, const void* src,int len)
{
	return memcpy(dest, src, len);
}
inline void* wmemcpy_s(wchar_t* dest, const  int destlen, const  wchar_t* src, int len)
{
	return wmemcpy(dest, src, len);
}
inline wchar_t * wcscpy_s(wchar_t* dest, const int destlen, const wchar_t* src)
{
	return wcscpy(dest, src);
}
inline wchar_t * wcscpy_s(wchar_t* dest, const  wchar_t* src)
{
	return wcscpy(dest, src);
}

#endif

namespace ygo
{
	struct Deck;
}

class LoginWindowEventReceiver : public irr::IEventReceiver
{
	irr::gui::IGUIWindow* loginWindow;
	virtual bool OnEvent(const SEvent& event);

	bool CheckLoginInputUI(const std::wstring& username, const std::wstring& password);

public:
	LoginWindowEventReceiver(irr::gui::IGUIWindow* loginWindow);
	std::function<void()> OnClickRegisterButton;
	std::function<void()> OnClose;

	bool SkipMainMenu = false;
	bool AutoLogin = false;
};

class RegisterWindowEventReceiver : public irr::IEventReceiver
{
	irr::gui::IGUIWindow* registerWindow;
	RegisterResult registerResult;
	virtual bool OnEvent(const SEvent& event);
public:
	RegisterWindowEventReceiver(irr::gui::IGUIWindow* registerWindow);
	std::function<void()> OnClickCancelButton;
};

class UserNormalEventHandler : public irr::IEventReceiver
{
	bool prevEnableSoundEffect = true;
public:
	UserNormalEventHandler();

	virtual bool OnEvent(const SEvent& event) override;

};


class User
{
	User();
	std::shared_ptr<Connector> _connector;
	std::shared_ptr<boost::asio::io_service::work> ios_work;
	boost::asio::io_service ios;
	std::thread thd;
	int points = 0;
	std::function<void(RegisterResult)> OnRegisterResult;
	std::function<void(const std::vector<ygo::HostPacket>)> roomListResultCallBack;
	std::function<void(const std::vector<ygo::HostPacket_V2>)> roomListResultCallBack_V2;
	std::function<void(LoginFailedReason)> OnLoginFailed;
	std::function<void()> OnLoginSuccess;
	
	byte token[16];

	
	PlatformString Nickname;
	std::map<PlatformString, std::vector <char>> userCardDesksBinary;


	std::string ipStr;
	int port = 0;
	int roomport = 0;

	irr::gui::IGUIWindow* infoWindow;
	irr::gui::IGUIStaticText* pAccountStaticText;
	irr::gui::IGUIStaticText* pNickNameStaticText;
	irr::gui::IGUIStaticText* pMeanStaticText;
	irr::gui::IGUIStaticText* pRankStaticText;
	irr::gui::IGUIStaticText* pWinCountStaticText;
	irr::gui::IGUIStaticText* pLoseCountStaticText;
	irr::gui::IGUIStaticText* pDPStaticText;
	irr::gui::IGUIStaticText* pOnlineUserCountStaticText;

	double mean = 0;
	double rank = 0;
	uint32 win_count = 0;
	uint32 lose_count = 0;

	uint32 onlineUserCount = 0;

	
	irr::gui::IGUIWindow* registerWindow;
	

	std::shared_ptr<LoginWindowEventReceiver> loginWindowEventReceiver;
	std::shared_ptr<RegisterWindowEventReceiver> registerWindowEventReceiver;

	
	struct HeartBeat 
	{
		std::mutex _mutex;
		std::atomic_bool _started = { false };
		std::uint32_t _heartbeatInterval = 0;
		std::chrono::time_point<std::chrono::system_clock> _lastSentHeartbeatTime;
		std::chrono::time_point<std::chrono::system_clock> _lastRecvTime;
	};
	HeartBeat _heartBeat;
	
	std::atomic<bool> ShowingDisconnectedWindow = { false };

	std::thread _heartBeatThread;

public:
	UserNormalEventHandler normalEventHander;
	PlatformString Username;
	PlatformString Password;
	irr::gui::IGUIWindow* loginWindow;

	void Init();
	void Destroy();

	void InitGUI(irr::gui::IGUIEnvironment* env);
	void InitInfoWindow(irr::gui::IGUIEnvironment* env);
	void InitSkin(irr::gui::IGUIEnvironment* env);
	void RefreshInfoWindow();
	std::tuple<unsigned int, byte*> GetToken() const;

	void SetToken(byte t[16]);

	const std::map<PlatformString, std::vector <char>> GetUserCardDesksBinary() const;

	static User* Instance()
	{
		static User user;
		return &user;
	}

	int GetPoints();

	void UpdateCardDeck(PlatformString deckname, ygo::Deck& deck);


	void Connect(std::function<void()> successCallback, std::function<void()> failedCallback);

	void SetConnectAddress(std::string ipStr, int port, int roomport);

	void Login(PlatformString username, PlatformString password, std::function<void()> successCallback, std::function<void(LoginFailedReason)> failedCallback);

	void Disconnect();

	void HandleReceiveLobbyMessage(int msgType, std::shared_ptr<std::vector<char>> data);

	bool LoadDeck(ygo::Deck& deck, PlatformString name);

	void QueryRoomList(ygo::HostRequest req, std::function<void(const std::vector<ygo::HostPacket>)> resultCallback);

	void QueryRoomList_V2(ygo::HostRequest req, std::function<void(const std::vector<ygo::HostPacket_V2>)> resultCallback);

	unsigned long GetServerIPHostOrder();

	void DeleteDeck(const PlatformString name);

	void Register(const PlatformString& username, const PlatformString& password, const PlatformString& nickname, const PlatformString& email, std::function<void(RegisterResult)> resultCallback);

	void SetInfoWindowVisible(bool value);

	void HandleReadError();

	void SendChatMsg(PlatformString msg);

	void SetSkinWindowUnTrans();

	void SetSkinColor();

	int GetRoomPort();

	void ShowInfoWindow(bool val);

	void HideLoginWindow();

};