/*-------------------------------------------------------------------------
   8052.h: Register Declarations for the Intel 8052 Processor

   Copyright (C) 2000, Bela Torok / bela.torok@kssg.ch

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License 
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

#ifndef REG8052_H
#define REG8052_H

#include <8051.h>     /* load definitions for the 8051 core */

#ifdef REG8051_H
#undef REG8051_H
#endif

/* define 8052 specific registers only */

/* T2CON */
SFR(T2CON, 0xC8);

/* RCAP2 L & H */
SFR(RCAP2L, 0xCA);
SFR(RCAP2H, 0xCB);
SFR(TL2, 0xCC);
SFR(TH2, 0xCD);

/*  IE  */
SBIT(ET2, 0xA8, 5)     ; /* Enable timer2 interrupt */

/*  IP  */
SBIT(PT2, 0xB8, 5)     ; /* T2 interrupt priority bit */

/* T2CON bits */
SBIT(T2CON_0, 0xC8, 0);
SBIT(T2CON_1, 0xC8, 1);
SBIT(T2CON_2, 0xC8, 2);
SBIT(T2CON_3, 0xC8, 3);
SBIT(T2CON_4, 0xC8, 4);
SBIT(T2CON_5, 0xC8, 5);
SBIT(T2CON_6, 0xC8, 6);
SBIT(T2CON_7, 0xC8, 7);

SBIT(CP_RL2, 0xC8, 0);
SBIT(C_T2,   0xC8, 1);
SBIT(TR2,    0xC8, 2);
SBIT(EXEN2,  0xC8, 3);
SBIT(TCLK,   0xC8, 4);
SBIT(RCLK,   0xC8, 5);
SBIT(EXF2,   0xC8, 6);
SBIT(TF2,    0xC8, 7);

#endif
