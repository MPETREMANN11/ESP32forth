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
 * ESP32forth v7.0.7.21a
 * Updated: Marc PETREMANN - 25 jan. 2026
 */

#if defined(CONFIG_IDF_TARGET_ESP32)
# define UEFORTH_PLATFORM_IS_ESP32 (-1)
#else
# define UEFORTH_PLATFORM_IS_ESP32 0
#endif

#if defined(CONFIG_IDF_TARGET_ESP32S2)
# define UEFORTH_PLATFORM_IS_ESP32S2 (-1)
#else
# define UEFORTH_PLATFORM_IS_ESP32S2 0
#endif

#if defined(CONFIG_IDF_TARGET_ESP32S3)
# define UEFORTH_PLATFORM_IS_ESP32S3 (-1)
#else
# define UEFORTH_PLATFORM_IS_ESP32S3 0
#endif

#if defined(CONFIG_IDF_TARGET_ESP32C3)
# define UEFORTH_PLATFORM_IS_ESP32C3 (-1)
#else
# define UEFORTH_PLATFORM_IS_ESP32C3 0
#endif

#if defined(BOARD_HAS_PSRAM)
# define UEFORTH_PLATFORM_HAS_PSRAM (-1)
#else
# define UEFORTH_PLATFORM_HAS_PSRAM 0
#endif

#if defined(__XTENSA__)
# define UEFORTH_PLATFORM_IS_XTENSA (-1)
#else
# define UEFORTH_PLATFORM_IS_XTENSA 0
#endif

#if defined(__riscv)
# define UEFORTH_PLATFORM_IS_RISCV (-1)
#else
# define UEFORTH_PLATFORM_IS_RISCV 0
#endif
#define STACK_CELLS 512
#define MINIMUM_FREE_SYSTEM_HEAP (64 * 1024)

// Default on several options.
#define ENABLE_SPIFFS_SUPPORT
#define ENABLE_WIFI_SUPPORT
#define ENABLE_MDNS_SUPPORT
#define ENABLE_I2C_SUPPORT
#define ENABLE_SOCKETS_SUPPORT
#define ENABLE_FREERTOS_SUPPORT
#define ENABLE_SD_SUPPORT
#define ENABLE_ESP32_FORTH_FAULT_HANDLING

// LEDC changed interface in v3.x+
#if ESP_ARDUINO_VERSION_MAJOR >= 3
# define ENABLE_LEDC_V3_SUPPORT
#else
# define ENABLE_LEDC_V2_SUPPORT
#endif

// SD_MMC does not work on ESP32-S2 / ESP32-C3
#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3)
# define ENABLE_SD_MMC_SUPPORT
#endif

// Serial2 does not work on ESP32-S2 / ESP32-C3
#if !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32C3)
# define ENABLE_SERIAL2_SUPPORT
#endif

// No DACS on ESP32-S3 and ESP32-C3.
#if !defined(CONFIG_IDF_TARGET_ESP32S3) && !defined(CONFIG_IDF_TARGET_ESP32C3)
# define ENABLE_DAC_SUPPORT
#endif

// ESP32-C3 doesn't support fault handling yet.
#if !defined(CONFIG_IDF_TARGET_ESP32C3)
#endif

#if !defined(USER_VOCABULARIES)
# define USER_VOCABULARIES
#endif

// #if defined(CONFIG_IDF_TARGET_ESP32)
// # define UEFORTH_PLATFORM_IS_ESP32 -1
// #else
// # define UEFORTH_PLATFORM_IS_ESP32 0
// #endif

// #if defined(CONFIG_IDF_TARGET_ESP32S2)
// # define UEFORTH_PLATFORM_IS_ESP32S2 -1
// #else
// # define UEFORTH_PLATFORM_IS_ESP32S2 0
// #endif

// #if defined(CONFIG_IDF_TARGET_ESP32S3)
// # define UEFORTH_PLATFORM_IS_ESP32S3 -1
// #else
// # define UEFORTH_PLATFORM_IS_ESP32S3 0
// #endif

// #if defined(CONFIG_IDF_TARGET_ESP32C3)
// # define UEFORTH_PLATFORM_IS_ESP32C3 -1
// #else
// # define UEFORTH_PLATFORM_IS_ESP32C3 0
// #endif

// #if defined(BOARD_HAS_PSRAM)
// # define UEFORTH_PLATFORM_HAS_PSRAM -1
// #else
// # define UEFORTH_PLATFORM_HAS_PSRAM 0
// #endif

#define VOCABULARY_LIST \
  V(forth) V(internals) \
  V(rtos) V(SPIFFS) V(serial) V(SD) V(SD_MMC) V(ESP) \
  V(ledc) V(Wire) V(WiFi) V(sockets) \
  OPTIONAL_CAMERA_VOCABULARY \
  OPTIONAL_BLUETOOTH_VOCABULARY \
  OPTIONAL_INTERRUPTS_VOCABULARIES \
  OPTIONAL_OLED_VOCABULARY \
  OPTIONAL_RMT_VOCABULARY \
  OPTIONAL_SPI_FLASH_VOCABULARY \
  OPTIONAL_HTTP_CLIENT_VOCABULARY \
  OPTIONAL_ESPNOW_VOCABULARY \
  OPTIONAL_SPI_VOCABULARY \
  USER_VOCABULARIES
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef intptr_t cell_t;
typedef uintptr_t ucell_t;

#define XV(flags, name, op, code) Z(flags, name, op, code)
#define YV(flags, op, code) Z(flags, #op, op, code)
#define X(name, op, code) Z(forth, name, op, code)
#define Y(op, code) Z(forth, #op, op, code)

#define NIP (--sp)
#define NIPn(n) (sp -= (n))
#define DROP (tos = *sp--)
#define DROPn(n) (NIPn(n-1), DROP)
#define DUP (*++sp = tos)
#define PUSH DUP; tos = (cell_t)

#define PARK   *++rp = (cell_t) ip; *++rp = (cell_t) fp; DUP; *++rp = (cell_t) sp;
#define UNPARK sp = (cell_t *) *rp--; DROP; fp = (float *) *rp--; ip = (cell_t *) *rp--;

// proposition patch COPILOT
// #define PARK   *++rp = (cell_t) ip; *++rp = (cell_t) fp; DUP; *++rp = (cell_t) sp;
// #define UNPARK sp = (cell_t *) *rp--; g_sys->sp = sp; DROP; fp = (float *) *rp--; ip = (cell_t *) *rp--;


#define THROWIT(n) \
  rp = *g_sys->throw_handler; *g_sys->throw_handler = (cell_t *) *rp--; UNPARK; tos = (n);

#define TOFLAGS(xt) ((uint8_t *) (((cell_t *) (xt)) - 1))
#define TONAMELEN(xt) (TOFLAGS(xt) + 1)
#define TOPARAMS(xt) ((uint16_t *) (TOFLAGS(xt) + 2))
#define TOSIZE(xt) (CELL_ALIGNED(*TONAMELEN(xt)) + sizeof(cell_t) * (3 + *TOPARAMS(xt)))
#define TOLINK(xt) (((cell_t *) (xt)) - 2)
#define TONAME(xt) ((*TOFLAGS(xt) & BUILTIN_MARK) ? (*(char **) TOLINK(xt)) \
    : (((char *) TOLINK(xt)) - CELL_ALIGNED(*TONAMELEN(xt))))
#define TOBODY(xt) (((cell_t *) xt) + ((void *) *((cell_t *) xt) == ADDROF(DOCREATE) || \
                                       (void *) *((cell_t *) xt) == ADDROF(DODOES) ? 2 : 1))

#ifndef COMMA
# define COMMA(n) *g_sys->heap++ = (cell_t) (n)
# define CCOMMA(n) *(uint8_t *) g_sys->heap = (n); \
                   g_sys->heap = (cell_t *) (1 + ((cell_t) g_sys->heap));
# define DOES(ip) **g_sys->current = (cell_t) ADDROF(DODOES); (*g_sys->current)[1] = (cell_t) ip
# define DOIMMEDIATE() *TOFLAGS(*g_sys->current) |= IMMEDIATE
# define UNSMUDGE() *TOFLAGS(*g_sys->current) &= ~SMUDGE; finish()
#endif

#ifndef SSMOD_FUNC
# if __SIZEOF_POINTER__ == 8
typedef __int128_t dcell_t;
# elif __SIZEOF_POINTER__ == 4 || defined(_M_IX86)
typedef int64_t dcell_t;
# else
#  error "unsupported cell size"
# endif
# define SSMOD_FUNC dcell_t d = (dcell_t) *sp * (dcell_t) sp[-1]; \
                    --sp; cell_t a = (cell_t) (d / tos); \
                    a = a * tos == d ? a : a - ((d < 0) ^ (tos < 0)); \
                    *sp = (cell_t) (d - ((dcell_t) a) * tos); tos = a
#endif

#ifdef WEB_DUMP
// Use */mod as the base for the web version.
# define SLASHMOD_FUNC DUP; *sp = 1; SSMOD_FUNC
# define SLASH_FUNC SLASHMOD_FUNC; NIP
# define MOD_FUNC SLASHMOD_FUNC; DROP
# define CELLSLASH_FUNC DUP; tos = sizeof(cell_t); SLASH_FUNC
#else
// Use separate versions for non-web so throw has the right depth.
# define SLASHMOD_FUNC cell_t d = *sp; cell_t a = d / tos; \
                       cell_t b = a * tos == d ? a : a - ((d < 0) ^ (tos < 0)); \
                       *sp = d - b * tos; tos = b
# define SLASH_FUNC cell_t d = *sp; cell_t a = d / tos; NIP; \
                    tos = a * tos == d ? a : a - ((d < 0) ^ (tos < 0))
# define MOD_FUNC cell_t d = *sp; cell_t a = d / tos; \
                  cell_t b = a * tos == d ? a : a - ((d < 0) ^ (tos < 0)); \
                  NIP; tos = d - b * tos
# define CELLSLASH_FUNC tos = tos < 0 ? ~(~tos / sizeof(cell_t)) : tos / sizeof(cell_t)
#endif

typedef struct {
  const char *name;
  union {
    struct {
      uint8_t flags, name_length;
      uint16_t vocabulary;
    };
    cell_t multi;  // Forces cell alignment throughout.
  };
  const void *code;
} BUILTIN_WORD;

#define TIER0_OPCODE_LIST \
  YV(internals, NOP, ) \
  X("0=", ZEQUAL, tos = !tos ? -1 : 0) \
  X("0<", ZLESS, tos = (tos|0) < 0 ? -1 : 0) \
  X("U<", ULESS, tos = ((ucell_t) *sp) < ((ucell_t) tos) ? -1 : 0; --sp) \
  X("+", PLUS, tos += *sp--) \
  X("U/MOD", USMOD, w = *sp; *sp = (ucell_t) w % (ucell_t) tos; \
                    tos = (ucell_t) w / (ucell_t) tos) \
  X("*/MOD", SSMOD, SSMOD_FUNC) \
  Y(LSHIFT, tos = (*sp << tos); --sp) \
  Y(RSHIFT, tos = (((ucell_t) *sp) >> tos); --sp) \
  Y(ARSHIFT, tos = (*sp >> tos); --sp) \
  Y(AND, tos &= *sp--) \
  Y(OR, tos |= *sp--) \
  Y(XOR, tos ^= *sp--) \
  X("DUP", ALTDUP, DUP) \
  Y(SWAP, w = tos; tos = *sp; *sp = w) \
  Y(OVER, DUP; tos = sp[-1]) \
  X("DROP", ALTDROP, DROP) \
  X("@", AT, tos = *(cell_t *) tos) \
  X("SL@", SLAT, tos = *(int32_t *) tos) \
  X("UL@", ULAT, tos = *(uint32_t *) tos) \
  X("SW@", SWAT, tos = *(int16_t *) tos) \
  X("UW@", UWAT, tos = *(uint16_t *) tos) \
  X("C@", CAT, tos = *(uint8_t *) tos) \
  X("!", STORE, *(cell_t *) tos = *sp--; DROP) \
  X("L!", LSTORE, *(int32_t *) tos = *sp--; DROP) \
  X("W!", WSTORE, *(int16_t *) tos = *sp--; DROP) \
  X("C!", CSTORE, *(uint8_t *) tos = *sp--; DROP) \
  X("SP@", SPAT, DUP; tos = (cell_t) sp) \
  X("SP!", SPSTORE, sp = (cell_t *) tos; DROP) \
  X("RP@", RPAT, DUP; tos = (cell_t) rp) \
  X("RP!", RPSTORE, rp = (cell_t *) tos; DROP) \
  X(">R", TOR, *++rp = tos; DROP) \
  X("R>", FROMR, DUP; tos = *rp; --rp) \
  X("R@", RAT, DUP; tos = *rp) \
  Y(EXECUTE, w = tos; DROP; JMPW) \
  YV(internals, BRANCH, ip = (cell_t *) *ip) \
  YV(internals, 0BRANCH, if (!tos) ip = (cell_t *) *ip; else ++ip; DROP) \
  YV(internals, DONEXT, *rp = *rp - 1; if (~*rp) ip = (cell_t *) *ip; else (--rp, ++ip)) \
  YV(internals, DOLIT, DUP; tos = *ip++) \
  YV(internals, DOSET, *((cell_t *) *ip) = tos; ++ip; DROP) \
  YV(internals, DOCOL, ++rp; *rp = (cell_t) ip; ip = (cell_t *) (w + sizeof(cell_t))) \
  YV(internals, DOCON, DUP; tos = *(cell_t *) (w + sizeof(cell_t))) \
  YV(internals, DOVAR, DUP; tos = w + sizeof(cell_t)) \
  YV(internals, DOCREATE, DUP; tos = w + sizeof(cell_t) * 2) \
  YV(internals, DODOES, DUP; tos = w + sizeof(cell_t) * 2; \
                        ++rp; *rp = (cell_t) ip; \
                        ip = (cell_t *) *(cell_t *) (w + sizeof(cell_t))) \
  YV(internals, ALITERAL, COMMA(g_sys->DOLIT_XT); COMMA(tos); DROP) \
  Y(CELL, DUP; tos = sizeof(cell_t)) \
  XV(internals, "LONG-SIZE", LONG_SIZE, DUP; tos = sizeof(long)) \
  Y(FIND, tos = find((const char *) *sp, tos); --sp) \
  Y(PARSE, DUP; tos = parse(tos, sp)) \
  XV(internals, "S>NUMBER?", \
      CONVERT, tos = convert((const char *) *sp, tos, g_sys->base, sp); \
      if (!tos) --sp) \
  Y(CREATE, DUP; DUP; tos = parse(32, sp); \
            create((const char *) *sp, tos, 0, ADDROF(DOCREATE)); \
            COMMA(0); DROPn(2)) \
  Y(VARIABLE, DUP; DUP; tos = parse(32, sp); \
              create((const char *) *sp, tos, 0, ADDROF(DOVAR)); \
              COMMA(0); DROPn(2)) \
  Y(CONSTANT, DUP; DUP; tos = parse(32, sp); \
              create((const char *) *sp, tos, 0, ADDROF(DOCON)); \
              DROPn(2); COMMA(tos); DROP) \
  X("DOES>", DOES, DOES(ip); ip = (cell_t *) *rp; --rp) \
  Y(IMMEDIATE, DOIMMEDIATE()) \
  X(">BODY", TOBODY, tos = (cell_t) TOBODY(tos)) \
  XV(internals, "'SYS", SYS, DUP; tos = (cell_t) g_sys) \
  YV(internals, YIELD, PARK; return rp) \
  X(":", COLON, DUP; DUP; tos = parse(32, sp); \
                create((const char *) *sp, tos, SMUDGE, ADDROF(DOCOL)); \
                g_sys->state = -1; --sp; DROP) \
  YV(internals, EVALUATE1, PARK; rp = evaluate1(rp); UNPARK; w = tos; DROP; if (w) JMPW) \
  Y(EXIT, ip = (cell_t *) *rp--) \
  XV(internals, "'builtins", TBUILTINS, DUP; tos = (cell_t) &g_sys->builtins->code) \
  XV(forth_immediate, ";", SEMICOLON, COMMA(g_sys->DOEXIT_XT); UNSMUDGE(); g_sys->state = 0)
#define TIER1_OPCODE_LIST \
  XV(internals, "0", ZERO, PUSH 0) \
  XV(internals, "1", ONE, PUSH 1) \
  XV(internals, "-1", NEGATIVEONE, PUSH -1) \
  Y(nip, NIP) \
  Y(rdrop, --rp) \
  XV(forth, "*/", STARSLASH, SSMOD_FUNC; NIP) \
  X("*", STAR, tos *= *sp--) \
  X("/mod", SLASHMOD, SLASHMOD_FUNC) \
  X("/", SLASH, SLASH_FUNC) \
  X("mod", MOD, MOD_FUNC) \
  Y(invert, tos = ~tos) \
  Y(negate, tos = -tos) \
  X("-", MINUS, tos = (*sp--) - tos) \
  Y(rot, w = sp[-1]; sp[-1] = *sp; *sp = tos; tos = w) \
  X("-rot", MROT, w = tos; tos = *sp; *sp = sp[-1]; sp[-1] = w) \
  X("?dup", QDUP, if (tos) DUP) \
  X("<", LESS, tos = *sp < tos ? -1 : 0; --sp) \
  X(">", GREATER, tos = *sp > tos ? -1 : 0; --sp) \
  X("<=", LESSEQ, tos = *sp <= tos ? -1 : 0; --sp) \
  X(">=", GREATEREQ, tos = *sp >= tos ? -1 : 0; --sp) \
  X("=", EQUAL, tos = *sp == tos ? -1 : 0; --sp) \
  X("<>", NOTEQUAL, tos = *sp != tos ? -1 : 0; --sp) \
  X("0<>", ZNOTEQUAL, tos = tos ? -1 : 0) \
  Y(bl, DUP; tos = ' ') \
  Y(nl, DUP; tos = '\n') \
  X("1+", ONEPLUS, ++tos) \
  X("1-", ONEMINUS, --tos) \
  X("2*", TWOSTAR, tos = tos << 1) \
  X("2/", TWOSLASH, tos = tos >> 1) \
  X("4*", FOURSTAR, tos = tos << 2) \
  X("4/", FOURSLASH, tos = tos >> 2) \
  X("+!", PLUSSTORE, *((cell_t *) tos) += *sp--; DROP) \
  X("cell+", CELLPLUS, tos += sizeof(cell_t)) \
  Y(cells, tos *= sizeof(cell_t)) \
  X("cell/", CELLSLASH, CELLSLASH_FUNC) \
  X("2drop", TWODROP, NIP; DROP) \
  X("2dup", TWODUP, DUP; tos = sp[-1]; DUP; tos = sp[-1]) \
  X("3dup", THREEDUP, sp += 3; sp[-2] = tos; sp[-1] = sp[-4]; *sp = sp[-3]) \
  X("2@", TWOAT, DUP; *sp = *(cell_t *) tos; tos = ((cell_t *) tos)[1]) \
  X("2!", TWOSTORE, *(cell_t *) tos = sp[-1]; \
      ((cell_t *) tos)[1] = *sp; sp -= 2; DROP) \
  Y(cmove, memmove((void *) *sp, (void *) sp[-1], tos); sp -= 2; DROP) \
  X("cmove>", cmove2, memmove((void *) *sp, (void *) sp[-1], tos); sp -= 2; DROP) \
  Y(fill, memset((void *) sp[-1], tos, *sp); sp -= 2; DROP) \
  Y(erase, memset((void *) *sp, 0, tos); NIP; DROP) \
  Y(blank, memset((void *) *sp, ' ', tos); NIP; DROP) \
  Y(min, tos = tos < *sp ? tos : *sp; NIP) \
  Y(max, tos = tos > *sp ? tos : *sp; NIP) \
  Y(abs, tos = tos < 0 ? -tos : tos) \
  Y(here, DUP; tos = (cell_t) g_sys->heap) \
  Y(allot, g_sys->heap = (cell_t *) (tos + (cell_t) g_sys->heap); DROP) \
  X(",", COMMA, COMMA(tos); DROP) \
  X("c,", CCOMMA, CCOMMA(tos); DROP) \
  XV(internals, "'heap", THEAP, DUP; tos = (cell_t) &g_sys->heap) \
  Y(current, DUP; tos = (cell_t) &g_sys->current) \
  XV(internals, "'context", TCONTEXT, DUP; tos = (cell_t) &g_sys->context) \
  XV(internals, "'latestxt", TLATESTXT, DUP; tos = (cell_t) &g_sys->latestxt) \
  XV(internals, "'notfound", TNOTFOUND, DUP; tos = (cell_t) &g_sys->notfound) \
  XV(internals, "'heap-start", THEAP_START, DUP; tos = (cell_t) &g_sys->heap_start) \
  XV(internals, "'heap-size", THEAP_SIZE, DUP; tos = (cell_t) &g_sys->heap_size) \
  XV(internals, "'stack-cells", TSTACK_CELLS, DUP; tos = (cell_t) &g_sys->stack_cells) \
  XV(internals, "'boot", TBOOT, DUP; tos = (cell_t) &g_sys->boot) \
  XV(internals, "'boot-size", TBOOT_SIZE, DUP; tos = (cell_t) &g_sys->boot_size) \
  XV(internals, "'tib", TTIB, DUP; tos = (cell_t) &g_sys->tib) \
  X("#tib", NTIB, DUP; tos = (cell_t) &g_sys->ntib) \
  X(">in", TIN, DUP; tos = (cell_t) &g_sys->tin) \
  Y(state, DUP; tos = (cell_t) &g_sys->state) \
  Y(base, DUP; tos = (cell_t) &g_sys->base) \
  XV(internals, "'argc", ARGC, DUP; tos = (cell_t) &g_sys->argc) \
  XV(internals, "'argv", ARGV, DUP; tos = (cell_t) &g_sys->argv) \
  XV(internals, "'runner", RUNNER, DUP; tos = (cell_t) &g_sys->runner) \
  XV(internals, "'throw-handler", TTHROW_HANDLER, DUP; tos = (cell_t) &g_sys->throw_handler) \
  Y(context, DUP; tos = (cell_t) (g_sys->context + 1)) \
  Y(latestxt, DUP; tos = (cell_t) g_sys->latestxt) \
  XV(forth_immediate, "[", LBRACKET, g_sys->state = 0) \
  XV(forth_immediate, "]", RBRACKET, g_sys->state = -1) \
  YV(forth_immediate, literal, COMMA(g_sys->DOLIT_XT); COMMA(tos); DROP)
#define TIER2_OPCODE_LIST \
  X(">flags", TOFLAGS, tos = *TOFLAGS(tos)) \
  X(">flags&", TOFLAGSAT, tos = (cell_t) TOFLAGS(tos)) \
  X(">params", TOPARAMS, tos = *TOPARAMS(tos)) \
  X(">size", TOSIZE, tos = TOSIZE(tos)) \
  X(">link&", TOLINKAT, tos = (cell_t) TOLINK(tos)) \
  X(">link", TOLINK, tos = *TOLINK(tos)) \
  X(">name", TONAME, DUP; *sp = (cell_t) TONAME(tos); tos = *TONAMELEN(tos)) \
  Y(aligned, tos = CELL_ALIGNED(tos)) \
  Y(align, g_sys->heap = (cell_t *) CELL_ALIGNED(g_sys->heap)) \
  YV(internals, fill32, cell_t c = tos; DROP; cell_t n = tos; DROP; \
                        uint32_t *a = (uint32_t *) tos; DROP; \
                        for (;n;--n) *a++ = c)
#include <math.h>

#define FLOATING_POINT_LIST \
  YV(internals, DOFLIT, *++fp = *(float *) ip; ++ip) \
  X("FP@", FPAT, DUP; tos = (cell_t) fp) \
  X("FP!", FPSTORE, fp = (float *) tos; DROP) \
  X("SF@", FAT, *++fp = *(float *) tos; DROP) \
  X("SF!", FSTORE, *(float *) tos = *fp--; DROP) \
  Y(FDUP, fp[1] = *fp; ++fp) \
  Y(FNIP, fp[-1] = *fp; --fp) \
  Y(FDROP, --fp) \
  Y(FOVER, fp[1] = fp[-1]; ++fp) \
  Y(FSWAP, ft = fp[-1]; fp[-1] = *fp; *fp = ft) \
  Y(FROT, ft = fp[-2]; fp[-2] = fp[-1]; fp[-1] = *fp; *fp = ft) \
  Y(FNEGATE, *fp = -*fp) \
  X("F0<", FZLESS, DUP; tos = *fp < 0.0f ? -1 : 0; --fp) \
  X("F0=", FZEQUAL, DUP; tos = *fp == 0.0f ? -1 : 0; --fp) \
  X("F=", FEQUAL, DUP; tos = fp[-1] == fp[0] ? -1 : 0; fp -= 2) \
  X("F<", FLESS, DUP; tos = fp[-1] < fp[0] ? -1 : 0; fp -= 2) \
  X("F>", FGREATER, DUP; tos = fp[-1] > fp[0] ? -1 : 0; fp -= 2) \
  X("F<>", FNEQUAL, DUP; tos = fp[-1] != fp[0] ? -1 : 0; fp -= 2) \
  X("F<=", FLESSEQ, DUP; tos = fp[-1] <= fp[0] ? -1 : 0; fp -= 2) \
  X("F>=", FGREATEREQ, DUP; tos = fp[-1] >= fp[0] ? -1 : 0; fp -= 2) \
  X("F+", FPLUS, fp[-1] = fp[-1] + *fp; --fp) \
  X("F-", FMINUS, fp[-1] = fp[-1] - *fp; --fp) \
  X("F*", FSTAR, fp[-1] = fp[-1] * *fp; --fp) \
  X("F/", FSLASH, fp[-1] = fp[-1] / *fp; --fp) \
  X("1/F", FINVERSE, *fp = 1.0f / *fp) \
  X("S>F", STOF, *++fp = (float) tos; DROP) \
  X("F>S", FTOS, DUP; tos = (cell_t) *fp--) \
  XV(internals, "S>FLOAT?", FCONVERT, \
      ++fp; tos = fconvert((const char *) *sp, tos, fp)|0; if (!tos) --fp; --sp) \
  Y(SFLOAT, DUP; tos = sizeof(float)) \
  Y(SFLOATS, tos *= sizeof(float)) \
  X("SFLOAT+", SFLOATPLUS, tos += sizeof(float)) \
  X("PI", PI_CONST, *++fp = (float) 3.14159265359) \
  Y(FSIN, *fp = sin(+*fp)) \
  Y(FCOS, *fp = cos(+*fp)) \
  Y(FSINCOS, fp[1] = cos(+*fp); *fp = sin(+*fp); ++fp) \
  Y(FATAN2, fp[-1] = atan2(+fp[-1], +*fp); --fp) \
  X("F**", FSTARSTAR, fp[-1] = pow(+fp[-1], +*fp); --fp) \
  Y(FLOOR, *fp = floor(+*fp)) \
  Y(FEXP, *fp = exp(+*fp)) \
  Y(FLN, *fp = log(+*fp)) \
  Y(FABS, *fp = fabs(+*fp)) \
  Y(FMIN, fp[-1] = fmin(+fp[-1], +*fp); --fp) \
  Y(FMAX, fp[-1] = fmax(+fp[-1], +*fp); --fp) \
  Y(FSQRT, *fp = sqrt(+*fp))

#ifndef CALLTYPE
# define CALLTYPE
#endif

#ifdef __cplusplus
typedef cell_t (CALLTYPE *call_t)(...);
#else
typedef cell_t (CALLTYPE *call_t)();
#endif

#define ct0 ((call_t) n0)

#define CALLING_OPCODE_LIST \
  YV(internals, CALLCODE, float *t_fp = fp; DUP; \
      sp = (cell_t *) (*(call_t*) (w + sizeof(cell_t)))(sp, &t_fp); \
      fp = t_fp; DROP) \
  YV(internals, CALL0, n0 = ct0()) \
  YV(internals, CALL1, n0 = ct0(n1); --sp) \
  YV(internals, CALL2, n0 = ct0(n2, n1); sp -= 2) \
  YV(internals, CALL3, n0 = ct0(n3, n2, n1); sp -= 3) \
  YV(internals, CALL4, n0 = ct0(n4, n3, n2, n1); sp -= 4) \
  YV(internals, CALL5, n0 = ct0(n5, n4, n3, n2, n1); sp -= 5) \
  YV(internals, CALL6, n0 = ct0(n6, n5, n4, n3, n2, n1); sp -= 6) \
  YV(internals, CALL7, n0 = ct0(n7, n6, n5, n4, n3, n2, n1); sp -= 7) \
  YV(internals, CALL8, n0 = ct0(n8, n7, n6, n5, n4, n3, n2, n1); sp -= 8) \
  YV(internals, CALL9, n0 = ct0(n9, n8, n7, n6, n5, n4, n3, n2, n1); sp -= 9) \
  YV(internals, CALL10, n0 = ct0(n10, n9, n8, n7, n6, n5, n4, n3, n2, n1); sp -= 10) \
  YV(internals, CALL11, n0 = ct0(n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1); sp -= 11) \
  YV(internals, CALL12, n0 = ct0(n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1); sp -= 12) \
  YV(internals, CALL13, n0 = ct0(n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1); sp -= 13) \
  YV(internals, CALL14, n0 = ct0(n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1); sp -= 14) \
  YV(internals, CALL15, n0 = ct0(n15, n14, n13, n12, n11, n10, n9, n8, n7, n6, n5, n4, n3, n2, n1); sp -= 15)
#define IMMEDIATE 1
#define SMUDGE 2
#define BUILTIN_FORK 4
#define BUILTIN_MARK 8

typedef struct {
  cell_t *heap, **current, ***context;
  cell_t *latestxt, notfound;
  cell_t *heap_start;
  cell_t heap_size, stack_cells;
  const char *boot;
  cell_t boot_size;
  const char *tib;
  cell_t ntib, tin, state, base;
  int argc;
  char **argv;
  cell_t *(*runner)(cell_t *rp);  // pointer to forth_run
  cell_t **throw_handler;

  // Layout not used by Forth.
  cell_t *rp;  // spot to park main thread
  cell_t *sp;  // ** rajout sur suggestion CoPILOT GITHUB
  cell_t DOLIT_XT, DOFLIT_XT, DOEXIT_XT, YIELD_XT;
  void *DOCREATE_OP;
  const BUILTIN_WORD *builtins;
} G_SYS;

static G_SYS *g_sys = 0;
static cell_t *forth_run(cell_t *init_rp);
#ifndef SIM_PRINT_ONLY

# include <dirent.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/select.h>

// Hook to pull in words from optional userwords.h
# if __has_include("userwords.h")
#  include "userwords.h"
# else
#  define USER_WORDS
# endif

// Hook to pull in words from optional assemblers.
# if __has_include("assemblers.h")
#  include "assemblers.h"
# else
#  define OPTIONAL_ASSEMBLERS_SUPPORT
# endif

// Hook to pull in optional interrupts and timers support.
# if __has_include("interrupts.h")
#  include "interrupts.h"
# else
#  define OPTIONAL_INTERRUPTS_VOCABULARIES
#  define OPTIONAL_INTERRUPTS_SUPPORT
# endif

// Hook to pull in optional Oled support.
# if __has_include("oled.h")
#  include "oled.h"
# else
#  define OPTIONAL_OLED_VOCABULARY
#  define OPTIONAL_OLED_SUPPORT
# endif

// Hook to pull in optional ESP32-CAM camera support.
# if __has_include("camera.h")
#  include "camera.h"
# else
#  define OPTIONAL_CAMERA_VOCABULARY
#  define OPTIONAL_CAMERA_SUPPORT
# endif

// Hook to pull in optional RMT (Remote Control) support.
# if __has_include("rmt.h")
#  include "rmt.h"
# else
#  define OPTIONAL_RMT_VOCABULARY
#  define OPTIONAL_RMT_SUPPORT
# endif

// Hook to pull in optional serial bluetooth support.
# if __has_include("serial-bluetooth.h")
#  include "serial-bluetooth.h"
# else
#  define OPTIONAL_BLUETOOTH_VOCABULARY
#  define OPTIONAL_SERIAL_BLUETOOTH_SUPPORT
# endif

// Hook to pull in optional SPI flash support.
# if __has_include("spi-flash.h")
#  include "spi-flash.h"
# else
#  define OPTIONAL_SPI_FLASH_VOCABULARY
#  define OPTIONAL_SPI_FLASH_SUPPORT
# endif

// Hook to pull in optional HTTPClient support.
# if __has_include("http-client.h")
#  include "http-client.h"
# else
#  define OPTIONAL_HTTP_CLIENT_VOCABULARY
#  define OPTIONAL_HTTP_CLIENT_SUPPORT
# endif

// Hook to pull in optional espnow
#if __has_include("espnow-voc.h")
# include "espnow-voc.h"
#else
# define OPTIONAL_ESPNOW_VOCABULARY
# define OPTIONAL_ESPNOW_SUPPORT
#endif

// Hook to pull in optional SPI support.
# if __has_include("spi-voc.h")
#  include "spi-voc.h"
# else
#  define OPTIONAL_SPI_VOCABULARY
#  define OPTIONAL_SPI_SUPPORT
# endif




static cell_t ResizeFile(cell_t fd, cell_t size);

#endif

#define PLATFORM_OPCODE_LIST \
  USER_WORDS \
  EXTERNAL_OPTIONAL_MODULE_SUPPORT \
  REQUIRED_PLATFORM_SUPPORT \
  REQUIRED_ESP_SUPPORT \
  REQUIRED_MEMORY_SUPPORT \
  REQUIRED_SERIAL_SUPPORT \
  OPTIONAL_SERIAL2_SUPPORT \
  REQUIRED_ARDUINO_GPIO_SUPPORT \
  REQUIRED_SYSTEM_SUPPORT \
  REQUIRED_FILES_SUPPORT \
  OPTIONAL_LEDC_V2_SUPPORT \
  OPTIONAL_LEDC_V3_SUPPORT \
  OPTIONAL_DAC_SUPPORT \
  OPTIONAL_SPIFFS_SUPPORT \
  OPTIONAL_WIFI_SUPPORT \
  OPTIONAL_MDNS_SUPPORT \
  OPTIONAL_SD_SUPPORT \
  OPTIONAL_SD_MMC_SUPPORT \
  OPTIONAL_I2C_SUPPORT \
  OPTIONAL_SOCKETS_SUPPORT \
  OPTIONAL_FREERTOS_SUPPORT \
  CALLING_OPCODE_LIST \
  FLOATING_POINT_LIST

#define EXTERNAL_OPTIONAL_MODULE_SUPPORT \
  OPTIONAL_ASSEMBLERS_SUPPORT \
  OPTIONAL_CAMERA_SUPPORT \
  OPTIONAL_INTERRUPTS_SUPPORT \
  OPTIONAL_OLED_SUPPORT \
  OPTIONAL_RMT_SUPPORT \
  OPTIONAL_SERIAL_BLUETOOTH_SUPPORT \
  OPTIONAL_SPI_FLASH_SUPPORT \
  OPTIONAL_HTTP_CLIENT_SUPPORT \
  OPTIONAL_SPI_SUPPORT \
  OPTIONAL_ESPNOW_SUPPORT

#define REQUIRED_MEMORY_SUPPORT \
  YV(internals, MALLOC, SET malloc(n0)) \
  YV(internals, SYSFREE, free(a0); DROP) \
  YV(internals, REALLOC, SET realloc(a1, n0); NIP) \
  YV(internals, heap_caps_malloc, SET heap_caps_malloc(n1, n0); NIP) \
  YV(internals, heap_caps_free, heap_caps_free(a0); DROP) \
  YV(internals, heap_caps_realloc, \
      tos = (cell_t) heap_caps_realloc(a2, n1, n0); NIPn(2)) \
  YV(internals, heap_caps_get_total_size, n0 = heap_caps_get_total_size(n0)) \
  YV(internals, heap_caps_get_free_size, n0 = heap_caps_get_free_size(n0)) \
  YV(internals, heap_caps_get_minimum_free_size, \
      n0 = heap_caps_get_minimum_free_size(n0)) \
  YV(internals, heap_caps_get_largest_free_block, \
      n0 = heap_caps_get_largest_free_block(n0))

#define REQUIRED_PLATFORM_SUPPORT \
  X("ESP32?", IS_ESP32, PUSH UEFORTH_PLATFORM_IS_ESP32) \
  X("ESP32-S2?", IS_ESP32S2, PUSH UEFORTH_PLATFORM_IS_ESP32S2) \
  X("ESP32-S3?", IS_ESP32S3, PUSH UEFORTH_PLATFORM_IS_ESP32S3) \
  X("ESP32-C3?", IS_ESP32C3, PUSH UEFORTH_PLATFORM_IS_ESP32C3) \
  X("PSRAM?", HAS_PSRAM, PUSH UEFORTH_PLATFORM_HAS_PSRAM) \
  X("Xtensa?", IS_XTENSA, PUSH UEFORTH_PLATFORM_IS_XTENSA) \
  X("RISC-V?", IS_RISCV, PUSH UEFORTH_PLATFORM_IS_RISCV)

#define REQUIRED_ESP_SUPPORT \
  YV(ESP, getHeapSize, PUSH ESP.getHeapSize()) \
  YV(ESP, getFreeHeap, PUSH ESP.getFreeHeap()) \
  YV(ESP, getMaxAllocHeap, PUSH ESP.getMaxAllocHeap()) \
  YV(ESP, getChipModel, PUSH ESP.getChipModel()) \
  YV(ESP, getChipCores, PUSH ESP.getChipCores()) \
  YV(ESP, getFlashChipSize, PUSH ESP.getFlashChipSize()) \
  YV(ESP, getCpuFreqMHz, PUSH ESP.getCpuFreqMHz()) \
  YV(ESP, getSketchSize, PUSH ESP.getSketchSize()) \
  YV(ESP, deepSleep, ESP.deepSleep(tos); DROP) \
  YV(ESP, getEfuseMac, PUSH (cell_t) ESP.getEfuseMac(); PUSH (cell_t) (ESP.getEfuseMac() >> 32)) \
  YV(ESP, esp_log_level_set, esp_log_level_set(c1, (esp_log_level_t) n0); DROPn(2))

#define REQUIRED_SYSTEM_SUPPORT \
  X("MS-TICKS", MS_TICKS, PUSH millis()) \
  XV(internals, "RAW-YIELD", RAW_YIELD, yield()) \
  XV(internals, "RAW-TERMINATE", RAW_TERMINATE, ESP.restart())

#define REQUIRED_SERIAL_SUPPORT \
  XV(serial, "Serial.begin", SERIAL_BEGIN, Serial.begin(tos); DROP) \
  XV(serial, "Serial.end", SERIAL_END, Serial.end()) \
  XV(serial, "Serial.available", SERIAL_AVAILABLE, PUSH Serial.available()) \
  XV(serial, "Serial.readBytes", SERIAL_READ_BYTES, n0 = Serial.readBytes(b1, n0); NIP) \
  XV(serial, "Serial.write", SERIAL_WRITE, n0 = Serial.write(b1, n0); NIP) \
  XV(serial, "Serial.flush", SERIAL_FLUSH, Serial.flush()) \
  XV(serial, "Serial.setDebugOutput", SERIAL_DEBUG_OUTPUT, Serial.setDebugOutput(n0); DROP)

#ifndef ENABLE_SERIAL2_SUPPORT
# define OPTIONAL_SERIAL2_SUPPORT
#else
# define OPTIONAL_SERIAL2_SUPPORT \
  XV(serial, "Serial2.begin", SERIAL2_BEGIN, Serial2.begin(tos); DROP) \
  XV(serial, "Serial2.end", SERIAL2_END, Serial2.end()) \
  XV(serial, "Serial2.available", SERIAL2_AVAILABLE, PUSH Serial2.available()) \
  XV(serial, "Serial2.readBytes", SERIAL2_READ_BYTES, n0 = Serial2.readBytes(b1, n0); NIP) \
  XV(serial, "Serial2.write", SERIAL2_WRITE, n0 = Serial2.write(b1, n0); NIP) \
  XV(serial, "Serial2.flush", SERIAL2_FLUSH, Serial2.flush()) \
  XV(serial, "Serial2.setDebugOutput", SERIAL2_DEBUG_OUTPUT, Serial2.setDebugOutput(n0); DROP)
#endif

#define REQUIRED_ARDUINO_GPIO_SUPPORT \
  Y(pinMode, pinMode(n1, n0); DROPn(2)) \
  Y(digitalWrite, digitalWrite(n1, n0); DROPn(2)) \
  Y(digitalRead, n0 = digitalRead(n0)) \
  Y(analogRead, n0 = analogRead(n0)) \
  Y(pulseIn, n0 = pulseIn(n2, n1, n0); NIPn(2))

#define REQUIRED_FILES_SUPPORT \
  X("R/O", R_O, PUSH O_RDONLY) \
  X("W/O", W_O, PUSH O_WRONLY) \
  X("R/W", R_W, PUSH O_RDWR) \
  Y(BIN, ) \
  X("CLOSE-FILE", CLOSE_FILE, tos = close(tos); tos = tos ? errno : 0) \
  X("FLUSH-FILE", FLUSH_FILE, fsync(tos); /* fsync has no impl and returns ENOSYS :-( */ tos = 0) \
  X("OPEN-FILE", OPEN_FILE, cell_t mode = n0; DROP; cell_t len = n0; DROP; \
    memcpy(filename, a0, len); filename[len] = 0; \
    n0 = open(filename, mode, 0777); PUSH n0 < 0 ? errno : 0) \
  X("CREATE-FILE", CREATE_FILE, cell_t mode = n0; DROP; cell_t len = n0; DROP; \
    memcpy(filename, a0, len); filename[len] = 0; \
    n0 = open(filename, mode | O_CREAT | O_TRUNC); PUSH n0 < 0 ? errno : 0) \
  X("DELETE-FILE", DELETE_FILE, cell_t len = n0; DROP; \
    memcpy(filename, a0, len); filename[len] = 0; \
    n0 = unlink(filename); n0 = n0 ? errno : 0) \
  X("RENAME-FILE", RENAME_FILE, \
    cell_t len = n0; DROP; memcpy(filename, a0, len); filename[len] = 0; DROP; \
    cell_t len2 = n0; DROP; memcpy(filename2, a0, len2); filename2[len2] = 0; \
    n0 = rename(filename2, filename); n0 = n0 ? errno : 0) \
  X("WRITE-FILE", WRITE_FILE, cell_t fd = n0; DROP; cell_t len = n0; DROP; \
    n0 = write(fd, a0, len); n0 = n0 != len ? errno : 0) \
  X("READ-FILE", READ_FILE, cell_t fd = n0; DROP; cell_t len = n0; DROP; \
    n0 = read(fd, a0, len); PUSH n0 < 0 ? errno : 0) \
  X("FILE-POSITION", FILE_POSITION, \
    n0 = (cell_t) lseek(n0, 0, SEEK_CUR); PUSH n0 < 0 ? errno : 0) \
  X("REPOSITION-FILE", REPOSITION_FILE, cell_t fd = n0; DROP; \
    n0 = (cell_t) lseek(fd, tos, SEEK_SET); n0 = n0 < 0 ? errno : 0) \
  X("RESIZE-FILE", RESIZE_FILE, cell_t fd = n0; DROP; n0 = ResizeFile(fd, tos)) \
  X("FILE-SIZE", FILE_SIZE, struct stat st; w = fstat(n0, &st); \
    n0 = (cell_t) st.st_size; PUSH w < 0 ? errno : 0) \
  X("NON-BLOCK", NON_BLOCK, n0 = fcntl(n0, F_SETFL, O_NONBLOCK); \
    n0 = n0 < 0 ? errno : 0) \
  X("OPEN-DIR", OPEN_DIR, memcpy(filename, a1, n0); filename[n0] = 0; \
    n1 = (cell_t) opendir(filename); n0 = n1 ? 0 : errno) \
  X("CLOSE-DIR", CLOSE_DIR, n0 = closedir((DIR *) n0); n0 = n0 ? errno : 0) \
  YV(internals, READDIR, \
    struct dirent *ent = readdir((DIR *) n0); SET (ent ? ent->d_name: 0))

#ifndef ENABLE_LEDC_V2_SUPPORT
# define OPTIONAL_LEDC_V2_SUPPORT
#else
# define OPTIONAL_LEDC_V2_SUPPORT \
  YV(ledc, ledcSetup, \
      n0 = (cell_t) (1000000 * ledcSetup(n2, n1 / 1000.0, n0)); NIPn(2)) \
  YV(ledc, ledcAttachPin, ledcAttachPin(n1, n0); DROPn(2)) \
  YV(ledc, ledcDetachPin, ledcDetachPin(n0); DROP) \
  YV(ledc, ledcRead, n0 = ledcRead(n0)) \
  YV(ledc, ledcReadFreq, n0 = (cell_t) (1000000 * ledcReadFreq(n0))) \
  YV(ledc, ledcWrite, ledcWrite(n1, n0); DROPn(2)) \
  YV(ledc, ledcWriteTone, \
      n0 = (cell_t) (1000000 * ledcWriteTone(n1, n0 / 1000.0)); NIP) \
  YV(ledc, ledcWriteNote, \
      tos = (cell_t) (1000000 * ledcWriteNote(n2, (note_t) n1, n0)); NIPn(2))
#endif

#ifndef ENABLE_LEDC_V3_SUPPORT
# define OPTIONAL_LEDC_V3_SUPPORT
#else
# define OPTIONAL_LEDC_V3_SUPPORT \
  YV(ledc, ledcAttach, n0 = ledcAttach(n2, n1, n0); NIPn(2)) \
  YV(ledc, ledcAttachChannel, n0 = ledcAttachChannel(n3, n2, n1, n0); NIPn(3)) \
  YV(ledc, ledcDetach, n0 = ledcDetach(n0)) \
  YV(ledc, ledcRead, n0 = ledcRead(n0)) \
  YV(ledc, ledcReadFreq, n0 = ledcReadFreq(n0)) \
  YV(ledc, ledcWrite, ledcWrite(n1, n0); DROPn(2)) \
  YV(ledc, ledcWriteTone, n0 = ledcWriteTone(n1, n0); NIP) \
  YV(ledc, ledcWriteNote, n0 = ledcWriteNote(n2, (note_t) n1, n0); NIPn(2)) \
  YV(ledc, ledcChangeFrequency, n0 = ledcChangeFrequency(n2, n1, n0); NIPn(2))
#endif

#ifndef ENABLE_DAC_SUPPORT
# define OPTIONAL_DAC_SUPPORT
# else
# define OPTIONAL_DAC_SUPPORT \
  Y(dacWrite, dacWrite(n1, n0); DROPn(2))
#endif

#ifndef ENABLE_SPIFFS_SUPPORT
// Provide a default failing SPIFFS.begin
# define OPTIONAL_SPIFFS_SUPPORT \
  X("SPIFFS.begin", SPIFFS_BEGIN, NIPn(2); n0 = 0)
#else
# ifndef SIM_PRINT_ONLY
#  include "SPIFFS.h"
# endif
# define OPTIONAL_SPIFFS_SUPPORT \
  XV(SPIFFS, "SPIFFS.begin", SPIFFS_BEGIN, \
      tos = SPIFFS.begin(n2, c1, n0); NIPn(2)) \
  XV(SPIFFS, "SPIFFS.end", SPIFFS_END, SPIFFS.end()) \
  XV(SPIFFS, "SPIFFS.format", SPIFFS_FORMAT, PUSH SPIFFS.format()) \
  XV(SPIFFS, "SPIFFS.totalBytes", SPIFFS_TOTAL_BYTES, PUSH SPIFFS.totalBytes()) \
  XV(SPIFFS, "SPIFFS.usedBytes", SPIFFS_USED_BYTES, PUSH SPIFFS.usedBytes())
#endif

#ifndef ENABLE_FREERTOS_SUPPORT
# define OPTIONAL_FREERTOS_SUPPORT
#else
# ifndef SIM_PRINT_ONLY
#  include "freertos/FreeRTOS.h"
#  include "freertos/task.h"
# endif
# define OPTIONAL_FREERTOS_SUPPORT \
  YV(rtos, vTaskDelete, vTaskDelete((TaskHandle_t) n0); DROP) \
  YV(rtos, xTaskCreatePinnedToCore, n0 = xTaskCreatePinnedToCore((TaskFunction_t) a6, \
        c5, n4, a3, (UBaseType_t) n2, (TaskHandle_t *) a1, (BaseType_t) n0); NIPn(6)) \
  YV(rtos, xPortGetCoreID, PUSH xPortGetCoreID())
#endif

#ifndef ENABLE_SOCKETS_SUPPORT
# define OPTIONAL_SOCKETS_SUPPORT
#else
# ifndef SIM_PRINT_ONLY
#  include <errno.h>
#  include <netdb.h>
#  include <sys/select.h>
#  include <sys/socket.h>
#  include <sys/time.h>
#  include <sys/types.h>
#  include <sys/un.h>
#  include <sys/poll.h>
# endif
# define OPTIONAL_SOCKETS_SUPPORT \
  YV(sockets, socket, n0 = socket(n2, n1, n0); NIPn(2)) \
  YV(sockets, setsockopt, n0 = setsockopt(n4, n3, n2, a1, n0); NIPn(4)) \
  YV(sockets, bind, n0 = bind(n2, (struct sockaddr *) a1, n0); NIPn(2)) \
  YV(sockets, listen, n0 = listen(n1, n0); NIP) \
  YV(sockets, connect, n0 = connect(n2, (struct sockaddr *) a1, n0); NIPn(2)) \
  YV(sockets, sockaccept, n0 = accept(n2, (struct sockaddr *) a1, (socklen_t *) a0); NIPn(2)) \
  YV(sockets, select, n0 = select(n4, (fd_set *) a3, (fd_set *) a2, (fd_set *) a1, (struct timeval *) a0); NIPn(4)) \
  YV(sockets, poll, n0 = poll((struct pollfd *) a2, (nfds_t) n1, n0); NIPn(2)) \
  YV(sockets, send, n0 = send(n3, a2, n1, n0); NIPn(3)) \
  YV(sockets, sendto, n0 = sendto(n5, a4, n3, n2, (const struct sockaddr *) a1, n0); NIPn(5)) \
  YV(sockets, sendmsg, n0 = sendmsg(n2, (const struct msghdr *) a1, n0); NIPn(2)) \
  YV(sockets, recv, n0 = recv(n3, a2, n1, n0); NIPn(3)) \
  YV(sockets, recvfrom, n0 = recvfrom(n5, a4, n3, n2, (struct sockaddr *) a1, (socklen_t *) a0); NIPn(5)) \
  YV(sockets, recvmsg, n0 = recvmsg(n2, (struct msghdr *) a1, n0); NIPn(2)) \
  YV(sockets, gethostbyname, n0 = (cell_t) gethostbyname(c0)) \
  XV(sockets, "errno", ERRNO, PUSH errno)
#endif

#ifndef ENABLE_SD_SUPPORT
# define OPTIONAL_SD_SUPPORT
#else
# ifndef SIM_PRINT_ONLY
#  include "SD.h"
# endif
# define OPTIONAL_SD_SUPPORT \
  XV(SD, "SD.begin", SD_BEGIN, PUSH SD.begin()) \
  XV(SD, "SD.beginFull", SD_BEGIN_FULL, \
      tos = SD.begin(n5, *(SPIClass*)a4, n3, c2, n1, n0); NIPn(5)) \
  XV(SD, "SD.beginDefaults", SD_BEGIN_DEFAULTS, \
      PUSH SS; PUSH &SPI; PUSH 4000000; PUSH "/sd"; PUSH 5; PUSH false) \
  XV(SD, "SD.end", SD_END, SD.end()) \
  XV(SD, "SD.cardType", SD_CARD_TYPE, PUSH SD.cardType()) \
  XV(SD, "SD.totalBytes", SD_TOTAL_BYTES, PUSH SD.totalBytes()) \
  XV(SD, "SD.usedBytes", SD_USED_BYTES, PUSH SD.usedBytes())
#endif

#ifndef ENABLE_SD_MMC_SUPPORT
# define OPTIONAL_SD_MMC_SUPPORT
#else
# ifndef SIM_PRINT_ONLY
#  include "SD_MMC.h"
# endif
# define OPTIONAL_SD_MMC_SUPPORT \
  XV(SD_MMC, "SD_MMC.begin", SD_MMC_BEGIN, PUSH SD_MMC.begin()) \
  XV(SD_MMC, "SD_MMC.beginFull", SD_MMC_BEGIN_FULL, tos = SD_MMC.begin(c2, n1, n0); NIPn(2)) \
  XV(SD_MMC, "SD_MMC.beginDefaults", SD_MMC_BEGIN_DEFAULTS, \
      PUSH "/sdcard"; PUSH false; PUSH false) \
  XV(SD_MMC, "SD_MMC.end", SD_MMC_END, SD_MMC.end()) \
  XV(SD_MMC, "SD_MMC.cardType", SD_MMC_CARD_TYPE, PUSH SD_MMC.cardType()) \
  XV(SD_MMC, "SD_MMC.totalBytes", SD_MMC_TOTAL_BYTES, PUSH SD_MMC.totalBytes()) \
  XV(SD_MMC, "SD_MMC.usedBytes", SD_MMC_USED_BYTES, PUSH SD_MMC.usedBytes()) \
  XV(SD_MMC, "SD_MMC.setPins", SD_MMC_SET_PINS, n0 = SD_MMC.setPins(n5, n4, n3, n2, n1, n0); NIPn(5))
#endif

#ifndef ENABLE_I2C_SUPPORT
# define OPTIONAL_I2C_SUPPORT
#else
# ifndef SIM_PRINT_ONLY
#  include <Wire.h>
# endif
# define OPTIONAL_I2C_SUPPORT \
  XV(Wire, "Wire.begin", WIRE_BEGIN, n0 = Wire.begin(n1, n0); NIP) \
  XV(Wire, "Wire.setClock", WIRE_SET_CLOCK, Wire.setClock(n0); DROP) \
  XV(Wire, "Wire.getClock", WIRE_GET_CLOCK, PUSH Wire.getClock()) \
  XV(Wire, "Wire.setTimeout", WIRE_SET_TIMEOUT, Wire.setTimeout(n0); DROP) \
  XV(Wire, "Wire.getTimeout", WIRE_GET_TIMEOUT, PUSH Wire.getTimeout()) \
  XV(Wire, "Wire.beginTransmission", WIRE_BEGIN_TRANSMISSION, Wire.beginTransmission(n0); DROP) \
  XV(Wire, "Wire.endTransmission", WIRE_END_TRANSMISSION, SET Wire.endTransmission(n0)) \
  XV(Wire, "Wire.requestFrom", WIRE_REQUEST_FROM, n0 = Wire.requestFrom(n2, n1, n0); NIPn(2)) \
  XV(Wire, "Wire.write", WIRE_WRITE, n0 = Wire.write(b1, n0); NIP) \
  XV(Wire, "Wire.available", WIRE_AVAILABLE, PUSH Wire.available()) \
  XV(Wire, "Wire.read", WIRE_READ, PUSH Wire.read()) \
  XV(Wire, "Wire.peek", WIRE_PEEK, PUSH Wire.peek()) \
  XV(Wire, "Wire.flush", WIRE_FLUSH, Wire.flush())
#endif

#ifndef ENABLE_WIFI_SUPPORT
# define OPTIONAL_WIFI_SUPPORT
#else
# ifndef SIM_PRINT_ONLY
#  include <WiFi.h>
#  include <WiFiClient.h>

static IPAddress ToIP(cell_t ip) {
  return IPAddress(ip & 0xff, ((ip >> 8) & 0xff), ((ip >> 16) & 0xff), ((ip >> 24) & 0xff));
}

static cell_t FromIP(IPAddress ip) {
  cell_t ret = 0;
  ret = (ret << 8) | ip[3];
  ret = (ret << 8) | ip[2];
  ret = (ret << 8) | ip[1];
  ret = (ret << 8) | ip[0];
  return ret;
}
# endif

# define OPTIONAL_WIFI_SUPPORT \
  /* WiFi */ \
  XV(WiFi, "WiFi.config", WIFI_CONFIG, \
      WiFi.config(ToIP(n3), ToIP(n2), ToIP(n1), ToIP(n0)); DROPn(4)) \
  XV(WiFi, "WiFi.begin", WIFI_BEGIN, WiFi.begin(c1, c0); DROPn(2)) \
  XV(WiFi, "WiFi.disconnect", WIFI_DISCONNECT, WiFi.disconnect()) \
  XV(WiFi, "WiFi.status", WIFI_STATUS, PUSH WiFi.status()) \
  XV(WiFi, "WiFi.macAddress", WIFI_MAC_ADDRESS, WiFi.macAddress(b0); DROP) \
  XV(WiFi, "WiFi.localIP", WIFI_LOCAL_IPS, PUSH FromIP(WiFi.localIP())) \
  XV(WiFi, "WiFi.mode", WIFI_MODE, WiFi.mode((wifi_mode_t) n0); DROP) \
  XV(WiFi, "WiFi.setTxPower", WIFI_SET_TX_POWER, WiFi.setTxPower((wifi_power_t) n0); DROP) \
  XV(WiFi, "WiFi.getTxPower", WIFI_GET_TX_POWER, PUSH WiFi.getTxPower()) \
  XV(WiFi, "WiFi.softAP", WIFI_SOFTAP, n0 = WiFi.softAP(c1, c0); NIP) \
  XV(WiFi, "WiFi.softAPIP", WIFI_SOFTAP_IP, PUSH FromIP(WiFi.softAPIP())) \
  XV(WiFi, "WiFi.softAPBroadcastIP", WIFI_SOFTAP_BROADCASTIP, PUSH FromIP(WiFi.softAPBroadcastIP())) \
  XV(WiFi, "WiFi.softAPNetworkID", WIFI_SOFTAP_NETWORKID, PUSH FromIP(WiFi.softAPNetworkID())) \
  XV(WiFi, "WiFi.softAPConfig", WIFI_SOFTAP_CONFIG, n0 = WiFi.softAPConfig(ToIP(n2), ToIP(n1), ToIP(n0))) \
  XV(WiFi, "WiFi.softAPdisconnect", WIFI_SOFTAP_DISCONNECT, n0 = WiFi.softAPdisconnect(n0)) \
  XV(WiFi, "WiFi.softAPgetStationNum", WIFI_SOFTAP_GET_STATION_NUM, PUSH WiFi.softAPgetStationNum())
#endif

#ifndef ENABLE_MDNS_SUPPORT
# define OPTIONAL_MDNS_SUPPORT
#else
# ifndef SIM_PRINT_ONLY
#  include <ESPmDNS.h>
# endif
# define OPTIONAL_MDNS_SUPPORT \
  /* mDNS */ \
  X("MDNS.begin", MDNS_BEGIN, n0 = MDNS.begin(c0))
#endif
static char filename[PATH_MAX];
static char filename2[PATH_MAX];

#define PRINT_ERRORS 0

#define CELL_MASK (sizeof(cell_t) - 1)
#define CELL_LEN(n) (((n) + CELL_MASK) / sizeof(cell_t))
#define FIND(name) find((name), sizeof(name) - 1)
#define UPPER(ch) (((ch) >= 'a' && (ch) <= 'z') ? ((ch) & 0x5F) : (ch))
#define CELL_ALIGNED(a) ((((cell_t) (a)) + CELL_MASK) & ~CELL_MASK)
#define IMMEDIATE 1
#define SMUDGE 2
#define BUILTIN_FORK 4
#define BUILTIN_MARK 8

// Maximum ALSO layers.
#define VOCABULARY_DEPTH 16

#if PRINT_ERRORS
#include <stdio.h>
#endif

enum {
#define V(name) VOC_ ## name,
  VOCABULARY_LIST
#undef V
};

enum {
#define V(name) VOC_ ## name ## _immediate = VOC_ ## name + (IMMEDIATE << 8),
  VOCABULARY_LIST
#undef V
};

static cell_t convert(const char *pos, cell_t n, cell_t base, cell_t *ret) {
  *ret = 0;
  cell_t negate = 0;
  if (!n) { return 0; }
  if (*pos == '-') { negate = -1; ++pos; --n; }
  if (*pos == '$') { base = 16; ++pos; --n; }
  if (*pos == '%') { base =  2; ++pos; --n; }
  for (; n; --n) {
    uintptr_t d = UPPER(*pos) - '0';
    if (d > 9) {
      d -= 7;
      if (d < 10) { return 0; }
    }
    if (d >= (uintptr_t) base) { return 0; }
    *ret = *ret * base + d;
    ++pos;
  }
  if (negate) { *ret = -*ret; }
  return -1;
}

static cell_t fconvert(const char *pos, cell_t n, float *ret) {
  *ret = 0;
  cell_t negate = 0;
  cell_t has_dot = 0;
  cell_t exp = 0;
  float shift = 1.0;
  if (!n) { return 0; }
  if (*pos == '-') { negate = -1; ++pos; --n; }
  for (; n; --n) {
    if (*pos >= '0' && *pos <= '9') {
      if (has_dot) {
        shift = shift * 0.1f;
        *ret = *ret + (*pos - '0') * shift;
      } else {
        *ret = *ret * 10 + (*pos - '0');
      }
    } else if (*pos == 'e' || *pos == 'E') {
      break;
    } else if (*pos == '.') {
      if (has_dot) { return 0; }
      has_dot = -1;
    } else {
      return 0;
    }
    ++pos;
  }
  if (!n) { return 0; }  // must have E
  ++pos; --n;
  if (n) {
    if (!convert(pos, n, 10, &exp)) { return 0; }
  }
  if (exp < -128 || exp > 128) { return 0; }
  for (; exp < 0; ++exp) { *ret *= 0.1f; }
  for (; exp > 0; --exp) { *ret *= 10.0f; }
  if (negate) { *ret = -*ret; }
  return -1;
}

static cell_t same(const char *a, const char *b, cell_t len) {
  for (;len && UPPER(*a) == UPPER(*b); --len, ++a, ++b);
  return len == 0;
}

static cell_t find(const char *name, cell_t len) {
  if (len == 0) {
    return 0;
  }
  for (cell_t ***voc = g_sys->context; *voc; ++voc) {
    cell_t xt = (cell_t) **voc;
    while (xt) {
      if ((*TOFLAGS(xt) & BUILTIN_FORK)) {
        cell_t vocab = TOLINK(xt)[3];
        for (int i = 0; g_sys->builtins[i].name; ++i) {
          if (g_sys->builtins[i].vocabulary == vocab &&
              len == g_sys->builtins[i].name_length &&
              same(name, g_sys->builtins[i].name, len)) {
            return (cell_t) &g_sys->builtins[i].code;
          }
        }
      }
      if (!(*TOFLAGS(xt) & SMUDGE) &&
          len == *TONAMELEN(xt) &&
          same(name, TONAME(xt), len)) {
        return xt;
      }
      xt = *TOLINK(xt);
    }
  }
  return 0;
}

static void finish(void) {
  if (g_sys->latestxt && !*TOPARAMS(g_sys->latestxt)) {
    cell_t sz = g_sys->heap - &g_sys->latestxt[1];
    if (sz < 0 || sz > 0xffff) { sz = 0xffff; }
    *TOPARAMS(g_sys->latestxt) = sz;
  }
}

static void create(const char *name, cell_t nlength, cell_t flags, void *op) {
  finish();
  g_sys->heap = (cell_t *) CELL_ALIGNED(g_sys->heap);
  for (cell_t n = nlength; n; --n) { CCOMMA(*name++); }  // name
  g_sys->heap = (cell_t *) CELL_ALIGNED(g_sys->heap);
  COMMA(*g_sys->current);  // link
  COMMA((nlength << 8) | flags);  // flags & length
  *g_sys->current = g_sys->heap;
  g_sys->latestxt = g_sys->heap;
  COMMA(op);  // code
}

static int match(char sep, char ch) {
  return sep == ch || (sep == ' ' && (ch == '\t' || ch == '\n' || ch == '\r'));
}

static cell_t parse(cell_t sep, cell_t *ret) {
  if (sep == ' ') {
    while (g_sys->tin < g_sys->ntib &&
           match(sep, g_sys->tib[g_sys->tin])) { ++g_sys->tin; }
  }
  cell_t start = g_sys->tin;
  while (g_sys->tin < g_sys->ntib &&
         !match(sep, g_sys->tib[g_sys->tin])) { ++g_sys->tin; }
  cell_t len = g_sys->tin - start;
  if (g_sys->tin < g_sys->ntib) { ++g_sys->tin; }
  *ret = (cell_t) (g_sys->tib + start);
  return len;
}

static cell_t *evaluate1(cell_t *rp) {
  cell_t call = 0;
  cell_t tos, *sp, *ip;
  float *fp;
  UNPARK;
  cell_t name;
  cell_t len = parse(' ', &name);
  if (len == 0) { DUP; tos = 0; PARK; return rp; }  // ignore empty
  cell_t xt = find((const char *) name, len);
  if (xt) {
    if (g_sys->state && !(*TOFLAGS(xt) & IMMEDIATE)) {
      COMMA(xt);
    } else {
      call = xt;
    }
  } else {
#if PRINT_ERRORS
    fprintf(stderr, "CANT FIND: ");
    fwrite((void *) name, 1, len, stderr);
    fprintf(stderr, "\n");
#endif
    return 0;
  }
  PUSH call;
  PARK;
  return rp;
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

static cell_t *forth_run(cell_t *initrp);

static void forth_init(int argc, char *argv[],
                       void *heap, cell_t heap_size,
                       const char *src, cell_t src_len) {
  g_sys = (G_SYS *) heap;
  memset(g_sys, 0, sizeof(G_SYS));
  g_sys->heap_start = (cell_t *) heap;
  g_sys->heap_size = heap_size;
  g_sys->stack_cells = STACK_CELLS;

  // Start heap after G_SYS area.
  g_sys->heap = g_sys->heap_start + sizeof(G_SYS) / sizeof(cell_t);
  g_sys->heap += 4;  // Leave a little room.

  // Allocate stacks.
  float *fp = (float *) (g_sys->heap + 1); g_sys->heap += STACK_CELLS;
  cell_t *rp = g_sys->heap + 1; g_sys->heap += STACK_CELLS;
  cell_t *sp = g_sys->heap + 1; g_sys->heap += STACK_CELLS;

  // Exposer le SP dans la structure système pour consultation externe.
  g_sys->sp = sp;

  // FORTH worldlist (relocated when vocabularies added).
  cell_t *forth_wordlist = g_sys->heap;
  COMMA(0);
  // Vocabulary stack.
  g_sys->current = (cell_t **) forth_wordlist;
  g_sys->context = (cell_t ***) g_sys->heap;
  g_sys->latestxt = 0;
  COMMA(forth_wordlist);
  for (int i = 0; i < VOCABULARY_DEPTH; ++i) { COMMA(0); }

  // Setup boot text.
  g_sys->boot = src;
  g_sys->boot_size = src_len;

  forth_run(0);
#define V(name) \
  create(#name "-builtins", sizeof(#name "-builtins") - 1, \
      BUILTIN_FORK, g_sys->DOCREATE_OP); \
  COMMA(VOC_ ## name);
  VOCABULARY_LIST
#undef V
  g_sys->latestxt = 0;  // So last builtin doesn't get wrong size.
  g_sys->DOLIT_XT = FIND("DOLIT");
  g_sys->DOFLIT_XT = FIND("DOFLIT");
  g_sys->DOEXIT_XT = FIND("EXIT");
  g_sys->YIELD_XT = FIND("YIELD");
  g_sys->notfound = FIND("DROP");

  // Init code.
  cell_t *start = g_sys->heap;
  COMMA(FIND("EVALUATE1"));
  COMMA(FIND("BRANCH"));
  COMMA(start);

  g_sys->argc = argc;
  g_sys->argv = argv;
  g_sys->base = 10;
  g_sys->tib = src;
  g_sys->ntib = src_len;

  *++rp = (cell_t) start;
  *++rp = (cell_t) fp;
  *++rp = (cell_t) sp;
  g_sys->rp = rp;
  g_sys->runner = forth_run;
}
#if defined(ENABLE_ESP32_FORTH_FAULT_HANDLING)

# if defined(CONFIG_IDF_TARGET_ESP32C3)

#include <setjmp.h>
#include "riscv/csr.h"
#include "esp_heap_caps.h"

#define FORTH_VECTOR_TABLE_SIZE 32

static __thread jmp_buf g_forth_fault;
static __thread int g_forth_signal;
static void **g_forth_vector_table;
extern void *_vector_table;

#define FAULT_ENTRY \
  if (setjmp(g_forth_fault)) { THROWIT(g_forth_signal); }

static void forth_faults_setup(void) {
  g_forth_vector_table = (void **) malloc(sizeof(void *) * FORTH_VECTOR_TABLE_SIZE);
  //g_forth_vector_table = (void **) heap_caps_malloc(sizeof(void *) * FORTH_VECTOR_TABLE_SIZE,
  //                                                  MALLOC_CAP_EXEC);
  void **vector_table = (void **) &_vector_table;
  for (int i = 0; i < FORTH_VECTOR_TABLE_SIZE; ++i) {
    g_forth_vector_table[i] = vector_table[i];
  }
  // TODO: Actually apply it.
/*
  uint32_t mtvec_val = (uint32_t) g_forth_vector_table;
  mtvec_val |= 1;
  RV_WRITE_CSR(mtvec, mtvec_val);
*/
  //rv_utils_set_mtvec((uint32_t) g_forth_vector_table);
}

# else

#include <setjmp.h>
#include "soc/soc.h"
#include "xtensa_api.h"   // #include <xtensa/xtensa_api.h>

static __thread jmp_buf g_forth_fault;
static __thread int g_forth_signal;
static __thread uint32_t g_forth_setlevel;

#define FAULT_ENTRY \
  if (setjmp(g_forth_fault)) { THROWIT(g_forth_signal); }

static void IRAM_ATTR forth_exception_handler(XtExcFrame *frame) {
  switch (frame->exccause) {
    case EXCCAUSE_LOAD_STORE_ERROR:
    case EXCCAUSE_LOAD_PROHIBITED:
    case EXCCAUSE_STORE_PROHIBITED:
    case EXCCAUSE_LOAD_STORE_DATA_ERROR:
    case EXCCAUSE_LOAD_STORE_RING:
    case EXCCAUSE_LOAD_STORE_ADDR_ERROR:
      g_forth_signal = -9;
      break;
    case EXCCAUSE_DIVIDE_BY_ZERO: g_forth_signal = -10; break;
    case EXCCAUSE_UNALIGNED: g_forth_signal = -23; break;
    default: g_forth_signal = -256 - frame->exccause; break;
  }
  XTOS_RESTORE_INTLEVEL(g_forth_setlevel);
  longjmp(g_forth_fault, 1);
}

static void forth_faults_setup(void) {
  // Install exception handler for everything, as window + alloca handlers
  // don't actually get dispatched.
  for (int i = 0; i < 64; ++i) {
    xt_set_exception_handler(i, forth_exception_handler);
  }
  uint32_t default_setlevel = XTOS_SET_INTLEVEL(XCHAL_EXCM_LEVEL);
  XTOS_RESTORE_INTLEVEL(default_setlevel);
  g_forth_setlevel = default_setlevel;
}

# endif

#else

#define forth_faults_setup()
#define FAULT_ENTRY

#endif
#define SET tos = (cell_t)

#define n0 tos
#define n1 (*sp)
#define n2 sp[-1]
#define n3 sp[-2]
#define n4 sp[-3]
#define n5 sp[-4]
#define n6 sp[-5]
#define n7 sp[-6]
#define n8 sp[-7]
#define n9 sp[-8]
#define n10 sp[-9]
#define n11 sp[-10]
#define n12 sp[-11]
#define n13 sp[-12]
#define n14 sp[-13]
#define n15 sp[-14]

#define a0 ((void *) tos)
#define a1 (*(void **) &n1)
#define a2 (*(void **) &n2)
#define a3 (*(void **) &n3)
#define a4 (*(void **) &n4)
#define a5 (*(void **) &n5)
#define a6 (*(void **) &n6)

#define b0 ((uint8_t *) tos)
#define b1 (*(uint8_t **) &n1)
#define b2 (*(uint8_t **) &n2)
#define b3 (*(uint8_t **) &n3)
#define b4 (*(uint8_t **) &n4)
#define b5 (*(uint8_t **) &n5)
#define b6 (*(uint8_t **) &n6)

#define c0 ((char *) tos)
#define c1 (*(char **) &n1)
#define c2 (*(char **) &n2)
#define c3 (*(char **) &n3)
#define c4 (*(char **) &n4)
#define c5 (*(char **) &n5)
#define c6 (*(char **) &n6)

#define JMPW goto **(void **) w
#define NEXT w = *ip++; JMPW
#define ADDROF(x) (&& OP_ ## x)

static cell_t *forth_run(cell_t *init_rp) {
  static const BUILTIN_WORD builtins[] = {
#define Z(flags, name, op, code) \
    name, ((VOC_ ## flags >> 8) & 0xff) | BUILTIN_MARK, \
    sizeof(name) - 1, (VOC_ ## flags & 0xff), && OP_ ## op,
    PLATFORM_OPCODE_LIST
    TIER2_OPCODE_LIST
    TIER1_OPCODE_LIST
    TIER0_OPCODE_LIST
#undef Z
    0, 0, 0, 0, 0,
  };

  if (!init_rp) {
    g_sys->DOCREATE_OP = ADDROF(DOCREATE);
    g_sys->builtins = builtins;
    forth_faults_setup();
    return 0;
  }
  cell_t *ip, *rp, *sp, tos, w;
  float *fp, ft;
  rp = init_rp; UNPARK; FAULT_ENTRY; NEXT;
#define Z(flags, name, op, code) OP_ ## op: { code; } NEXT;
  PLATFORM_OPCODE_LIST
  TIER2_OPCODE_LIST
  TIER1_OPCODE_LIST
  TIER0_OPCODE_LIST
#undef Z
}
const char boot[] = R"""(
: (   bl nl + 1- parse drop drop ; immediate ( Now can do comments! )
( bl=32 nl=10 so nl+32-1=41, right paren )
: \   nl parse drop drop ; immediate
: #!   nl parse drop drop ; immediate  ( shebang for scripts )
( Stack Baseline )
sp@ constant sp0
rp@ constant rp0
fp@ constant fp0

( Quoting Words )
: ' bl parse 2dup find dup >r -rot r> 0= 'notfound @ execute 2drop ;
: ['] ' aliteral ; immediate
: char bl parse drop c@ ;
: [char] char aliteral ; immediate

( Core Control Flow )
create BEGIN ' nop @ ' begin !        : begin   ['] begin , here ; immediate
create AGAIN ' branch @ ' again !     : again   ['] again , , ; immediate
create UNTIL ' 0branch @ ' until !    : until   ['] until , , ; immediate
create AHEAD ' branch @ ' ahead !     : ahead   ['] ahead , here 0 , ; immediate
create THEN ' nop @ ' then !          : then   ['] then , here swap ! ; immediate
create IF ' 0branch @ ' if !          : if   ['] if , here 0 , ; immediate
create ELSE ' branch @ ' else !       : else   ['] else , here 0 , swap here swap ! ; immediate
create WHILE ' 0branch @ ' while !    : while   ['] while , here 0 , swap ; immediate
create REPEAT ' branch @ ' repeat !   : repeat   ['] repeat , , here swap ! ; immediate
create AFT ' branch @ ' aft !         : aft   drop ['] aft , here 0 , here swap ; immediate

( Recursion )
: recurse   current @ @ aliteral ['] execute , ; immediate

( Tools to build postpone later out of recognizers )
: immediate? ( xt -- f ) >flags 1 and 0= 0= ;
: postpone, ( xt -- ) aliteral ['] , , ;

( Rstack nest depth )
variable nest-depth

( FOR..NEXT )
create FOR ' >r @ ' for !         : for   1 nest-depth +! ['] for , here ; immediate
create NEXT ' donext @ ' next !   : next   -1 nest-depth +! ['] next , , ; immediate

( Define a data type for Recognizers. )
: RECTYPE: ( xt-interpret xt-compile xt-postpone "name" -- ) CREATE , , , ;
: do-notfound ( a n -- ) -1 'notfound @ execute ;
' do-notfound ' do-notfound ' do-notfound  RECTYPE: RECTYPE-NONE
' execute     ' ,           ' postpone,    RECTYPE: RECTYPE-WORD
' execute     ' execute     ' ,            RECTYPE: RECTYPE-IMM
' drop        ' execute     ' execute      RECTYPE: RECTYPE-NUM

: RECOGNIZE ( c-addr len addr1 -- i*x addr2 )
  dup @ for aft
    cell+ 3dup >r >r >r @ execute
    dup RECTYPE-NONE <> if rdrop rdrop rdrop rdrop exit then
    drop r> r> r>
  then next
  drop RECTYPE-NONE
;

( Define a recognizer stack. )
create RECSTACK 0 , bl 2/ ( 16 no numbers yet ) cells allot
: +RECOGNIZER ( xt -- ) 1 RECSTACK +! RECSTACK dup @ cells + ! ;
: -RECOGNIZER ( -- ) -1 RECSTACK +! ;
: GET-RECOGNIZERS ( -- xtn..xt1 n )
   RECSTACK @ for RECSTACK r@ cells + @ next ;
: SET-RECOGNIZERS ( xtn..xt1 n -- )
   0 RECSTACK ! for aft +RECOGNIZER then next ;

( Create recognizer based words. )
: postpone ( "name" -- ) bl parse RECSTACK RECOGNIZE @ execute ; immediate
: +evaluate1 ( -- )
  bl parse dup 0= if 2drop exit then
  RECSTACK RECOGNIZE state @ 1+ 1+ cells + @ execute
;

( Setup recognizing words. )
: REC-FIND ( c-addr len -- xt addr1 | addr2 )
  find dup if
    dup immediate? if RECTYPE-IMM else RECTYPE-WORD then
  else
    drop RECTYPE-NONE
  then
;
' REC-FIND +RECOGNIZER

( Setup recognizing integers. )
: REC-NUM ( c-addr len -- n addr1 | addr2 )
  s>number? if
    ['] aliteral RECTYPE-NUM
  else
    RECTYPE-NONE
  then
;
' REC-NUM +RECOGNIZER

: interpret0 begin +evaluate1 again ; interpret0

( Useful stack/heap words )
: depth ( -- n ) sp@ sp0 - cell/ ;
: fdepth ( -- n ) fp@ fp0 - 4 / ;
: remaining ( -- n ) 'heap-start @ 'heap-size @ + 'heap @ - ;
: used ( -- n ) 'heap @ sp@ 'stack-cells @ cells + - 28 + ;

( DO..LOOP )
variable leaving
: leaving,   here leaving @ , leaving ! ;
: leaving(   leaving @ 0 leaving !   2 nest-depth +! ;
: )leaving   leaving @ swap leaving !  -2 nest-depth +!
             begin dup while dup @ swap here swap ! repeat drop ;
: DO ( n n -- .. ) swap r> -rot >r >r >r ;
: do ( lim s -- ) leaving( postpone DO here ; immediate
: ?DO ( n n -- n n f .. )
   2dup = if 2drop r> @ >r else swap r> cell+ -rot >r >r >r then ;
: ?do ( lim s -- ) leaving( postpone ?DO leaving, here ; immediate
: UNLOOP   r> rdrop rdrop >r ;
: LEAVE   r> rdrop rdrop @ >r ;
: leave   postpone LEAVE leaving, ; immediate
: +LOOP ( n -- ) r> r> dup r@ - >r rot + r> -rot
                       dup r@ - -rot >r >r xor 0<
                 if r> cell+ rdrop rdrop >r else r> @ >r then ;
: +loop ( n -- ) postpone +LOOP , )leaving ; immediate
: LOOP   r> r> dup r@ - >r 1+ r> -rot
               dup r@ - -rot >r >r xor 0<
         if r> cell+ rdrop rdrop >r else r> @ >r then ;
: loop   postpone LOOP , )leaving ; immediate
create I ' r@ @ ' i !  ( i is same as r@ )
: J ( -- n ) rp@ 3 cells - @ ;
: K ( -- n ) rp@ 5 cells - @ ;

( Exceptions )
variable handler
handler 'throw-handler !
: catch ( xt -- n )
  fp@ >r sp@ >r handler @ >r rp@ handler ! execute
  r> handler ! rdrop rdrop 0 ;
: throw ( n -- )
  dup if handler @ rp! r> handler !
         r> swap >r sp! drop r> r> fp! else drop then ;
' throw 'notfound !

( Values )
: value ( n -- ) constant ;
: value-bind ( xt-val xt )
   >r >body state @ if
     r@ ['] ! = if rdrop ['] doset , , else aliteral r> , then
   else r> execute then ;
: to ( n -- ) ' ['] ! value-bind ; immediate
: +to ( n -- ) ' ['] +! value-bind ; immediate

( Deferred Words )
: defer ( "name" -- ) create 0 , does> @ dup 0= throw execute ;
: is ( xt "name -- ) postpone to ; immediate
( Defer I/O to platform specific )
defer type
defer key
defer key?
defer terminate
: bye   0 terminate ;
: emit ( n -- ) >r rp@ 1 type rdrop ;
: space bl emit ;   : cr 13 emit nl emit ;

( Numeric Output )
variable hld
: pad ( -- a ) here 80 + ;
: digit ( u -- c ) 9 over < 7 and + 48 + ;
: extract ( n base -- n c ) u/mod swap digit ;
: <# ( -- ) pad hld ! ;
: hold ( c -- ) hld @ 1 - dup hld ! c! ;
: # ( u -- u ) base @ extract hold ;
: #s ( u -- 0 ) begin # dup while repeat ;
: sign ( n -- ) 0< if 45 hold then ;
: #> ( w -- b u ) drop hld @ pad over - ;
: str ( n -- b u ) dup >r abs <# #s r> sign #> ;
: hex ( -- ) 16 base ! ;   : octal ( -- ) 8 base ! ;
: decimal ( -- ) 10 base ! ;   : binary ( -- ) 2 base ! ;
: u. ( u -- ) <# #s #> type space ;
: . ( w -- ) base @ 10 xor if u. exit then str type space ;
: ? ( a -- ) @ . ;
: n. ( n -- ) base @ swap decimal <# #s #> type base ! ;

( Strings )
: parse-quote ( -- a n ) [char] " parse ;
: $place ( a n -- ) for aft dup c@ c, 1+ then next drop ;
: zplace ( a n -- ) $place 0 c, align ;
: $@   r@ dup cell+ swap @ r> dup @ 1+ aligned + cell+ >r ;
: s"   parse-quote state @ if postpone $@ dup , zplace
       else dup here swap >r >r zplace r> r> then ; immediate
: ."   postpone s" state @ if postpone type else type then ; immediate
: z"   postpone s" state @ if postpone drop else drop then ; immediate
: r"   parse-quote state @ if swap aliteral aliteral then ; immediate
: r|   [char] | parse state @ if swap aliteral aliteral then ; immediate
: r~   [char] ~ parse state @ if swap aliteral aliteral then ; immediate
: s>z ( a n -- z ) here >r zplace r> ;
: z>s ( z -- a n ) 0 over begin dup c@ while 1+ swap 1+ swap repeat drop ;

( Better Errors )
: notfound ( a n n -- )
   dup if cr ." ERROR: " >r type r> ."  NOT FOUND!" cr throw else drop then ;
' notfound 'notfound !

( Abort )
: abort   -1 throw ;
: abort"   postpone ." postpone cr -2 aliteral postpone throw ; immediate

( Input )
: raw.s   depth 0 max for aft sp@ r@ cells - @ . then next ;
variable echo -1 echo !   variable arrow -1 arrow !  0 value wascr
: *emit ( n -- ) dup 13 = if drop cr else emit then ;
: ?echo ( n -- ) echo @ if *emit else drop then ;
: ?arrow.   arrow @ if >r >r raw.s r> r> ." --> " then ;
: *key ( -- n )
  begin
    key
    dup nl = if
      drop wascr if 0 else 13 exit then
    then
    dup 13 = to wascr
    dup if exit else drop then
  again ;
: eat-till-cr   begin *key dup 13 = if ?echo exit else drop then again ;
: accept ( a n -- n ) ?arrow. 0 swap begin 2dup < while
     *key
     dup 13 = if ?echo drop nip exit then
     dup 8 = over 127 = or if
       drop over if rot 1- rot 1- rot 8 ?echo bl ?echo 8 ?echo then
     else
       dup ?echo
       >r rot r> over c! 1+ -rot swap 1+ swap
     then
   repeat drop nip
   eat-till-cr
;
200 constant input-limit
: tib ( -- a ) 'tib @ ;
create input-buffer   input-limit allot
: tib-setup   input-buffer 'tib ! ;
: refill   tib-setup tib input-limit accept #tib ! 0 >in ! -1 ;

( Stack Guards )
sp0 'stack-cells @ 2 3 */ cells + constant sp-limit
: ?stack   sp@ sp0 < if ." STACK UNDERFLOW " -4 throw then
           sp-limit sp@ < if ." STACK OVERFLOW " -3 throw then ;

( REPL )
: prompt   ."  ok" cr ;
: evaluate-buffer   begin >in @ #tib @ < while ?stack +evaluate1 repeat ?stack ;
: evaluate ( a n -- ) 'tib @ >r #tib @ >r >in @ >r
                      #tib ! 'tib ! 0 >in ! evaluate-buffer
                      r> >in ! r> #tib ! r> 'tib ! ;
: evaluate&fill
   begin >in @ #tib @ >= if prompt refill drop then evaluate-buffer again ;
: quit
   #tib @ >in !
   begin ['] evaluate&fill catch  ?dup
        if 
            ."  ERROR " .  cr
            0 state ! sp0 sp! fp0 fp! rp0 rp! 
        then
   again ;
variable boot-prompt
: free. ( nf nu -- ) 2dup swap . ." free + " . ." used = " 2dup + . ." total ("
                     over + 100 -rot */ n. ." % free)" ;
: raw-ok   ."  v7.0.7.21a - rev 9ae74fa18335b0378a98bd8c693b468cc1265ee5" cr
           boot-prompt @ if boot-prompt @ execute then
           ." Forth dictionary: " remaining used free. cr
           ." 3 x Forth stacks: " 'stack-cells @ cells . ." bytes each" cr
           quit ;
( Interpret time conditionals )

: DEFINED? ( "name" -- xt|0 )
   begin bl parse dup 0= while 2drop refill 0= throw repeat
   find state @ if aliteral then ; immediate
defer [SKIP]
: [THEN] ; immediate
: [ELSE] [SKIP] ; immediate
: [IF] 0= if [SKIP] then ; immediate
: [SKIP]' 0 begin postpone defined? dup if
    dup ['] [IF] = if swap 1+ swap then
    dup ['] [ELSE] = if swap dup 0 <= if 2drop exit then swap then
    dup ['] [THEN] = if swap 1- dup 0< if 2drop exit then swap then
  then drop again ;
' [SKIP]' is [SKIP]
( Implement Vocabularies )
( normal: link, flags&len, code )
( vocab:  link, flags&len, code | link , len=0, voclink )
variable last-vocabulary
: vocabulary ( "name" )
  create current @ 2 cells + , 0 , last-vocabulary @ ,
  current @ @ last-vocabulary !
  does> context ! ;
: definitions   context @ current ! ;
vocabulary FORTH
' forth >body @ >link ' forth >body !
forth definitions

( Make it easy to transfer words between vocabularies )
: xt-find& ( xt -- xt& ) context @ begin 2dup @ <> while @ >link& repeat nip ;
: xt-hide ( xt -- ) xt-find& dup @ >link swap ! ;
8 constant BUILTIN_MARK
: xt-transfer ( xt --  ) dup >flags BUILTIN_MARK and if drop exit then
  dup xt-hide   current @ @ over >link& !   current @ ! ;
: transfer ( "name" ) ' xt-transfer ;
: }transfer ;
: transfer{ begin ' dup ['] }transfer = if drop exit then xt-transfer again ;

( Watered down versions of these )
: only   forth 0 context cell+ ! ;
: voc-stack-end ( -- a ) context begin dup @ while cell+ repeat ;
: also   context context cell+ voc-stack-end over - 2 cells + cmove> ;
: previous
  voc-stack-end context cell+ = throw
  context cell+ context voc-stack-end over - cell+ cmove ;
: sealed   0 last-vocabulary @ >body ! ;

( Hide some words in an internals vocabulary )
vocabulary internals   internals definitions

( Vocabulary chain for current scope, place at the -1 position )
variable scope   scope context cell - !

transfer{
  xt-find& xt-hide xt-transfer
  voc-stack-end last-vocabulary notfound
  *key *emit wascr eat-till-cr
  immediate? input-buffer ?echo ?arrow. arrow
  evaluate-buffer evaluate&fill aliteral value-bind
  leaving( )leaving leaving leaving,
  parse-quote digit $@ raw.s
  tib-setup input-limit sp-limit ?stack
  [SKIP] [SKIP]' raw-ok boot-prompt free.
  $place zplace BUILTIN_MARK
  nest-depth handler +evaluate1 do-notfound interpret0
}transfer

( Move branching opcodes to separate vocabulary. )
vocabulary internalized  internalized definitions
: cleave   ' >link xt-transfer ;
cleave begin   cleave again   cleave until
cleave ahead   cleave then    cleave if
cleave else    cleave while   cleave repeat
cleave aft     cleave for     cleave next
cleave do      cleave ?do     cleave +loop
cleave loop    cleave leave
forth definitions

( Move recognizers to separate vocabulary )
vocabulary recognizers  recognizers definitions
transfer{
  REC-FIND REC-NUM
  RECTYPE: RECTYPE-NONE RECTYPE-WORD RECTYPE-IMM RECTYPE-NUM
  SET-RECOGNIZERS GET-RECOGNIZERS
  -RECOGNIZER +RECOGNIZER RECSTACK
  RECOGNIZE
}transfer
forth definitions

( Make DOES> switch to compile mode when interpreted )
(
forth definitions internals
' does>
: does>   state @ if postpone does> exit then
          ['] constant @ current @ @ dup >r !
          here r> cell+ ! postpone ] ; immediate
xt-hide
forth definitions
)
: sf, ( r -- ) here sf! sfloat allot ;

: afliteral ( r -- ) ['] DOFLIT , sf, align ;
: fliteral   afliteral ; immediate

: fconstant ( r "name" ) create sf, align does> sf@ ;
: fvariable ( "name" ) create sfloat allot align ;

6 value precision
: set-precision ( n -- ) to precision ;

( Add recognizer for floats. )
also recognizers definitions
: REC-FNUM ( c-addr len -- f addr1 | addr2 )
  s>float? if
    ['] afliteral RECTYPE-NUM
  else
    RECTYPE-NONE
  then
;
' REC-FNUM +RECOGNIZER
previous definitions

internals definitions
: #f+s ( r -- ) fdup precision for aft 10e f* then next
                precision for aft fdup f>s 10 mod [char] 0 + hold 0.1e f* then next
                [char] . hold fdrop f>s #s ;
forth definitions internals

: #fs ( r -- ) fdup f0< if fnegate #f+s [char] - hold else #f+s then ;
: f. ( r -- ) <# #fs #> type space ;
: f.s   ." <" fdepth n. ." > "
        fdepth 0 max for aft fp@ r@ sfloats - sf@ f. then next ;

forth definitions
( Vocabulary for building C-style structures )

vocabulary structures   structures definitions

variable last-align
variable last-typer
: typer ( xt@ xt! sz "name" -- )
   create dup , 1 max cell min , , ,
   does> dup last-typer !  dup cell+ @ last-align !  @ ;
: sc@ ( a -- c ) c@ dup 127 > if 256 - then ;
' sc@ ' c! 1 typer i8
' c@  ' c! 1 typer u8
' sw@ ' w! 2 typer i16
' uw@ ' w! 2 typer u16
' sl@ ' l! 4 typer i32
' ul@ ' l! 4 typer u32
' @   ' !  8 typer i64  ( Wrong on 32-bit! )
' @   ' !  cell typer ptr
long-size cell = [IF]
  : long   ptr ;
[ELSE]
  : long   i32 ;
[THEN]

variable last-struct
: struct ( "name" ) 0 0 0 typer latestxt >body last-struct !
                    1 last-align ! ;
: align-by ( a n -- a ) 1- dup >r + r> invert and ;
: max! ( n a -- ) swap over @ max swap ! ;
: struct-align ( n -- )
  dup last-struct @ cell+ max!
  last-struct @ @ swap align-by last-struct @ ! ;
: field ( n "name" )
  last-align @ struct-align
  create last-struct @ @ ,   last-struct @ +!  last-typer @ ,
  does> @ + ;

: field-op ( n "name" -- )
  >r ' dup >body cell+ @ r> cells + @
  state @ if >r , r> , else >r execute r> execute then ;
: !field ( n "name" -- ) 2 field-op ; immediate
: @field ( "name" -- n ) 3 field-op ; immediate

forth definitions
( Words with OS assist )
: allocate ( n -- a ior ) malloc dup 0= ;
: free ( a -- ior ) sysfree 0 ;
: resize ( a n -- a ior ) realloc dup 0= ;
( Migrate various words to separate vocabularies, and constants )

forth definitions internals
: read-dir ( dh -- a n ) readdir dup if z>s else 0 then ;
forth definitions

vocabulary ESP   ESP definitions
transfer ESP-builtins
only forth definitions

vocabulary Wire   Wire definitions
transfer wire-builtins
forth definitions

vocabulary WiFi   WiFi definitions
transfer WiFi-builtins
( WiFi Modes )
0 constant WIFI_MODE_NULL
1 constant WIFI_MODE_STA
2 constant WIFI_MODE_AP
3 constant WIFI_MODE_APSTA
forth definitions

vocabulary SD   SD definitions
transfer SD-builtins
forth definitions

vocabulary SD_MMC   SD_MMC definitions
transfer SD_MMC-builtins
forth definitions

vocabulary SPIFFS   SPIFFS definitions
transfer SPIFFS-builtins
forth definitions

vocabulary ledc  ledc definitions
transfer ledc-builtins
forth definitions

vocabulary Serial   Serial definitions
transfer Serial-builtins
forth definitions

vocabulary sockets   sockets definitions
transfer sockets-builtins
1 constant SOCK_STREAM
2 constant SOCK_DGRAM
3 constant SOCK_RAW

2 constant AF_INET
16 constant sizeof(sockaddr_in)
1 constant SOL_SOCKET
2 constant SO_REUSEADDR

: bs, ( n -- ) dup 8 rshift c, c, ;
: s, ( n -- ) dup c, 8 rshift c, ;
: l, ( n -- ) dup s, 16 rshift s, ;
: sockaddr   create 16 c, AF_INET c, 0 bs, 0 l, 0 l, 0 l, ;
: ->port@ ( a -- n ) 2 + >r r@ c@ 8 lshift r> 1+ c@ + ;
: ->port! ( n a --  ) 2 + >r dup 8 rshift r@ c! r> 1+ c! ;
: ->addr@ ( a -- n ) 4 + ul@ ;
: ->addr! ( n a --  ) 4 + l! ;
: ->h_addr ( hostent -- n ) 2 cells + 8 + @ @ ul@ ;
: ip# ( n -- n ) dup 255 and n. [char] . emit 8 rshift ;
: ip. ( n -- ) ip# ip# ip# 255 and n. ;
forth definitions

vocabulary rtos   rtos definitions
transfer rtos-builtins
forth definitions

internals definitions
( Heap Capabilities )
1 0 lshift constant MALLOC_CAP_EXEC
1 1 lshift constant MALLOC_CAP_32BIT
1 2 lshift constant MALLOC_CAP_8BIT
1 3 lshift constant MALLOC_CAP_DMA
1 10 lshift constant MALLOC_CAP_SPIRAM
1 11 lshift constant MALLOC_CAP_INTERNAL
1 12 lshift constant MALLOC_CAP_DEFAULT
1 13 lshift constant MALLOC_CAP_IRAM_8BIT
1 14 lshift constant MALLOC_CAP_RETENTION
1 15 lshift constant MALLOC_CAP_RTCRAM
forth definitions
( Words built after boot )

( For tests and asserts )
: assert ( f -- ) 0= throw ;

( Print spaces )
: spaces ( n -- ) 0 max for aft space then next ;

internals definitions

( Temporary for platforms without CALLCODE )
DEFINED? CALLCODE 0= [IF]
  create CALLCODE
[THEN]

( Safe memory access, i.e. aligned )
cell 1- constant cell-mask
: cell-base ( a -- a ) cell-mask invert and ;
: cell-shift ( a -- a ) cell-mask and 8 * ;
: ca@ ( a -- n ) dup cell-base @ swap cell-shift rshift 255 and ;

( Print address line leaving room )
: dump-line ( a -- a ) cr <# #s #> 20 over - >r type r> spaces ;

( Semi-dangerous word to trim down the system heap )
DEFINED? realloc [IF]
: relinquish ( n -- ) negate 'heap-size +! 'heap-start @ 'heap-size @ realloc drop ;
[THEN]

forth definitions internals

( Examine Memory )
\ moved at end

( Remove from Dictionary )
: forget ( "name" ) ' dup >link current @ !  >name drop here - allot ;

internals definitions
1 constant IMMEDIATE_MARK
2 constant SMUDGE
4 constant BUILTIN_FORK
16 constant NONAMED
32 constant +TAB
64 constant -TAB
128 constant ARGS_MARK
: mem= ( a a n -- f)
   for aft 2dup c@ swap c@ <> if 2drop rdrop 0 exit then 1+ swap 1+ then next 2drop -1 ;
forth definitions also internals
: :noname ( -- xt ) 0 , current @ @ , NONAMED SMUDGE or ,
                    here dup current @ ! ['] mem= @ , postpone ] ;
: str= ( a n a n -- f) >r swap r@ <> if rdrop 2drop 0 exit then r> mem= ;
: startswith? ( a n a n -- f ) >r swap r@ < if rdrop 2drop 0 exit then r> mem= ;
: .s   ." <" depth n. ." > " raw.s cr ;
only forth definitions

( Tweak indent on branches )
internals internalized definitions

: flags'or! ( n -- ) ' >flags& dup >r c@ or r> c! ;
+TAB flags'or! BEGIN
-TAB flags'or! AGAIN
-TAB flags'or! UNTIL
+TAB flags'or! AHEAD
-TAB flags'or! THEN
+TAB flags'or! IF
+TAB -TAB or flags'or! ELSE
+TAB -TAB or flags'or! WHILE
-TAB flags'or! REPEAT
+TAB flags'or! AFT
+TAB flags'or! FOR
-TAB flags'or! NEXT
+TAB flags'or! DO
ARGS_MARK +TAB or flags'or! ?DO
ARGS_MARK -TAB or flags'or! +LOOP
ARGS_MARK -TAB or flags'or! LOOP
ARGS_MARK flags'or! LEAVE

forth definitions 

( Definitions building to SEE and ORDER )
internals definitions
variable indent
: see. ( xt -- ) >name type space ;
: icr   cr indent @ 0 max 4* spaces ;
: indent+! ( n -- ) indent +! icr ;
: see-one ( xt -- xt+1 )
   dup cell+ swap @
   dup ['] DOLIT = if drop dup @ . cell+ exit then
   dup ['] DOSET = if drop ." TO " dup @ cell - see. cell+ icr exit then
   dup ['] DOFLIT = if drop dup sf@ <# [char] e hold #fs #> type space cell+ exit then
   dup ['] $@ = if drop ['] s" see.
                   dup @ dup >r >r dup cell+ r> type cell+ r> 1+ aligned +
                   [char] " emit space exit then
   dup ['] DOES> = if icr then
   dup >flags -TAB AND if -1 indent+! then
   dup see.
   dup >flags +TAB AND if
     1 indent+!
   else
     dup >flags -TAB AND if icr then
   then
   dup ['] ! = if icr then
   dup ['] +! = if icr then
   dup  @ ['] BRANCH @ =
   over @ ['] 0BRANCH @ = or
   over @ ['] DONEXT @ = or
   over >flags ARGS_MARK and or
       if swap cell+ swap then
   drop
;
: see-loop   dup >body swap >params 1- cells over +
             begin 2dup < while swap see-one swap repeat 2drop ;
: ?see-flags   >flags IMMEDIATE_MARK and if ." IMMEDIATE " then ;
: see-xt ( xt -- )
  dup @ ['] see-loop @ = if
    ['] : see.  dup see.
    1 indent ! icr
    dup see-loop
    -1 indent+! ['] ; see.
    ?see-flags cr
    exit
  then
  dup >flags BUILTIN_FORK and if ." Built-in-fork: " see. exit then
  dup @ ['] input-buffer @ = if ." CREATE/VARIABLE: " see. cr exit then
  dup @ ['] SMUDGE @ = if ." DOES>/CONSTANT: " see. cr exit then
  dup @ ['] callcode @ = if ." Code: " see. cr exit then
  dup >params 0= if ." Built-in: " see. cr exit then
  ." Unsupported: " see. cr ;

: nonvoc? ( xt -- f )
  dup 0= if exit then dup >name nip swap >flags NONAMED BUILTIN_FORK or and or ;
: see-vocabulary ( voc )
  @ begin dup nonvoc? while dup see-xt >link repeat drop cr ;
: >vocnext ( xt -- xt ) >body 2 cells + @ ;
: see-all
  last-vocabulary @ begin dup while
    ." VOCABULARY " dup see. cr ." ------------------------" cr
    dup >body see-vocabulary
    >vocnext
  repeat drop cr ;
: voclist-from ( voc -- ) begin dup while dup see. cr >vocnext repeat drop ;
: voclist   last-vocabulary @ voclist-from ;
: voc. ( voc -- ) 2 cells - see. ;
: vocs. ( voc -- ) dup voc. @ begin dup while
    dup nonvoc? 0= if ." >> " dup 2 cells - voc. then
    >link
  repeat drop cr ;

( Words to measure size of things )
: size-vocabulary ( voc )
  @ begin dup nonvoc? while
    dup >params . dup >size . dup . dup see. cr >link
  repeat drop ;
: size-all
  last-vocabulary @ begin dup while
    0 . 0 . 0 . dup see. cr
    dup >body size-vocabulary
    >vocnext
  repeat drop cr ;

forth definitions also internals
: see   ' see-xt ;
: order   context begin dup @ while dup @ vocs. cell+ repeat drop ;
only forth definitions

( List words in Dictionary / Vocabulary )
internals definitions
70 value line-width
0 value line-pos
: onlines ( xt -- xt )
   line-pos line-width > if cr 0 to line-pos then
   dup >name nip 1+ line-pos + to line-pos ;
: vins. ( voc -- )
  >r 'builtins begin dup >link while
    dup >params r@ = if dup onlines see. then
    3 cells +
  repeat drop rdrop ;
: ins. ( n xt -- n ) cell+ @ vins. ;
: ?ins. ( xt -- xt ) dup >flags BUILTIN_FORK and if dup ins. then ;
forth definitions also internals
: vlist   0 to line-pos context @ @
          begin dup nonvoc? while ?ins. dup onlines see. >link repeat drop cr ;
: words   0 to line-pos context @ @
          begin dup while ?ins. dup onlines see. >link repeat drop cr ;
only forth definitions
( Lazy loaded code words )
: asm r|

also forth definitions
vocabulary asm
internals definitions

: ca! ( n a -- ) dup cell-base >r cell-shift swap over lshift
                 swap 255 swap lshift invert r@ @ and or r> ! ;

also asm definitions

variable code-start
variable code-at

DEFINED? posix [IF]
also posix
: reserve ( n -- )
  0 swap PROT_READ PROT_WRITE PROT_EXEC or or
  MAP_ANONYMOUS MAP_PRIVATE or -1 0 mmap code-start ! ;
previous
4096 reserve
[THEN]

DEFINED? esp [IF]
also esp
: reserve ( n -- ) MALLOC_CAP_EXEC heap_caps_malloc code-start ! ;
previous
1024 reserve
[THEN]

code-start @ code-at !

: chere ( -- a ) code-at @ ;
: callot ( n -- ) code-at +! ;
: code1, ( n -- ) chere ca! 1 callot ;
: code2, ( n -- ) dup code1, 8 rshift code1, ;
: code3, ( n -- ) dup code2, 16 rshift code1, ;
: code4, ( n -- ) dup code2, 16 rshift code2, ;
cell 8 = [IF]
: code,  dup code4, 32 rshift code4, ;
[ELSE]
: code,  code4, ;
[THEN]
: end-code   previous ;

also forth definitions

: code ( "name" ) create ['] callcode @ latestxt !
                  code-at @ latestxt cell+ ! also asm ;

previous previous previous
asm

| evaluate ;
( Local Variables )

( NOTE: These are not yet gforth compatible )

internals definitions

( Leave a region for locals definitions )
1024 constant locals-capacity  128 constant locals-gap
create locals-area locals-capacity allot
variable locals-here  locals-area locals-here !
: <>locals   locals-here @ here locals-here ! here - allot ;

: local@ ( n -- ) rp@ + @ ;
: local! ( n -- ) rp@ + ! ;
: local+! ( n -- ) rp@ + +! ;

variable scope-depth
variable local-op   ' local@ local-op !
: scope-exit   scope-depth @ for aft postpone rdrop then next ;
: scope-clear
   scope-exit
   scope-depth @ negate nest-depth +!
   0 scope-depth !   0 scope !   locals-area locals-here ! ;
: do-local ( n -- ) nest-depth @ + cells negate aliteral
                    local-op @ ,  ['] local@ local-op ! ;
: scope-create ( a n -- )
   dup >r $place align ( name )
   scope @ , r> 8 lshift 1 or , ( IMMEDIATE ) here scope ! ( link, flags&length )
   ['] scope-clear @ ( docol) ,
   nest-depth @ negate aliteral postpone do-local ['] exit ,
   1 scope-depth +!  1 nest-depth +!
;

: ?room   locals-here @ locals-area - locals-capacity locals-gap - >
          if scope-clear -1 throw then ;

: }? ( a n -- ) 1 <> if drop 0 exit then c@ [char] } = ;
: --? ( a n -- ) s" --" str= ;
: (to) ( xt -- ) ['] local! local-op ! execute ;
: (+to) ( xt -- ) ['] local+! local-op ! execute ;

also forth definitions

: (local) ( a n -- )
   dup 0= if 2drop exit then 
   ?room <>locals scope-create <>locals postpone >r ;
: {   bl parse
      dup 0= if scope-clear -1 throw then
      2dup --? if 2drop [char] } parse 2drop exit then
      2dup }? if 2drop exit then
      recurse (local) ; immediate
( TODO: Hide the words overriden here. )
: ;   scope-clear postpone ; ; immediate
: exit   scope-exit postpone exit ; immediate
: to ( n -- ) ' dup >flags if (to) else ['] ! value-bind then ; immediate
: +to ( n -- ) ' dup >flags if (+to) else ['] +! value-bind then ; immediate

only forth definitions
internals definitions
variable cases
forth definitions internals

: CASE ( n -- ) cases @  0 cases ! ; immediate
: ENDCASE   postpone drop cases @ for aft postpone then then next
            cases ! ; immediate
: OF ( n -- ) postpone over postpone = postpone if postpone drop ; immediate
: ENDOF   1 cases +! postpone else ; immediate

forth definitions
( Cooperative Tasks )

vocabulary tasks   tasks definitions also internals

variable task-list

: .tasks   task-list @ begin dup 2 cells - see. @ dup task-list @ = until drop ;

forth definitions tasks also internals

: pause
  rp@ sp@ task-list @ cell+ !
  task-list @ @ task-list !
  task-list @ cell+ @ sp! rp!
;

( Check if there are other tasks. )
: pause? ( -- f ) task-list @ dup @ <> ;

: task ( xt dsz rsz "name" )
   create here >r 0 , 0 , ( link, sp )
   swap here cell+ r@ cell+ ! cells allot
   here r@ cell+ @ ! cells allot
   dup 0= if drop else
     here r@ cell+ @ @ ! ( set rp to point here )
     , postpone pause ['] branch , here 3 cells - ,
   then rdrop ;

: start-task ( t -- )
   task-list @ if
     task-list @ @ over !
     task-list @ !
   else
     dup task-list !
     dup !
   then
;

DEFINED? ms-ticks [IF]
  : ms ( n -- ) ms-ticks >r begin pause ms-ticks r@ - over >= until rdrop drop ;
[THEN]

tasks definitions
0 0 0 task main-task   main-task start-task
only forth definitions
( Byte Stream / Ring Buffer )

vocabulary streams   streams definitions

: stream ( n "name" ) create 1+ dup , 0 , 0 , allot align ;
: >write ( st -- wr ) cell+ ;   : >read ( st -- rd ) 2 cells + ;
: >offset ( n st -- a ) 3 cells + + ;
: stream# ( sz -- n ) >r r@ >write @ r@ >read @ - r> @ mod ;
: full? ( st -- f ) dup stream# swap @ 1- = ;
: empty? ( st -- f ) stream# 0= ;
: wait-write ( st -- ) begin dup full? while pause repeat drop ;
: wait-read ( st -- ) begin dup empty? while pause repeat drop ;
: ch>stream ( ch st -- )
   dup wait-write
   >r r@ >write @ r@ >offset c!
   r@ >write @ 1+ r@ @ mod r> >write ! ;
: stream>ch ( st -- ch )
   dup wait-read
   >r r@ >read @ r@ >offset c@
   r@ >read @ 1+ r@ @ mod r> >read ! ;
: >stream ( a n st -- )
   swap for aft over c@ over ch>stream swap 1+ swap then next 2drop ;
: stream> ( a n st -- )
   begin over 1 > over empty? 0= and while
   dup stream>ch >r rot dup r> swap c! 1+ rot 1- rot repeat 2drop 0 swap c! ;

forth definitions
: dump-file ( a n a n -- )
  w/o create-file throw
  >r r@ write-file throw
  r> close-file drop
;

: cp ( "src" "dst" -- )
  bl parse r/o bin open-file throw { inf }
  bl parse w/o bin create-file throw { outf }
  begin
    here 80 inf read-file throw
    dup 0= if drop outf close-file throw inf close-file throw exit then
    here swap outf write-file throw
  again
;

: mv ( "src" "dst" -- ) bl parse bl parse rename-file throw ;
: rm ( "path" -- ) bl parse delete-file throw ;

: touch ( "path" -- )
  bl parse 2dup w/o open-file
  if drop w/o create-file throw then
  close-file throw
;

internals definitions

: cremit ( ch -- ) dup nl = if drop cr else emit then ;
: crtype ( a n -- ) for aft dup c@ cremit 1+ then next drop ;

forth definitions internals

: cat ( "path" -- )
  bl parse r/o bin open-file throw { fh }
  begin
    here 80 fh read-file throw
    dup 0= if drop fh close-file throw exit then
    here swap crtype
  again
;

DEFINED? read-dir [IF]
: ls ( "path" -- )
  bl parse dup 0= if 2drop s" ." then
  open-dir throw { dh } begin
    dh read-dir dup 0= if
      2drop dh close-dir throw exit
    then type cr
  again
;
[THEN]

internals definitions
( Leave some room for growth of starting system. )
0 value saving-base
: park-heap ( -- a ) saving-base ;
: park-forth ( -- a ) saving-base cell+ ;
: 'cold ( -- a ) saving-base 2 cells + ;
: setup-saving-base
  here to saving-base  16 cells allot  0 'cold ! ;

' forth >body constant forth-wordlist

: save-name
  'heap @ park-heap !
  forth-wordlist @ park-forth !
  w/o create-file throw >r
  saving-base here over - r@ write-file throw
  r> close-file throw ;

: restore-name ( "name" -- )
  r/o open-file throw >r
  saving-base r@ file-size throw r@ read-file throw drop
  r> close-file throw
  park-heap @ 'heap !
  park-forth @ forth-wordlist !
  'cold @ dup if execute else drop then ;

defer remember-filename
: default-remember-filename   s" myforth" ;
' default-remember-filename is remember-filename

forth definitions also internals

: save ( "name" -- ) bl parse save-name ;
: restore ( "name" -- ) bl parse restore-name ;
: remember   remember-filename save-name ;
: startup: ( "name" ) ' 'cold ! remember ;
: revive   remember-filename restore-name ;
: reset   remember-filename delete-file throw ;

only forth definitions
( Including Files )

internals definitions

: ends/ ( a n -- f ) 1- + c@ [char] / = ;
: dirname ( a n -- )
   dup if
     2dup ends/ if 1- then
   then
   begin dup while
     2dup ends/ if exit then 1-
   repeat ;

: starts./ ( a n -- f )
   2 < if drop 0 exit then
   2 s" ./" str= ;

: starts../ ( a n -- f )
   3 < if drop 0 exit then
   3 s" ../" str= ;

0 value sourcefilename&
0 value sourcefilename#
: sourcefilename ( -- a n ) sourcefilename& sourcefilename# ;
: sourcefilename! ( a n -- ) to sourcefilename# to sourcefilename& ;
: sourcedirname ( -- a n ) sourcefilename dirname ;

: include-file ( fh -- )
   dup file-size throw
   dup allocate throw
   swap over >r
   rot read-file throw
   r@ swap evaluate
   r> free throw ;

: raw-included ( a n -- )
   r/o open-file throw
   dup >r include-file
   r> close-file throw ;

0 value included-files

: path-join { a a# b b# -- a n }
  b if
    b c@ [char] / = if 0 to a# then
  then
  a# b# + { r# } r# cell+ cell+ allocate throw { r }
  2 cells +to r
  begin b b# starts./ while
    2 +to b -2 +to b#
    a# b# + to r#
  repeat
  begin b b# starts../ a# 0<> and while
    3 +to b -3 +to b#
    a a# dirname to a# to a
    a# b# + to r#
  repeat
  a r a# cmove b r a# + b# cmove
  r# r cell - !
  r r# ;
: include+ 2 cells - { a }
  included-files a ! a to included-files ;

forth definitions internals

: included ( a n -- )
   sourcefilename >r >r
   >r >r sourcedirname r> r> path-join 2dup sourcefilename!
   ['] raw-included catch if
      ." Error including: " sourcefilename type cr
      -38 throw
   then
   sourcefilename& include+
   r> r> sourcefilename! ;

: include ( "name" -- ) bl parse included ;

: included? { a n -- f }
  sourcedirname a n path-join to n to a
  included-files begin dup while
    dup cell+ cell+ over cell+ @ a n str= if
      a 2 cells - free throw drop -1 exit
    then @
  repeat
  a 2 cells - free throw ;

: required ( a n -- ) 2dup included? if 2drop else included then ;
: needs ( "name" -- ) bl parse required ;

: file-exists? ( "name" -- f ) r/o open-file if drop 0 else close-file throw -1 then ;

forth
( Block Files )
internals definitions
\ : clobber-line ( a -- a' ) dup 63 blank 63 + nl over c! 1+ ;
\ : clobber ( a -- ) 15 for clobber-line next drop ;
0 value block-dirty
create block-data 1024 allot
forth definitions internals

-1 value block-fid   variable scr   -1 value block-id
: open-blocks ( a n -- )
   block-fid 0< 0= if block-fid close-file throw -1 to block-fid then
   2dup r/w open-file if drop r/w create-file throw else nip nip then to block-fid ;
: use ( "name" -- ) bl parse open-blocks ;
defer default-use
internals definitions
: common-default-use s" blocks.fb" open-blocks ;
' common-default-use is default-use
: use?!   block-fid 0< if default-use then ;
: grow-blocks ( n -- ) 1024 * block-fid file-size throw max block-fid resize-file throw ;
forth definitions internals
\ : save-buffers
\    block-dirty if
\      block-id grow-blocks block-id 1024 * block-fid reposition-file throw
\      block-data 1024 block-fid write-file throw
\      block-fid flush-file throw
\      0 to block-dirty
\    then ;
\ : block ( n -- a ) use?! dup block-id = if drop block-data exit then
\                    save-buffers dup grow-blocks
\                    dup 1024 * block-fid reposition-file throw
\                    block-data clobber
\                    block-data 1024 block-fid read-file throw drop
\                    to block-id block-data ;
\ : buffer ( n -- a ) use?! dup block-id = if drop block-data exit then
\                     save-buffers to block-id block-data ;
\ : empty-buffers   -1 to block-id ;
\ : update   -1 to block-dirty ;
\ : flush   save-buffers empty-buffers ;

( Loading )
\ : load ( n -- ) block 1024 evaluate ;
\ : thru ( a b -- ) over - 1+ for aft dup >r load r> 1+ then next drop ;

( Utility )
\ : copy ( from to -- )
\    swap block pad 1024 cmove pad swap block 1024 cmove update ;

( Editing )
\ : list ( n -- ) scr ! ." Block " scr @ . cr scr @ block
\    15 for dup 63 type [char] | emit space 15 r@ - . cr 64 + next drop ;
internals definitions
\ : @line ( n -- ) 64 * scr @ block + ;
\ : e' ( n -- ) @line clobber-line drop update ;
forth definitions internals
\ vocabulary editor   also editor definitions
\ : l    scr @ list ;   : n    1 scr +! l ;  : p   -1 scr +! l ;
\ : wipe   15 for r@ e' next l ;   : e   e' l ;
\ : d ( n -- ) dup 1+ @line swap @line 15 @line over - cmove 15 e ;
\ : r ( n "line" -- ) 0 parse 64 min rot dup e @line swap cmove l ;
\ : a ( n "line" -- ) dup @line over 1+ @line 16 @line over - cmove> r ;
only forth definitions
( ANSI Codes )
vocabulary ansi   ansi definitions
: esc   27 emit ;   : bel   7 emit ;
: clear-to-eol   esc ." [0K" ;
: scroll-down   esc ." D" ;
: scroll-up   esc ." M" ;
: hide   esc ." [?25l" ;
: show   esc ." [?25h" ;
: terminal-save   esc ." [?1049h" ;
: terminal-restore   esc ." [?1049l" ;

forth definitions ansi
: fg ( n -- ) esc ." [38;5;" n. ." m" ;
: bg ( n -- ) esc ." [48;5;" n. ." m" ;
: normal   esc ." [0m" ;
: at-xy ( x y -- ) esc ." [" 1+ n. ." ;" 1+ n. ." H" ;
: page   esc ." [2J" esc ." [H" ;
: set-title ( a n -- ) esc ." ]0;" type bel ;
forth
( Lazy loaded visual editor. )

: visual r|

also DEFINED? termios [IF] termios [THEN]
also internals
also ansi
also forth
current @
vocabulary visual  visual definitions
vocabulary insides  insides definitions

256 constant max-path
create filename max-path allot 0 value filename#
0 value fileh

10 constant start-size
start-size allocate throw value text
start-size value capacity
0 value length
0 value caret

: up ( n -- n ) begin dup 0 > over text + c@ nl <> and while 1- repeat 1- 0 max ;
: nup ( n -- n ) 10 for up next ;
: down ( n -- n ) begin dup length < over text + c@ nl <> and while 1+ repeat 1+ length min ;
: ndown ( n -- n ) 10 for down next ;

: update
    caret nup dup 0<> if 1+ 1+ then { before }
    before ndown ndown { after }
    page
    text before + caret before - crtype
    caret length < text caret + c@ nl <> and if
      1 bg text caret + c@ emit normal
      text caret + 1+ after caret - 1- 0 max crtype
    else
      1 bg space normal
      text caret + after caret - crtype
    then normal
;

: insert ( ch -- )
  length capacity = if text capacity 1+ 2* >r r@ 1+ resize throw to text r> to capacity then
  text caret + dup 1+ length caret - cmove>
  text caret + c!
  1 +to caret
  1 +to length
  update
;

: handle-esc
    key
    dup [char] [ = if drop
       key
       dup [char] A = if drop caret up to caret update exit then
       dup [char] B = if drop caret down to caret update exit then
       dup [char] C = if drop caret 1+ length min to caret update exit then
       dup [char] D = if drop caret 1- 0 max to caret update exit then
       dup [char] 5 = if drop key drop caret 8 for up next to caret update exit then
       dup [char] 6 = if drop key drop caret 8 for down next to caret update exit then
       drop
       exit
    then
    drop
;

: delete
    length caret > if
      text caret + dup 1+ swap length caret - 1- 0 max cmove
      -1 +to length
      update
    then
;

: backspace
    caret 0 > if
        -1 +to caret
        delete
    then
;

: load ( a n -- )
     0 to caret
     dup to filename#
     filename swap cmove
     filename filename# r/o open-file 0= if
         to fileh
         fileh file-size throw to capacity
         text capacity 1+ resize throw to text
         capacity to length
         text length fileh read-file throw drop
         fileh close-file throw
     else
         drop
         0 to capacity
         0 to length
     then
;

: save
     filename filename# w/o create-file throw to fileh
     text length fileh write-file throw
     fileh close-file throw
;

: quit-edit
     page filename filename# type cr ." SAVE? "
     begin
         key 95 and
         dup [char] Y = if drop save 123 throw then
         dup [char] N = if drop 123 throw then
         drop
     again
;

: handle-key ( ch -- )
    dup 27 = if drop handle-esc exit then
    dup [char] D [char] @ - = if delete exit then
    dup [char] H [char] @ - = over 127 = or if drop backspace exit then
    dup [char] L [char] @ - = if drop update exit then
    ( Support either Ctrl-S or Ctrl-P )
    dup [char] S [char] @ - =
      over [char] P [char] @ - = or
      if drop save update ." saved" exit then
    dup [char] X [char] @ - = if drop quit-edit then
    dup [char] Q [char] @ - = if drop quit-edit then
    dup 13 = if drop nl insert exit then
    dup bl >= if insert else drop then
;

: ground   depth 0<> throw ;
: step   *key handle-key ground ;

DEFINED? raw-mode 0= [IF]
    : raw-mode ;
    : normal-mode ;
[THEN]

: run
    raw-mode update
    begin
        ['] step catch
        dup 123 = if drop normal-mode page exit then
        if ." FAILURE!" then
    again
;

visual definitions insides

: edit ( <filename> ) bl parse load run ;

previous previous previous previous current ! visual
| evaluate ;
( Add a yielding task so pause yields )
internals definitions
: yield-step   raw-yield yield ;
' yield-step 100 100 task yield-task
yield-task start-task
forth definitions

( Set up Basic I/O )
internals definitions also serial
: serial-type ( a n -- ) Serial.write drop ;
: serial-key ( -- n )
   begin pause Serial.available until 0 >r rp@ 1 Serial.readBytes drop r> ;
: serial-key? ( -- n ) Serial.available ;
also forth definitions
: default-type serial-type ;
: default-key serial-key ;
: default-key? serial-key? ;
' default-type is type
' default-key is key
' default-key? is key?
' raw-terminate is terminate
only forth definitions

also ledc also serial also SPIFFS

( Map Arduino / ESP32 things to shorter names. )
: pin ( n pin# -- ) swap digitalWrite ;
: adc ( n -- n ) analogRead ;

( Basic Ardiuno Constants )
0 constant LOW
1 constant HIGH
1 constant INPUT
2 constant OUTPUT
2 constant LED

( Startup Setup )
-1 echo !
115200 Serial.begin
100 ms
-1 z" /spiffs" 10 SPIFFS.begin drop
led OUTPUT pinMode
high led pin

internals definitions also ESP
: esp32-stats
  getChipModel z>s type ."    "
  getCpuFreqMHz . ." MHz   "
  getChipCores .  ." cores   "
  getFlashChipSize . ." bytes flash" cr
  ."      System Heap: " getFreeHeap getHeapSize free. cr
  ."                   " getMaxAllocHeap . ." bytes max contiguous" cr ;
' esp32-stats internals boot-prompt !
only forth definitions

( Setup entry )
internals : ok   ." ESP32forth" raw-ok ; forth
( Lazy loaded HTTP Daemon )
: httpd r|

vocabulary httpd   httpd definitions
also sockets
also internals

1 constant max-connections
2048 constant chunk-size
create chunk chunk-size allot
0 value chunk-filled
256 constant body-chunk-size
create body-chunk body-chunk-size allot
0 value body-1st-read
0 value body-read

-1 value sockfd   -1 value clientfd
sockaddr httpd-port   sockaddr client   variable client-len

: client-type ( a n -- ) clientfd write-file throw ;
: client-read ( -- n ) 0 >r rp@ 1 clientfd read-file throw 1 <> throw ;
: client-emit ( ch -- ) >r rp@ 1 client-type rdrop ;
: client-cr   13 client-emit nl client-emit ;

: server ( port -- )
  httpd-port ->port!  ." Listening on port " httpd-port ->port@ . cr
  AF_INET SOCK_STREAM 0 socket to sockfd
(  sockfd SOL_SOCKET SO_REUSEADDR 1 >r rp@ 4 setsockopt rdrop throw )
  sockfd non-block throw
  sockfd httpd-port sizeof(sockaddr_in) bind throw
  sockfd max-connections listen throw
;

: upper ( ch -- ch )
  dup [char] a >= over [char] z <= and if 95 and then ;
: strcase= ( a n a n -- f )
  >r swap r@ <> if rdrop 2drop 0 exit then r>
  for aft
    2dup c@ upper swap c@ upper <> if rdrop 2drop 0 exit then
    1+ swap 1+ swap
  then next
  2drop -1
;

variable goal   variable goal#
: end< ( n -- f ) chunk-filled < ;
: in@<> ( n ch -- f ) >r chunk + c@ r> <> ;
: skipto ( n ch -- n )
   >r begin dup r@ in@<> over end< and while 1+ repeat rdrop ;
: skipover ( n ch -- n ) skipto 1+ ;
: eat ( n ch -- n a n ) >r dup r> skipover swap 2dup - 1- >r chunk + r> ;
: crnl= ( n -- f ) dup chunk + c@ 13 = swap 1+ chunk + c@ nl = and ;
: header ( a n -- a n )
  goal# ! goal ! 0 nl skipover
  begin dup end< while
    dup crnl= if drop chunk 0 exit then
    [char] : eat goal @ goal# @ strcase= if 1+ 13 eat rot drop exit then
    nl skipover
  repeat drop chunk 0
;
: content-length ( -- n )
  s" Content-Length" header s>number? 0= if 0 then ;
: body ( -- a n ) ( reads a part of body )
  body-1st-read if
    body-read content-length >= if
      0 0 exit
    then
    body-chunk body-chunk-size clientfd read-file throw dup +to body-read
    body-chunk swap exit
  then
  -1 to body-1st-read
  0 to body-read
  0 nl skipover
  begin dup end< while
    dup crnl= if
      2 + chunk-filled over - swap chunk + swap
      dup +to body-read exit
    then
    nl skipover
  repeat drop chunk 0
;
: completed? ( -- f )
  0 begin dup end< while
    dup crnl= if drop -1 exit then
    nl skipover
  repeat drop 0
;
: read-headers
  0 to body-1st-read
  0 to chunk-filled
  begin completed? 0= while
    chunk chunk-filled + chunk-size chunk-filled -
      clientfd read-file throw +to chunk-filled
  repeat
;

: handleClient
  clientfd close-file drop
  -1 to clientfd
  sockfd client client-len sockaccept
  dup 0< if drop 0 exit then
  to clientfd
  chunk chunk-size erase
  read-headers
  -1
;

: hasHeader ( a n -- f ) 2drop header 0 0 strcase= 0= ;
: method ( -- a n ) 0 bl eat rot drop ;
: path ( -- a n ) 0 bl skipover bl eat rot drop ;
: send ( a n -- ) client-type ;

: response ( mime$ result$ status -- )
  s" HTTP/1.0 " client-type <# #s #> client-type
  bl client-emit client-type client-cr
  s" Content-type: " client-type client-type client-cr
  client-cr ;
: ok-response ( mime$ -- ) s" OK" 200 response ;
: bad-response ( -- ) s" text/plain" s" Bad Request" 400 response ;
: notfound-response ( -- ) s" text/plain" s" Not Found" 404 response ;

only forth definitions
httpd
| evaluate ;
( Lazy loaded Server Terminal )

defer web-interface
:noname r~
httpd
also streams also httpd
vocabulary web-interface   also web-interface definitions

r|
<!html>
<head>
<title>esp32forth</title>
<style>
body {
  padding: 5px;
  background-color: #111;
  color: #2cf;
  overflow: hidden;
}
#prompt {
  width: 100%;
  padding: 5px;
  font-family: monospace;
  background-color: #ff8;
}
#output {
  width: 100%;
  height: 80%;
  resize: none;
  overflow-y: scroll;
  word-break: break-all;
}
</style>
<link rel="icon" href="data:,">
</head>
<body>
<h2>ESP32forth v7</h2>
Upload File: <input id="filepick" type="file" name="files[]"></input><br/>
<button onclick="ask('hex\n')">hex</button>
<button onclick="ask('decimal\n')">decimal</button>
<button onclick="ask('words\n')">words</button>
<button onclick="ask('low led pin\n')">LED OFF</button>
<button onclick="ask('high led pin\n')">LED ON</button>
<br/>
<textarea id="output" readonly></textarea>
<input id="prompt" type="prompt"></input><br/>
<script>
var prompt = document.getElementById('prompt');
var filepick = document.getElementById('filepick');
var output = document.getElementById('output');
function httpPost(url, data, callback) {
  var r = new XMLHttpRequest();
  r.onreadystatechange = function() {
    if (this.readyState == XMLHttpRequest.DONE) {
      if (this.status === 200) {
        callback(this.responseText);
      } else {
        callback(null);
      }
    }
  };
  r.open('POST', url);
  r.send(data);
}
setInterval(function() { ask(''); }, 300);
function ask(cmd, callback) {
  httpPost('/input', cmd, function(data) {
    if (data !== null) { output.value += data; }
    output.scrollTop = output.scrollHeight;  // Scroll to the bottom
    if (callback !== undefined) { callback(); }
  });
}
prompt.onkeyup = function(event) {
  if (event.keyCode === 13) {
    event.preventDefault();
    ask(prompt.value + '\n');
    prompt.value = '';
  }
};
filepick.onchange = function(event) {
  if (event.target.files.length > 0) {
    var reader = new FileReader();
    reader.onload = function(e) {
      var parts = e.target.result.replace(/[\r]/g, '').split('\n');
      function upload() {
        if (parts.length === 0) { filepick.value = ''; return; }
        ask(parts.shift(), upload);
      }
      upload();
    }
    reader.readAsText(event.target.files[0]);
  }
};
window.onload = function() {
  ask('\n');
  prompt.focus();
};
</script>
| constant index-html# constant index-html

variable webserver
2000 constant out-size
200 stream input-stream
out-size stream output-stream
create out-string out-size 1+ allot align

: handle-index
   s" text/html" ok-response
   index-html index-html# send
;

: handle-input
   begin body dup >r input-stream >stream pause r> 0= until
   out-string out-size output-stream stream>
   s" text/plain" ok-response
   out-string z>s send
;

: serve-type ( a n -- ) output-stream >stream ;
: serve-key ( -- n ) input-stream stream>ch ;

: handle1
  handleClient if
    s" /" path str= if handle-index exit then
    s" /input" path str= if handle-input exit then
    notfound-response
  then
;

: do-serve    begin handle1 pause again ;
' do-serve 1000 1000 task webserver-task

: server ( port -- )
   server
   ['] serve-key is key
   ['] serve-type is type
   webserver-task start-task
;

only forth definitions
web-interface
~ evaluate ; is web-interface

( Lazy loaded Server Terminal )

:noname [ ' web-interface >body @ ] literal execute
r|
also streams also sockets also WiFi also web-interface

: login ( z z -- )
   WIFI_MODE_STA Wifi.mode
   WiFi.begin begin WiFi.localIP 0= while 100 ms repeat WiFi.localIP ip. cr
   z" forth" MDNS.begin if ." MDNS started" else ." MDNS failed" then cr ;
: webui ( z z -- ) login 80 server ;

only forth definitions
web-interface
| evaluate ; is web-interface
: login web-interface forth r| login | evaluate ;
: webui web-interface forth r| webui | evaluate ;
vocabulary registers   registers definitions

( Tools for working with bit masks )
: m! ( val shift mask a -- )
   dup >r @ over invert and >r >r lshift r> and r> or r> ! ;
: m@ ( shift mask a -- val ) @ and swap rshift ;

only forth definitions
( Lazy loaded Telnet )
: telnetd r|

vocabulary telnetd   telnetd definitions also sockets

-1 value sockfd   -1 value clientfd
sockaddr telnet-port   sockaddr client   variable client-len

defer broker

: telnet-emit ( ch -- ) >r rp@ 1 clientfd write-file rdrop if broker then ;
: telnet-type ( a n -- ) for aft dup c@ telnet-emit 1+ then next drop ;
: telnet-key ( -- n ) 0 >r rp@ 1 clientfd read-file swap 1 <> or if rdrop broker then r> ;

: connection ( n -- )
  dup 0< if drop exit then to clientfd
  0 echo !
  ['] telnet-key is key
  ['] telnet-type is type quit ;

: wait-for-connection
  begin
    sockfd client client-len sockaccept
    dup 0 >= if exit else drop then
  again
;

: broker-connection
  rp0 rp! sp0 sp!
  begin
    ['] default-key is key   ['] default-type is type
    -1 echo !
    ." Listening on port " telnet-port ->port@ . cr
    wait-for-connection
    ." Connected: " dup . cr connection
  again ;
' broker-connection is broker

: server ( port -- )
  telnet-port ->port!
  AF_INET SOCK_STREAM 0 socket to sockfd
  sockfd non-block throw
  sockfd telnet-port sizeof(sockaddr_in) bind throw
  sockfd 1 listen throw   broker ;

only forth definitions
telnetd
| evaluate ;
internals DEFINED? assembler-source [IF]
  assembler-source evaluate
[THEN] forth
internals DEFINED? xtensa-assembler-source [IF]
  xtensa-assembler-source evaluate
[THEN] forth
internals DEFINED? riscv-assembler-source [IF]
  riscv-assembler-source evaluate
[THEN] forth

internals DEFINED? camera-source [IF]
  camera-source evaluate
[THEN] forth

internals DEFINED? interrupts-source [IF]
  interrupts-source evaluate
[THEN] forth

internals DEFINED? oled-source [IF]
  oled-source evaluate
[THEN] forth

internals DEFINED? spi-source [IF]
  spi-source evaluate
[THEN] forth

internals DEFINED? espnow-source [IF]
  espnow-source evaluate
[THEN] forth

DEFINED? rmt-builtins [IF]
  vocabulary rmt   rmt definitions
  transfer rmt-builtins
  forth definitions
[THEN]

internals DEFINED? serial-bluetooth-source [IF]
  serial-bluetooth-source evaluate
[THEN] forth

internals DEFINED? spi-flash-source [IF]
  spi-flash-source evaluate
[THEN] forth

internals DEFINED? HTTPClient-builtins [IF]
  vocabulary HTTPClient   HTTPClient definitions
  transfer HTTPClient-builtins
  forth definitions
[THEN] forth
internals definitions


( Change default block source on arduino )
: arduino-default-use s" /spiffs/blocks.fb" open-blocks ;
' arduino-default-use is default-use

( Setup remember file )
: arduino-remember-filename   s" /spiffs/myforth" ;
' arduino-remember-filename is remember-filename

( Check for autoexec.fs and run if present.
  Failing that, try to revive save image. )
: autoexec
   ( Allow skip start files if key hit within 100 ms )
   10 for key? if rdrop exit then 10 ms next
   s" /spiffs/autoexec.fs" 2dup file-exists?
     if included else 2drop then
   ['] revive catch drop ;
' autoexec ( leave on the stack for fini.fs )

forth definitions
internals
\ dump tool
: dump ( addr len -- )
    cr cr ." --addr---  "
    ." 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  ------chars-----"
    2dup + { END_ADDR }             \ store latest address to dump
    swap { START_ADDR }             \ store START address to dump
    START_ADDR 16 / 16 * { 0START_ADDR } \ calc. addr for loop start
    16 / 1+ { LINES }
    base @ { myBASE }               \ save current base
    hex
    \ outer loop
    LINES 0 do
        0START_ADDR i 16 * +        \ calc start address for current line
        cr <# # # # #  [char] - hold # # # # #> type
        space space     \ and display address
        \ first inner loop, display bytes
        16 0 do
            \ calculate real address
            0START_ADDR j 16 * i + +
            ca@ <# # # #> type space \ display byte in format: NN
        loop 
        space
        \ second inner loop, display chars
        16 0 do
            \ calculate real address
            0START_ADDR j 16 * i + +
            \ display char if code in interval 32-127
            ca@     dup 32 < over 127 > or
            if      drop [char] . emit
            else    emit
            then
        loop 
    loop
    myBASE base !               \ restore current base
    cr cr
  ;

create crlf 13 C, 10 C,
: RECORDFILE  ( "filename" "filecontents" "<EOF>" -- )
    bl parse
    W/O CREATE-FILE throw >R
    BEGIN
        tib #tib accept
        tib over
        S" <EOF>" startswith?
        DUP IF
            swap drop
        ELSE
            swap
            tib swap
            R@ WRITE-FILE throw
            crlf 1+ 1 R@ WRITE-FILE throw
        THEN
    UNTIL
    R> CLOSE-FILE throw
;
: MAIN ( -- )
    s" /spiffs/main.fs"       included
  ;

forth definitions
internals definitions
( TODO: Figure out why this has to happen so late. )
transfer internals-builtins
forth definitions internals
( Bring a forth to the top of the vocabulary. )
transfer forth
( Move heap to save point, with a gap. )
setup-saving-base
forth
execute ( assumes an xt for autoboot is on the dstack )
ok
)""";

// Work around lack of ftruncate
static cell_t ResizeFile(cell_t fd, cell_t size) {
  struct stat st;
  char buf[256];
  cell_t t = fstat(fd, &st);
  if (t < 0) { return errno; }
  if (size < st.st_size) {
    // TODO: Implement truncation
    return ENOSYS;
  }
  cell_t oldpos = lseek(fd, 0, SEEK_CUR);
  if (oldpos < 0) { return errno; }
  t = lseek(fd, 0, SEEK_END);
  if (t < 0) { return errno; }
  memset(buf, 0, sizeof(buf));
  while (st.st_size < size) {
    cell_t len = sizeof(buf);
    if (size - st.st_size < len) {
      len = size - st.st_size;
    }
    t = write(fd, buf, len);
    if (t != len) {
      return errno;
    }
    st.st_size += t;
  }
  t = lseek(fd, oldpos, SEEK_SET);
  if (t < 0) { return errno; }
  return 0;
}

void setup() {
  cell_t fh = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
  cell_t hc = heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);
  if (fh - hc < MINIMUM_FREE_SYSTEM_HEAP) {
    hc = fh - MINIMUM_FREE_SYSTEM_HEAP;
  }
  cell_t *heap = (cell_t *) malloc(hc);
  forth_init(0, 0, heap, hc, boot, sizeof(boot));
}

void loop() {
  g_sys->rp = forth_run(g_sys->rp);
}
