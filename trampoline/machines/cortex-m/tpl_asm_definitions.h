/**
 * @file tpl_asm_definitions.h
 *
 * @section descr File description
 *
 * Common definitions for ARM assembler source files
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005+
 * Copyright ESEO for function and data structures documentation and ARM port
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#ifndef TPL_ASM_DEFINITIONS_H
#define TPL_ASM_DEFINITIONS_H

/*
 * tpl_kern data structure offsets (for use into assembler sources)
 */
.equ TPL_KERN_OFFSET_S_RUNNING, 0
.equ TPL_KERN_OFFSET_S_ELECTED, 4
.equ TPL_KERN_OFFSET_RUNNING, 8
.equ TPL_KERN_OFFSET_ELECTED, 12
.equ TPL_KERN_OFFSET_RUNNING_ID, 16
.equ TPL_KERN_OFFSET_ELECTED_ID, 20
.equ TPL_KERN_OFFSET_NEED_SWITCH, 24
.equ TPL_KERN_OFFSET_NEED_SCHEDULE, 25

/*
 * ARM modes codes
 */
.equ CPSR_USR_MODE, 0b10000
.equ CPSR_FIQ_MODE, 0b10001
.equ CPSR_IRQ_MODE, 0b10010
.equ CPSR_SVC_MODE, 0b10011
.equ CPSR_ABT_MODE, 0b10111
.equ CPSR_UND_MODE, 0b11011
.equ CPSR_SYS_MODE, 0b11111
.equ CPSR_MODE_MASK, 0b11111

/*
 * Interrupt masks
 */
.equ CPSR_IRQ_LOCKED, 0b10000000
.equ CPSR_FIQ_LOCKED, 0b01000000

#endif
/* TPL_ASM_DEFINITIONS_H */

/* Enf of file tpl_asm_definitions.h */
