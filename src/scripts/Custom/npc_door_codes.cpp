/*######
## npc_door_codes
######*/

// Based on an awesome script by Pixeel - TC2 Forums

#include "ScriptPCH.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "Chat.h"

bool verification1Code(Player *pPlayer, Creature *pCreature, const char* sCode)
{
    bool verification1 = false;

    QueryResult_AutoPtr result = WorldDatabase.PQuery( "SELECT `secret`, `gameobject_entry`, `opentimesec` FROM `npc_door_codes` WHERE `secret` = '%s' AND `creature_entry` = %u LIMIT 1", sCode, pCreature->GetEntry() );
    
    if(result)
    {
        Field *fields = result->Fetch();
          verification1 = true;
          uint32 targetGo = fields[1].GetUInt32();
          GameObject* SecureDoor = pCreature->FindNearestGameObject(targetGo, 10.0f);
              if (!SecureDoor)
              {
                  return false;
              }
     
              if (SecureDoor)
              {
                  SecureDoor->UseDoorOrButton(fields[2].GetUInt32());
              }
    }
    
    if(!result)
    {
        verification1 = false;
    }
    
    return verification1;
}

bool GossipHello_npc_door_codes(Player *pPlayer, Creature *pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM_EXTENDED(0, "I know the secret word.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1, "", 0, true);
    pPlayer->ADD_GOSSIP_ITEM(0, "I do not know the secret word.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->PlayerTalkClass->SendGossipMenu(91020,pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_door_codes(Player *pPlayer, Creature *pCreature, uint32 sender, uint32 action )
{
    if(action == GOSSIP_ACTION_INFO_DEF+2)
    {
        pCreature->MonsterWhisper("Sorry, No one comes in without the secret word.", pPlayer->GetGUID());
        pPlayer->CLOSE_GOSSIP_MENU();
    }
    return true;
}

bool GossipSelectWithCode_npc_door_codes( Player *player, Creature *pCreature, uint32 sender, uint32 action, const char* sCode )
{
    if(sender == GOSSIP_SENDER_MAIN)
    {
        if(action == GOSSIP_ACTION_INFO_DEF+1)
        {
            verification1Code(player, pCreature, sCode);
            player->CLOSE_GOSSIP_MENU();
            return true;
        }
    }
    return false;
}

void AddSC_npc_door_codes()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_door_codes";
    newscript->pGossipHello =           &GossipHello_npc_door_codes;
    newscript->pGossipSelect =          &GossipSelect_npc_door_codes;
    newscript->pGossipSelectWithCode =  &GossipSelectWithCode_npc_door_codes;
    newscript->RegisterSelf();
    
}


