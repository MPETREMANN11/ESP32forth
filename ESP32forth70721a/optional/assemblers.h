/*
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/*
 * ESP32forth Assemblers v7.0.7.21
 * Revision: 9ae74fa18335b0378a98bd8c693b468cc1265ee5
 */

#define OPTIONAL_ASSEMBLERS_SUPPORT \
  XV(internals, "assembler-source", ASSEMBLER_SOURCE, \
      PUSH assembler_source; PUSH sizeof(assembler_source) - 1) \
  PLATFORM_ASSEMBLER_SUPPORT

const char assembler_source[] = R"""(
( Lazy loaded assembler/disassembler framework )
: assembler r|

current @
also internals
also asm definitions

-1 1 rshift invert constant high-bit
: odd? ( n -- f ) 1 and ;
: >>1 ( n -- n ) 1 rshift ;
: enmask ( n m -- n )
  0 -rot cell 8 * 1- for
    rot >>1 -rot
    dup odd? if
      over odd? if rot high-bit or -rot then
      swap >>1 swap
    then
    >>1
  next
  2drop
;
: demask ( n m -- n )
  0 >r begin dup while
    dup 0< if over 0< if r> 2* 1+ >r else r> 2* >r then then
    2* swap 2* swap
  repeat 2drop r>
;

variable length   variable pattern   variable mask
: bit! ( n a -- ) dup @ 2* rot 1 and or swap ! ;

: >opmask& ( xt -- a ) >body ;
: >next ( xt -- xt ) >body cell+ @ ;
: >inop ( a -- a ) >body 2 cells + @ ;
: >printop ( a -- a ) >body 3 cells + @ ;

variable operands
: for-operands ( xt -- )
   >r operands @ begin dup while r> 2dup >r >r execute r> >next repeat rdrop drop ;

: reset-operand ( xt -- ) >opmask& 0 swap ! ;
: reset   0 length !  0 mask !  0 pattern !  ['] reset-operand for-operands ;
: advance-operand ( xt -- ) >opmask& 0 swap bit! ;
: advance   ['] advance-operand for-operands ;

: skip  1 length +!  0 mask bit!  0 pattern bit!  advance ;
: bit ( n -- ) 1 length +!  1 mask bit!  pattern bit!  advance ;
: bits ( val n ) 1- for dup r@ rshift bit next drop ;
: o   0 bit ;   : l   1 bit ;

( struct: pattern next inop printop )
: operand ( inop printop "name" )
   create 0 , operands @ , latestxt operands ! swap , ,
   does> skip 1 swap +! ;
: names ( n "names"*n --) 0 swap 1- for dup constant 1+ next drop ;

: coden, ( val n -- ) 8 / 1- for dup code1, 8 rshift next drop ;

( struct: length pattern mask [xt pattern]* 0 )
variable opcodes
: op-snap ( xt -- ) dup >opmask& @ if dup , >opmask& @ , else drop then ;
: >xt ( a -- xt ) 2 cells - ;
: >length ( xt -- a ) >body cell+ @ ;
: >pattern ( xt -- a ) >body 2 cells + @ ;
: >mask ( xt -- a ) >body 3 cells + @ ;
: >operands ( xt -- a ) >body 4 cells + ;
: op ( "name" )
   create opcodes @ , latestxt opcodes !
          length @ , pattern @ , mask @ ,
          ['] op-snap for-operands 0 , reset
   does> >xt >r
         r@ >pattern
         0 r@ >operands begin dup @ while >r 1+ r> 2 cells + repeat
         swap for aft
           2 cells - dup >r swap >r dup cell+ @ >r @ >inop execute r> enmask r> or r>
         then next
         drop
         r> >length coden,
;

: for-ops ( xt -- )
   >r opcodes @ begin dup while r> 2dup >r >r execute r> >body @ repeat rdrop drop ;

: m@ ( a -- n ) 0 swap cell 0 do dup ca@ i 8 * lshift swap >r or r> 1+ loop drop ;
: m. ( n n -- ) base @ hex >r >r <# r> 1- for # # next #> type r> base ! ;
: sextend ( n n -- n ) cell 8 * swap - dup >r lshift r> arshift ;

variable istep
variable address
: matchit ( a xt -- a )
  >r dup m@ r@ >mask and r@ >pattern = if
    r@ >operands begin dup @ while
      >r dup m@ r@ cell+ @ demask r@ @ >printop execute r> 2 cells +
    repeat drop
    r@ see.
    r@ >length 8 / istep !
  then rdrop ;
: disasm1 ( a -- a )
  dup address ! dup . ."  --  " 0 istep ! ['] matchit for-ops
  istep @ 0= if 1 istep ! ." UNKNOWN!!!" then
  9 emit 9 emit ." -- " dup m@ istep @ m.
  istep @ +
  cr
;
: disasm ( a n -- ) for aft disasm1 then next drop ;

previous previous
also forth definitions
: assembler asm ;
previous
assembler
current !

| evaluate ;
)""";

#if defined(__riscv)

# define PLATFORM_ASSEMBLER_SUPPORT \
   XV(internals, "riscv-assembler-source", RISCV_ASSEMBLER_SOURCE, \
       PUSH riscv_assembler_source; PUSH sizeof(riscv_assembler_source) - 1)

const char riscv_assembler_source[] = R"""(
RISC-V? [IF]

( Lazy loaded RISC-V assembler )
: riscv-assembler r|

current @
also assembler definitions
vocabulary riscv riscv definitions

32 names zero x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11 x12 x13 x14 x15 x16 x17 x18 x19 x20 x21 x22 x23 x24 x25 x26 x27 x28 x29 x30 x31
: nop ;
: reg. ( n -- ) dup if base @ >r decimal ." x" . r> base ! else drop ." zero " then ;
: register ( -- in print ) ['] nop ['] reg. ;

: reg>reg' ( n -- n ) 8 - 7 and ;
: reg'. ( n -- ) 8 + reg. ;
: register' ( -- in print ) ['] reg>reg' ['] reg'. ;

: numeric ( -- in print ) ['] nop ['] . ;

numeric operand i   : iiii   i i i i ;

( Offsets for JAL )
: >ofs ( n -- n ) chere - dup 20 rshift 1 and 31 12 - lshift
                          over 1 rshift $3ff and 21 12 - lshift or
                          over 11 rshift 1 and 20 12 - lshift or
                          swap 12 rshift $ff and or ;
: ofs. ( n -- ) dup 31 12 - rshift 1 and 20 lshift
                over 21 12 - rshift $3ff and 1 lshift or
                over 20 12 - rshift 1 and 11 lshift or
                swap $ff and 12 lshift or 21 sextend address @ + . ;
' >ofs ' ofs. operand ofs
: offset   20 for aft ofs then next ;

register  operand rd#    : rd    rd# rd# rd# rd# rd# ;
register' operand rd#'   : rd'   rd#' rd#' rd#' ;
register  operand rs1#   : rs1   rs1# rs1# rs1# rs1# rs1# ;
register' operand rs1#'  : rs1'  rs1#' rs1#' rs1#' ;
register  operand rs2#   : rs2   rs2# rs2# rs2# rs2# rs2# ;
register' operand rs2#'  : rs2'  rs2#' rs2#' rs2#' ;

: R-TYPE { fn7 fn3 opc }
  fn7 7 bits  rs2  rs1  fn3 3 bits  rd      opc 7 bits OP ;
: I-TYPE { fn3 opc }
  iiii iiii iiii   rs1  fn3 3 bits  rd      opc 7 bits OP ;
: S-TYPE { fn3 opc }
  iiii i i i  rs2  rs1  fn3 3 bits  i iiii  opc 7 bits OP ;
: B-TYPE { fn3 opc }
  iiii i i i  rs2  rs1  fn3 3 bits  i iiii  opc 7 bits OP ;
: U-TYPE { opc }
  iiii iiii iiii iiii iiii          rd      opc 7 bits OP ;
: J-TYPE { opc }
  offset                            rd      opc 7 bits OP ;

$37 U-TYPE LUI,
$17 U-TYPE AUIPC,
$6F J-TYPE JAL,
$0 $67 I-TYPE JALR,

$0 $63 B-TYPE BEQ,   $1 $63 B-TYPE BNE,
$4 $63 B-TYPE BLT,   $5 $63 B-TYPE BGE,
$6 $63 B-TYPE BLTU,  $7 $63 B-TYPE BGEU,

$0 $03 I-TYPE LB,   $1 $03 I-TYPE LH,   $2 $03 I-TYPE LW,
$4 $03 I-TYPE LBU,  $5 $03 I-TYPE LHU,

$0 $23 S-TYPE SB,  $1 $23 S-TYPE SH,  $2 $23 S-TYPE SW,

$0 $13 I-TYPE ADDI,  $2 $13 I-TYPE SLTI,  $3 $13 I-TYPE SLTIU,
$4 $13 I-TYPE XORI,  $6 $13 I-TYPE ORI,   $7 $13 I-TYPE ANDI,

$00 $1 $13 R-TYPE SLLI,
$00 $5 $13 R-TYPE SRLI,  $20 $5 $13 R-TYPE SRAI,

$00 $0 $33 R-TYPE ADD,   $20 $0 $33 R-TYPE SUB,

$00 $1 $33 R-TYPE SLL,  $00 $2 $33 R-TYPE SLT,
$00 $3 $33 R-TYPE SLTU, $00 $4 $33 R-TYPE XOR,

$00 $5 $33 R-TYPE SRL,  $20 $5 $33 R-TYPE SRA,

$00 $6 $33 R-TYPE OR,   $20 $7 $33 R-TYPE AND,

( TODO FENCE, FENCE.I )

o o o o o o o o o o o o   o o o o o  o o o  o o o o o  o o o l l l l OP ECALL,
o o o o o o o o o o o l   o o o o o  o o o  o o o o o  o o o l l l l OP EBREAK,

( TODO CSR* )

o o o  o o o o o o o o  o o o  o o  OP C.ILL,
o o o  i i i i i i i i  rd'    o o  OP C.ADDI4SP,
o o l  i i i  rs1' i i  rd'    o o  OP C.FLD,
o l o  i i i  rs1' i i  rd'    o o  OP C.LW,
o l l  i i i  rs1' i i  rd'    o o  OP C.FLW,
( 4 reserved )
l o l  i i i  rs1' i i  rd'    o o  OP C.FSD,
l l o  i i i  rs1' i i  rd'    o o  OP C.SW,
l l l  i i i  rs1' i i  rd'    o o  OP C.FSW,

o o o  o  o o o o o  o o o o o   o l  OP C.NOP,
o o o  i  rs1        i i i i i   o l  OP C.ADDI,
o o l  i  i i i i i  i i i i i   o l  OP C.JAL,
o l o  i  rd         i i i i i   o l  OP C.LI,
o l l  i  rd         i i i i i   o l  OP C.LUI,
l o o  i  o o  rs1'  i i i i i   o l  OP C.SRLI,
l o o  i  o l  rs1'  i i i i i   o l  OP C.SRAI,
l o o  i  l o  rs1'  i i i i i   o l  OP C.ANDI,
l o o  o  l l  rs1'  o o  rs2'   o l  OP C.SUB,
l o o  o  l l  rs1'  o l  rs2'   o l  OP C.XOR,
l o o  o  l l  rs1'  l o  rs2'   o l  OP C.OR,
l o o  o  l l  rs1'  l l  rs2'   o l  OP C.AND,
l o o  l  l l  rs1'  o o  rs2'   o l  OP C.SUBW,
l o o  l  l l  rs1'  o l  rs2'   o l  OP C.ADDW,
l o l  i  i i i i i  i i i i i   o l  OP C.J,
l l o  i  i i  rs1'  i i i i i   o l  OP BEQZ,
l l l  i  i i  rs1'  i i i i i   o l  OP BNEZ,

o o o  i  rs1        i i i i i   l o  OP C.SLLI,
o o l  i  rd         i i i i i   l o  OP C.FLDSP,
o l o  i  rd         i i i i i   l o  OP C.LWSP,
o l l  i  rd         i i i i i   l o  OP C.FLWSP,
l o o  o  rs1        o o o o o   l o  OP C.JR,
l o o  o  rd         rs2         l o  OP C.MV,
l o o  l  o o o o o  o o o o o   l o  OP C.EBREAK,
l o o  l  rs1        o o o o o   l o  OP C.JALR,
l o o  l  rd         rs2         l o  OP C.ADD,
l o l  i  i i i i i  rs2         l o  OP C.FSDSP,
l l o  i  i i i i i  rs2         l o  OP C.SWSP,
l l l  i  i i i i i  rs2         l o  OP C.FSWSP,

also forth definitions
: riscv-assembler riscv ;
previous previous
riscv-assembler
current !

| evaluate ;

[THEN]
)""";

#else

# define PLATFORM_ASSEMBLER_SUPPORT \
   XV(internals, "xtensa-assembler-source", XTENSA_ASSEMBLER_SOURCE, \
       PUSH xtensa_assembler_source; PUSH sizeof(xtensa_assembler_source) - 1)

const char xtensa_assembler_source[] = R"""(
Xtensa? [IF]

( Lazy loaded xtensa assembler )
: xtensa-assembler r|

current @
also assembler definitions
vocabulary xtensa xtensa definitions

16 names a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 a10 a11 a12 a13 a14 a15
: nop ;
: reg. ( n -- ) base @ >r decimal ." a" . r> base ! ;
: register ( -- in print ) ['] nop ['] reg. ;
: numeric ( -- in print ) ['] nop ['] . ;

numeric operand im
: imm4   im im im im ;
: imm8   imm4 imm4 ;
: imm16   imm8 imm8 ;
: sr   imm8 ;

( Offsets for J and branches )
: >ofs ( n -- n ) chere - 4 - ;
: ofs8. ( n -- ) 8 sextend address @ + 4 + . ;
: ofs12. ( n -- ) 12 sextend address @ + 4 + . ;
: ofs18. ( n -- ) 18 sextend address @ + 4 + . ;
' >ofs ' ofs8. operand ofs8
' >ofs ' ofs12. operand ofs12
' >ofs ' ofs18. operand ofs18
: offset8   8 for aft ofs8 then next ;
: offset12   12 for aft ofs12 then next ;
: offset18   18 for aft ofs18 then next ;

( Offsets for CALL* )
: >cofs ( n -- n ) chere - 2 rshift 1- ;
: cofs. ( n -- ) 18 sextend 1+ 2 lshift address @ 3 invert and + . ;
' >cofs ' cofs. operand cofs
: coffset18   18 for aft cofs then next ;

( Frame size of ENTRY )
: >entry12 ( n -- n ) 3 rshift ;
: entry12. ( n -- ) 3 lshift . ;
' >entry12 ' entry12. operand entry12'
: entry12   12 for aft entry12' then next ;

: >sa ( n -- n ) 32 swap - ;
: sa. ( n -- ) 32 swap - . ;
' >sa ' sa. operand sa

numeric operand x   : xxxx   x x x x ;
numeric operand i   : iiii   i i i i ;
numeric operand w
numeric operand y
numeric operand b   : bbbb   b b b b ;

register operand r   : rrrr   r r r r ;
register operand s   : ssss   s s s s ;
register operand t   : tttt   t t t t ;

imm4     ssss     tttt     l o o o  OP L32I.N,
imm4     ssss     tttt     l o o l  OP S32I.N,
rrrr     ssss     tttt     l o l o  OP ADD.N,
rrrr     ssss     imm4     l o l l  OP ADDI.N,
iiii     ssss     l o i i  l l o o  OP BEQZ.N,
iiii     ssss     l l i i  l l o o  OP BNEZ.N,
iiii     ssss     o i i i  l l o o  OP MOVI.N,
o o o o  ssss     tttt     l l o l  OP MOV.N,
l l l l  ssss     o o l o  l l o l  OP BREAK.N,
l l l l  o o o o  o o o o  l l o l  OP RET.N,
l l l l  o o o o  o o o l  l l o l  OP RETW.N,
l l l l  o o o o  o o l l  l l o l  OP NOP.N,
l l l l  o o o o  o l l o  l l o l  OP ILL.N,

o o o o  o o o o  o o o o  o o o o  o o o o  o o o o  OP ILL,
o o o o  o o o o  o o o o  o o o o  l o o o  o o o o  OP RET,
o o o o  o o o o  o o o o  o o o o  l o o l  o o o o  OP RETW,
o o o o  o o o o  o o l o  o o o o  o o o o  o o o o  OP ISYNC,
o o o o  o o o o  o o l o  o o o o  o o o l  o o o o  OP RSYNC,
o o o o  o o o o  o o l o  o o o o  o o l o  o o o o  OP ESYNC,
o o o o  o o o o  o o l o  o o o o  o o l l  o o o o  OP DSYNC,
o o o o  o o o o  o o l o  o o o o  l o o o  o o o o  OP EXCW,
o o o o  o o o o  o o l o  o o o o  l l o o  o o o o  OP MEMW,
o o o o  o o o o  o o l o  o o o o  l l o l  o o o o  OP EXTW,
o o o o  o o o o  o o l o  o o o o  l l l l  o o o o  OP NOP,
o o o o  o o o o  o o l l  o o o o  o o o o  o o o o  OP RFE,
o o o o  o o o o  o o l l  o o o o  o o l o  o o o o  OP RFME,
o o o o  o o o o  o o l l  o o o l  o o o o  o o o o  OP RFUE,
o o o o  o o o o  o o l l  o o l o  o o o o  o o o o  OP RFDE,
o o o o  o o o o  o o l l  o l o o  o o o o  o o o o  OP RFWO,
o o o o  o o o o  o o l l  o l o l  o o o o  o o o o  OP RFWU,
o o o o  o o o o  o l o l  o o o o  o o o o  o o o o  OP SYSCALL,
o o o o  o o o o  o l o l  o o o l  o o o o  o o o o  OP SIMCALL,
l l l l  o o o l  l l l o  o o o s  o o o l  o o o o  OP RFDD,
l l l l  o o o l  l l l o  o o o o  o o o o  o o o o  OP RFDO,

o l l o  o o o o  rrrr  o o o o  tttt  o o o o  OP NEG,
o l l o  o o o o  rrrr  o o o l  tttt  o o o o  OP ABS,
o l l o  o o o l  sr             tttt  o o o o  OP XSR,

: ALU   4 bits  o o o o  rrrr  ssss  tttt  o o o o  OP ;
              $1 ALU AND,   $2 ALU OR,     $3 ALU XOR,
( $6 ABS/NEG )
$8 ALU ADD,   $9 ALU ADDX2, $a ALU ADDX4,  $b ALU ADDX8,
$c ALU SUB,   $d ALU SUBX2, $e ALU SUBX4,  $f ALU SUBX8,

: ANYALL   o o o o  o o o o  l o 2 bits  ssss  tttt  o o o o  OP ;
$0 ANYALL ANY4,  $1 ANYALL ALL4,  $2 ANYALL ANY8,  $3 ANYALL ALL8,

: ALU2   4 bits  o o l o  rrrr  ssss  tttt  o o o o  OP ;
$0 ALU2 ANDB,  $1 ALU2 ANDBC,  $2 ALU2 ORB,    $3 ALU2 ORBC,
$4 ALU2 XORB,
$8 ALU2 MULL,                  $a ALU2 MULUH,  $b ALU2 MULSH,
$c ALU2 QUOU,  $d ALU2 QUOS,   $e ALU2 REMU,   $f ALU2 REMS,

: BRANCH1   offset8  4 bits  ssss  tttt  o l l l  OP ;
$0 BRANCH1 BNONE,  $1 BRANCH1 BEQ,  $2 BRANCH1 BLT,  $3 BRANCH1 BLTU,
$4 BRANCH1 BALL,   $5 BRANCH1 BBC,
offset8  o l l b  ssss  bbbb  o l l l  OP BBCI,
$8 BRANCH1 BANY,   $9 BRANCH1 BNE,  $a BRANCH1 BGE,  $b BRANCH1 BGEU,
$c BRANCH1 BNALL,  $d BRANCH1 BBS,
offset8  l l l b  ssss  bbbb  o l l l  OP BBSI,

: BRANCH2   offset12  ssss  4 bits  o l l o  OP ;
: BRANCH2a   offset8  rrrr     ssss  4 bits  o l l o  OP ;
: BRANCH2e   entry12  ssss  4 bits  o l l o  OP ;
( $0 J, )  $1 BRANCH2 BEQZ,  $2 BRANCH2a BEQI,  $3 BRANCH2e ENTRY,
( $4 J, )  $5 BRANCH2 BNEZ,  $6 BRANCH2a BNEI,  ( BRANCH2b's )
( $8 J, )  $9 BRANCH2 BLTZ,  $a BRANCH2a BLTI,  $b BRANCH2a BLTUI,
( $c J, )  $d BRANCH2 BGEZ,  $e BRANCH2a BGEI,  $f BRANCH2a BGEUI,
offset18  o o  o l l o  OP J,
: BRANCH2b   offset8  4 bits  ssss  o l l l  o l l o  OP ;
$0 BRANCH2b BF,    $1 BRANCH2b BT,
$8 BRANCH2b LOOP,  $9 BRANCH2b LOOPNEZ,  $a BRANCH2b LOOPGTZ,

: CALLOP   coffset18  2 bits  o l o l  OP ;
0 CALLOP CALL0,  1 CALLOP CALL4,  2 CALLOP CALL8,  3 CALLOP CALL12,

: CALLXOP   o o o o  o o o o  o o o o  ssss  l l 2 bits  o o o o  OP ;
0 CALLXOP CALLX0,  1 CALLXOP CALLX4,  2 CALLXOP CALLX8,  3 CALLXOP CALLX12,

o o o o  o o o o  o l o o  ssss  tttt  o o o o  OP BREAK,
o o l l  o o l l  rrrr     ssss  tttt  o o o o  OP CLAMPS,

: CACHING1 imm8  o l l l  ssss  4 bits  o o l o  OP ;
$0 CACHING1 DPFR,  $1 CACHING1 DPFW,   $2 CACHING1 DPFRO,  $3 CACHING1 DPFWO,
$4 CACHING1 DHWB,  $5 CACHING1 DHWBI,  $6 CACHING1 DHI,    $7 CACHING1 DII,
( $8 CACHING2 )    ( ?? )              ( ?? )              ( ?? )
$c CACHING1 IPF,   ( $d CACHING2 )     $e CACHING1 IHI,    $f CACHING1 III,

: CACHING2 imm4  4 bits  o l l l  ssss  4 bits  o o l o  OP ;
$0 $8 CACHING2 DPFL,  $2 $8 CACHING2 DHU,    $3 $8 CACHING2 DIU,
$4 $8 CACHING2 DIWB,  $5 $8 CACHING2 DIWBI,
$0 $d CACHING2 IPFL,  $2 $d CACHING2 IHU,    $3 $d CACHING2 IIU,

iiii  iiii     l o l o  iiii  tttt     o o l o  OP MOVI,

: LDSTORE  imm8  4 bits  ssss  tttt  o o l o  OP ;
$0 LDSTORE L8UI,  $1 LDSTORE L16UI,  $2 LDSTORE L32I,    ( $3 CACHING )
$4 LDSTORE S8I,   $5 LDSTORE S16I,   $6 LDSTORE S32I,
                  $9 LDSTORE L16SI,  ( $a MOVI )         $b LDSTORE L32AI,
$c LDSTORE ADDI,  $d LDSTORE ADDMI,  $e LDSTORE S32C1I,  $f LDSTORE S32RI,

o l o o  l o o l  rrrr  ssss  tttt  o o o o  OP S32E,

x x x x  o l o sa  rrrr  sa sa sa sa  tttt  o o o o  OP EXTUI,

imm16  tttt  o o o l  OP L32R,
l o o l  o o o o  o o w w  ssss  o o o o  o l o o  OP LDDEC,
l o o o  o o o o  o o w w  ssss  o o o o  o l o o  OP LDINC,
imm8  o o o o  ssss  tttt  o o l l  OP LSI,
imm8  l o o o  ssss  tttt  o o l l  OP LSIU,

o l o l  o o o o  l l o o  ssss  o o o o  o o o o  OP IDTLB,
o l o l  o o o o  o l o o  ssss  o o o o  o o o o  OP IITLB,
o o o o  o o o o  o o o o  ssss  l o l o  o o o o  OP JX,
l l l l  o o o l  l o o o  ssss  tttt     o o o o  OP LDCT,
l l l l  o o o l  o o o o  ssss  tttt     o o o o  OP LICT,
l l l l  o o o l  o o l o  ssss  tttt     o o o o  OP LICW,
o o o o  l o o l  rrrr     ssss  tttt     o o o o  OP L32E,
o o o o  l o o o  rrrr     ssss  tttt     o o o o  OP LSX,
o o o l  l o o o  rrrr     ssss  tttt     o o o o  OP LSXU,
o o l o  o o o o  rrrr     ssss  tttt     o o o o  OP MOV,

: CONDOP   4 bits  o o l l  rrrr  ssss  tttt  o o o o  OP ;
$4 CONDOP MIN,     $5 CONDOP MAX,     $6 CONDOP MINU,    $7 CONDOP MAXU,
$8 CONDOP MOVEQZ,  $9 CONDOP MOVNEZ,  $a CONDOP MOVLTZ,  $b CONDOP MOVGEZ,
$c CONDOP MOVF,

: ALU.S   4 bits  l o l o  rrrr  ssss  tttt  o o o o  OP ;
$0 ALU.S ADD.S,    $1 ALU.S SUB.S,    $2 ALU.S MUL.S,
$4 ALU.S MADD.S,   $5 ALU.S MSUB.S,
$8 ALU.S ROUND.S,  $9 ALU.S TRUNC.S,  $a ALU.S FLOOR.S,  $b ALU.S CEIL.S,
$c ALU.S FLOAT.S,  $d ALU.S UFLOAT.S, $e ALU.S UTRUNC.S,
: ALU2.S   l l l l  l o l o  rrrr  ssss  4 bits  o o o o  OP ;
$0 ALU2.S MOV.S,   $1 ALU2.S ABS.S,
$4 ALU2.S RFR,     $5 ALU2.S WFR,     $6 ALU2.S NEG.S,

: CMPSOP   4 bits  l o l l  rrrr  ssss  tttt  o o o o  OP ;
                     $1 CMPSOP UN.S,      $2 CMPSOP OEQ.S,    $3 CMPSOP UEQ.S,
$4 CMPSOP OLT.S,     $5 CMPSOP ULT.S,     $6 CMPSOP OLE.S,    $7 CMPSOP ULE.S,
$8 CMPSOP MOVEQZ.S,  $9 CMPSOP MOVNEZ.S,  $a CMPSOP MOVLTZ.S, $b CMPSOP MOVGEZ.S,
$c CMPSOP MOVF.S,    $d CMPSOP MOVT.S,

o o o o  o o o o  o o o l  ssss  tttt     o o o o  OP MOVSP,

l l o l  o o l l  rrrr  ssss  tttt  o o o o  OP MOVT,
: MUL.AA o l l l  o l 2 bits  o o o o  ssss  tttt  o l o o  OP ;
0 MUL.AA MUL.AA.LL,   1 MUL.AA MUL.AA.HL,  2 MUL.AA MUL.AA.LH,  3 MUL.AA MUL.AA.HH,
: MUL.AD o l l l  o l 2 bits  o o o o  ssss  o y o o  o l o o  OP ;
0 MUL.AD MUL.AD.LL,   1 MUL.AD MUL.AD.HL,  2 MUL.AD MUL.AD.LH,  3 MUL.AD MUL.AD.HH,
: MUL.DA o l l o  o l 2 bits  o x o o  o o o o  tttt  o l o o  OP ;
0 MUL.DA MUL.DA.LL,  1 MUL.DA MUL.DA.HL,  2 MUL.DA MUL.DA.LH,  3 MUL.DA MUL.DA.HH,
: MUL.DD o o l o  o l 2 bits  o x o o  o o o o  o y o o  o l o o  OP ;
0 MUL.DD MUL.DD.LL,  1 MUL.DD MUL.DD.HL,  2 MUL.DD MUL.DD.LH,  3 MUL.DD MUL.DD.HH,
l l o l  o o o l  rrrr  ssss  tttt  o o o o  OP MUL16S,
l l o o  o o o l  rrrr  ssss  tttt  o o o o  OP MUL16U,
: MULA.AA o l l l  l o 2 bits  ssss  tttt o l o o  OP ;
0 MULA.AA MULA.AA.LL,  1 MULA.AA MULA.AA.HL,  2 MULA.AA MULA.AA.LH,  3 MULA.AA MULA.AA.HH,
: MULA.AD o o l l  l o 2 bits ssss  tttt o l o o  OP ;
0 MULA.AD MULA.AD.LL,  1 MULA.AD MULA.AD.HL,  2 MULA.AD MULA.AD.LH,  3 MULA.AD MULA.AD.HH,
: MULA.DA o l l o  l o 2 bits  o x o o  o o o o  tttt o l o o  OP ;
0 MULA.DA MULA.DA.LL,  1 MULA.DA MULA.DA.HL,  2 MULA.DA MULA.DA.LH,  3 MULA.DA MULA.DA.HH,
: MULA.DA.LDDEC o l o l  l o 2 bits  o x o o  o o o o  tttt o l o o  OP ;
0 MULA.DA.LDDEC MULA.DA.LL.LDDEC,  1 MULA.DA.LDDEC MULA.DA.HL.LDDEC,
2 MULA.DA.LDDEC MULA.DA.LH.LDDEC,  3 MULA.DA.LDDEC MULA.DA.HH.LDDEC,
: MULA.DA.LDINC o l o o  l o 2 bits  o x w o  o o o o  tttt o l o o  OP ;
0 MULA.DA.LDINC MULA.DA.LL.LDINC,  1 MULA.DA.LDINC MULA.DA.HL.LDINC,
2 MULA.DA.LDINC MULA.DA.LH.LDINC,  3 MULA.DA.LDINC MULA.DA.HH.LDINC,
: MULA.DD o o l o  l o 2 bits  o x o o  o o o o  o y o o  o l o o  OP ;
0 MULA.DD MULA.DD.LL,  1 MULA.DD MULA.DD.HL,  2 MULA.DD MULA.DD.LH,  3 MULA.DD MULA.DD.HH,
: MULA.DD.LDDEC o o o l  l o 2 bits  o x w w  o o o o  tttt o l o o  OP ;
0 MULA.DD.LDDEC MULA.DD.LL.LDDEC,  1 MULA.DD.LDDEC MULA.DD.HL.LDDEC,
2 MULA.DD.LDDEC MULA.DD.LH.LDDEC,  3 MULA.DD.LDDEC MULA.DD.HH.LDDEC,
: MULA.DD.LDINC o o o o  l o 2 bits  o x w w  o o o o  tttt o l o o  OP ;
0 MULA.DD.LDINC MULA.DD.LL.LDINC,  1 MULA.DD.LDINC MULA.DD.HL.LDINC,
2 MULA.DD.LDINC MULA.DD.LH.LDINC,  3 MULA.DD.LDINC MULA.DD.HH.LDINC,
: MULS.AA o l l l  l o 2 bits  o o o o  ssss  tttt  o l o o  OP ;
0 MULS.AA MULA.AA.LL,  1 MULS.AA MULA.AA.HL,  2 MULS.AA MULA.AA.LH,  3 MULS.AA MULA.AA.HH,
: MULS.AD o o l l  l o 2 bits  o o o o  ssss  o y o o  o l o o  OP ;
0 MULS.AD MULA.AD.LL,  1 MULS.AD MULA.AD.HL,  2 MULS.AD MULA.AD.LH,  3 MULS.AD MULA.AD.HH,
: MULS.DA o l l o  l o 2 bits  o x o o  o o o o  tttt  o l o o  OP ;
0 MULS.DA MULA.DA.LL,  1 MULS.DA MULA.DA.HL,  2 MULS.DA MULA.DA.LH,  3 MULS.DA MULA.DA.HH,
: MULS.DD o o l o  l o 2 bits  o x o o  o o o o  o y o o  o l o o  OP ;
0 MULS.DD MULA.DD.LL,  1 MULS.DD MULA.DD.HL,  2 MULS.DD MULA.DD.LH,  3 MULS.DD MULA.DD.HH,

o l o o  o o o o  l l l o  ssss  tttt  o o o o  OP NSA,
o l o o  o o o o  l l l l  ssss  tttt  o o o o  OP NSAU,

o l o l  o o o o  l l o l  ssss  tttt  o o o o  OP PDTLB,
o l o l  o o o o  o l o l  ssss  tttt  o o o o  OP PITLB,
o l o l  o o o o  l o l l  ssss  tttt  o o o o  OP RDTLB0,
o l o l  o o o o  l l l l  ssss  tttt  o o o o  OP RDTLB1,
o l o o  o o o o  o l l o  ssss  tttt  o o o o  OP RER,
o l o l  o o o o  o o l l  ssss  tttt  o o o o  OP RITLB0,
o l o l  o o o o  o l l l  ssss  tttt  o o o o  OP RITLB1,
o l o o  o o o o  l o o o  o o o o  imm4  o o o o  OP ROTW,
o o o o  o o o o  o o l l  imm4  o o o l  o o o o  OP RFI,
o o o o  o o o o  o l l o  imm4  tttt  o o o o  OP RSIL,
o o o o  o o l l  sr  tttt  o o o o  OP RSR,
l l l o  o o l l  rrrr  ssss  tttt  o o o o  OP RUR,
l l l l  o o o l  l o o l  ssss  tttt  o o o o  OP SDCT,
o o l o  o o l l  rrrr  ssss  tttt  o o o o  OP SEXT,
l l l l  o o o l  o o o l  ssss  tttt  o o o o  OP SICT,
l l l l  o o o l  o o l l  ssss  tttt  o o o o  OP SICW,
l o l o  o o o l  rrrr  ssss  o o o o  o o o o  OP SLL,
o o o sa  o o o l  rrrr  ssss  sa sa sa sa  o o o o  OP SLLI,
l o l l  o o o l  rrrr  o o o o  tttt  o o o o  OP SRA,
o o l x  o o o l  rrrr  xxxx  tttt  o o o o  OP SRAI,
l o o o  o o o l  rrrr  ssss  tttt  o o o o  OP SRC,
l o o l  o o o l  rrrr  o o o o  tttt  o o o o  OP SRL,
o l o o  o o o l  rrrr  xxxx  tttt  o o o o  OP SRLI,
o l o o  o o o o  o o l l  ssss  o o o o  o o o o  OP SSA8B,
o l o o  o o o o  o o l o  ssss  o o o o  o o o o  OP SSA8L,
o l o o  o o o o  o l o o  xxxx  o o o x  o o o o  OP SSAI,
imm8  o l o o  ssss  tttt  o o l l  OP SSI,
imm8  l l o o  ssss  tttt  o o l l  OP SSIU,
o l o o  o o o o  o o o l  ssss  o o o o  o o o o  OP SSL,
o l o o  o o o o  o o o o  ssss  o o o o  o o o o  OP SSR,
o l o o  l o o o  rrrr  ssss  tttt  o o o o  OP SSX,
o l o l  l o o o  rrrr  ssss  tttt  o o o o  OP SSXU,
( TODO: UMUL.AA.* )
o o o o  o o o o  o l l l  imm4  o o o o  o o o o  OP WAITI,
o l o l  o o o o  l l l o  ssss  tttt     o o o o  OP WDTLB,
o l o o  o o o o  o l l l  ssss  tttt     o o o o  OP WER,
o l o l  o o o o  o l l o  ssss  tttt     o o o o  OP WITLB,
o o o l  o o l l  sr             tttt     o o o o  OP WSR,
l l l l  o o l l  sr             tttt     o o o o  OP WUR,

also forth definitions
: xtensa-assembler xtensa ;
previous previous
xtensa-assembler
current !

| evaluate ;

[THEN]
)""";

#endif
