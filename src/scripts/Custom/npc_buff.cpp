/* ScriptData
SDName: NPC_Buff
SD%Complete: 95
SDComment: Buff Master
SDCategory: NPC
EndScriptData */

#include "ScriptPCH.h"

bool GossipHello_NPC_Buff(Player *player, Creature *_Creature)
{
    // Main Menu for Alliance
    if ( player->GetTeam() == ALLIANCE )
    {
        player->ADD_GOSSIP_ITEM( 7, "Small Spells ->", GOSSIP_SENDER_MAIN, 1000);
        player->ADD_GOSSIP_ITEM( 7, "GM Spells ->", GOSSIP_SENDER_MAIN, 3000);
        player->ADD_GOSSIP_ITEM( 7, "Other Gifts ->", GOSSIP_SENDER_MAIN, 4000);
    }
    else // Main Menu for Horde
    {
        player->ADD_GOSSIP_ITEM( 7, "Small Spells ->", GOSSIP_SENDER_MAIN, 1000);
        player->ADD_GOSSIP_ITEM( 7, "GM Spells ->", GOSSIP_SENDER_MAIN, 3000);
        player->ADD_GOSSIP_ITEM( 7, "Other Gifts ->", GOSSIP_SENDER_MAIN, 4000);
    }

    player->ADD_GOSSIP_ITEM( 10, "Remove Resurrect Sickness", GOSSIP_SENDER_MAIN, 5000);

    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

return true;
}

void SendDefaultMenu_NPC_Buff(Player *player, Creature *_Creature, uint32 action )
{

// Not allow in combat
if(!player->getAttackers().empty())
{
    player->CLOSE_GOSSIP_MENU();
    _Creature->MonsterSay("You are in combat!", LANG_UNIVERSAL, NULL);
    return;
}

switch(action)
{

case 1000: // Small Buff
    player->ADD_GOSSIP_ITEM( 5, "Give me Mark of the Wild"                          , GOSSIP_SENDER_MAIN, 1001);
    player->ADD_GOSSIP_ITEM( 5, "Give me Thorns"                                    , GOSSIP_SENDER_MAIN, 1005);
    player->ADD_GOSSIP_ITEM( 5, "Give me Amplify Magic"                             , GOSSIP_SENDER_MAIN, 1010);
    player->ADD_GOSSIP_ITEM( 5, "Give me Arcane Intellect"                          , GOSSIP_SENDER_MAIN, 1015);
    player->ADD_GOSSIP_ITEM( 5, "Give me Dampen Magic"                              , GOSSIP_SENDER_MAIN, 1025);
    player->ADD_GOSSIP_ITEM( 5, "Give me Blessing of Kings"                         , GOSSIP_SENDER_MAIN, 1030);
    player->ADD_GOSSIP_ITEM( 5, "Give me Blessing of Might"                         , GOSSIP_SENDER_MAIN, 1035);
    player->ADD_GOSSIP_ITEM( 5, "Give me Blessing of Wisdom"                        , GOSSIP_SENDER_MAIN, 1040);
    player->ADD_GOSSIP_ITEM( 5, "Give me Divine Spirit"                             , GOSSIP_SENDER_MAIN, 1045);
    player->ADD_GOSSIP_ITEM( 5, "Give me Power Word: Fortitude"                     , GOSSIP_SENDER_MAIN, 1050);
    player->ADD_GOSSIP_ITEM( 5, "Give me Shadow Protection"                         , GOSSIP_SENDER_MAIN, 1055);
    player->ADD_GOSSIP_ITEM( 7, "<- Main Menu"                                      , GOSSIP_SENDER_MAIN, 5005);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
    break;

case 3000: // GM Buff
    player->ADD_GOSSIP_ITEM( 5, "Give me Agamaggan's Agility"                       , GOSSIP_SENDER_MAIN, 3001);
    player->ADD_GOSSIP_ITEM( 5, "Give me Agamaggan's Strength"                      , GOSSIP_SENDER_MAIN, 3005);
    player->ADD_GOSSIP_ITEM( 5, "Give me Increased Stamina"                         , GOSSIP_SENDER_MAIN, 3020);
    player->ADD_GOSSIP_ITEM( 5, "Give me Razorhide"                                 , GOSSIP_SENDER_MAIN, 3025);
    player->ADD_GOSSIP_ITEM( 5, "Give me Rising Spirit"                             , GOSSIP_SENDER_MAIN, 3030);
    player->ADD_GOSSIP_ITEM( 5, "Give me Spirit of the Wind"                        , GOSSIP_SENDER_MAIN, 3035);
    player->ADD_GOSSIP_ITEM( 5, "Give me Wisdom of Agamaggan"                       , GOSSIP_SENDER_MAIN, 3040);
    player->ADD_GOSSIP_ITEM( 7, "<- Main Menu"                                      , GOSSIP_SENDER_MAIN, 5005);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
    break;

case 4000: // Player Tools
    player->ADD_GOSSIP_ITEM( 5, "Give me Gold"                                      , GOSSIP_SENDER_MAIN, 4001);
    player->ADD_GOSSIP_ITEM( 5, "Give me Soul Shards"                               , GOSSIP_SENDER_MAIN, 4005);
    player->ADD_GOSSIP_ITEM( 5, "Maximize my Skills"
    , GOSSIP_SENDER_MAIN, 4007);
    player->ADD_GOSSIP_ITEM( 5, "Heal Me, Please"                                   , GOSSIP_SENDER_MAIN, 4010);
    player->ADD_GOSSIP_ITEM( 7, "<- Main Menu"                                      , GOSSIP_SENDER_MAIN, 5005);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
    break;

case 5005: // Back To Main Menu
    // Main Menu for Alliance
    if ( player->GetTeam() == ALLIANCE )
    {
        player->ADD_GOSSIP_ITEM( 7, "Small Spells ->", GOSSIP_SENDER_MAIN, 1000);
        player->ADD_GOSSIP_ITEM( 7, "GM Spells ->", GOSSIP_SENDER_MAIN, 3000);
        player->ADD_GOSSIP_ITEM( 7, "Other Gifts ->", GOSSIP_SENDER_MAIN, 4000);
    }
    else // Main Menu for Horde
    {
        player->ADD_GOSSIP_ITEM( 7, "Small Spells ->", GOSSIP_SENDER_MAIN, 1000);
        player->ADD_GOSSIP_ITEM( 7, "GM Spells ->", GOSSIP_SENDER_MAIN, 3000);
        player->ADD_GOSSIP_ITEM( 7, "Other Gifts ->", GOSSIP_SENDER_MAIN, 4000);
    }

    player->ADD_GOSSIP_ITEM( 10, "Remove Resurrect Sickness", GOSSIP_SENDER_MAIN, 5000);
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
    break;

// Small Buff

case 1001: // Give me Mark of the Wild
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,9885,false);
break;

case 1005: // Give me Thorns
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,26992,false);
break;

case 1010: // Give me Amplify Magic
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,33946,false);
break;

case 1015: // Give me Arcane Intellect
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,27126,false);
break;

case 1025: // Give me Dampen Magic
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,33944,false);
break;

case 1030: // Give me Blessing of Kings
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,20217,false);
break;

case 1035: // Give me Blessing of Might
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,19838,false);
break;

case 1040: // Give me Blessing of Wisdom
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,25290,false);
break;

case 1045: // Give me Divine Spirit
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,25312,false);
break;

case 1050: // Give me Power Word: Fortitude
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,10938,false);
break;

case 1055: // Give me Shadow Protection
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,25433,false);
break;

// GM Buff

case 3001: // Give me Agamaggan's Agility
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,17013,false);
break;

case 3005: // Give me Agamaggan's Strength
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,16612,false);
break;

case 3020: // Give me Increased Stamina
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,25661,false);
break;

case 3025: // Give me Razorhide
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,16610,false);
break;

case 3030: // Give me Rising Spirit
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,10767,false);
break;

case 3035: // Give me Spirit of the Wind
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,16618,false);
break;

case 3040: // Give me Wisdom of Agamaggan
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,7764,false);
break;

// Player Tools

case 4001://Give me Gold
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,46642,false); // 5000 gold
break;

case 4005://Give me Soul Shards
    player->CLOSE_GOSSIP_MENU();
    player->CastSpell(player,24827,false);
break;

case 4007: // Update Skill to Max for Level      
    player->CLOSE_GOSSIP_MENU();
    player->UpdateSkillsToMaxSkillsForLevel();
break;

case 4010: // Heal me please
    player->CLOSE_GOSSIP_MENU();
    _Creature->CastSpell(player,38588,false);
break;

case 5000://Remove Res Sickness
    if(!player->HasAura(SPELL_ID_PASSIVE_RESURRECTION_SICKNESS,0)) {
        _Creature->MonsterWhisper("You are not suffering from resurrection sickness.", player->GetGUID());
        GossipHello_NPC_Buff(player, _Creature);
        return;
    }

    _Creature->CastSpell(player,38588,false); // Healing effect
    player->RemoveAurasDueToSpell(SPELL_ID_PASSIVE_RESURRECTION_SICKNESS);
    player->CLOSE_GOSSIP_MENU();
    break;

    player->CLOSE_GOSSIP_MENU();

    } // end of switch
} //end of function

bool GossipSelect_NPC_Buff(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
    // Main menu
    if (sender == GOSSIP_SENDER_MAIN)
        SendDefaultMenu_NPC_Buff( player, _Creature, action );
    return true;
}

void AddSC_npc_buff()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "npc_buff";
    newscript->pGossipHello = &GossipHello_NPC_Buff;
    newscript->pGossipSelect = &GossipSelect_NPC_Buff;
    newscript->RegisterSelf();
}

