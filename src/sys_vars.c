#include "constants/species.h"
#include "friend_group.h"
#include "math_util.h"
#include "sys_vars.h"

static const u16 _020FE4A4[] = {
    0x06F2,
    0xAD7C,
};

static const u16 _020FE4A8[] = {
    0x6208,
    0xF229,
    0x0382,
    0x1228,
};

BOOL SetScriptVar(SCRIPT_STATE* state, u16 var_id, u16 value) {
    u16* var_ptr = GetVarAddr(state, var_id);
    if (var_id < VAR_BASE || var_id > SPECIAL_VAR_BASE) {
        GF_ASSERT(FALSE);
        return FALSE;
    }

    if (var_ptr == NULL) {
        return FALSE;
    }

    *var_ptr = value;
    return TRUE;
}

u16 GetScriptVar(SCRIPT_STATE* state, u16 var_id) {
    u16* var_ptr = GetVarAddr(state, var_id);
    if (var_ptr == NULL) {
        return 0;
    }

    return *var_ptr;
}

void ScriptState_SetFollowerTrainerNum(SCRIPT_STATE* state, u16 trainer_num) {
    SetScriptVar(state, VAR_FOLLOWER_TRAINER_NUM, trainer_num);
}

u16 ScriptState_GetFollowerTrainerNum(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_FOLLOWER_TRAINER_NUM);
}

void SetStarterToScriptState(SCRIPT_STATE* state, u16 starter) {
    SetScriptVar(state, VAR_PLAYER_STARTER, starter);
}

u16 GetStarterFromScriptState(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_PLAYER_STARTER);
}

// This will always return Turtwig.
u16 DPPtLeftover_GetRivalSpecies(SCRIPT_STATE* state) {
    u16 rival_starter_species;

    u16 player_starter_species = GetScriptVar(state, VAR_PLAYER_STARTER);
    if (player_starter_species == SPECIES_TURTWIG) {
        rival_starter_species = SPECIES_CHIMCHAR;
    } else {
        rival_starter_species = SPECIES_TURTWIG;
        if (player_starter_species == SPECIES_CHIMCHAR) {
            rival_starter_species = SPECIES_PIPLUP;
        }
    }

    return rival_starter_species;
}

// This will always return Chimchar.
u16 DPPtLeftover_GetFriendStarterSpecies(SCRIPT_STATE* state) {
    u16 friend_starter_species;

    u16 player_starter_species = GetScriptVar(state, VAR_PLAYER_STARTER);
    if (player_starter_species == SPECIES_TURTWIG) {
        friend_starter_species = SPECIES_PIPLUP;
    } else {
        friend_starter_species = SPECIES_TURTWIG;
        if (player_starter_species != SPECIES_CHIMCHAR) {
            friend_starter_species = SPECIES_CHIMCHAR;
        }
    }

    return friend_starter_species;
}

u16 ScriptState_GetFishingCompetitionLengthRecord(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_MAGIKARP_SIZE_RECORD);
}

void ScriptState_SetFishingCompetitionLengthRecord(SCRIPT_STATE* state, u16 record) {
    SetScriptVar(state, VAR_MAGIKARP_SIZE_RECORD, record);
}

u16 sub_02066B70(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_UNOWN_REPORT_LEVEL);
}

u16 sub_02066B80(s32 a0) {
    GF_ASSERT(a0 >= 0 && (u32)a0 < NELEMS(_020FE4A4));
    return _020FE4A4[a0];
}

void sub_02066B9C(SCRIPT_STATE* state, u32 a1) {
    SetScriptVar(state, VAR_UNK_4043 + a1, sub_02066B80(a1));
}

BOOL sub_02066BC0(SCRIPT_STATE* state, u32 a1) {
    u16 var1 = GetScriptVar(state, VAR_UNK_4043 + a1);
    u16 var2 = sub_02066B80(a1);
    return var1 == var2;
}

void sub_02066BE8(SCRIPT_STATE* state, u32 a1, u16 value) {
    if (a1 < NELEMS(_020FE4A8)) {
        SetScriptVar(state, VAR_UNK_4045 + a1, value);
    }
}

u32 sub_02066C00(s32 a0) {
    GF_ASSERT(a0 >= 0 && a0 < (s32)NELEMS(_020FE4A8));
    return _020FE4A8[a0];
}

void sub_02066C1C(SCRIPT_STATE* state, s32 a1) {
    GF_ASSERT(a1 >= 0 && a1 < (s32)NELEMS(_020FE4A8));
    SetScriptVar(state, VAR_UNK_4036 + a1, sub_02066C00(a1));
}

void sub_02066C4C(SCRIPT_STATE* state, s32 a1) {
    GF_ASSERT(a1 >= 0 && a1 < (s32)NELEMS(_020FE4A8));
    SetScriptVar(state, VAR_UNK_4036 + a1, 0);
}

BOOL sub_02066C74(SCRIPT_STATE* state, s32 a1) {
    GF_ASSERT(a1 >= 0 && a1 < (s32)NELEMS(_020FE4A8));
    u16 var1 = GetScriptVar(state, VAR_UNK_4036 + a1);
    u32 var2 = sub_02066C00(a1);
    return var1 == var2;
}

void sub_02066CAC(SCRIPT_STATE* state, u32 id) {
    SetScriptVar(state, VAR_LOTO_NUMBER_LO, id & 0xFFFF);
#ifdef BUGFIX_LOTO_NUMBER_HI
    SetScriptVar(state, VAR_LOTO_NUMBER_HI, id >> 16);
#else
    SetScriptVar(state, VAR_LOTO_NUMBER_LO, id >> 16);
#endif
}

u32 ScriptState_GetLotoId(SCRIPT_STATE* state) {
    u16 lo = GetScriptVar(state, VAR_LOTO_NUMBER_LO);
    u16 hi = GetScriptVar(state, VAR_LOTO_NUMBER_HI);

    return hi << 16 | lo;
}

void ScriptState_RollLotoId(SCRIPT_STATE* state) {
    u16 lo = LCRandom();
    u16 hi = LCRandom();

    sub_02066CAC(state, hi << 16 | lo);
}

void Save_LCRNGAdvanceLotoID(SAVEDATA* savedata) {
    SCRIPT_STATE* state = SavArray_Flags_get(savedata);
    SAV_FRIEND_GRP* friend_groups = sub_0202C854(savedata);
    u32 rand_id = sub_0202C7DC(friend_groups) * 0x41C64E6D + 12345;

    sub_02066CAC(state, rand_id);
}

u16 sub_02066D40(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_UNK_4041);
}

void sub_02066D50(SCRIPT_STATE* state, u16 value) {
    SetScriptVar(state, VAR_UNK_4041, value);
}

void sub_02066D60(SAVEDATA* savedata) {
    SCRIPT_STATE* state = SavArray_Flags_get(savedata);
    u32 rand = LCRandom() % 98;

    sub_02066D50(state, rand + 2);
}

void sub_02066D80(SCRIPT_STATE* state) {
    u16 var = GetScriptVar(state, VAR_UNK_4042);
    u16 unk_value = 10000;
    if (var < 10000) {
        unk_value = var + 1;
    }

    SetScriptVar(state, VAR_UNK_4042, unk_value);
}

u16 sub_02066DA8(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_UNK_4042);
}

u16 sub_02066DB8(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_UNK_404B);
}

void sub_02066DC8(SCRIPT_STATE* state, u16 value) {
    SetScriptVar(state, VAR_UNK_404B, value);
}

u16 sub_02066DD8(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_BATTLE_FACTORY_PRINT_PROGRESS);
}

u16 sub_02066DE8(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_BATTLE_HALL_PRINT_PROGRESS);
}

u16 sub_02066DF8(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_BATTLE_CASTLE_PRINT_PROGRESS);
}

u16 sub_02066E08(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_BATTLE_ARCADE_PRINT_PROGRESS);
}

u16 sub_02066E18(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_BATTLE_TOWER_PRINT_PROGRESS);
}

u16 sub_02066E28(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_UNK_404C);
}

void sub_02066E38(SCRIPT_STATE* state, u16 value) {
    SetScriptVar(state, VAR_UNK_404C, value);
}

u16 sub_02066E48(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_UNK_4052);
}

BOOL sub_02066E58(SCRIPT_STATE* state) {
    u16 var = GetScriptVar(state, VAR_SCENE_ROCKET_TAKEOVER);
    if (var < 2 || var > 4) {
        return FALSE;
    }

    return TRUE;
}

u16 sub_02066E74(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_UNK_4057);
}

void sub_02066E84(SCRIPT_STATE* state, u16 value) {
    SetScriptVar(state, VAR_UNK_4057, value);
}

void ScriptState_UpdateBuenasPasswordSet(SCRIPT_STATE* state) {
    u16 set = GetScriptVar(state, VAR_BUENAS_PASSWORD_SET);
    u16 new_set = LCRandom() % 30;

    if (set == new_set) {
        new_set = (new_set + 1) % 30;
    }

    SetScriptVar(state, VAR_BUENAS_PASSWORD_SET, new_set);
}

u16 ScriptState_GetBuenasPasswordSet(SCRIPT_STATE* state) {
    return GetScriptVar(state, VAR_BUENAS_PASSWORD_SET);
}
