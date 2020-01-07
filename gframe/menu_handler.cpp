#include "config.h"
#include "menu_handler.h"
#include "netserver.h"
#include "duelclient.h"
#include "deck_manager.h"
#include "replay_mode.h"
#include "single_mode.h"
#include "image_manager.h"
#include "sound_manager.h"
#include "game.h"

namespace ygo {

void UpdateDeck() {
	BufferIO::CopyWStr(mainGame->cbDeckSelect->getItem(mainGame->cbDeckSelect->getSelected()),
		mainGame->gameConf.lastdeck, 64);
	char deckbuf[1024];
	char* pdeck = deckbuf;
	BufferIO::WriteInt32(pdeck, deckManager.current_deck.main.size() + deckManager.current_deck.extra.size());
	BufferIO::WriteInt32(pdeck, deckManager.current_deck.side.size());
	for(size_t i = 0; i < deckManager.current_deck.main.size(); ++i)
		BufferIO::WriteInt32(pdeck, deckManager.current_deck.main[i]->first);
	for(size_t i = 0; i < deckManager.current_deck.extra.size(); ++i)
		BufferIO::WriteInt32(pdeck, deckManager.current_deck.extra[i]->first);
	for(size_t i = 0; i < deckManager.current_deck.side.size(); ++i)
		BufferIO::WriteInt32(pdeck, deckManager.current_deck.side[i]->first);
	DuelClient::SendBufferToServer(CTOS_UPDATE_DECK, deckbuf, pdeck - deckbuf);
}
bool MenuHandler::OnEvent(const irr::SEvent& event) {
	if(mainGame->dField.OnCommonEvent(event))
		return false;
	switch(event.EventType) {
	case irr::EET_GUI_EVENT: {
		irr::gui::IGUIElement* caller = event.GUIEvent.Caller;
		s32 id = caller->getID();
		if(mainGame->wQuery->isVisible() && id != BUTTON_YES && id != BUTTON_NO) {
			mainGame->wQuery->getParent()->bringToFront(mainGame->wQuery);
			break;
		}
		if(mainGame->wReplaySave->isVisible() && id != BUTTON_REPLAY_SAVE && id != BUTTON_REPLAY_CANCEL) {
			mainGame->wReplaySave->getParent()->bringToFront(mainGame->wReplaySave);
			break;
		}
		switch(event.GUIEvent.EventType) {
		case irr::gui::EGET_ELEMENT_HOVERED: {
			if(event.GUIEvent.Caller->getType() == EGUIET_EDIT_BOX)
				mainGame->SetCursor(event.GUIEvent.Caller->isEnabled() ? ECI_IBEAM : ECI_NORMAL);
			break;
		}
		case irr::gui::EGET_ELEMENT_LEFT: {
			if(event.GUIEvent.Caller->getType() == EGUIET_EDIT_BOX)
				mainGame->SetCursor(ECI_NORMAL);
			break;
		}
		case irr::gui::EGET_BUTTON_CLICKED: {
			if(id < 110)
				soundManager.PlaySoundEffect(SOUND_MENU);
			else
				soundManager.PlaySoundEffect(SOUND_BUTTON);
			switch(id) {
			case BUTTON_MODE_EXIT: {
				mainGame->device->closeDevice();
				break;
			}
			case BUTTON_OTHER: {
				mainGame->HideElement(mainGame->wMainMenu);
				mainGame->ShowElement(mainGame->wOther);
				mainGame->btnYST->setEnabled(true);
				mainGame->btnKANabell->setEnabled(true);
				mainGame->btnOCNK->setEnabled(true);
				mainGame->btnLantern->setEnabled(true);
				mainGame->btnOtherExit->setEnabled(true);
				break;
			}
			case BUTTON_YST: {
                system("start https://yugioh-list.com/");
				return true;
				break;
			}
			case BUTTON_KANABELL: {
                system("start https://www.ka-nabell.com");
				return true;
				break;
			}
			case BUTTON_OCNK: {
                system("start http://netsaru2012.ocnk.net/");
				return true;
				break;
			}
			case BUTTON_LANTERN: {
                system("start https://github.com/getlantern/download");
				return true;
				break;
			}
			case BUTTON_OTHER_EXIT: {
				mainGame->HideElement(mainGame->wOther);
				mainGame->ShowElement(mainGame->wMainMenu);
				if(exit_on_return)
					mainGame->device->closeDevice();
				break;
			}
			case BUTTON_SYS: {
				mainGame->chkSkin->setEnabled(true);
				mainGame->chkD3D->setEnabled(true);
				mainGame->chkAutoSearch->setEnabled(true);
				mainGame->chkMultiKeywords->setEnabled(true);
				mainGame->chkRegex->setEnabled(true);
				mainGame->cbFont->setEnabled(true);
				mainGame->btnHeadS->setEnabled(true);
				mainGame->btnCoverS->setEnabled(true);
				mainGame->btnBgS->setEnabled(true);
				mainGame->btnSystemExit->setEnabled(true);
				mainGame->HideElement(mainGame->wMainMenu);
				mainGame->ShowElement(mainGame->wSystem);
				break;
			}
			case BUTTON_SYS_EXIT: {
				mainGame->HideElement(mainGame->wSystem);
				mainGame->ShowElement(mainGame->wMainMenu);
				if(exit_on_return)
					mainGame->device->closeDevice();
				break;
			}
			case BUTTON_HDS: {
				mainGame->cbHDS->setEnabled(true);
				mainGame->btnHDSOK->setEnabled(true);
				mainGame->btnHDSExit->setEnabled(true);
				mainGame->cbHDS->setSelected(0);
				mainGame->HideElement(mainGame->wSystem);
				mainGame->ShowElement(mainGame->wHDS);
				break;
			}
			case BUTTON_HDS_EXIT: {
				mainGame->HideElement(mainGame->wHDS);
				mainGame->ShowElement(mainGame->wSystem);
				if(exit_on_return)
					mainGame->device->closeDevice();
				break;
			}
			case BUTTON_CRS: {
				mainGame->cbCRS->setEnabled(true);
				mainGame->btnCoverOK1->setEnabled(true);
				mainGame->btnCoverOK2->setEnabled(true);
				mainGame->btnCoverOK3->setEnabled(true);
				mainGame->btnCoverOK4->setEnabled(true);
				mainGame->btnCRSExit->setEnabled(true);
				mainGame->cbCRS->setSelected(0);
				mainGame->HideElement(mainGame->wSystem);
				mainGame->ShowElement(mainGame->wCRS);
				break;
			}
			case BUTTON_CRS_EXIT: {
				mainGame->HideElement(mainGame->wCRS);
				mainGame->ShowElement(mainGame->wSystem);
				if(exit_on_return)
					mainGame->device->closeDevice();
				break;
			}
			case BUTTON_BGS: {
				mainGame->cbBGS->setEnabled(true);
				mainGame->btnBGOK1->setEnabled(true);
				mainGame->btnBGOK2->setEnabled(true);
				mainGame->btnBGOK3->setEnabled(true);
				mainGame->btnBGSExit->setEnabled(true);
				mainGame->cbBGS->setSelected(0);
				mainGame->HideElement(mainGame->wSystem);
				mainGame->ShowElement(mainGame->wBGS);
				break;
			}
			case BUTTON_BGS_EXIT: {
				mainGame->HideElement(mainGame->wBGS);
				mainGame->ShowElement(mainGame->wSystem);
				if(exit_on_return)
					mainGame->device->closeDevice();
				break;
			}
			case BUTTON_LAN_MODE: {
				mainGame->btnCreateHost->setEnabled(true);
				mainGame->btnJoinHost->setEnabled(true);
				mainGame->btnJoinCancel->setEnabled(true);
				mainGame->btnSC->setEnabled(true);
				mainGame->btnRM->setEnabled(true);
				mainGame->HideElement(mainGame->wMainMenu);
				mainGame->ShowElement(mainGame->wLanWindow);
				break;
			}
			case BUTTON_SERVER_CHOOSE: {
				mainGame->btn233->setEnabled(true);
				mainGame->btn23333->setEnabled(true);
				mainGame->btn7210->setEnabled(true);
				mainGame->btn222->setEnabled(true);
				mainGame->btnSCExit->setEnabled(true);
				mainGame->ShowElement(mainGame->wSC);
				mainGame->PopupElement(mainGame->wSC);
				break;
			}
			case BUTTON_233: {
				wcscpy(mainGame->gameConf.lasthost, L"s1.ygo233.com");
				wcscpy(mainGame->gameConf.lastport, L"233");
				wchar_t buf[256];
				wchar_t buff[256];
				myswprintf(buf, L"%s", mainGame->gameConf.lasthost);
				mainGame->ebJoinHost->setText(buf);
				myswprintf(buff, L"%s", mainGame->gameConf.lastport);
				mainGame->ebJoinPort->setText(buff);
				mainGame->HideElement(mainGame->wSC);
				break;
			}
			case BUTTON_23333: {
				wcscpy(mainGame->gameConf.lasthost, L"s1.ygo233.com");
				wcscpy(mainGame->gameConf.lastport, L"23333");
				wchar_t buf[256];
				wchar_t buff[256];
				myswprintf(buf, L"%s", mainGame->gameConf.lasthost);
				mainGame->ebJoinHost->setText(buf);
				myswprintf(buff, L"%s", mainGame->gameConf.lastport);
				mainGame->ebJoinPort->setText(buff);
				mainGame->HideElement(mainGame->wSC);
				break;
			}
			case BUTTON_765: {
				wcscpy(mainGame->gameConf.lasthost, L"2pick.mycard.moe");
				wcscpy(mainGame->gameConf.lastport, L"765");
				wchar_t buf[256];
				wchar_t buff[256];
				myswprintf(buf, L"%s", mainGame->gameConf.lasthost);
				mainGame->ebJoinHost->setText(buf);
				myswprintf(buff, L"%s", mainGame->gameConf.lastport);
				mainGame->ebJoinPort->setText(buff);
				mainGame->HideElement(mainGame->wSC);
				break;
			}
			case BUTTON_JP: {
				wcscpy(mainGame->gameConf.lasthost, L"ygosvrjp.tk");
				wcscpy(mainGame->gameConf.lastport, L"7911");
				wchar_t buf[256];
				wchar_t buff[256];
				myswprintf(buf, L"%s", mainGame->gameConf.lasthost);
				mainGame->ebJoinHost->setText(buf);
				myswprintf(buff, L"%s", mainGame->gameConf.lastport);
				mainGame->ebJoinPort->setText(buff);
				mainGame->HideElement(mainGame->wSC);
				break;
			}
			case BUTTON_7210: {
				wcscpy(mainGame->gameConf.lasthost, L"koishi.moecube.com");
				wcscpy(mainGame->gameConf.lastport, L"7210");
				wchar_t buf[256];
				wchar_t buff[256];
				myswprintf(buf, L"%s", mainGame->gameConf.lasthost);
				mainGame->ebJoinHost->setText(buf);
				myswprintf(buff, L"%s", mainGame->gameConf.lastport);
				mainGame->ebJoinPort->setText(buff);
				mainGame->HideElement(mainGame->wSC);
				break;
			}
			case BUTTON_222: {
				wcscpy(mainGame->gameConf.lasthost, L"ayane.mycard.moe");
				wcscpy(mainGame->gameConf.lastport, L"222");
				wchar_t buf[256];
				wchar_t buff[256];
				myswprintf(buf, L"%s", mainGame->gameConf.lasthost);
				mainGame->ebJoinHost->setText(buf);
				myswprintf(buff, L"%s", mainGame->gameConf.lastport);
				mainGame->ebJoinPort->setText(buff);
				mainGame->HideElement(mainGame->wSC);
				break;
			}
			case BUTTON_NK: {
				wcscpy(mainGame->gameConf.lasthost, L"nekoparaserver.com");
				wcscpy(mainGame->gameConf.lastport, L"23331");
				wchar_t buf[256];
				wchar_t buff[256];
				myswprintf(buf, L"%s", mainGame->gameConf.lasthost);
				mainGame->ebJoinHost->setText(buf);
				myswprintf(buff, L"%s", mainGame->gameConf.lastport);
				mainGame->ebJoinPort->setText(buff);
				mainGame->HideElement(mainGame->wSC);
				break;
			}
			case BUTTON_SC_EXIT: {
				mainGame->HideElement(mainGame->wSC);
					if(exit_on_return)
				mainGame->device->closeDevice();
				break;
			}
			case BUTTON_ROOM_CODE: {
				mainGame->btnRM1->setEnabled(true);
				mainGame->btnRM2->setEnabled(true);
				mainGame->btnRM3->setEnabled(true);
				mainGame->btnRM4->setEnabled(true);
				mainGame->btnRM5->setEnabled(true);
				mainGame->btnRM6->setEnabled(true);
				mainGame->btnRM7->setEnabled(true);
				mainGame->btnRM8->setEnabled(true);
				mainGame->btnRM9->setEnabled(true);
				mainGame->btnRM10->setEnabled(true);
				mainGame->btnRM11->setEnabled(true);
				mainGame->btnRM12->setEnabled(true);
				mainGame->btnRM13->setEnabled(true);
				mainGame->btnRM14->setEnabled(true);
				mainGame->btnRMCM->setEnabled(true);
				mainGame->btnRMWN->setEnabled(true);
				mainGame->btnRMAI->setEnabled(true);
				mainGame->btnRMA->setEnabled(true);
				mainGame->btnRMExit->setEnabled(true);
				mainGame->ShowElement(mainGame->wRM);
				mainGame->PopupElement(mainGame->wRM);
				break;
			}
			case BUTTON_RM1: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sM", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM2: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sT", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM3: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sOT", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM4: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sTO", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM5: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sLP4000", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM6: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sTM5", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM7: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sST8", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM8: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sDR2", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM9: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sLF2", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM10: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sNF", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM11: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sNU", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM12: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sNC", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM13: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sNS", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RM14: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sPR", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RMCM: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%s,", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RMWN: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%s#", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RMAI: {
				const wchar_t* pstr = mainGame->ebJoinPass->getText();
				wchar_t buff[1024];
				swprintf(buff, L"%sAI", pstr);
				mainGame->ebJoinPass->setText(buff);
				break;
			}
			case BUTTON_RMA: {
                system("start https://ygo233.com/usage");
				return true;
				break;
			}
			case BUTTON_RM_EXIT: {
				mainGame->HideElement(mainGame->wRM);
					if(exit_on_return)
				mainGame->device->closeDevice();
				break;
			}
			case BUTTON_JOIN_HOST: {
				bot_mode = false;
				char ip[20];
				const wchar_t* pstr = mainGame->ebJoinHost->getText();
				BufferIO::CopyWStr(pstr, ip, 16);
				unsigned int remote_addr = htonl(inet_addr(ip));
				if(remote_addr == -1) {
					char hostname[100];
					char port[6];
					BufferIO::CopyWStr(pstr, hostname, 100);
					BufferIO::CopyWStr(mainGame->ebJoinPort->getText(), port, 6);
					struct evutil_addrinfo hints;
					struct evutil_addrinfo *answer = NULL;
					memset(&hints, 0, sizeof(hints));
					hints.ai_family = AF_INET;
					hints.ai_socktype = SOCK_STREAM;
					hints.ai_protocol = IPPROTO_TCP;
					hints.ai_flags = EVUTIL_AI_ADDRCONFIG;
					int status = evutil_getaddrinfo(hostname, port, &hints, &answer);
					if(status != 0) {
						mainGame->gMutex.lock();
						soundManager.PlaySoundEffect(SOUND_INFO);
						mainGame->env->addMessageBox(L"", dataManager.GetSysString(1412));
						mainGame->gMutex.unlock();
						break;
					} else {
						sockaddr_in * sin = ((struct sockaddr_in *)answer->ai_addr);
						evutil_inet_ntop(AF_INET, &(sin->sin_addr), ip, 20);
						remote_addr = htonl(inet_addr(ip));
					}
				}
				unsigned int remote_port = _wtoi(mainGame->ebJoinPort->getText());
				BufferIO::CopyWStr(pstr, mainGame->gameConf.lasthost, 100);
				BufferIO::CopyWStr(mainGame->ebJoinPort->getText(), mainGame->gameConf.lastport, 20);
				if(DuelClient::StartClient(remote_addr, remote_port, false)) {
					mainGame->btnCreateHost->setEnabled(false);
					mainGame->btnJoinHost->setEnabled(false);
					mainGame->btnJoinCancel->setEnabled(false);
				}
				break;
			}
			case BUTTON_JOIN_CANCEL: {
				mainGame->HideElement(mainGame->wLanWindow);
				mainGame->ShowElement(mainGame->wMainMenu);
				if(exit_on_return)
					mainGame->device->closeDevice();
				break;
			}
			case BUTTON_LAN_REFRESH: {
				DuelClient::BeginRefreshHost();
				break;
			}
			case BUTTON_CREATE_HOST: {
				mainGame->btnHostConfirm->setEnabled(true);
				mainGame->btnHostCancel->setEnabled(true);
				mainGame->HideElement(mainGame->wLanWindow);
				mainGame->ShowElement(mainGame->wCreateHost);
				break;
			}
			case BUTTON_HOST_CONFIRM: {
				bot_mode = false;
				BufferIO::CopyWStr(mainGame->ebServerName->getText(), mainGame->gameConf.gamename, 20);
				if(!NetServer::StartServer(mainGame->gameConf.serverport))
					break;
				if(!DuelClient::StartClient(0x7f000001, mainGame->gameConf.serverport)) {
					NetServer::StopServer();
					break;
				}
				mainGame->btnHostConfirm->setEnabled(false);
				mainGame->btnHostCancel->setEnabled(false);
				break;
			}
			case BUTTON_HOST_CANCEL: {
				mainGame->btnCreateHost->setEnabled(true);
				mainGame->btnJoinHost->setEnabled(true);
				mainGame->btnJoinCancel->setEnabled(true);
				mainGame->HideElement(mainGame->wCreateHost);
				mainGame->ShowElement(mainGame->wLanWindow);
				break;
			}
			case BUTTON_HP_DUELIST: {
				mainGame->cbDeckSelect->setEnabled(true);
				DuelClient::SendPacketToServer(CTOS_HS_TODUELIST);
				break;
			}
			case BUTTON_HP_OBSERVER: {
				DuelClient::SendPacketToServer(CTOS_HS_TOOBSERVER);
				break;
			}
			case BUTTON_HP_KICK: {
				int id = 0;
				while(id < 4) {
					if(mainGame->btnHostPrepKick[id] == caller)
						break;
					id++;
				}
				CTOS_Kick csk;
				csk.pos = id;
				DuelClient::SendPacketToServer(CTOS_HS_KICK, csk);
				break;
			}
			case BUTTON_HP_READY: {
				if(mainGame->cbDeckSelect->getSelected() == -1 ||
					!deckManager.LoadDeck(mainGame->cbDeckSelect->getItem(mainGame->cbDeckSelect->getSelected()))) {
					break;
				}
				UpdateDeck();
				DuelClient::SendPacketToServer(CTOS_HS_READY);
				mainGame->cbDeckSelect->setEnabled(false);
				break;
			}
			case BUTTON_HP_NOTREADY: {
				DuelClient::SendPacketToServer(CTOS_HS_NOTREADY);
				mainGame->cbDeckSelect->setEnabled(true);
				break;
			}
			case BUTTON_HP_START: {
				DuelClient::SendPacketToServer(CTOS_HS_START);
				break;
			}
			case BUTTON_HP_CANCEL: {
				DuelClient::StopClient();
				mainGame->btnCreateHost->setEnabled(true);
				mainGame->btnJoinHost->setEnabled(true);
				mainGame->btnJoinCancel->setEnabled(true);
				mainGame->btnStartBot->setEnabled(true);
				mainGame->btnBotCancel->setEnabled(true);
				mainGame->HideElement(mainGame->wHostPrepare);
				if(bot_mode)
					mainGame->ShowElement(mainGame->wSinglePlay);
				else
					mainGame->ShowElement(mainGame->wLanWindow);
				mainGame->wChat->setVisible(false);
				if(exit_on_return)
					mainGame->device->closeDevice();
				break;
			}
			case BUTTON_REPLAY_MODE: {
				mainGame->HideElement(mainGame->wMainMenu);
				mainGame->ShowElement(mainGame->wReplay);
				mainGame->ebRepStartTurn->setText(L"1");
				mainGame->stReplayInfo->setText(L"");
				mainGame->RefreshReplay();
				break;
			}
			case BUTTON_SINGLE_MODE: {
				mainGame->HideElement(mainGame->wMainMenu);
				mainGame->ShowElement(mainGame->wSinglePlay);
				mainGame->RefreshSingleplay();
				mainGame->RefreshBot();
				break;
			}
			case BUTTON_LOAD_REPLAY: {
				if(open_file) {
					ReplayMode::cur_replay.OpenReplay(open_file_name);
					open_file = false;
				} else {
					if(mainGame->lstReplayList->getSelected() == -1)
						break;
					if(!ReplayMode::cur_replay.OpenReplay(mainGame->lstReplayList->getListItem(mainGame->lstReplayList->getSelected())))
						break;
				}
				mainGame->ClearCardInfo();
				mainGame->wCardImg->setVisible(true);
				mainGame->wInfos->setVisible(true);
				mainGame->wReplay->setVisible(true);
				mainGame->wReplayControl->setVisible(true);
				mainGame->btnReplayStart->setVisible(false);
				mainGame->btnReplayPause->setVisible(true);
				mainGame->btnReplayStep->setVisible(false);
				mainGame->btnReplayUndo->setVisible(false);
				mainGame->wPhase->setVisible(true);
				mainGame->dField.Clear();
				mainGame->HideElement(mainGame->wReplay);
				mainGame->device->setEventReceiver(&mainGame->dField);
				unsigned int start_turn = _wtoi(mainGame->ebRepStartTurn->getText());
				if(start_turn == 1)
					start_turn = 0;
				ReplayMode::StartReplay(start_turn);
				break;
			}
			case BUTTON_DELETE_REPLAY: {
				int sel = mainGame->lstReplayList->getSelected();
				if(sel == -1)
					break;
				mainGame->gMutex.lock();
				wchar_t textBuffer[256];
				myswprintf(textBuffer, L"%ls\n%ls", mainGame->lstReplayList->getListItem(sel), dataManager.GetSysString(1363));
				mainGame->SetStaticText(mainGame->stQMessage, 310, mainGame->guiFont, textBuffer);
				mainGame->PopupElement(mainGame->wQuery);
				mainGame->gMutex.unlock();
				prev_operation = id;
				prev_sel = sel;
				break;
			}
			case BUTTON_RENAME_REPLAY: {
				int sel = mainGame->lstReplayList->getSelected();
				if(sel == -1)
					break;
				mainGame->gMutex.lock();
				mainGame->wReplaySave->setText(dataManager.GetSysString(1364));
				mainGame->ebRSName->setText(mainGame->lstReplayList->getListItem(sel));
				mainGame->PopupElement(mainGame->wReplaySave);
				mainGame->gMutex.unlock();
				prev_operation = id;
				prev_sel = sel;
				break;
			}
			case BUTTON_CANCEL_REPLAY: {
				mainGame->HideElement(mainGame->wReplay);
				mainGame->ShowElement(mainGame->wMainMenu);
				break;
			}
			case BUTTON_EXPORT_DECK: {
				if(mainGame->lstReplayList->getSelected() == -1)
					break;
				Replay replay;
				wchar_t ex_filename[256];
				wchar_t namebuf[4][20];
				wchar_t filename[256];
				myswprintf(ex_filename, L"%ls", mainGame->lstReplayList->getListItem(mainGame->lstReplayList->getSelected()));
				if(!replay.OpenReplay(ex_filename))
					break;
				const ReplayHeader& rh = replay.pheader;
				if(rh.flag & REPLAY_SINGLE_MODE)
					break;
				int max = (rh.flag & REPLAY_TAG) ? 4 : 2;
				//player name
				for(int i = 0; i < max; ++i)
					replay.ReadName(namebuf[i]);
				//skip pre infos
				for(int i = 0; i < 4; ++i)
					replay.ReadInt32();
				//deck
				for(int i = 0; i < max; ++i) {
					int main = replay.ReadInt32();
					Deck tmp_deck;
					for(int j = 0; j < main; ++j)
						tmp_deck.main.push_back(dataManager.GetCodePointer(replay.ReadInt32()));
					int extra = replay.ReadInt32();
					for(int j = 0; j < extra; ++j)
						tmp_deck.extra.push_back(dataManager.GetCodePointer(replay.ReadInt32()));
					myswprintf(filename, L"%ls %ls", ex_filename, namebuf[i]);
					deckManager.SaveDeck(tmp_deck, filename);
				}
				mainGame->stACMessage->setText(dataManager.GetSysString(1335));
				mainGame->PopupElement(mainGame->wACMessage, 20);
				break;
			}
			case BUTTON_BOT_START: {
				int sel = mainGame->lstBotList->getSelected();
				if(sel == -1)
					break;
				bot_mode = true;
#ifdef _WIN32
				if(!NetServer::StartServer(mainGame->gameConf.serverport))
					break;
				if(!DuelClient::StartClient(0x7f000001, mainGame->gameConf.serverport)) {
					NetServer::StopServer();
					break;
				}
				STARTUPINFOW si;
				PROCESS_INFORMATION pi;
				ZeroMemory(&si, sizeof(si));
				si.cb = sizeof(si);
				ZeroMemory(&pi, sizeof(pi));
				wchar_t cmd[MAX_PATH];
				int flag = 0;
				flag += (mainGame->chkBotHand->isChecked() ? 0x1 : 0);
				myswprintf(cmd, L"Bot.exe \"%ls\" %d %d", mainGame->botInfo[sel].command, flag, mainGame->gameConf.serverport);
				if(!CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
				{
					NetServer::StopServer();
					break;
				}
#else
				if(fork() == 0) {
					usleep(100000);
					char arg1[512];
					BufferIO::EncodeUTF8(mainGame->botInfo[sel].command, arg1);
					int flag = 0;
					flag += (mainGame->chkBotHand->isChecked() ? 0x1 : 0);
					char arg2[8];
					sprintf(arg2, "%d", flag);
					char arg3[8];
					sprintf(arg3, "%d", mainGame->gameConf.serverport);
					execl("./bot", "bot", arg1, arg2, arg3, NULL);
					exit(0);
				} else {
					if(!NetServer::StartServer(mainGame->gameConf.serverport))
						break;
					if(!DuelClient::StartClient(0x7f000001, mainGame->gameConf.serverport)) {
						NetServer::StopServer();
						break;
					}
				}
#endif
				mainGame->btnStartBot->setEnabled(false);
				mainGame->btnBotCancel->setEnabled(false);
				break;
			}
			case BUTTON_LOAD_SINGLEPLAY: {
				if(!open_file && mainGame->lstSinglePlayList->getSelected() == -1)
					break;
				mainGame->singleSignal.SetNoWait(false);
				SingleMode::StartPlay();
				break;
			}
			case BUTTON_CANCEL_SINGLEPLAY: {
				mainGame->HideElement(mainGame->wSinglePlay);
				mainGame->ShowElement(mainGame->wMainMenu);
				break;
			}
			case BUTTON_DECK_EDIT: {
				mainGame->RefreshDeck(mainGame->cbDBDecks);
				if(open_file && deckManager.LoadDeck(open_file_name)) {
#ifdef WIN32
					wchar_t *dash = wcsrchr(open_file_name, L'\\');
#else
					wchar_t *dash = wcsrchr(open_file_name, L'/');
#endif
					wchar_t *dot = wcsrchr(open_file_name, L'.');
					if(dash && dot) {
						wchar_t deck_name[256];
						wcsncpy(deck_name, dash + 1, dot - dash - 1);
						deck_name[dot - dash - 1] = L'\0';
						mainGame->ebDeckname->setText(deck_name);
						mainGame->cbDBDecks->setSelected(-1);
					} else {
						for(size_t i = 0; i < mainGame->cbDBDecks->getItemCount(); ++i) {
							if(!wcscmp(mainGame->cbDBDecks->getItem(i), open_file_name)) {
								mainGame->cbDBDecks->setSelected(i);
								break;
							}
						}
					}
					open_file = false;
				} else if(mainGame->cbDBDecks->getSelected() != -1) {
					deckManager.LoadDeck(mainGame->cbDBDecks->getItem(mainGame->cbDBDecks->getSelected()));
					mainGame->ebDeckname->setText(L"");
				}
				mainGame->HideElement(mainGame->wMainMenu);
				mainGame->deckBuilder.Initialize();
				break;
			}
			case BUTTON_YES: {
				mainGame->HideElement(mainGame->wQuery);
				if(prev_operation == BUTTON_DELETE_REPLAY) {
					if(Replay::DeleteReplay(mainGame->lstReplayList->getListItem(prev_sel))) {
						mainGame->stReplayInfo->setText(L"");
						mainGame->lstReplayList->removeItem(prev_sel);
					}
				}
				prev_operation = 0;
				prev_sel = -1;
				break;
			}
			case BUTTON_NO: {
				mainGame->HideElement(mainGame->wQuery);
				prev_operation = 0;
				prev_sel = -1;
				break;
			}
			case BUTTON_REPLAY_SAVE: {
				mainGame->HideElement(mainGame->wReplaySave);
				if(prev_operation == BUTTON_RENAME_REPLAY) {
					wchar_t newname[256];
					BufferIO::CopyWStr(mainGame->ebRSName->getText(), newname, 256);
					if(mywcsncasecmp(newname + wcslen(newname) - 4, L".yrp", 4)) {
						myswprintf(newname, L"%ls.yrp", mainGame->ebRSName->getText());
					}
					if(Replay::RenameReplay(mainGame->lstReplayList->getListItem(prev_sel), newname)) {
						mainGame->lstReplayList->setItem(prev_sel, newname, -1);
					} else {
						mainGame->env->addMessageBox(L"", dataManager.GetSysString(1365));
					}
				}
				prev_operation = 0;
				prev_sel = -1;
				break;
			}
			case BUTTON_REPLAY_CANCEL: {
				mainGame->HideElement(mainGame->wReplaySave);
				prev_operation = 0;
				prev_sel = -1;
				break;
			}
			}
			break;
		}
		case irr::gui::EGET_LISTBOX_CHANGED: {
			switch(id) {
			case LISTBOX_LAN_HOST: {
				int sel = mainGame->lstHostList->getSelected();
				if(sel == -1)
					break;
				if(DuelClient::is_srvpro) {
					mainGame->ebJoinPass->setText(DuelClient::hosts_srvpro[sel].c_str());
					break;
				}
				int addr = DuelClient::hosts[sel].ipaddr;
				int port = DuelClient::hosts[sel].port;
				wchar_t buf[20];
				myswprintf(buf, L"%d.%d.%d.%d", addr & 0xff, (addr >> 8) & 0xff, (addr >> 16) & 0xff, (addr >> 24) & 0xff);
				mainGame->ebJoinHost->setText(buf);
				myswprintf(buf, L"%d", port);
				mainGame->ebJoinPort->setText(buf);
				break;
			}
			case LISTBOX_REPLAY_LIST: {
				int sel = mainGame->lstReplayList->getSelected();
				if(sel == -1)
					break;
				if(!ReplayMode::cur_replay.OpenReplay(mainGame->lstReplayList->getListItem(sel)))
					break;
				wchar_t infobuf[256];
				std::wstring repinfo;
				time_t curtime = ReplayMode::cur_replay.pheader.seed;
				tm* st = localtime(&curtime);
				wcsftime(infobuf, 256, L"%Y/%m/%d %H:%M:%S\n", st);
				repinfo.append(infobuf);
				wchar_t namebuf[4][20];
				ReplayMode::cur_replay.ReadName(namebuf[0]);
				ReplayMode::cur_replay.ReadName(namebuf[1]);
				if(ReplayMode::cur_replay.pheader.flag & REPLAY_TAG) {
					ReplayMode::cur_replay.ReadName(namebuf[2]);
					ReplayMode::cur_replay.ReadName(namebuf[3]);
				}
				if(ReplayMode::cur_replay.pheader.flag & REPLAY_TAG)
					myswprintf(infobuf, L"%ls\n%ls\n===VS===\n%ls\n%ls\n", namebuf[0], namebuf[1], namebuf[2], namebuf[3]);
				else
					myswprintf(infobuf, L"%ls\n===VS===\n%ls\n", namebuf[0], namebuf[1]);
				repinfo.append(infobuf);
				mainGame->ebRepStartTurn->setText(L"1");
				mainGame->SetStaticText(mainGame->stReplayInfo, 180, mainGame->guiFont, repinfo.c_str());
				break;
			}
			case LISTBOX_SINGLEPLAY_LIST: {
				int sel = mainGame->lstSinglePlayList->getSelected();
				if(sel == -1)
					break;
				const wchar_t* name = mainGame->lstSinglePlayList->getListItem(sel);
				wchar_t fname[256];
				myswprintf(fname, L"./single/%ls", name);
				FILE *fp;
#ifdef _WIN32
				fp = _wfopen(fname, L"rb");
#else
				char filename[256];
				BufferIO::EncodeUTF8(fname, filename);
				fp = fopen(filename, "rb");
#endif
				if(!fp) {
					mainGame->stSinglePlayInfo->setText(L"");
					break;
				}
				char linebuf[1024];
				wchar_t wlinebuf[1024];
				std::wstring message = L"";
				bool in_message = false;
				while(fgets(linebuf, 1024, fp)) {
					if(!strncmp(linebuf, "--[[message", 11)) {
						size_t len = strlen(linebuf);
						char* msgend = strrchr(linebuf, ']');
						if(len <= 13) {
							in_message = true;
							continue;
						} else if(len > 15 && msgend) {
							*(msgend - 1) = '\0';
							BufferIO::DecodeUTF8(linebuf + 12, wlinebuf);
							message.append(wlinebuf);
							break;
						}
					}
					if(!strncmp(linebuf, "]]", 2)) {
						in_message = false;
						break;
					}
					if(in_message) {
						BufferIO::DecodeUTF8(linebuf, wlinebuf);
						message.append(wlinebuf);
					}
				}
				fclose(fp);
				mainGame->SetStaticText(mainGame->stSinglePlayInfo, 200, mainGame->guiFont, message.c_str());
				break;
			}
			case LISTBOX_BOT_LIST: {
				int sel = mainGame->lstBotList->getSelected();
				if(sel == -1)
					break;
				mainGame->SetStaticText(mainGame->stBotInfo, 200, mainGame->guiFont, mainGame->botInfo[sel].desc);
				break;
			}
			}
			break;
		}
		case irr::gui::EGET_CHECKBOX_CHANGED: {
			switch(id) {
			case CHECKBOX_HP_READY: {
				if(!caller->isEnabled())
					break;
				mainGame->env->setFocus(mainGame->wHostPrepare);
				if(static_cast<irr::gui::IGUICheckBox*>(caller)->isChecked()) {
					if(mainGame->cbDeckSelect->getSelected() == -1 ||
					        !deckManager.LoadDeck(mainGame->cbDeckSelect->getItem(mainGame->cbDeckSelect->getSelected()))) {
						static_cast<irr::gui::IGUICheckBox*>(caller)->setChecked(false);
						break;
					}
					UpdateDeck();
					DuelClient::SendPacketToServer(CTOS_HS_READY);
					mainGame->cbDeckSelect->setEnabled(false);
				} else {
					DuelClient::SendPacketToServer(CTOS_HS_NOTREADY);
					mainGame->cbDeckSelect->setEnabled(true);
				}
				break;
			}
			}
			break;
		}
		case irr::gui::EGET_COMBO_BOX_CHANGED: {
			switch(id) {
			case COMBOBOX_BOT_RULE: {
				mainGame->RefreshBot();
				break;
			}
			}
			break;
		}
		case irr::gui::EGET_EDITBOX_ENTER: {
			switch(id) {
			case EDITBOX_CHAT: {
				if(mainGame->dInfo.isReplay)
					break;
				const wchar_t* input = mainGame->ebChatInput->getText();
				if(input[0]) {
					unsigned short msgbuf[256];
					if(mainGame->dInfo.isStarted) {
						if(mainGame->dInfo.player_type < 7) {
							if(mainGame->dInfo.isTag && (mainGame->dInfo.player_type % 2))
								mainGame->AddChatMsg((wchar_t*)input, 2);
							else
								mainGame->AddChatMsg((wchar_t*)input, 0);
						} else
							mainGame->AddChatMsg((wchar_t*)input, 10);
					} else
						mainGame->AddChatMsg((wchar_t*)input, 7);
					int len = BufferIO::CopyWStr(input, msgbuf, 256);
					DuelClient::SendBufferToServer(CTOS_CHAT, msgbuf, (len + 1) * sizeof(short));
					mainGame->ebChatInput->setText(L"");
				}
				break;
			}
			}
			break;
		}
		default: break;
		}
		break;
	}
	case irr::EET_KEY_INPUT_EVENT: {
		switch(event.KeyInput.Key) {
		case irr::KEY_KEY_R: {
			if(!event.KeyInput.PressedDown && !mainGame->HasFocus(EGUIET_EDIT_BOX))
				mainGame->textFont->setTransparency(true);
			break;
		}
		case irr::KEY_ESCAPE: {
			if(!mainGame->HasFocus(EGUIET_EDIT_BOX))
				mainGame->device->minimizeWindow();
			break;
		}
		default: break;
		}
		break;
	}
	default: break;
	}
	return false;
}

}
