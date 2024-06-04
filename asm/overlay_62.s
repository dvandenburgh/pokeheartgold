	.include "asm/macros.inc"
	.include "overlay_62.inc"
	.include "global.inc"
	.public gApplication_TitleScreen

	.text

    .public MicTest_Init
    .public MicTest_Main
    .public MicTest_Exit
    .public ov62_021E5A84
    .public ov62_021E5A8C
    .public ov62_021E5A94
    .public MicTest_AverageMicInput

	thumb_func_start ov62_021E63E8
ov62_021E63E8: ; 0x021E63E8
	push {r3, r4, r5, r6, r7, lr}
	sub sp, #0x18
	mov r4, #0
	str r3, [sp, #0x10]
	add r7, r2, #0
	str r4, [sp, #0x14]
	add r1, r4, #0
	add r3, r0, #0
_021E63F8:
	ldr r2, [r3, #0x50]
	cmp r2, #0
	bne _021E6414
	mov r2, #0x30
	mul r2, r1
	add r1, r1, #4
	add r3, r0, #0
	lsl r1, r1, #2
	add r0, r0, r1
	ldr r0, [r0, #8]
	add r3, #0x24
	add r4, r3, r2
	str r0, [sp, #0x14]
	b _021E641C
_021E6414:
	add r1, r1, #1
	add r3, #0x30
	cmp r1, #3
	blt _021E63F8
_021E641C:
	cmp r4, #0
	beq _021E6472
	bl LCRandom
	ldr r1, _021E6478 ; =0x00001002
	mul r1, r0
	asr r0, r1, #0xf
	lsr r0, r0, #0x10
	add r0, r1, r0
	asr r1, r0, #0x10
	ldr r0, _021E647C ; =0x00001333
	add r6, r1, r0
	bl LCRandom
	mov r1, #0x3e
	mul r1, r0
	asr r0, r1, #0xf
	lsr r0, r0, #0x10
	add r0, r1, r0
	asr r5, r0, #0x10
	add r5, #0x3c
	bl LCRandom
	lsl r1, r0, #4
	asr r0, r1, #0xf
	lsr r0, r0, #0x10
	add r0, r1, r0
	asr r0, r0, #0x10
	str r6, [sp]
	add r0, r0, #6
	str r5, [sp, #4]
	str r0, [sp, #8]
	mov r0, #1
	str r0, [sp, #0xc]
	ldr r0, [sp, #0x14]
	ldr r3, [sp, #0x10]
	add r1, r4, #0
	add r2, r7, #0
	bl ov62_021E6480
	add sp, #0x18
	mov r0, #1
	pop {r3, r4, r5, r6, r7, pc}
_021E6472:
	mov r0, #0
	add sp, #0x18
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021E6478: .word 0x00001002
_021E647C: .word 0x00001333
	thumb_func_end ov62_021E63E8

	thumb_func_start ov62_021E6480
ov62_021E6480: ; 0x021E6480
	push {r3, r4, r5, r6, r7, lr}
	add r7, r0, #0
	add r5, r2, #0
	ldr r0, _021E656C ; =ov62_021E6570
	add r4, r1, #0
	mov r2, #0
	add r6, r3, #0
	bl SysTask_CreateOnMainQueue
	str r0, [r4, #0x2c]
	bl SysTask_GetData
	add r4, r0, #0
	ldr r0, [sp, #0x24]
	str r7, [r4]
	str r0, [r4, #4]
	cmp r5, #0
	ble _021E64B6
	lsl r0, r5, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _021E64C4
_021E64B6:
	lsl r0, r5, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_021E64C4:
	bl _ffix
	str r0, [r4, #8]
	cmp r6, #0
	ble _021E64E0
	lsl r0, r6, #0xc
	bl _fflt
	add r1, r0, #0
	mov r0, #0x3f
	lsl r0, r0, #0x18
	bl _fadd
	b _021E64EE
_021E64E0:
	lsl r0, r6, #0xc
	bl _fflt
	mov r1, #0x3f
	lsl r1, r1, #0x18
	bl _fsub
_021E64EE:
	bl _ffix
	str r0, [r4, #0xc]
	mov r0, #0
	str r0, [r4, #0x10]
	ldr r0, [sp, #0x1c]
	add r0, #0x5a
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_SinDegNoWrap
	add r2, r0, #0
	ldr r0, [sp, #0x18]
	asr r3, r2, #0x1f
	asr r7, r0, #0x1f
	add r1, r7, #0
	str r0, [sp]
	bl _ll_mul
	add r2, r0, #0
	mov r0, #2
	mov r3, #0
	lsl r0, r0, #0xa
	add r0, r2, r0
	adc r1, r3
	lsl r1, r1, #0x14
	lsr r0, r0, #0xc
	orr r0, r1
	str r0, [r4, #0x14]
	ldr r0, [sp, #0x1c]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GF_SinDegNoWrap
	add r2, r0, #0
	ldr r0, [sp]
	add r1, r7, #0
	asr r3, r2, #0x1f
	bl _ll_mul
	mov r3, #2
	mov r2, #0
	lsl r3, r3, #0xa
	add r3, r0, r3
	adc r1, r2
	lsl r0, r1, #0x14
	lsr r1, r3, #0xc
	orr r1, r0
	neg r0, r1
	str r0, [r4, #0x18]
	str r2, [r4, #0x1c]
	mov r0, #6
	str r2, [r4, #0x20]
	lsl r0, r0, #0xc
	str r0, [r4, #0x24]
	ldr r0, [sp, #0x20]
	add r1, r5, #0
	str r0, [r4, #0x28]
	ldr r0, [r4]
	add r2, r6, #0
	bl UnkImageStruct_SetSpritePositionXY
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
_021E656C: .word ov62_021E6570
	thumb_func_end ov62_021E6480

	thumb_func_start ov62_021E6570
ov62_021E6570: ; 0x021E6570
	push {r3, r4, r5, r6, r7, lr}
	add r5, r1, #0
	add r7, r0, #0
	ldr r0, [r5, #4]
	cmp r0, #0
	beq _021E658C
	sub r0, r0, #1
	str r0, [r5, #4]
	bne _021E65FC
	ldr r0, [r5]
	mov r1, #1
	bl UnkImageStruct_SetSpriteVisibleFlag
	pop {r3, r4, r5, r6, r7, pc}
_021E658C:
	add r0, r5, #0
	add r0, #8
	add r1, #0x14
	add r2, r0, #0
	bl VEC_Add
	ldr r0, [r5, #8]
	ldr r2, [r5, #0x20]
	lsl r0, r0, #4
	asr r6, r0, #0x10
	ldr r0, [r5, #0xc]
	ldr r1, [r5, #0x24]
	lsl r0, r0, #4
	add r3, r2, r1
	mov r1, #0x5a
	asr r4, r0, #0x10
	add r0, r5, #0
	lsl r1, r1, #0xe
	add r0, #0x20
	str r3, [r5, #0x20]
	cmp r3, r1
	ble _021E65C6
	add r2, r1, #0
_021E65BA:
	ldr r3, [r0]
	sub r3, r3, r1
	str r3, [r0]
	ldr r3, [r5, #0x20]
	cmp r3, r2
	bgt _021E65BA
_021E65C6:
	lsl r0, r3, #4
	lsr r0, r0, #0x10
	bl GF_SinDegNoWrap
	ldr r1, [r5, #0x28]
	add r2, r4, #0
	mul r0, r1
	asr r0, r0, #0xc
	add r0, r6, r0
	lsl r0, r0, #0x10
	asr r1, r0, #0x10
	ldr r0, [r5]
	bl UnkImageStruct_SetSpritePositionXY
	mov r0, #0xf
	mvn r0, r0
	cmp r4, r0
	bge _021E65FC
	ldr r0, [r5]
	mov r1, #0
	bl UnkImageStruct_SetSpriteVisibleFlag
	add r0, r7, #0
	bl SysTask_Destroy
	mov r0, #0
	str r0, [r5, #0x2c]
_021E65FC:
	pop {r3, r4, r5, r6, r7, pc}
	.balign 4, 0
	thumb_func_end ov62_021E6570

	thumb_func_start ov62_021E6600
ov62_021E6600: ; 0x021E6600
	push {r4, r5, r6, lr}
	mov r4, #0
	add r5, r0, #0
	add r6, r4, #0
_021E6608:
	ldr r0, [r5, #0x50]
	cmp r0, #0
	beq _021E6614
	bl SysTask_Destroy
	str r6, [r5, #0x50]
_021E6614:
	add r4, r4, #1
	add r5, #0x30
	cmp r4, #3
	blt _021E6608
	pop {r4, r5, r6, pc}
	.balign 4, 0
	thumb_func_end ov62_021E6600

	thumb_func_start ov62_021E6620
ov62_021E6620: ; 0x021E6620
	ldr r3, _021E6628 ; =TouchscreenHitbox_FindRectAtTouchNew
	ldr r0, _021E662C ; =_021E6650
	bx r3
	nop
_021E6628: .word TouchscreenHitbox_FindRectAtTouchNew
_021E662C: .word _021E6650
	thumb_func_end ov62_021E6620

	thumb_func_start ov62_021E6630
ov62_021E6630: ; 0x021E6630
	push {r3, lr}
	bl ov62_021E6620
	cmp r0, #0
	beq _021E6644
	ldr r0, _021E664C ; =gSystem
	ldr r1, [r0, #0x48]
	mov r0, #2
	tst r0, r1
	beq _021E6648
_021E6644:
	mov r0, #1
	pop {r3, pc}
_021E6648:
	mov r0, #0
	pop {r3, pc}
	.balign 4, 0
_021E664C: .word gSystem
	thumb_func_end ov62_021E6630

	.rodata

_021E6650:
	.byte 0xA0, 0xC0, 0xB8, 0xF8, 0xFF, 0x00, 0x00, 0x00

.public ov62_021E6658
ov62_021E6658: ; 0x021E6658
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov62_021E6668
ov62_021E6668: ; 0x021E6668
	.byte 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
	.byte 0x00, 0x40, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00

.public ov62_021E667C
ov62_021E667C: ; 0x021E667C
	.byte 0x20, 0x00, 0x00, 0x00
	.byte 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00

.public ov62_021E6694
ov62_021E6694: ; 0x021E6694
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
	.byte 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00

.public ov62_021E66B0
ov62_021E66B0: ; 0x021E66B0
	.byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00

.public ov62_021E66D0
ov62_021E66D0: ; 0x021E66D0
	.byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

.public ov62_021E66F8
ov62_021E66F8: ; 0x021E66F8
	.byte 0x00, 0x01, 0x01, 0x09, 0x03, 0x00, 0x01, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0F, 0x00, 0x04, 0x05, 0x08, 0x16, 0x05, 0x00, 0x01, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0F, 0x00, 0x04, 0x18, 0x15, 0x06, 0x02, 0x00, 0x6F, 0x00
	.byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0E, 0x00

.public ov62_021E6728
ov62_021E6728: ; 0x021E6728
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x01, 0x00, 0x01, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1D, 0x02, 0x00, 0x02, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1E, 0x01, 0x00, 0x01, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00

.public ov62_021E67C8
ov62_021E67C8: ; 0x021E67C8
	.byte 0x40, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x0A, 0x00, 0x00
	.byte 0x03, 0x0A, 0x00, 0x00, 0x02, 0x0A, 0x00, 0x00, 0x04, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x60, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
	.byte 0x01, 0x0A, 0x00, 0x00, 0x03, 0x0A, 0x00, 0x00, 0x02, 0x0A, 0x00, 0x00, 0x04, 0x0A, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x80, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x01, 0x00, 0x00, 0x00, 0x01, 0x0B, 0x00, 0x00, 0x03, 0x0B, 0x00, 0x00, 0x02, 0x0B, 0x00, 0x00
	.byte 0x04, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0xD8, 0x00, 0xB0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x0D, 0x00, 0x00, 0x03, 0x0D, 0x00, 0x00
	.byte 0x02, 0x0D, 0x00, 0x00, 0x04, 0x0D, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x0C, 0x00, 0x00
	.byte 0x03, 0x0C, 0x00, 0x00, 0x02, 0x0C, 0x00, 0x00, 0x04, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.public gApplication_MicTest
gApplication_MicTest:
	.word MicTest_Init, MicTest_Exit, MicTest_Main, 0xFFFFFFFF

	.bss

.public _021E68E0
_021E68E0:
	.space 0x20
