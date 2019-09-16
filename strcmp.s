;
; int strcmp2 (const char* s1, const char* s2);
;

        .export         _strcmp2
        .importzp       ptr1, ptr2
        .import         popptr1, popax

_strcmp2:
        sta     ptr2            ; Save s2
        stx     ptr2+1
        jsr     popptr1         ; Get s1
;       jsr 	popax
;       sta		ptr1
;       stx		ptr1+1
        ldy     #0             ; Y=0 guaranteed by popptr1

loop:   lda     (ptr1),y
        cmp     (ptr2),y
        bne     L1
        ora     (ptr2),y
        cmp     #0
        beq     L3
        iny
        jmp     loop


L1:     ldx     #$FF
        rts

L3:     ldx     #0
                rts
