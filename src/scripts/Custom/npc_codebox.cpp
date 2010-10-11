/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/* ScriptData
SDName: NPC_Code_Codebox
SD%Complete: 100
SDComment: Makes an NPC give a gift if a valid code is entered
SDCategory: NPCs
EndScriptData */

#include "ScriptPCH.h"
#include <cstring>

#define C_GOSSIP_MENU 907
#define D_ENTER_CODE "I have a code!"
#define D_CANCEL_CODE "Nevermind..."
#define SAY_INV_FULL "Your code is right, but you have no space in your bags!  Come back when you have some free space."
#define SAY_EXPIRED "Sorry, your code has expired.  Come back when you have a valid one."
#define SAY_INVALID "Sorry, your code is not valid.  Come back when you have a new one."
#define SAY_GIVE_PRESENT "This is for you!  I hope you enjoy it."
#define SAY_EXTRA_BUFF "How lucky you are!  You get an item and a buff!"
#define SAY_GIVE_BUFF "Here is a special buff, just for you!"
#define SAY_GOODBYE "Nice to see you!  Goodbye!"

/*
 Returns TRUE if the gift is given.
 Returns FALSE if the code is wrong
   or if the gift can't be given (i.e. full inventory).

 ~elegos
*/
bool checkCode(Player *player, Creature *_Creature, const char* sCode)
{
    // Some variables declarations.
    int32 item_id = 0;
    int32 aura_id = 0;
    uint32 quantity = 0;
    int32 uses = 0;
    bool check = false;

    extern DatabaseType WorldDatabase;
    uint32 creatureID = _Creature->GetEntry();

    QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `item_id`, `aura_id`, `quantity`, `uses`  FROM `npc_codes` WHERE `code` = '%s' AND npc_id = %i LIMIT 1",sCode,creatureID);
    if(result)
    {
        check = true;
        Field *fields = result->Fetch();

        item_id = fields[0].GetInt32();
        aura_id = fields[1].GetInt32();
        quantity = fields[2].GetUInt32();
        uses = fields[3].GetUInt32();

        if(uses != 0)
        {
            if(item_id > 0)
            {
                ItemPosCountVec dest;
                uint8 canStoreNewItem = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item_id, quantity);
                if(canStoreNewItem == EQUIP_ERR_OK)
                {
                    Item *newItem = NULL;
                    newItem = player->StoreNewItem(dest,item_id,quantity,true);
                    player->SendNewItem(newItem,quantity,true,false);
                }
                else
                {
                    _Creature->MonsterWhisper(SAY_INV_FULL, player->GetGUID());
                    check = false;
                }
            }
            else if(item_id < 0)
            {
                uint32 id = item_id * -1;
                uint32 num;
                uint32 item;
                QueryResult_AutoPtr items = WorldDatabase.PQuery("SELECT `item_id` FROM `npc_codes_item_loot` WHERE `group_id` = '%i'",id);
                if(items)
                {
                    num = rand()%items->GetRowCount();
                    Field *i;
                    uint32 count = 0;
                    do
                    {
                        i = items->Fetch();
                        item = i[0].GetUInt32();
                        ++count;
                    }while((count < num) && (items->NextRow()));
                    ItemPosCountVec dest;
                    uint8 canStoreNewItem = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item, quantity);
                    if(canStoreNewItem == EQUIP_ERR_OK)
                    {
                        Item *newItem = NULL;
                        newItem = player->StoreNewItem(dest,item,quantity,true);
                        player->SendNewItem(newItem,quantity,true,false);
                    }
                    else
                    {
                        _Creature->MonsterWhisper(SAY_INV_FULL, player->GetGUID());
                        check = false;
                    }
                }
            }
            if(aura_id > 0 && check == true)
            {
                _Creature->CastSpell(player, aura_id, true);
            }
            else if(aura_id < 0 && check == true)
            {
                uint32 id = aura_id * -1;
                uint32 num;
                uint32 aura;
                QueryResult_AutoPtr auras = WorldDatabase.PQuery("SELECT `aura_id` FROM `npc_codes_aura_loot` WHERE `group_id` = '%i'",id);
                if(auras)
                {
                    num = rand()%auras->GetRowCount();
                    Field *a;
                    uint32 count = 0;
                    do
                    {
                        a = auras->Fetch();
                        aura = a[0].GetUInt32();
                        ++count;
                    }while((count < num) && (auras->NextRow()));
                    _Creature->CastSpell(player, aura, true);
                }
            }
        }
        else
        {
            check = false;
            _Creature->MonsterWhisper(SAY_EXPIRED, player->GetGUID());
        }
    }
    else
    {
        check = false;
        _Creature->MonsterWhisper(SAY_INVALID, player->GetGUID());
    }


    if(check == true)
    {
        if(uses > 0)
        {
            WorldDatabase.PExecute("UPDATE `npc_codes` SET `uses` = (`uses` - 1) WHERE `code` = '%s' LIMIT 1",sCode);
        }
        if(item_id != 0 && aura_id == 0)
            _Creature->MonsterSay(SAY_GIVE_PRESENT, LANG_UNIVERSAL, NULL);
        if(item_id != 0 && aura_id != 0)
            _Creature->MonsterSay(SAY_EXTRA_BUFF, LANG_UNIVERSAL, NULL);
        if(item_id == 0 && aura_id != 0)
            _Creature->MonsterSay(SAY_GIVE_BUFF, LANG_UNIVERSAL, NULL);
    }
    return check;
}

//This function is called when the player opens the gossip menubool
bool GossipHello_npc_codebox(Player *player, Creature *_Creature)
{
    int32 text_id = C_GOSSIP_MENU;
    
    extern DatabaseType WorldDatabase;
    uint32 creatureID = _Creature->GetEntry();

    QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `npc_text_id` FROM `npc_codes_text` WHERE npc_id = %i LIMIT 1",creatureID);
    if(result)
    {
        Field *fields = result->Fetch();
        text_id = fields[0].GetInt32();
    }

    player->ADD_GOSSIP_ITEM_EXTENDED(0, D_ENTER_CODE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1, "", 0, true);
    player->ADD_GOSSIP_ITEM(0, D_CANCEL_CODE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    player->PlayerTalkClass->SendGossipMenu(text_id,_Creature->GetGUID());
    return true;
}

//This function is called when the player clicks an option on the gossip menubool
bool GossipSelect_npc_codebox(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
    if(action == GOSSIP_ACTION_INFO_DEF+2)
    {
        _Creature->MonsterSay(SAY_GOODBYE, LANG_UNIVERSAL, NULL); 
        player->CLOSE_GOSSIP_MENU();
    }
    return true;
}

bool GossipSelectWithCode_npc_codebox( Player *player, Creature *_Creature, uint32 sender, uint32 action, const char* sCode )
{
    if(sender == GOSSIP_SENDER_MAIN)
    {
        if(action == GOSSIP_ACTION_INFO_DEF+1)
        {
            checkCode(player, _Creature, sCode);
            player->CLOSE_GOSSIP_MENU();
            return true;
        }
    }
    return false;
}

void AddSC_npc_codebox()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_codebox";
    newscript->pGossipHello =           &GossipHello_npc_codebox;
    newscript->pGossipSelect =          &GossipSelect_npc_codebox;
    newscript->pGossipSelectWithCode =  &GossipSelectWithCode_npc_codebox;
    newscript->RegisterSelf();
}

