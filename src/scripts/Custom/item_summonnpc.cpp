/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * Modified for OregonCore - Copyright (2010) Oregon <http://www.oregoncore.com>
 * Modifications by Celtus - 2010-09-13
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: item_summonnpc
SD%Complete: 100
SDComment: Summon NPC
SDCategory: Items
EndScriptData */

#include "ScriptPCH.h"
#include <cstring>

bool ItemUse_item_summonnpc(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{

    // Declare Variables
    uint32 itemId = pItem->GetEntry();
    uint32 npcId;
    uint32 despawnSec;
    extern DatabaseType WorldDatabase;

if ((pPlayer->isInCombat()) || (pPlayer->isInFlight()) || (pPlayer->isDead()))
{
      pPlayer->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
        return false;
}
if (pPlayer->IsMounted()) // Is player mounted
{
      pPlayer->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem, NULL);
        return true;
}
{
        // Fetch the NPC Id
        QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT `creature_entry`, `despawn` FROM `item_summonnpc` WHERE `item_entry` = '%i' LIMIT 1",itemId);
    if(result)
    {
        Field *fields = result->Fetch();
        // Read NPC Id from the query result
        npcId = fields[0].GetInt32();
        // Read time before despawn
        despawnSec = fields[1].GetUInt32() * 1000;
        // Summon the NPC
	pPlayer->SummonCreature(npcId,pPlayer->GetPositionX() ,pPlayer->GetPositionY()-2, pPlayer->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, despawnSec);
        return true;
    } else {
        // Summons failed - show info in log
        outstring_log ("Item_SummonNPC Failed: %i)",itemId);
    }
}
        return false;
}
void AddSC_item_summonnpc()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_summonnpc";
    newscript->pItemUse = &ItemUse_item_summonnpc;
    newscript->RegisterSelf();
}
