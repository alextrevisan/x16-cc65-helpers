;
; File generated by cc65 v 2.18 - Git 4e3abf4
;
	.fopt		compiler,"cc65 v 2.18 - Git 4e3abf4"
	.setcpu		"6502"
	.smart		on
	.autoimport	on
	.case		on
	.debuginfo	off
	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.macpack	longbranch
	.export		_strcmp

; ---------------------------------------------------------------
; int __near__ __fastcall__ strcmp (__near__ const unsigned char *, __near__ const unsigned char *)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_strcmp: near

.segment	"CODE"

	jsr     pushax
	ldy     #$03
	lda     (sp),y
	sta     ptr1+1
	dey
	lda     (sp),y
	sta     ptr1
	ldx     #$00
	lda     (ptr1,x)
	bne     L000C
	dey
	lda     (sp),y
	sta     ptr1+1
	lda     (sp,x)
	sta     ptr1
	lda     (ptr1,x)
	beq     L000B
	jmp     L000C
L000B:	jmp     incsp4
L000C:	lda     #$01
	jmp     incsp4

.endproc
