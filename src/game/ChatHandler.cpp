/*
 * Copyright (C) 2010-2012 OregonCore <http://www.oregoncore.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Database/DatabaseEnv.h"
#include "Chat.h"
#include "ChannelMgr.h"
#include "Group.h"
#include "Guild.h"
#include "Language.h"
#include "Log.h"
#include "Opcodes.h"
#include "MapManager.h"
#include "Player.h"
#include "SpellAuras.h"
#include "CreatureAI.h"
#include "Util.h"
#include "../irc/IRCClient.h"

bool WorldSession::processChatmessageFurtherAfterSecurityChecks(std::string& msg, uint32 lang)
{
    if (lang != LANG_ADDON)
    {
        // strip invisible characters for non-addon messages
        if (sWorld.getConfig(CONFIG_CHAT_FAKE_MESSAGE_PREVENTING))
            stripLineInvisibleChars(msg);

        if (sWorld.getConfig(CONFIG_CHAT_STRICT_LINK_CHECKING_SEVERITY) && GetSecurity() < SEC_MODERATOR
                && !ChatHandler(this).isValidChatMessage(msg.c_str()))
        {
            sLog.outError("Player %s (GUID: %u) sent a chatmessage with an invalid link: %s", GetPlayer()->GetName(),
                    GetPlayer()->GetGUIDLow(), msg.c_str());
            if (sWorld.getConfig(CONFIG_CHAT_STRICT_LINK_CHECKING_KICK))
                KickPlayer();
            return false;
        }
    }

    return true;
}

void WorldSession::HandleMessagechatOpcode(WorldPacket & recv_data)
{
    uint32 type;
    uint32 lang;

    recv_data >> type;
    recv_data >> lang;

    if (type >= MAX_CHAT_MSG_TYPE)
    {
        sLog.outError("CHAT: Wrong message type received: %u", type);
        return;
    }

    //sLog.outDebug("CHAT: packet received. type %u, lang %u", type, lang);

    // prevent talking at unknown language (cheating)
    LanguageDesc const* langDesc = GetLanguageDescByID(lang);
    if (!langDesc)
    {
        SendNotification(LANG_UNKNOWN_LANGUAGE);
        return;
    }
    if (langDesc->skill_id != 0 && !_player->HasSkill(langDesc->skill_id))
    {
        // also check SPELL_AURA_COMPREHEND_LANGUAGE (client offers option to speak in that language)
        Unit::AuraList const& langAuras = _player->GetAurasByType(SPELL_AURA_COMPREHEND_LANGUAGE);
        bool foundAura = false;
        for (Unit::AuraList::const_iterator i = langAuras.begin();i != langAuras.end(); ++i)
        {
            if ((*i)->GetModifier()->m_miscvalue == int32(lang))
            {
                foundAura = true;
                break;
            }
        }
        if (!foundAura)
        {
            SendNotification(LANG_NOT_LEARNED_LANGUAGE);
            return;
        }
    }

    if (lang == LANG_ADDON)
    {
        if (sWorld.getConfig(CONFIG_CHATLOG_ADDON))
        {
            std::string msg = "";
            recv_data >> msg;

            if (msg.empty())
            {
                sLog.outDebug("Player %s send empty addon msg", GetPlayer()->GetName());
                return;
            }

            sLog.outChat("[ADDON] Player %s sends: %s",
                GetPlayer()->GetName(), msg.c_str());
        }

        // Disabled addon channel?
        if (!sWorld.getConfig(CONFIG_ADDON_CHANNEL))
            return;
    }
    // LANG_ADDON should not be changed nor be affected by flood control
    else
    {
        // send in universal language if player in .gmon mode (ignore spell effects)
        if (_player->isGameMaster())
            lang = LANG_UNIVERSAL;
        else
        {
            // send in universal language in two side iteration allowed mode
            if (sWorld.getConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT))
                lang = LANG_UNIVERSAL;
            else
            {
                switch(type)
                {
                    case CHAT_MSG_PARTY:
                    case CHAT_MSG_RAID:
                    case CHAT_MSG_RAID_LEADER:
                    case CHAT_MSG_RAID_WARNING:
                        // allow two side chat at group channel if two side group allowed
                        if (sWorld.getConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP))
                            lang = LANG_UNIVERSAL;
                        break;
                    case CHAT_MSG_GUILD:
                    case CHAT_MSG_OFFICER:
                        // allow two side chat at guild channel if two side guild allowed
                        if (sWorld.getConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GUILD))
                            lang = LANG_UNIVERSAL;
                        break;
                }
            }

            // but overwrite it by SPELL_AURA_MOD_LANGUAGE auras (only single case used)
            Unit::AuraList const& ModLangAuras = _player->GetAurasByType(SPELL_AURA_MOD_LANGUAGE);
            if (!ModLangAuras.empty())
                lang = ModLangAuras.front()->GetModifier()->m_miscvalue;
        }

        if (!_player->CanSpeak())
        {
            std::string timeStr = secsToTimeString(m_muteTime - time(NULL));
            SendNotification(GetOregonString(LANG_WAIT_BEFORE_SPEAKING),timeStr.c_str());
            return;
        }

        if (type != CHAT_MSG_AFK && type != CHAT_MSG_DND)
            GetPlayer()->UpdateSpeakTime();
    }

    if (GetPlayer()->HasAura(1852,0) && type != CHAT_MSG_WHISPER)
    {
        std::string msg="";
        recv_data >> msg;
        if (ChatHandler(this).ParseCommands(msg.c_str()) == 0)
        {
            SendNotification(GetOregonString(LANG_GM_SILENCE), GetPlayer()->GetName());
            return;
        }
    }

    switch(type)
    {
        case CHAT_MSG_SAY:
        case CHAT_MSG_EMOTE:
        case CHAT_MSG_YELL:
        {
            std::string msg = "";
            recv_data >> msg;

            if (msg.empty())
                break;

            if (ChatHandler(this).ParseCommands(msg.c_str()) > 0)
                break;

            if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
                return;

            if (msg.empty())
                break;

            if (type == CHAT_MSG_SAY)
                GetPlayer()->Say(msg, lang);
            else if (type == CHAT_MSG_EMOTE)
                GetPlayer()->TextEmote(msg);
            else if (type == CHAT_MSG_YELL)
                GetPlayer()->Yell(msg, lang);
        } break;

        case CHAT_MSG_WHISPER:
        {
            std::string to, msg;
            recv_data >> to;
            recv_data >> msg;

            if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
                return;

            if (msg.empty())
                break;

            if (!normalizePlayerName(to))
            {
                SendPlayerNotFoundNotice(to);
                break;
            }

            Player *player = objmgr.GetPlayer(to.c_str());
            uint32 tSecurity = GetSecurity();
            uint32 pSecurity = player ? player->GetSession()->GetSecurity() : SEC_PLAYER;
            if (!player || (tSecurity == SEC_PLAYER && pSecurity > SEC_PLAYER && !player->isAcceptWhispers()))
            {
                SendPlayerNotFoundNotice(to);
                return;
            }

            if (!sWorld.getConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHAT) && tSecurity == SEC_PLAYER && pSecurity == SEC_PLAYER)
            {
                uint32 sidea = GetPlayer()->GetTeam();
                uint32 sideb = player->GetTeam();
                if (sidea != sideb)
                {
                    SendWrongFactionNotice();
                    return;
                }
            }

            if (GetPlayer()->HasAura(1852,0) && !player->isGameMaster())
            {
                SendNotification(GetOregonString(LANG_GM_SILENCE), GetPlayer()->GetName());
                return;
            }

            GetPlayer()->Whisper(msg, lang, player->GetGUID());
        } break;

        case CHAT_MSG_PARTY:
        {
            std::string msg;
            recv_data >> msg;

            if (msg.empty())
                break;

            if (ChatHandler(this).ParseCommands(msg.c_str()) > 0)
                break;

            if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
                return;

            if (msg.empty())
                break;

            // if player is in battleground, he cannot say to battleground members by /p
            Group *group = GetPlayer()->GetOriginalGroup();
            // so if player hasn't OriginalGroup and his player->GetGroup() is BG raid, then return
            if (!group && (!(group = GetPlayer()->GetGroup()) || group->isBGGroup()))
                return;

            // ChatSpy
            GetPlayer()->HandleChatSpyMessage(msg, CHAT_MSG_PARTY, lang);
            for(GroupReference *itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                if(Player *pl = itr->getSource())
                    pl->HandleChatSpyMessage(msg, CHAT_MSG_PARTY, lang, GetPlayer());

            WorldPacket data;
            ChatHandler::FillMessageData(&data, this, type, lang, NULL, 0, msg.c_str(), NULL);
            group->BroadcastPacket(&data, false, group->GetMemberGroup(GetPlayer()->GetGUID()));

            if (sWorld.getConfig(CONFIG_CHATLOG_PARTY))
                sLog.outChat("[PARTY] Player %s tells group with leader %s: %s",
                    GetPlayer()->GetName(), group->GetLeaderName(), msg.c_str());
        }
        break;
        case CHAT_MSG_GUILD:
        {
            std::string msg;
            recv_data >> msg;

            if (msg.empty())
                break;

            if (ChatHandler(this).ParseCommands(msg.c_str()) > 0)
                break;

            if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
                return;

            if (msg.empty())
                break;

            if (GetPlayer()->GetGuildId())
            {
                Guild *guild = objmgr.GetGuildById(GetPlayer()->GetGuildId());
                if (guild)
                    {
                        guild->BroadcastToGuild(this, msg, lang == LANG_ADDON ? LANG_ADDON : LANG_UNIVERSAL);
                        GetPlayer()->HandleChatSpyMessage(msg, CHAT_MSG_OFFICER, lang);
                    }

                if (lang != LANG_ADDON && sWorld.getConfig(CONFIG_CHATLOG_GUILD))
                {
                    sLog.outChat("[GUILD] Player %s tells guild %s: %s",
                        GetPlayer()->GetName(), guild->GetName().c_str(), msg.c_str());
                }
                else if (lang == LANG_ADDON && sWorld.getConfig(CONFIG_CHATLOG_ADDON))
                {
                    sLog.outChat("[ADDON] Player %s sends to guild %s: %s",
                        GetPlayer()->GetName(), guild->GetName().c_str(), msg.c_str());
                }
            }

            break;
        }
        case CHAT_MSG_OFFICER:
        {
            std::string msg;
            recv_data >> msg;

            if (msg.empty())
                break;

            if (ChatHandler(this).ParseCommands(msg.c_str()) > 0)
                break;

            if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
                return;

            if (msg.empty())
                break;

            if (GetPlayer()->GetGuildId())
            {
                Guild *guild = objmgr.GetGuildById(GetPlayer()->GetGuildId());
                if (guild)
                    {
                        guild->BroadcastToOfficers(this, msg, lang == LANG_ADDON ? LANG_ADDON : LANG_UNIVERSAL);
                        GetPlayer()->HandleChatSpyMessage(msg, CHAT_MSG_OFFICER, lang);
                    }

                if (sWorld.getConfig(CONFIG_CHATLOG_GUILD))
                    sLog.outChat("[OFFICER] Player %s tells guild %s officers: %s",
                        GetPlayer()->GetName(), guild->GetName().c_str(), msg.c_str());
            }
            break;
        }
        case CHAT_MSG_RAID:
        {
            std::string msg;
            recv_data >> msg;

            if (msg.empty())
                break;

            if (ChatHandler(this).ParseCommands(msg.c_str()) > 0)
                break;

            if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
                return;

            if (msg.empty())
                break;

            // if player is in battleground, he cannot say to battleground members by /ra
            Group *group = GetPlayer()->GetOriginalGroup();
            // so if player hasn't OriginalGroup and his player->GetGroup() is BG raid or his group isn't raid, then return
            if (!group && !(group = GetPlayer()->GetGroup()) || group->isBGGroup() || !group->isRaidGroup())
                return;

            // ChatSpy
            GetPlayer()->HandleChatSpyMessage(msg, CHAT_MSG_RAID, lang);
            for(GroupReference *itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                if(Player *pl = itr->getSource())
                    pl->HandleChatSpyMessage(msg, CHAT_MSG_RAID, lang, GetPlayer());

            WorldPacket data;
            ChatHandler::FillMessageData(&data, this, CHAT_MSG_RAID, lang, "", 0, msg.c_str(), NULL);
            group->BroadcastPacket(&data, false);

            if (sWorld.getConfig(CONFIG_CHATLOG_RAID))
                sLog.outChat("[RAID] Player %s tells raid with leader %s: %s",
                    GetPlayer()->GetName(), group->GetLeaderName(), msg.c_str());
        } break;
        case CHAT_MSG_RAID_LEADER:
        {
            std::string msg;
            recv_data >> msg;

            if (msg.empty())
                break;

            if (ChatHandler(this).ParseCommands(msg.c_str()) > 0)
                break;

            if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
                return;

            if (msg.empty())
                break;

            // if player is in battleground, he cannot say to battleground members by /ra
            Group *group = GetPlayer()->GetOriginalGroup();
            if (!group && !(group = GetPlayer()->GetGroup()) || group->isBGGroup() || !group->isRaidGroup() || !group->IsLeader(GetPlayer()->GetGUID()))
                return;

            // ChatSpy
            GetPlayer()->HandleChatSpyMessage(msg, CHAT_MSG_RAID_LEADER, lang);
            for(GroupReference *itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                if(Player *pl = itr->getSource())
                    pl->HandleChatSpyMessage(msg, CHAT_MSG_RAID_LEADER, lang, GetPlayer());

            WorldPacket data;
            ChatHandler::FillMessageData(&data, this, CHAT_MSG_RAID_LEADER, lang, "", 0, msg.c_str(), NULL);
            group->BroadcastPacket(&data, false);

            if (sWorld.getConfig(CONFIG_CHATLOG_RAID))
                sLog.outChat("[RAID] Leader player %s tells raid: %s",
                    GetPlayer()->GetName(), msg.c_str());
        } break;
        case CHAT_MSG_RAID_WARNING:
        {
            std::string msg;
            recv_data >> msg;

            if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
                return;

            if (msg.empty())
                break;

            Group *group = GetPlayer()->GetGroup();
            if (!group || !group->isRaidGroup() || !(group->IsLeader(GetPlayer()->GetGUID()) || group->IsAssistant(GetPlayer()->GetGUID())) || group->isBGGroup())
                return;

            // ChatSpy
            GetPlayer()->HandleChatSpyMessage(msg, CHAT_MSG_RAID_WARNING, lang);
            for(GroupReference *itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                if(Player *pl = itr->getSource())
                    pl->HandleChatSpyMessage(msg, CHAT_MSG_RAID_WARNING, lang, GetPlayer());

            WorldPacket data;
            // in battleground, raid warning is sent only to players in battleground - code is ok
            ChatHandler::FillMessageData(&data, this, CHAT_MSG_RAID_WARNING, lang, "", 0, msg.c_str(),NULL);
            group->BroadcastPacket(&data, false);

            if (sWorld.getConfig(CONFIG_CHATLOG_RAID))
                sLog.outChat("[RAID] Leader player %s warns raid with: %s",
                    GetPlayer()->GetName(), msg.c_str());
        } break;

        case CHAT_MSG_BATTLEGROUND:
        {
            std::string msg;
            recv_data >> msg;

            if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
                return;

            if (msg.empty())
                break;

            //battleground raid is always in Player->GetGroup(), never in GetOriginalGroup()
            Group *group = GetPlayer()->GetGroup();
            if (!group || !group->isBGGroup())
                return;

            // ChatSpy
            GetPlayer()->HandleChatSpyMessage(msg, CHAT_MSG_BATTLEGROUND, lang);
            for(GroupReference *itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                if(Player *pl = itr->getSource())
                    pl->HandleChatSpyMessage(msg, CHAT_MSG_BATTLEGROUND, lang, GetPlayer());

            WorldPacket data;
            ChatHandler::FillMessageData(&data, this, CHAT_MSG_BATTLEGROUND, lang, "", 0, msg.c_str(), NULL);
            group->BroadcastPacket(&data, false);

            if (sWorld.getConfig(CONFIG_CHATLOG_BGROUND))
                sLog.outChat("[BATTLEGROUND] Player %s tells battleground with leader %s: %s",
                    GetPlayer()->GetName(), group->GetLeaderName(), msg.c_str());
        } break;

        case CHAT_MSG_BATTLEGROUND_LEADER:
        {
            std::string msg;
            recv_data >> msg;

            if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
                return;

            if (msg.empty())
                break;

            //battleground raid is always in Player->GetGroup(), never in GetOriginalGroup()
            Group *group = GetPlayer()->GetGroup();
            if (!group || !group->isBGGroup() || !group->IsLeader(GetPlayer()->GetGUID()))
                return;

            // ChatSpy
            GetPlayer()->HandleChatSpyMessage(msg, CHAT_MSG_BATTLEGROUND_LEADER, lang);
            for(GroupReference *itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
                if(Player *pl = itr->getSource())
                    pl->HandleChatSpyMessage(msg, CHAT_MSG_BATTLEGROUND_LEADER, lang, GetPlayer());

            WorldPacket data;
            ChatHandler::FillMessageData(&data, this, CHAT_MSG_BATTLEGROUND_LEADER, lang, "", 0, msg.c_str(),NULL);
            group->BroadcastPacket(&data, false);

            if (sWorld.getConfig(CONFIG_CHATLOG_BGROUND))
                sLog.outChat("[RAID] Leader player %s tells battleground: %s",
                    GetPlayer()->GetName(), msg.c_str());
        } break;

        case CHAT_MSG_CHANNEL:
        {
            std::string channel, msg;
            recv_data >> channel;
            recv_data >> msg;

            if (!processChatmessageFurtherAfterSecurityChecks(msg, lang))
                return;

            if (msg.empty())
                break;
			sIRC.Send_WoW_IRC(_player, channel, msg);
            if (ChannelMgr* cMgr = channelMgr(_player->GetTeam()))
            {
                if (Channel *chn = cMgr->GetChannel(channel, _player))
                {
                    {
                        chn->Say(_player->GetGUID(), msg.c_str(), lang);
                        GetPlayer()->HandleChatSpyMessage(msg, CHAT_MSG_CHANNEL, lang, NULL, channel);
                    }

                    if ((chn->HasFlag(CHANNEL_FLAG_TRADE) ||
                        chn->HasFlag(CHANNEL_FLAG_GENERAL) ||
                        chn->HasFlag(CHANNEL_FLAG_CITY) ||
                        chn->HasFlag(CHANNEL_FLAG_LFG)) &&
                        sWorld.getConfig(CONFIG_CHATLOG_SYSCHAN))
                            sLog.outChat("[SYSCHAN] Player %s tells channel %s: %s",
                                GetPlayer()->GetName(), chn->GetName().c_str(), msg.c_str());
                    else if (sWorld.getConfig(CONFIG_CHATLOG_CHANNEL))
                            sLog.outChat("[CHANNEL] Player %s tells channel %s: %s",
                                GetPlayer()->GetName(), chn->GetName().c_str(), msg.c_str());
                }
            }
        } break;

        case CHAT_MSG_AFK:
        {
            std::string msg;
            recv_data >> msg;

            if (!_player->isInCombat())
            {
                if (!msg.empty() || !_player->isAFK())
                {
                    if (msg.empty())
                        _player->afkMsg = GetOregonString(LANG_PLAYER_AFK_DEFAULT);
                    else
                        _player->afkMsg = msg;
                }
                if (msg.empty() || !_player->isAFK())
                {
                    _player->ToggleAFK();
                    if (_player->isAFK() && _player->isDND())
                        _player->ToggleDND();
                }
            }
        } break;

        case CHAT_MSG_DND:
        {
            std::string msg;
            recv_data >> msg;

            if (!msg.empty() || !_player->isDND())
            {
                if (msg.empty())
                    _player->dndMsg = GetOregonString(LANG_PLAYER_DND_DEFAULT);
                else
                    _player->dndMsg = msg;
            }
            if (msg.empty() || !_player->isDND())
            {
                _player->ToggleDND();
                if (_player->isDND() && _player->isAFK())
                    _player->ToggleAFK();
            }
        } break;

        default:
            sLog.outError("CHAT: unknown message type %u, lang: %u", type, lang);
            break;
    }
}

void WorldSession::HandleEmoteOpcode(WorldPacket & recv_data)
{
    if (!GetPlayer()->isAlive())
        return;

    uint32 emote;
    recv_data >> emote;
    GetPlayer()->HandleEmoteCommand(emote);
}

void WorldSession::HandleTextEmoteOpcode(WorldPacket & recv_data)
{
    if (!GetPlayer()->isAlive())
        return;

    if (!GetPlayer()->CanSpeak())
    {
        std::string timeStr = secsToTimeString(m_muteTime - time(NULL));
        SendNotification(GetOregonString(LANG_WAIT_BEFORE_SPEAKING),timeStr.c_str());
        return;
    }

    uint32 text_emote, emoteNum;
    uint64 guid;

    recv_data >> text_emote;
    recv_data >> emoteNum;
    recv_data >> guid;

    EmotesTextEntry const *em = sEmotesTextStore.LookupEntry(text_emote);
    if (!em)
        return;

    uint32 emote_anim = em->textid;

    switch(emote_anim)
    {
        case EMOTE_STATE_SLEEP:
        case EMOTE_STATE_SIT:
        case EMOTE_STATE_KNEEL:
        case EMOTE_ONESHOT_NONE:
            break;
        default:
            GetPlayer()->HandleEmoteCommand(emote_anim);
            break;
    }

    const char *nam = 0;
    uint32 namlen = 1;

    Unit* unit = ObjectAccessor::GetUnit(*_player, guid);
    if (unit)
    {
        nam = unit->GetName();
        namlen = (nam ? strlen(nam) : 0) + 1;
    }

    WorldPacket data;
    data.Initialize(SMSG_TEXT_EMOTE, (20+namlen));
    data << GetPlayer()->GetGUID();
    data << (uint32)text_emote;
    data << emoteNum;
    data << (uint32)namlen;
    if (namlen > 1)
        data.append(nam, namlen);
    else
        data << (uint8)0x00;

    GetPlayer()->SendMessageToSetInRange(&data,sWorld.getConfig(CONFIG_LISTEN_RANGE_TEXTEMOTE),true);

    //Send scripted event call
    if (unit && unit->GetTypeId() == TYPEID_UNIT && ((Creature*)unit)->AI())
        unit->ToCreature()->AI()->ReceiveEmote(GetPlayer(), text_emote);
}

void WorldSession::HandleChatIgnoredOpcode(WorldPacket& recv_data)
{
    uint64 iguid;
    uint8 unk;
    //sLog.outDebug("WORLD: Received CMSG_CHAT_IGNORED");

    recv_data >> iguid;
    recv_data >> unk;                                       // probably related to spam reporting

    Player *player = objmgr.GetPlayer(iguid);
    if (!player || !player->GetSession())
        return;

    WorldPacket data;
    ChatHandler::FillMessageData(&data, this, CHAT_MSG_IGNORED, LANG_UNIVERSAL, NULL, GetPlayer()->GetGUID(), GetPlayer()->GetName(), NULL);
    player->GetSession()->SendPacket(&data);
}

void WorldSession::SendPlayerNotFoundNotice(std::string name)
{
    WorldPacket data(SMSG_CHAT_PLAYER_NOT_FOUND, name.size()+1);
    data << name;
    SendPacket(&data);
}

void WorldSession::SendWrongFactionNotice()
{
    WorldPacket data(SMSG_CHAT_WRONG_FACTION, 0);
    SendPacket(&data);
}

