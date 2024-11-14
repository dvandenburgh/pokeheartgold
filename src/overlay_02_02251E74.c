#include "overlay_2/overlay_02_02251E74.h"

#include "constants/sndseq.h"

#include "fieldmap.h"
#include "follow_mon.h"
#include "map_header.h"
#include "math_util.h"
#include "msgdata.h"
#include "overlay_01_021F467C.h"
#include "overlay_01_021F944C.h"
#include "overlay_01_022053EC.h"
#include "phonebook_dat.h"
#include "sav_system_info.h"
#include "save_local_field_data.h"
#include "save_vars_flags.h"
#include "script_pokemon_util.h"
#include "sound_radio.h"
#include "sys_flags.h"
#include "sys_vars.h"
#include "task.h"
#include "unk_0200FA24.h"
#include "unk_02062108.h"
#include "unk_020689C8.h"
#include "unk_02092BE8.h"

String *GetPhoneBookEntryName(GearPhoneRingManager *gearPhone, HeapID heapId) {
    String *str;
    if (!gearPhone->unk_var0_0 || gearPhone->unk_var2 >= 75) {
        str = String_New(8, heapId);
    } else {
        int phoneMsg     = GetPhoneMessageGmm(gearPhone->unk_var2);
        MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, phoneMsg, heapId);
        str              = NewString_ReadMsgData(msgData, 0);
        DestroyMsgData(msgData);
    }
    return str;
}

void ov02_02251EB8(GearPhoneRingManager *gearPhone, u8 a1, u8 a2, u8 a3, u8 a4, u8 a5) {
    if (a1 >= 75) {
        gearPhone->unk_var2    = 0xFF;
        gearPhone->unk_arr5[0] = 0xFF;
        gearPhone->unk_arr5[1] = 0;
        GF_ASSERT(FALSE);
    } else {
        gearPhone->unk_var2    = a1;
        gearPhone->unk_arr5[0] = a2;
        gearPhone->unk_arr5[1] = a3;
    }

    gearPhone->unk_var3 = a4;
    gearPhone->unk_var4 = a5;
}

u8 ov02_02251EE8(GearPhoneRingManager *gearPhone, u8 *a1) {
    MI_CpuFill8(a1, 0, 5);
    if (gearPhone->unk_var2 >= 75) {
        GF_ASSERT(FALSE);
        a1[0] = 0xFF;
        return 0xFF;
    }
    a1[0] = gearPhone->unk_arr5[0];
    a1[1] = gearPhone->unk_arr5[1];
    a1[3] = gearPhone->unk_var3;
    a1[4] = gearPhone->unk_var4;
    a1[2] = gearPhone->unk_var7;
    return gearPhone->unk_var2;
}

BOOL ov02_02251F20(GearPhoneRingManager *gearPhone) {
    Location *position = LocalFieldData_GetCurrentPosition(Save_LocalFieldData_Get(gearPhone->saveData));

    if (MapHeader_GetField14_1E(position->mapId) == 0) {
        return FALSE;
    }

    if (gearPhone->unk_var0_0 || gearPhone->unk_var8 < gearPhone->unk_varC) {
        return FALSE;
    }

    PhoneBook *phoneBook = AllocAndReadPhoneBook(HEAP_ID_4);
    u32 var              = ov02_022521C0(gearPhone, phoneBook, position->mapId);
    if (var) {
        FreePhoneBook(phoneBook);
        if (var == 2) {
            gearPhone->sys->unkD2_7 = 1;
            StartMapSceneScript(gearPhone->sys, 0x7FF, 0);
            return TRUE;
        }
        ov02_022522AC(gearPhone, 1);
        return FALSE;
    }
    u32 rand = MTRandom() % 0x64;

    if (rand > gearPhone->unk_var12) {
        FreePhoneBook(phoneBook);
        gearPhone->unk_var8 = 0;
        return FALSE;
    }

    var = ov02_02251FDC(gearPhone, phoneBook, position->mapId);
    FreePhoneBook(phoneBook);

    if (var == 0) {
        gearPhone->unk_var8 = 0;
    }
    return FALSE;
}

#ifdef NONMATCHING
// FIXME: https://decomp.me/scratch/KW1m7 down to stack swaps between bugContestFlag and var -adrienn
u32 ov02_02251FDC(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId) {
    u8 ret;
    u8 var;
    u8 bugContestFlag;
    PhoneContact *contact;

    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(gearPhone->saveData);
    Save_PlayerData_GetProfileAddr(gearPhone->saveData);
    SAVE_MISC_DATA *miscData = Save_Misc_Get(gearPhone->saveData);
    u32 slot                 = GSPlayerMisc_FindEmptyGearPhonebookSlot(gearPhone->pokegearData);
    contact                  = GSPlayerMisc_AllocAndCopyPhonebook(gearPhone->pokegearData, HEAP_ID_4);
    u8 *ptr                  = AllocFromHeapAtEnd(HEAP_ID_4, slot);
    MI_CpuFill8(ptr, 0xFF, slot);
    u16 rand = LCRandom() % 1000;

    if (rand < 500) {
        var = 0;
    } else if (rand < 800) {
        var = 1;
    } else {
        var = 2;
    }

    u8 index = 0;
    for (u8 i = 0; i < slot; i++) {
        u8 id = contact[i].id;
        if (phoneBook->entries[id].unkF == var) {
            // doesn't match with a switch statement
            if (phoneBook->entries[id].unk1 == 0 || phoneBook->entries[id].unk1 == 12 || phoneBook->entries[id].unk1 == 11 || phoneBook->entries[id].unk1 == 10 || phoneBook->entries[id].unk1 == 14) {
                ptr[index++] = id;
            }
        }
    }

    if (index == 0) {
        FreeToHeap(contact);
        FreeToHeap(ptr);
        return 0;
    }

    MI_CpuFill8(contact, 0xFF, slot);
    SysInfo_RTC *rtc = Save_SysInfo_RTC_Get(gearPhone->saveData);
    u8 hour          = rtc->time.hour;
    bugContestFlag   = Save_VarsFlags_CheckBugContestFlag(varsFlags);
    ret              = 0;

    if (sub_0202AAD4(miscData, var) == index) {
        sub_0202AA9C(miscData, var);
    }

    for (u8 j = 0; j < index; j++) {
        if (mapId == phoneBook->entries[ptr[j]].mapId || sub_0202AA44(miscData, var, ptr[j]) || ptr[j] == 8 && (sub_02095FF8(hour) || Save_VarsFlags_IsInRocketTakeover(varsFlags))) {
            continue;
        }
        if (phoneBook->entries[ptr[j]].mapId != 0x60 || !bugContestFlag) {
            contact[ret++].id = ptr[j];
        }
    }

    if (ret != 0) {
        slot = (MTRandom() % (ret * 100));
        rand = slot / 100;
        ov02_02251EB8(gearPhone, contact[(u8)rand].id, var, index, 0, 0);
        sub_02092DF4(gearPhone);
        ov02_022522AC(gearPhone, 1);
    }

    FreeToHeap(contact);
    FreeToHeap(ptr);

    return ret;
}
#else
// clang-format off
extern u32 _u32_div_f(u32 a1, u32 a2);
extern s32 _s32_div_f(s32 a1, s32 a2);
asm u32 ov02_02251FDC(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId) {
    push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x30
	str r0, [sp, #8]
	ldr r0, [r0, #0x38]
	add r6, r1, #0
	str r2, [sp, #0xc]
	bl Save_VarsFlags_Get
	str r0, [sp, #0x1c]
	ldr r0, [sp, #8]
	ldr r0, [r0, #0x38]
	bl Save_PlayerData_GetProfileAddr
	ldr r0, [sp, #8]
	ldr r0, [r0, #0x38]
	bl Save_Misc_Get
	str r0, [sp, #0x18]
	ldr r0, [sp, #8]
	ldr r0, [r0, #0x30]
	bl GSPlayerMisc_FindEmptyGearPhonebookSlot
	str r0, [sp, #0x2c]
	ldr r0, [sp, #8]
	mov r1, #4
	ldr r0, [r0, #0x30]
	bl GSPlayerMisc_AllocAndCopyPhonebook
	str r0, [sp, #0x20]
	ldr r1, [sp, #0x2c]
	mov r0, #4
	bl AllocFromHeapAtEnd
	ldr r2, [sp, #0x2c]
	mov r1, #0xff
	add r4, r0, #0
	bl MI_CpuFill8
	bl LCRandom
	mov r1, #0xfa
	lsl r1, r1, #2
	bl _s32_div_f
	lsl r0, r1, #0x10
	lsr r1, r0, #0x10
	mov r0, #0x7d
	lsl r0, r0, #2
	cmp r1, r0
	bhs _02252046
	mov r0, #0
	str r0, [sp, #0x24]
	b _02252058
_02252046:
	mov r0, #0x32
	lsl r0, r0, #4
	cmp r1, r0
	bhs _02252054
	mov r0, #1
	str r0, [sp, #0x24]
	b _02252058
_02252054:
	mov r0, #2
	str r0, [sp, #0x24]
_02252058:
	ldr r0, [sp, #0x2c]
	mov r7, #0
	add r2, r7, #0
	cmp r0, #0
	bls _022520A2
_02252062:
	ldr r0, [sp, #0x20]
	mov r1, #0x14
	ldrb r3, [r0, r2]
	ldr r0, [r6, #4]
	mul r1, r3
	add r5, r0, r1
	ldrb r1, [r5, #0xf]
	ldr r0, [sp, #0x24]
	cmp r0, r1
	bne _02252096
	ldrb r0, [r5, #1]
	cmp r0, #0
	beq _0225208C
	cmp r0, #0xc
	beq _0225208C
	cmp r0, #0xb
	beq _0225208C
	cmp r0, #0xa
	beq _0225208C
	cmp r0, #0xe
	bne _02252096
_0225208C:
	add r1, r7, #1
	add r0, r7, #0
	lsl r1, r1, #0x18
	lsr r7, r1, #0x18
	strb r3, [r4, r0]
_02252096:
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	ldr r0, [sp, #0x2c]
	cmp r2, r0
	blo _02252062
_022520A2:
	cmp r7, #0
	bne _022520B8
	ldr r0, [sp, #0x20]
	bl FreeToHeap
	add r0, r4, #0
	bl FreeToHeap
	add sp, #0x30
	mov r0, #0
	pop {r3, r4, r5, r6, r7, pc}
_022520B8:
	ldr r0, [sp, #0x20]
	ldr r2, [sp, #0x2c]
	mov r1, #0xff
	bl MI_CpuFill8
	ldr r0, [sp, #8]
	ldr r0, [r0, #0x38]
	bl Save_SysInfo_RTC_Get
	ldr r0, [r0, #0x14]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x14]
	ldr r0, [sp, #0x1c]
	bl Save_VarsFlags_CheckBugContestFlag
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x10]
	mov r0, #0
	str r0, [sp, #0x28]
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #0x24]
	bl sub_0202AAD4
	cmp r0, r7
	bne _022520F6
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #0x24]
	bl sub_0202AA9C
_022520F6:
	mov r5, #0
	cmp r7, #0
	bls _02252164
_022520FC:
	ldrb r2, [r4, r5]
	mov r1, #0x14
	ldr r0, [r6, #4]
	mul r1, r2
	add r0, r0, r1
	ldrh r1, [r0, #6]
	ldr r0, [sp, #0xc]
	cmp r0, r1
	beq _0225215A
	ldr r0, [sp, #0x18]
	ldr r1, [sp, #0x24]
	bl sub_0202AA44
	cmp r0, #0
	bne _0225215A
	ldrb r0, [r4, r5]
	cmp r0, #8
	bne _02252134
	ldr r0, [sp, #0x14]
	bl sub_02095FF8
	cmp r0, #0
	bne _0225215A
	ldr r0, [sp, #0x1c]
	bl Save_VarsFlags_IsInRocketTakeover
	cmp r0, #0
	bne _0225215A
_02252134:
	ldrb r1, [r4, r5]
	mov r2, #0x14
	ldr r0, [r6, #4]
	mul r2, r1
	add r0, r0, r2
	ldrh r0, [r0, #6]
	cmp r0, #0x60
	bne _0225214A
	ldr r0, [sp, #0x10]
	cmp r0, #0
	bne _0225215A
_0225214A:
	ldr r0, [sp, #0x28]
	add r2, r0, #0
	add r2, r2, #1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	str r2, [sp, #0x28]
	ldr r2, [sp, #0x20]
	strb r1, [r2, r0]
_0225215A:
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, r7
	blo _022520FC
_02252164:
	ldr r0, [sp, #0x28]
	cmp r0, #0
	beq _022521AC
	bl MTRandom
	ldr r2, [sp, #0x28]
	mov r1, #0x64
	mul r1, r2
	bl _u32_div_f
	add r0, r1, #0
	mov r1, #0x64
	bl _u32_div_f
	add r1, r0, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r0, #0
	lsl r1, r1, #0x18
	str r0, [sp]
	lsr r2, r1, #0x18
	str r0, [sp, #4]
	ldr r1, [sp, #0x20]
	ldr r0, [sp, #8]
	ldrb r1, [r1, r2]
	ldr r2, [sp, #0x24]
	add r3, r7, #0
	bl ov02_02251EB8
	ldr r0, [sp, #8]
	bl sub_02092DF4
	ldr r0, [sp, #8]
	mov r1, #1
	bl ov02_022522AC
_022521AC:
	ldr r0, [sp, #0x20]
	bl FreeToHeap
	add r0, r4, #0
	bl FreeToHeap
	ldr r0, [sp, #0x28]
	add sp, #0x30
	pop {r3, r4, r5, r6, r7, pc}
}
// clang-format on
#endif

typedef struct UnkStruct_02253C86 {
    u8 unk0;
    u8 unk1;
    u16 unk2;
    u8 unk4;
    u8 unk5;
} UnkStruct_02253C86;

extern const UnkStruct_02253C86 ov02_02253C84[];

u32 ov02_022521C0(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId) {
    u8 r6 = ov02_02252218(gearPhone, phoneBook, mapId);
    if (r6 == 0xFF) {
        return FALSE;
    }
    ov02_02251EB8(gearPhone, ov02_02253C84[r6].unk0, 0xFF, 0, 3, ov02_02253C84[r6].unk2);
    sub_02092DF4(gearPhone);
    gearPhone->unk_var7 = r6;
    return ov02_02253C84[r6].unk4 + 1;
}

// FIXME: This is a fakematch from decomp.me, it doesn't match locally without the label https://decomp.me/scratch/YdDak
u8 ov02_02252218(GearPhoneRingManager *gearPhone, PhoneBook *phoneBook, u32 mapId) {
    u8 *ptr = AllocFromHeapAtEnd(HEAP_ID_4, 13);
    MI_CpuFill8(ptr, 0, 13);

    int cnt = 0;
    for (int i = 0; i < 13; i++) {
        if (sub_0202F08C(gearPhone->savingsData, i) == 0) {
            continue;
        }
        PhoneBookEntry *entry = &phoneBook->entries[ov02_02253C84[i].unk0];
        if (entry->unk0 == 6) {
            if (GSPlayerMisc_IsGearNumberRegistered(gearPhone->pokegearData, 6) != 0xFF) {
                // had to do this to match
                goto LABEL;
            } else {
                continue;
            }
        } else if (entry->mapId == mapId) {
            continue;
        }
LABEL:
        ptr[cnt++] = i;
    }

    if (cnt == 0) {
        FreeToHeap(ptr);
        return 0xFF;
    }

    u8 ret = ptr[LCRandom() % cnt];
    FreeToHeap(ptr);
    return ret;
}

void ov02_022522AC(GearPhoneRingManager *gearPhone, BOOL a1) {
    GearPhone_ToggleRinging(gearPhone, a1);
}

int GetRadioMusicPlayingSeq(void) {
    u16 seq = SndRadio_GetSeqNo();
    switch (seq) {
    case SEQ_GS_RADIO_MARCH:
    case SEQ_GS_P_RADIO_MARCH:
        return 1;
    case SEQ_GS_RADIO_KOMORIUTA:
    case SEQ_GS_P_RADIO_KOMORIUTA:
        return 2;
    case SEQ_GS_RADIO_R_101:
        return 3;
    case SEQ_GS_RADIO_R_201:
        return 4;
    case SEQ_GS_RADIO_UNKNOWN:
        return 6;
    case SEQ_GS_HUE:
    case SEQ_GS_P_HUE:
        return 5;
    }
    return 0;
}

BOOL ov02_02252334(FieldSystem *fieldSystem) {
    if (!MapObject_AreBitsSetForMovementScriptInit(PlayerAvatar_GetMapObject(fieldSystem->playerAvatar))) {
        return FALSE;
    }
    if (FollowMon_IsActive(fieldSystem) && fieldSystem->followMon.species == SPECIES_SHAYMIN) {
        Pokemon *mon = GetFirstAliveMonInParty_CrashIfNone(SaveArray_Party_Get(fieldSystem->saveData));
        u32 species  = GetMonData(mon, MON_DATA_SPECIES, NULL);
        u32 form     = GetMonData(mon, MON_DATA_FORM, NULL);
        GF_ASSERT(species == fieldSystem->followMon.species);
        // yes, this is explicitly checked twice
        if (species != fieldSystem->followMon.species) {
            return FALSE;
        }
        if (fieldSystem->followMon.form == SHAYMIN_SKY && form == SHAYMIN_LAND) {
            return TRUE;
        }
    }
    return FALSE;
}

void ov02_022523B4(TaskManager *taskMan) {
    void *data = AllocFromHeapAtEnd(HEAP_ID_FIELD, 12);
    TaskManager_Call(taskMan, ov02_022523D0, data);
}

typedef struct UnkStruct_022523D0 {
    int unk0;
    u32 unk4;
    u32 unk8;
} UnkStruct_022523D0;

BOOL ov02_022523D0(TaskManager *taskMan) {
    int *state = TaskManager_GetStatePtr(taskMan);
    VecFx32 pos;
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    UnkStruct_022523D0 *env  = TaskManager_GetEnvironment(taskMan);
    switch (*state) {
    case 0:
        MapObject_UnpauseMovement(FollowMon_GetMapObject(fieldSystem));
        TaskManager_Call(fieldSystem->taskman, ov01_02205A60, NULL);
        (*state)++;
        break;
    case 1:
        MapObject_GetPositionVec(FollowMon_GetMapObject(fieldSystem), &pos);
        u32 res;
        if (FollowMon_IsVisible(fieldSystem)) {
            env->unk4 = ov01_022052C4(fieldSystem->unk_44, &pos);
            res       = 1;
        } else {
            env->unk4 = 0;
            res       = 0;
        }
        env->unk8 = res;
        env->unk0 = 0;
        FollowMon_SetObjectParams(fieldSystem->followMon.mapObject, SPECIES_SHAYMIN, 0, fieldSystem->followMon.shiny);
        (*state)++;
        break;
    case 2:
        if (env->unk8 != 0) {
            if (++env->unk0 >= 4) {
                ov01_021FA930(fieldSystem->followMon.mapObject, 974);
                (*state)++;
            }
            break;
        }
        MapObjectManager *mapObjectMan = MapObject_GetManager(fieldSystem->followMon.mapObject);
        u32 spriteId                   = MapObject_GetSpriteID(fieldSystem->followMon.mapObject);
        sub_0205E420(fieldSystem->followMon.mapObject);
        ov01_021FA108(mapObjectMan, spriteId, fieldSystem->followMon.mapObject);
        sub_0205E38C(fieldSystem->followMon.mapObject, 974);
        (*state)++;
        break;
    case 3:
        if (ov01_021FA2D4(fieldSystem->followMon.mapObject) == 0) {
            fieldSystem->followMon.form = 0;
            (*state)++;
        }
        break;
    case 4:
        u32 var = 0;
        if (env->unk4 == 0) {
            var = 1;
        } else if (ov01_022052F4()) {
            sub_02068B48(env->unk4);
            var = 1;
        }
        if (var != 0) {
            FreeToHeap(env);
            MapObject_PauseMovement(FollowMon_GetMapObject(fieldSystem));
            return TRUE;
        }
        break;
    }
    return FALSE;
}

void FieldSystem_BeginCelebiTimeTravelCutsceneTask(FieldSystem *fieldSystem) {
    CelebiTimeTravelCutsceneTaskData *ptr = AllocFromHeapAtEnd(HEAP_ID_FIELD, sizeof(CelebiTimeTravelCutsceneTaskData));
    MI_CpuFill8(ptr, 0, sizeof(CelebiTimeTravelCutsceneTaskData));
    ptr->fieldSystem = fieldSystem;
    TaskManager_Call(fieldSystem->taskman, Task_CelebiTimeTravelCutscene, ptr);
}

BOOL Task_CelebiTimeTravelCutscene(TaskManager *taskMan) {
    int *state               = TaskManager_GetStatePtr(taskMan);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    CelebiTimeTravelCutsceneTaskData *data = TaskManager_GetEnvironment(taskMan);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(3, 0, 0, RGB_WHITE, 4, 1, HEAP_ID_4);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            CelebiCutscene_LoadResources(data);
            sub_02069DC8(FollowMon_GetMapObject(data->fieldSystem), 1);
            BeginNormalPaletteFade(3, 1, 0, RGB_WHITE, 4, 1, HEAP_ID_4);
            (*state)++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            sub_0205F484(FollowMon_GetMapObject(data->fieldSystem));
            data->unkF0 = 1;
            (*state)++;
        }
        break;
    case 3:
        if (++data->unkF2 >= 30) {
            CelebiCutscene_StartSwirlTask(data);
            data->unkF2 = 0;
            (*state)++;
        }
        break;
    case 4:
        if (++data->unkF2 >= 80) {
            BeginNormalPaletteFade(0, 0, 0, RGB_WHITE, 30, 1, HEAP_ID_4);
            (*state)++;
        }
        break;
    case 5:
        if (IsPaletteFadeFinished() && ov02_022526EC(data)) {
            CelebiCutscene_UnloadResources(data);
            ov01_021F46DC(&data->unk4);
            FreeToHeap(data);
            return TRUE;
        }
    }
    if (data->unkF0) {
        CelebiCutsceneAnimations_FrameAdvanceAndCheck(&data->animations[0]);
    }
    if (data->unkF1) {
        Field3dObject_Draw(&data->object3d);
    }

    return FALSE;
}

void CelebiCutscene_StartSwirlTask(CelebiTimeTravelCutsceneTaskData *data) {
    data->unk4 = ov01_021F467C(3, 13);
    SysTask_CreateOnMainQueue(Task_CelebiCutsceneSwirl, data, 0);
    data->unk2 = 0;
}

BOOL ov02_022526EC(CelebiTimeTravelCutsceneTaskData *data) {
    return data->unk2 != 0;
}

extern const UnkStruct_02253CE0 ov02_02253CE0;

void Task_CelebiCutsceneSwirl(SysTask *task, void *_data) {
    CelebiTimeTravelCutsceneTaskData *data = _data;
    switch (data->unkEE) {
    case 0:
        UnkStruct_02253CE0 unk = ov02_02253CE0;
        unk.unk8               = Camera_GetPerspectiveAngle(data->fieldSystem->camera);
        CelebiCutscene_InitSwirlData(data, &unk);
        data->unkEE++;
        break;
    case 1:
        if (CelebiCutscene_IsSwirlFinished(data)) {
            data->unk2 = 1;
            SysTask_Destroy(task);
        }
    }
}

void CelebiCutscene_InitSwirlData(CelebiTimeTravelCutsceneTaskData *data, UnkStruct_02253CE0 *a1) {
    data->isSwirlFinished = FALSE;
    data->unkE6           = 0;
    data->unkE0           = a1->unk0;
    data->unkE4           = data->unk0;
    data->perspective     = Camera_GetPerspectiveAngle(data->fieldSystem->camera);
    data->unkEC           = a1->unk8;
    data->unkE8           = a1->unk4;
    SysTask_CreateOnMainQueue(Task_CelebiCutsceneSwirlEffect, data, 1);
}

extern const VecFx32 ov02_02253CD4;

BOOL CelebiCutscene_SwirlEffect(CelebiTimeTravelCutsceneTaskData *data) {
    VecFx32 vec;
    MtxFx33 rotMatrix;
    VecFx32 vec2   = ov02_02253CD4;
    Camera *camera = data->fieldSystem->camera;

    data->unkE6++;
    int e6 = data->unkE6;
    int e8 = data->unkE8;

    int angle = data->unkEC - data->perspective;
    angle *= e6;
    angle /= e8;
    angle += data->perspective;
    u16 index = ((data->unkE0 * e6) / e8) + data->unkE4;
    MTX_RotZ33(&rotMatrix, FX_SinIdx(index), FX_CosIdx(index));
    MTX_MultVec33(&vec2, &rotMatrix, &vec);
    Camera_SetLookAtCamUp(&vec, camera);
    Camera_SetPerspectiveAngle(angle, camera);
    return data->unkE6 >= data->unkE8;
}

void Task_CelebiCutsceneSwirlEffect(SysTask *task, void *_data) {
    CelebiTimeTravelCutsceneTaskData *data = _data;
    if (CelebiCutscene_SwirlEffect(data)) {
        data->isSwirlFinished = TRUE;
        SysTask_Destroy(task);
    }
}

BOOL CelebiCutscene_IsSwirlFinished(CelebiTimeTravelCutsceneTaskData *data) {
    return data->isSwirlFinished != FALSE;
}

extern const u32 sCelebiCutsceneAnimationFiles[3]; // file ids 76-78

void CelebiCutscene_LoadResources(CelebiTimeTravelCutsceneTaskData *data) {
    u32 files[3];
    files = sCelebiCutsceneAnimationFiles;
    GF_ExpHeap_FndInitAllocator(&data->alloc, HEAP_ID_4, 32);
    Field3dModel_LoadFromFilesystem(&data->model, NARC_demo_legend, 75, HEAP_ID_4);
    for (u8 i = 0; i < 3; i++) {
        Field3dModelAnimation_LoadFromFilesystem(&data->animations[i], &data->model, NARC_demo_legend, files[i], HEAP_ID_4, &data->alloc);
    }
    Field3dObject_InitFromModel(&data->object3d, &data->model);
    for (u8 i = 0; i < 3; i++) {
        Field3dObject_AddAnimation(&data->object3d, &data->animations[i]);
    }
    Field3dObject_SetActiveFlag(&data->object3d, 1);
    CelebiCutsceneAnimations_FrameSet(&data->animations[0], 0);
    LocalMapObject *followMonObj = FollowMon_GetMapObject(data->fieldSystem);
    VecFx32 pos;
    MapObject_GetPositionVec(followMonObj, &pos);
    Field3dObject_SetPosEx(&data->object3d, pos.x, pos.y, pos.z);
    data->unkF1 = 1;
}

void CelebiCutscene_UnloadResources(CelebiTimeTravelCutsceneTaskData *data) {
    for (u8 i = 0; i < 3; i++) {
        Field3dModelAnimation_Unload(&data->animations[i], &data->alloc);
    }
    Field3dModel_Unload(&data->model);
}

void CelebiCutsceneAnimations_FrameSet(Field3DModelAnimation *animations, u32 frame) {
    for (u8 i = 0; i < 3; i++) {
        Field3dModelAnimation_FrameSet(&animations[i], frame);
    }
}

BOOL CelebiCutsceneAnimations_FrameAdvanceAndCheck(Field3DModelAnimation *animations) {
    u8 i;
    u8 cnt = 0;
    for (i = 0; i < 3; i++) {
        if (Field3dModelAnimation_FrameAdvanceAndCheck(&animations[i], 0x1000) != 0) {
            cnt++;
        }
    }

    return cnt == 3;
}

void FieldSystem_BeginSinjohCutsceneTask(FieldSystem *fieldSystem) {
    SinjohCutsceneData *data = AllocFromHeapAtEnd(HEAP_ID_FIELD, sizeof(SinjohCutsceneData));
    MI_CpuFill8(data, 0, sizeof(SinjohCutsceneData));
    data->fieldSystem = fieldSystem;
    TaskManager_Call(fieldSystem->taskman, Task_SinjohCutscene, data);
}

BOOL Task_SinjohCutscene(TaskManager *taskMan) {
    int *state               = TaskManager_GetStatePtr(taskMan);
    FieldSystem *fieldSystem = TaskManager_GetFieldSystem(taskMan);
    SinjohCutsceneData *data = TaskManager_GetEnvironment(taskMan);
    switch (*state) {
    case 0:
        BeginNormalPaletteFade(3, 0, 0, RGB_WHITE, 2, 1, HEAP_ID_4);
        (*state)++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            SinjohCutscene_LoadResources(data);
            BeginNormalPaletteFade(3, 1, 0, RGB_WHITE, 2, 1, HEAP_ID_4);
            (*state)++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            data->unk224 = 1;
            data->unk227 = 0;
            Field3dObject_SetActiveFlag(&data->object1AC, 1);
            (*state)++;
        }
        break;
    case 3:
        if (++data->unk227 >= 100) {
            data->unk225 = 1;
            data->unk227 = 0;
            (*state)++;
        }
        break;
    case 4:
        if (++data->unk227 >= 150) {
            BeginNormalPaletteFade(3, 0, 0, RGB_WHITE, 2, 1, HEAP_ID_4);
            (*state)++;
        }
        break;
    case 5:
        if (IsPaletteFadeFinished()) {
            SinjohCutscene_FreeResources(data);
            FreeToHeap(data);
            return TRUE;
        }
        break;
    }
    if (data->unk224) {
        ov02_02252EA8(&data->animations[0], 2);
        ov02_02252EA8(&data->animations[2], 2);
    }
    if (data->unk225) {
        ov02_02252EA8(&data->animation184, 2);
    }
    if (data->unk226) {
        Field3dObject_Draw(&data->object84);
        Field3dObject_Draw(&data->objectFC);
        Field3dObject_Draw(&data->object1AC);
    }
    return FALSE;
}
