/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
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
SDName: Tanaris
SD%Complete: 80
SDComment: Quest support: 648, 1560, 2954, 4005, 10277, 10279(Special flight path). Noggenfogger vendor
SDCategory: Tanaris
EndScriptData */

/* ContentData
mob_aquementas
npc_custodian_of_time
npc_marin_noggenfogger
npc_steward_of_time
npc_stone_watcher_of_norgannon
npc_OOX17
npc_tooga
EndContentData */

#include "precompiled.h"
#include "../../npc/npc_escortAI.h"

/*######
## mob_aquementas
######*/

#define AGGRO_YELL_AQUE     -1000350

#define SPELL_AQUA_JET      13586
#define SPELL_FROST_SHOCK   15089

struct TRINITY_DLL_DECL mob_aquementasAI : public ScriptedAI
{
    mob_aquementasAI(Creature *c) : ScriptedAI(c) {}

    uint32 SendItem_Timer;
    uint32 SwitchFaction_Timer;
    bool isFriendly;

    uint32 FrostShock_Timer;
    uint32 AquaJet_Timer;

    void Reset()
    {
        SendItem_Timer = 0;
        SwitchFaction_Timer = 10000;
        m_creature->setFaction(35);
        isFriendly = true;

        AquaJet_Timer = 5000;
        FrostShock_Timer = 1000;
    }

    void SendItem(Unit* receiver)
    {
        if (CAST_PLR(receiver)->HasItemCount(11169,1,false) &&
            CAST_PLR(receiver)->HasItemCount(11172,11,false) &&
            CAST_PLR(receiver)->HasItemCount(11173,1,false) &&
            !CAST_PLR(receiver)->HasItemCount(11522,1,true))
        {
            ItemPosCountVec dest;
            uint8 msg = CAST_PLR(receiver)->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 11522, 1, false);
            if (msg == EQUIP_ERR_OK)
                CAST_PLR(receiver)->StoreNewItem(dest, 11522, 1, true);
        }
    }

    void EnterCombat(Unit* who)
    {
        DoScriptText(AGGRO_YELL_AQUE, m_creature, who);
    }

    void UpdateAI(const uint32 diff)
    {
        if (isFriendly)
        {
            if (SwitchFaction_Timer < diff)
            {
                m_creature->setFaction(91);
                isFriendly = false;
            }else SwitchFaction_Timer -= diff;
        }

        if (!UpdateVictim())
            return;

        if (!isFriendly)
        {
            if (SendItem_Timer < diff)
            {
                if (m_creature->getVictim()->GetTypeId() == TYPEID_PLAYER)
                    SendItem(m_creature->getVictim());
                SendItem_Timer = 5000;
            }else SendItem_Timer -= diff;
        }

        if (FrostShock_Timer < diff)
        {
            DoCast(m_creature->getVictim(),SPELL_FROST_SHOCK);
            FrostShock_Timer = 15000;
        }else FrostShock_Timer -= diff;

        if (AquaJet_Timer < diff)
        {
            DoCast(m_creature,SPELL_AQUA_JET);
            AquaJet_Timer = 15000;
        }else AquaJet_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mob_aquementas(Creature* pCreature)
{
    return new mob_aquementasAI (pCreature);
}

/*######
## npc_custodian_of_time
######*/

#define WHISPER_CUSTODIAN_1     -1000150
#define WHISPER_CUSTODIAN_2     -1000151
#define WHISPER_CUSTODIAN_3     -1000152
#define WHISPER_CUSTODIAN_4     -1000153
#define WHISPER_CUSTODIAN_5     -1000154
#define WHISPER_CUSTODIAN_6     -1000155
#define WHISPER_CUSTODIAN_7     -1000156
#define WHISPER_CUSTODIAN_8     -1000157
#define WHISPER_CUSTODIAN_9     -1000158
#define WHISPER_CUSTODIAN_10    -1000159
#define WHISPER_CUSTODIAN_11    -1000160
#define WHISPER_CUSTODIAN_12    -1000161
#define WHISPER_CUSTODIAN_13    -1000162
#define WHISPER_CUSTODIAN_14    -1000163

struct TRINITY_DLL_DECL npc_custodian_of_timeAI : public npc_escortAI
{
    npc_custodian_of_timeAI(Creature *c) : npc_escortAI(c) {}

    void WaypointReached(uint32 i)
    {
        Player *pPlayer = Unit::GetPlayer(PlayerGUID);
        if (!pPlayer)
            return;

        switch(i)
        {
            case 0: DoScriptText(WHISPER_CUSTODIAN_1, m_creature, pPlayer); break;
            case 1: DoScriptText(WHISPER_CUSTODIAN_2, m_creature, pPlayer); break;
            case 2: DoScriptText(WHISPER_CUSTODIAN_3, m_creature, pPlayer); break;
            case 3: DoScriptText(WHISPER_CUSTODIAN_4, m_creature, pPlayer); break;
            case 5: DoScriptText(WHISPER_CUSTODIAN_5, m_creature, pPlayer); break;
            case 6: DoScriptText(WHISPER_CUSTODIAN_6, m_creature, pPlayer); break;
            case 7: DoScriptText(WHISPER_CUSTODIAN_7, m_creature, pPlayer); break;
            case 8: DoScriptText(WHISPER_CUSTODIAN_8, m_creature, pPlayer); break;
            case 9: DoScriptText(WHISPER_CUSTODIAN_9, m_creature, pPlayer); break;
            case 10: DoScriptText(WHISPER_CUSTODIAN_4, m_creature, pPlayer); break;
            case 13: DoScriptText(WHISPER_CUSTODIAN_10, m_creature, pPlayer); break;
            case 14: DoScriptText(WHISPER_CUSTODIAN_4, m_creature, pPlayer); break;
            case 16: DoScriptText(WHISPER_CUSTODIAN_11, m_creature, pPlayer); break;
            case 17: DoScriptText(WHISPER_CUSTODIAN_12, m_creature, pPlayer); break;
            case 18: DoScriptText(WHISPER_CUSTODIAN_4, m_creature, pPlayer); break;
            case 22: DoScriptText(WHISPER_CUSTODIAN_13, m_creature, pPlayer); break;
            case 23: DoScriptText(WHISPER_CUSTODIAN_4, m_creature, pPlayer); break;
            case 24:
                DoScriptText(WHISPER_CUSTODIAN_14, m_creature, pPlayer);
                DoCast(pPlayer, 34883);
                // below here is temporary workaround, to be removed when spell works properly
                pPlayer->AreaExploredOrEventHappens(10277);
                break;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (IsBeingEscorted)
            return;

        if (who->GetTypeId() == TYPEID_PLAYER)
        {
            if (who->HasAura(34877) && CAST_PLR(who)->GetQuestStatus(10277) == QUEST_STATUS_INCOMPLETE)
            {
                float Radius = 10.0;
                if (m_creature->IsWithinDistInMap(who, Radius))
                {
                    Start(false, false, who->GetGUID());
                }
            }
        }
    }

    void EnterCombat(Unit* who) { }
    void Reset() { }

    void UpdateAI(const uint32 diff)
    {
        npc_escortAI::UpdateAI(diff);
    }
};

CreatureAI* GetAI_npc_custodian_of_time(Creature* pCreature)
{
    npc_custodian_of_timeAI* custodian_of_timeAI = new npc_custodian_of_timeAI(pCreature);

    custodian_of_timeAI->FillPointMovementListForCreature();

    return custodian_of_timeAI;
}

/*######
## npc_marin_noggenfogger
######*/

bool GossipHello_npc_marin_noggenfogger(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pCreature->isVendor() && pPlayer->GetQuestRewardStatus(2662))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    pPlayer->SEND_GOSSIP_MENU(pCreature->GetNpcTextId(), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_marin_noggenfogger(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetGUID());

    return true;
}

/*######
## npc_steward_of_time
######*/

#define GOSSIP_ITEM_FLIGHT  "Please take me to the master's lair."

bool GossipHello_npc_steward_of_time(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(10279) == QUEST_STATUS_INCOMPLETE || pPlayer->GetQuestRewardStatus(10279))
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FLIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
        pPlayer->SEND_GOSSIP_MENU(9978, pCreature->GetGUID());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(9977, pCreature->GetGUID());

    return true;
}

bool QuestAccept_npc_steward_of_time(Player* pPlayer, Creature *creature, Quest const *quest)
{
    if (quest->GetQuestId() == 10279)                      //Quest: To The Master's Lair
        pPlayer->CastSpell(pPlayer,34891,true);               //(Flight through Caverns)

    return false;
}

bool GossipSelect_npc_steward_of_time(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF + 1)
        pPlayer->CastSpell(pPlayer,34891,true);               //(Flight through Caverns)

    return true;
}

/*######
## npc_stone_watcher_of_norgannon
######*/

#define GOSSIP_ITEM_NORGANNON_1     "What function do you serve?"
#define GOSSIP_ITEM_NORGANNON_2     "What are the Plates of Uldum?"
#define GOSSIP_ITEM_NORGANNON_3     "Where are the Plates of Uldum?"
#define GOSSIP_ITEM_NORGANNON_4     "Excuse me? We've been \"reschedueled for visitations\"? What does that mean?!"
#define GOSSIP_ITEM_NORGANNON_5     "So, what's inside Uldum?"
#define GOSSIP_ITEM_NORGANNON_6     "I will return when i have the Plates of Uldum."

bool GossipHello_npc_stone_watcher_of_norgannon(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(2954) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(1674, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_stone_watcher_of_norgannon(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action)
{
    switch (action)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(1675, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(1676, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(1677, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
            pPlayer->SEND_GOSSIP_MENU(1678, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+4:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_NORGANNON_6, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+5);
            pPlayer->SEND_GOSSIP_MENU(1679, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+5:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->AreaExploredOrEventHappens(2954);
            break;
    }
    return true;
}

/*######
## npc_OOX17
######*/

#define Q_OOX17             648
#define SPAWN_FIRST         7803
#define SPAWN_SECOND_1      5617
#define SPAWN_SECOND_2      7805
#define SAY_SCOFF           -1060004
#define SAY_CHICKEN_ACC     -1060000
#define SAY_CHICKEN_AGGRO_1 -1060001
#define SAY_CHICKEN_AGGRO_2 -1060002
#define SAY_CHICKEN_AMB     -1060003
#define SAY_CHICKEN_COMP    -1060005

struct TRINITY_DLL_DECL npc_OOX17AI : public npc_escortAI
{
    npc_OOX17AI(Creature *c) : npc_escortAI(c) {}

    void WaypointReached(uint32 i)
    {
        Player* pPlayer = Unit::GetPlayer(PlayerGUID);

        if (!pPlayer)
            return;

        switch(i) {
            case 23:
                m_creature->SummonCreature(SPAWN_FIRST, -8350.96, -4445.79, 10.10, 6.20, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                m_creature->SummonCreature(SPAWN_FIRST, -8355.96, -4447.79, 10.10, 6.27, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                m_creature->SummonCreature(SPAWN_FIRST, -8353.96, -4442.79, 10.10, 6.08, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                DoScriptText(SAY_CHICKEN_AMB, m_creature);
                break;

            case 56:
                m_creature->SummonCreature(SPAWN_SECOND_1, -7510.07, -4795.50, 9.35, 6.06, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                m_creature->SummonCreature(SPAWN_SECOND_2, -7515.07, -4797.50, 9.35, 6.22, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                m_creature->SummonCreature(SPAWN_SECOND_2, -7518.07, -4792.50, 9.35, 6.22, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000);
                DoScriptText(SAY_CHICKEN_AMB, m_creature);
                if (Unit* scoff = me->FindNearestCreature(SPAWN_SECOND_2, 30))
                    DoScriptText(SAY_SCOFF, scoff);
                break;

            case 86:
                if (pPlayer)
                {
                    DoScriptText(SAY_CHICKEN_COMP, m_creature);
                    pPlayer->GroupEventHappens(Q_OOX17, m_creature);
                }
                break;
        }
    }

    void Reset(){}

    void EnterCombat(Unit* who)
    {
        switch (rand()%2)
        {
        case 0: DoScriptText(SAY_CHICKEN_AGGRO_1, m_creature); break;
        case 1: DoScriptText(SAY_CHICKEN_AGGRO_2, m_creature); break;
        }
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->AI()->AttackStart(m_creature);
    }
};

bool QuestAccept_npc_OOX17(Player* pPlayer, Creature* pCreature, Quest const* quest)
{
    if (quest->GetQuestId() == Q_OOX17)
    {
        pCreature->setFaction(113);
        pCreature->SetHealth(pCreature->GetMaxHealth());
        pCreature->SetUInt32Value(UNIT_FIELD_BYTES_1,0);
        pCreature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2);
        DoScriptText(SAY_CHICKEN_ACC, pCreature);

        if (npc_escortAI* pEscortAI = CAST_AI(npc_OOX17AI, pCreature->AI()))
            pEscortAI->Start(true, false, pPlayer->GetGUID());
    }
    return true;
}

CreatureAI* GetAI_npc_OOX17(Creature* pCreature)
{
    npc_OOX17AI* OOX17AI = new npc_OOX17AI(pCreature);

    OOX17AI->AddWaypoint(0, -8843.73, -4374.44, 43.71);
    OOX17AI->AddWaypoint(1, -8834.68, -4373.88, 45.71);
    OOX17AI->AddWaypoint(2, -8832.93, -4373.85, 45.67);
    OOX17AI->AddWaypoint(3, -8829.21, -4373.72, 44.14);
    OOX17AI->AddWaypoint(4, -8825.10, -4373.56, 41.44);
    OOX17AI->AddWaypoint(5, -8818.88, -4372.75, 36.43);
    OOX17AI->AddWaypoint(6, -8803.37, -4369.68, 30.06);
    OOX17AI->AddWaypoint(7, -8786.68, -4366.18, 23.91);
    OOX17AI->AddWaypoint(8, -8764.97, -4366.94, 25.23);
    OOX17AI->AddWaypoint(9, -8745.49, -4363.16, 22.80);
    OOX17AI->AddWaypoint(10, -8724.13, -4353.55, 20.72);
    OOX17AI->AddWaypoint(11, -8706.77, -4346.14, 16.12);
    OOX17AI->AddWaypoint(12, -8688.27, -4372.85, 13.64);
    OOX17AI->AddWaypoint(13, -8668.76, -4380.38, 11.69);
    OOX17AI->AddWaypoint(14, -8645.19, -4388.62, 12.56);
    OOX17AI->AddWaypoint(15, -8614.73, -4398.60, 9.86);
    OOX17AI->AddWaypoint(16, -8560.33, -4411.27, 13.17);
    OOX17AI->AddWaypoint(17, -8536.45, -4416.49, 11.84);
    OOX17AI->AddWaypoint(18, -8503.48, -4423.70, 13.59);
    OOX17AI->AddWaypoint(19, -8471.91, -4430.60, 9.56);
    OOX17AI->AddWaypoint(20, -8441.36, -4435.31, 9.40);
    OOX17AI->AddWaypoint(21, -8403.41, -4441.16, 11.83);
    OOX17AI->AddWaypoint(22, -8371.24, -4446.13, 9.47);
    OOX17AI->AddWaypoint(23, -8353.96, -4448.79, 10.10); //Scorpid
    OOX17AI->AddWaypoint(24, -8336.40, -4446.39, 8.98);
    OOX17AI->AddWaypoint(25, -8303.78, -4441.96, 11.89);
    OOX17AI->AddWaypoint(26, -8272.20, -4433.31, 9.60);
    OOX17AI->AddWaypoint(27, -8224.76, -4419.39, 13.03);
    OOX17AI->AddWaypoint(28, -8193.31, -4406.04, 10.17);
    OOX17AI->AddWaypoint(29, -8155.65, -4397.74, 8.99);
    OOX17AI->AddWaypoint(30, -8129.25, -4394.57, 10.92);
    OOX17AI->AddWaypoint(31, -8104.86, -4399.03, 8.93);
    OOX17AI->AddWaypoint(32, -8063.15, -4423.40, 10.07);
    OOX17AI->AddWaypoint(33, -8032.15, -4443.47, 9.97);
    OOX17AI->AddWaypoint(34, -8015.39, -4454.33, 9.39);
    OOX17AI->AddWaypoint(35, -7981.64, -4482.44, 10.32);
    OOX17AI->AddWaypoint(36, -7958.83, -4503.98, 9.69);
    OOX17AI->AddWaypoint(37, -7932.45, -4528.91, 10.08);
    OOX17AI->AddWaypoint(38, -7904.09, -4566.67, 12.59);
    OOX17AI->AddWaypoint(39, -7883.33, -4593.91, 12.15);
    OOX17AI->AddWaypoint(40, -7862.83, -4624.53, 10.21);
    OOX17AI->AddWaypoint(41, -7840.79, -4654.26, 9.45);
    OOX17AI->AddWaypoint(42, -7826.17, -4673.99, 10.61);
    OOX17AI->AddWaypoint(43, -7807.86, -4698.69, 11.24);
    OOX17AI->AddWaypoint(44, -7793.88, -4717.55, 10.48);
    OOX17AI->AddWaypoint(45, -7778.68, -4738.05, 8.89);
    OOX17AI->AddWaypoint(46, -7746.42, -4780.39, 9.84);
    OOX17AI->AddWaypoint(47, -7724.11, -4772.75, 10.28);
    OOX17AI->AddWaypoint(48, -7697.98, -4763.80, 9.52);
    OOX17AI->AddWaypoint(49, -7665.33, -4752.62, 10.56);
    OOX17AI->AddWaypoint(50, -7641.47, -4750.33, 8.94);
    OOX17AI->AddWaypoint(51, -7620.08, -4753.96, 8.93);
    OOX17AI->AddWaypoint(52, -7603.15, -4757.53, 9.06);
    OOX17AI->AddWaypoint(53, -7579.43, -4767.07, 8.93);
    OOX17AI->AddWaypoint(54, -7558.51, -4779.01, 9.64);
    OOX17AI->AddWaypoint(55, -7536.40, -4789.32, 8.92);
    OOX17AI->AddWaypoint(56, -7512.07, -4793.50, 9.35); //Wastewander
    OOX17AI->AddWaypoint(57, -7490.79, -4788.80, 10.53);
    OOX17AI->AddWaypoint(58, -7469.10, -4785.11, 10.42);
    OOX17AI->AddWaypoint(59, -7453.18, -4782.41, 9.15);
    OOX17AI->AddWaypoint(60, -7426.27, -4777.83, 9.54);
    OOX17AI->AddWaypoint(61, -7393.84, -4770.19, 12.57);
    OOX17AI->AddWaypoint(62, -7367.25, -4764.17, 11.92);
    OOX17AI->AddWaypoint(63, -7341.00, -4752.11, 10.17);
    OOX17AI->AddWaypoint(64, -7321.62, -4744.97, 11.58);
    OOX17AI->AddWaypoint(65, -7302.35, -4744.35, 11.97);
    OOX17AI->AddWaypoint(66, -7281.00, -4743.66, 11.21);
    OOX17AI->AddWaypoint(67, -7258.33, -4742.93, 9.64);
    OOX17AI->AddWaypoint(68, -7236.70, -4742.24, 10.16);
    OOX17AI->AddWaypoint(69, -7217.52, -4743.87, 10.79);
    OOX17AI->AddWaypoint(70, -7201.86, -4746.32, 9.58);
    OOX17AI->AddWaypoint(71, -7182.01, -4749.41, 9.09);
    OOX17AI->AddWaypoint(72, -7159.61, -4752.90, 9.52);
    OOX17AI->AddWaypoint(73, -7139.58, -4756.02, 9.53);
    OOX17AI->AddWaypoint(74, -7122.60, -4754.91, 9.66);
    OOX17AI->AddWaypoint(75, -7101.06, -4753.87, 8.92);
    OOX17AI->AddWaypoint(76, -7082.79, -4752.99, 9.97);
    OOX17AI->AddWaypoint(77, -7061.81, -4751.98, 9.26);
    OOX17AI->AddWaypoint(78, -7035.12, -4754.39, 9.19);
    OOX17AI->AddWaypoint(79, -7013.90, -4758.64, 10.28);
    OOX17AI->AddWaypoint(80, -7001.71, -4769.73, 10.59);
    OOX17AI->AddWaypoint(81, -6984.95, -4788.61, 9.30);
    OOX17AI->AddWaypoint(82, -6970.41, -4788.77, 9.42);
    OOX17AI->AddWaypoint(83, -6957.16, -4788.92, 6.26);
    OOX17AI->AddWaypoint(84, -6951.29, -4802.73, 4.45);
    OOX17AI->AddWaypoint(85, -6944.81, -4816.58, 1.60);
    OOX17AI->AddWaypoint(86, -6942.06, -4839.40, 0.66,5000);

    return OOX17AI;
}

/*####
# npc_tooga
####*/

enum
{
    SAY_TOOG_THIRST             = -1000391,
    SAY_TOOG_WORRIED            = -1000392,
    SAY_TOOG_POST_1             = -1000393,
    SAY_TORT_POST_2             = -1000394,
    SAY_TOOG_POST_3             = -1000395,
    SAY_TORT_POST_4             = -1000396,
    SAY_TOOG_POST_5             = -1000397,
    SAY_TORT_POST_6             = -1000398,

    QUEST_TOOGA                 = 1560,
    NPC_TORTA                   = 6015,

    POINT_ID_TO_WATER           = 1,
    FACTION_TOOG_ESCORTEE       = 113
};

const float m_afToWaterLoc[] = {-7032.664551, -4906.199219, -1.606446};

//Script not fully complete, need to change faction.
struct MANGOS_DLL_DECL npc_toogaAI : public ScriptedAI
{
    npc_toogaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiNpcFlags = pCreature->GetUInt32Value(UNIT_NPC_FLAGS);
        m_uiPlayerGUID = 0;
    }

    uint64 m_uiPlayerGUID;
    uint32 m_uiNpcFlags;
    uint32 m_uiCheckPlayerTimer;
    uint32 m_uiPostEventTimer;
    uint32 m_uiPhasePostEvent;

    Unit* pTorta;

    void Reset()
    {
        m_uiCheckPlayerTimer = 2500;
        m_uiPostEventTimer = 5000;
        m_uiPhasePostEvent = 0;

        pTorta = NULL;
    }

    void MoveInLineOfSight(Unit *pWho)
    {
        if (pWho->GetEntry() == NPC_TORTA)
        {
            if (m_creature->IsWithinDistInMap(pWho, INTERACTION_DISTANCE))
            {
                if (!pTorta && CanDoComplete())
                {
                    pTorta = pWho;
                    m_uiPhasePostEvent = 1;
                }
            }
        }
    }

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);
        m_creature->LoadCreaturesAddon();

        if (m_creature->isAlive())
        {
            if (Unit* pUnit = Unit::GetUnit(*m_creature, m_uiPlayerGUID))
            {
                //for later development, it appear this kind return to combatStart, then resume to MoveFollow
                m_creature->GetMotionMaster()->MoveFollow(pUnit, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
            }
            else
            {
                m_creature->GetMotionMaster()->MoveTargetedHome();
            }
        }

        m_creature->SetLootRecipient(NULL);

        Reset();
    }

    void JustRespawned()
    {
        if (m_creature->getFaction() != m_creature->GetCreatureInfo()->faction_A)
            m_creature->setFaction(m_creature->GetCreatureInfo()->faction_A);

        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, m_creature->GetCreatureInfo()->npcflag);

        Reset();
    }

    void DoStart(Player* pPlayer)
    {
        m_uiPlayerGUID = pPlayer->GetGUID();
        m_creature->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_NONE);

        //m_creature->setFaction(FACTION_TOOG_ESCORTEE);

        m_creature->GetMotionMaster()->MoveFollow(pPlayer, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
    }

    bool CanDoComplete()
    {
        if (Player* pPlayer = (Player*)Unit::GetUnit(*m_creature, m_uiPlayerGUID))
        {
            if (pPlayer->GetQuestStatus(QUEST_TOOGA) == QUEST_STATUS_INCOMPLETE)
            {
                uint16 uiQuestLogSlot = pPlayer->FindQuestSlot(QUEST_TOOGA);

                if (uiQuestLogSlot < MAX_QUEST_LOG_SIZE)
                {
                    if (pPlayer->GetQuestSlotState(uiQuestLogSlot) != QUEST_STATE_FAIL)
                    {
                        pPlayer->GroupEventHappens(QUEST_TOOGA, m_creature);
                        m_creature->GetMotionMaster()->MovementExpired();
                        return true;
                    }
                }
            }
        }

        return false;
    }

    void MovementInform(uint32 uiMoveType, uint32 uiPointId)
    {
        if (uiMoveType != POINT_MOTION_TYPE || !m_uiPlayerGUID)
            return;

        if (uiPointId == POINT_ID_TO_WATER)
            m_creature->ForcedDespawn();
    }

    void JustDied(Unit* pKiller)
    {
        if (Player* pPlayer = Unit::GetPlayer(m_uiPlayerGUID))
            pPlayer->FailQuest(QUEST_TOOGA);

        m_uiPlayerGUID = 0;
        m_creature->GetMotionMaster()->MovementExpired();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
        {
            //we are doing the post-event, or...
            if (m_uiPhasePostEvent)
            {
                if (m_uiPostEventTimer < uiDiff)
                {
                    m_uiPostEventTimer = 5000;

                    if (!pTorta || !pTorta->isAlive())
                    {
                        //something happened, so just despawn (what can go wrong?)
                        m_creature->ForcedDespawn();
                        return;
                    }

                    switch(m_uiPhasePostEvent)
                    {
                        case 1:
                            DoScriptText(SAY_TOOG_POST_1, m_creature);
                            break;
                        case 2:
                            DoScriptText(SAY_TORT_POST_2, pTorta);
                            break;
                        case 3:
                            DoScriptText(SAY_TOOG_POST_3, m_creature);
                            break;
                        case 4:
                            DoScriptText(SAY_TORT_POST_4, pTorta);
                            break;
                        case 5:
                            DoScriptText(SAY_TOOG_POST_5, m_creature);
                            break;
                        case 6:
                            DoScriptText(SAY_TORT_POST_6, pTorta);
                            m_creature->GetMotionMaster()->MovementExpired();
                            m_creature->GetMotionMaster()->MovePoint(POINT_ID_TO_WATER, m_afToWaterLoc[0], m_afToWaterLoc[1], m_afToWaterLoc[2]);
                            break;
                    }

                    ++m_uiPhasePostEvent;
                }
                else
                    m_uiPostEventTimer -= uiDiff;
            }
            //...we are doing regular player check
            else if (m_uiPlayerGUID)
            {
                if (m_uiCheckPlayerTimer < uiDiff)
                {
                    m_uiCheckPlayerTimer = 5000;

                    switch(rand()%50)
                    {
                        case 10: DoScriptText(SAY_TOOG_THIRST, m_creature); break;
                        case 25: DoScriptText(SAY_TOOG_WORRIED, m_creature); break;
                    }

                    Unit* pUnit = Unit::GetUnit(*m_creature, m_uiPlayerGUID);

                    if (pUnit && !pUnit->isAlive())
                    {
                        m_uiPlayerGUID = 0;
                        m_creature->ForcedDespawn();
                    }
                }
                else
                    m_uiCheckPlayerTimer -= uiDiff;
            }

            return;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_tooga(Creature* pCreature)
{
    return new npc_toogaAI(pCreature);
}

bool QuestAccept_npc_tooga(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_TOOGA)
    {
        if (npc_toogaAI* pToogaAI = CAST_AI(npc_toogaAI, pCreature->AI()))
            pToogaAI->DoStart(pPlayer);
    }

    return true;
}

void AddSC_tanaris()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="mob_aquementas";
    newscript->GetAI = &GetAI_mob_aquementas;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_custodian_of_time";
    newscript->GetAI = &GetAI_npc_custodian_of_time;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_marin_noggenfogger";
    newscript->pGossipHello =  &GossipHello_npc_marin_noggenfogger;
    newscript->pGossipSelect = &GossipSelect_npc_marin_noggenfogger;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_steward_of_time";
    newscript->pGossipHello =  &GossipHello_npc_steward_of_time;
    newscript->pGossipSelect = &GossipSelect_npc_steward_of_time;
    newscript->pQuestAccept =  &QuestAccept_npc_steward_of_time;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_stone_watcher_of_norgannon";
    newscript->pGossipHello =  &GossipHello_npc_stone_watcher_of_norgannon;
    newscript->pGossipSelect = &GossipSelect_npc_stone_watcher_of_norgannon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_OOX17";
    newscript->GetAI = &GetAI_npc_OOX17;
    newscript->pQuestAccept = &QuestAccept_npc_OOX17;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tooga";
    newscript->GetAI = &GetAI_npc_tooga;
    newscript->pQuestAccept = &QuestAccept_npc_tooga;
    newscript->RegisterSelf();
}

