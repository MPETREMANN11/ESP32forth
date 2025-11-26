//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
 * ESP32forth Interrupts v7.0.7.21
 * Revision: 9ae74fa18335b0378a98bd8c693b468cc1265ee5
 */

#define INTERRUPT_STACK_CELLS 64

#include "esp_intr_alloc.h"
#include "driver/timer.h"
#include "driver/gpio.h"

static cell_t EspIntrAlloc(cell_t source, cell_t flags, cell_t xt, cell_t arg, void *ret);
static cell_t GpioIsrHandlerAdd(cell_t pin, cell_t xt, cell_t arg);
static cell_t TimerIsrCallbackAdd(cell_t group, cell_t timer, cell_t xt, cell_t arg, cell_t flags);
static void TimerInitNull(cell_t group, cell_t timer);

#define OPTIONAL_INTERRUPTS_VOCABULARIES V(interrupts) V(timers)
#define OPTIONAL_INTERRUPTS_SUPPORT \
  XV(internals, "interrupts-source", INTERRUPTS_SOURCE, \
      PUSH interrupts_source; PUSH sizeof(interrupts_source) - 1) \
  YV(interrupts, gpio_config, n0 = gpio_config((const gpio_config_t *) a0)) \
  YV(interrupts, gpio_reset_pin, n0 = gpio_reset_pin((gpio_num_t) n0)) \
  YV(interrupts, gpio_set_intr_type, n0 = gpio_set_intr_type((gpio_num_t) n1, (gpio_int_type_t) n0); NIP) \
  YV(interrupts, gpio_intr_enable, n0 = gpio_intr_enable((gpio_num_t) n0)) \
  YV(interrupts, gpio_intr_disable, n0 = gpio_intr_disable((gpio_num_t) n0)) \
  YV(interrupts, gpio_set_level, n0 = gpio_set_level((gpio_num_t) n1, n0); NIP) \
  YV(interrupts, gpio_get_level, n0 = gpio_get_level((gpio_num_t) n0)) \
  YV(interrupts, gpio_set_direction, n0 = gpio_set_direction((gpio_num_t) n1, (gpio_mode_t) n0); NIP) \
  YV(interrupts, gpio_set_pull_mode, n0 = gpio_set_pull_mode((gpio_num_t) n1, (gpio_pull_mode_t) n0); NIP) \
  YV(interrupts, gpio_wakeup_enable, n0 = gpio_wakeup_enable((gpio_num_t) n1, (gpio_int_type_t) n0); NIP) \
  YV(interrupts, gpio_wakeup_disable, n0 = gpio_wakeup_disable((gpio_num_t) n0)) \
  YV(interrupts, gpio_pullup_en, n0 = gpio_pullup_en((gpio_num_t) n0)) \
  YV(interrupts, gpio_pullup_dis, n0 = gpio_pullup_dis((gpio_num_t) n0)) \
  YV(interrupts, gpio_pulldown_en, n0 = gpio_pulldown_en((gpio_num_t) n0)) \
  YV(interrupts, gpio_pulldown_dis, n0 = gpio_pulldown_dis((gpio_num_t) n0)) \
  YV(interrupts, gpio_hold_en, n0 = gpio_hold_en((gpio_num_t) n0)) \
  YV(interrupts, gpio_hold_dis, n0 = gpio_hold_dis((gpio_num_t) n0)) \
  YV(interrupts, gpio_deep_sleep_hold_en, gpio_deep_sleep_hold_en()) \
  YV(interrupts, gpio_deep_sleep_hold_dis, gpio_deep_sleep_hold_dis()) \
  YV(interrupts, gpio_install_isr_service, n0 = gpio_install_isr_service(n0)) \
  YV(interrupts, gpio_uninstall_isr_service, gpio_uninstall_isr_service()) \
  YV(interrupts, gpio_isr_handler_add, n0 = GpioIsrHandlerAdd(n2, n1, n0); NIPn(2)) \
  YV(interrupts, gpio_isr_handler_remove, n0 = gpio_isr_handler_remove((gpio_num_t) n0)) \
  YV(interrupts, gpio_set_drive_capability, n0 = gpio_set_drive_capability((gpio_num_t) n1, (gpio_drive_cap_t) n0); NIP) \
  YV(interrupts, gpio_get_drive_capability, n0 = gpio_get_drive_capability((gpio_num_t) n1, (gpio_drive_cap_t *) a0); NIP) \
  YV(interrupts, esp_intr_alloc, n0 = EspIntrAlloc(n4, n3, n2, n1, a0); NIPn(4)) \
  YV(interrupts, esp_intr_free, n0 = esp_intr_free((intr_handle_t) n0)) \
  YV(timers, timer_isr_callback_add, n0 = TimerIsrCallbackAdd(n4, n3, n2, n1, n0); NIPn(4)) \
  YV(timers, timer_init_null, TimerInitNull(n1, n0); DROPn(2)) \
  YV(timers, timer_get_counter_value, \
      n0 = timer_get_counter_value((timer_group_t) n2, (timer_idx_t) n1, \
                                   (uint64_t *) a0); NIPn(2)) \
  YV(timers, timer_set_counter_value, \
      uint64_t val = *(uint64_t *) a0; \
      n0 = timer_set_counter_value((timer_group_t) n2, (timer_idx_t) n1, val); NIPn(2)) \
  YV(timers, timer_start, \
      n0 = timer_start((timer_group_t) n1, (timer_idx_t) n0); NIP) \
  YV(timers, timer_pause, \
      n0 = timer_pause((timer_group_t) n1, (timer_idx_t) n0); NIP) \
  YV(timers, timer_set_counter_mode, \
      n0 = timer_set_counter_mode((timer_group_t) n2, (timer_idx_t) n1, \
                                  (timer_count_dir_t) n0); NIPn(2)) \
  YV(timers, timer_set_auto_reload, \
      n0 = timer_set_auto_reload((timer_group_t) n2, (timer_idx_t) n1, \
                                 (timer_autoreload_t) n0); NIPn(2)) \
  YV(timers, timer_set_divider, \
      n0 = timer_set_divider((timer_group_t) n2, (timer_idx_t) n1, n0); NIPn(2)) \
  YV(timers, timer_set_alarm_value, uint64_t val = *(uint64_t *) a0; \
      n0 = timer_set_alarm_value((timer_group_t) n2, (timer_idx_t) n1, val); NIPn(2)) \
  YV(timers, timer_get_alarm_value, \
      n0 = timer_get_alarm_value((timer_group_t) n2, (timer_idx_t) n1, \
                                 (uint64_t *) a0); NIPn(2)) \
  YV(timers, timer_set_alarm, \
      n0 = timer_set_alarm((timer_group_t) n2, (timer_idx_t) n1, \
                           (timer_alarm_t) n0); NIPn(2)) \
  YV(timers, timer_group_intr_enable, \
      n0 = timer_group_intr_enable((timer_group_t) n1, (timer_intr_t) n0); NIP) \
  YV(timers, timer_group_intr_disable, \
      n0 = timer_group_intr_disable((timer_group_t) n1, (timer_intr_t) n0); NIP) \
  YV(timers, timer_enable_intr, \
      n0 = timer_enable_intr((timer_group_t) n1, (timer_idx_t) n0); NIP) \
  YV(timers, timer_disable_intr, \
      n0 = timer_disable_intr((timer_group_t) n1, (timer_idx_t) n0); NIP)

struct handle_interrupt_args {
  cell_t xt;
  cell_t arg;
};

static void IRAM_ATTR HandleInterrupt(void *arg) {
  struct handle_interrupt_args *args = (struct handle_interrupt_args *) arg;
  cell_t code[2];
  code[0] = args->xt;
  code[1] = g_sys->YIELD_XT;
  cell_t fstack[INTERRUPT_STACK_CELLS];
  cell_t rstack[INTERRUPT_STACK_CELLS];
  cell_t stack[INTERRUPT_STACK_CELLS];
  stack[0] = args->arg;
  cell_t *rp = rstack;
  *++rp = (cell_t) code;
  *++rp = (cell_t) (fstack + 1);
  *++rp = (cell_t) (stack + 1);
  forth_run(rp);
}

static bool IRAM_ATTR HandleInterruptAndRet(void *arg) {
  HandleInterrupt(arg);
  return true;
}

static cell_t EspIntrAlloc(cell_t source, cell_t flags, cell_t xt, cell_t arg, void *ret) {
  // NOTE: Leaks memory.
  struct handle_interrupt_args *args = (struct handle_interrupt_args *) malloc(sizeof(struct handle_interrupt_args));
  args->xt = xt;
  args->arg = arg;
  return esp_intr_alloc(source, flags, HandleInterrupt, args, (intr_handle_t *) ret);
}

static cell_t GpioIsrHandlerAdd(cell_t pin, cell_t xt, cell_t arg) {
  // NOTE: Leaks memory.
  struct handle_interrupt_args *args = (struct handle_interrupt_args *) malloc(sizeof(struct handle_interrupt_args));
  args->xt = xt;
  args->arg = arg;
  return gpio_isr_handler_add((gpio_num_t) pin, HandleInterrupt, args);
}

static void TimerInitNull(cell_t group, cell_t timer) {
  // Seems to be required starting in the 2.0 IDE.
  timer_config_t config;
  memset(&config, 0, sizeof(config));
  config.divider = 2;
  timer_init((timer_group_t) group, (timer_idx_t) timer, &config);
}

static cell_t TimerIsrCallbackAdd(cell_t group, cell_t timer, cell_t xt, cell_t arg, cell_t flags) {
  // NOTE: Leaks memory.
  struct handle_interrupt_args *args = (struct handle_interrupt_args *) malloc(sizeof(struct handle_interrupt_args));
  args->xt = xt;
  args->arg = arg;
  return timer_isr_callback_add((timer_group_t) group, (timer_idx_t) timer, HandleInterruptAndRet, args, flags);
}

const char interrupts_source[] = R"""(
vocabulary interrupts   interrupts definitions
transfer interrupts-builtins
DEFINED? gpio_config [IF]
0 constant ESP_INTR_FLAG_DEFAULT
: ESP_INTR_FLAG_LEVELn ( n=1-6 -- n ) 1 swap lshift ;
1 7 lshift constant ESP_INTR_FLAG_NMI
1 8 lshift constant ESP_INTR_FLAG_SHARED
1 9 lshift constant ESP_INTR_FLAG_EDGE
1 10 lshift constant ESP_INTR_FLAG_IRAM
1 11 lshift constant ESP_INTR_FLAG_INTRDISABLED
( Prefix these with # because GPIO_INTR_DISABLE conflicts with a function. )
0 constant #GPIO_INTR_DISABLE
1 constant #GPIO_INTR_POSEDGE
2 constant #GPIO_INTR_NEGEDGE
3 constant #GPIO_INTR_ANYEDGE
4 constant #GPIO_INTR_LOW_LEVEL
5 constant #GPIO_INTR_HIGH_LEVEL
( Easy word to trigger on any change to a pin )
ESP_INTR_FLAG_DEFAULT gpio_install_isr_service drop
: pinchange ( xt pin ) dup #GPIO_INTR_ANYEDGE gpio_set_intr_type throw
                       swap 0 gpio_isr_handler_add throw ;
[THEN]
forth definitions

internals definitions
transfer timers-builtins
forth definitions

( Lazy loaded timers )
: timers r|

vocabulary timers   timers definitions
  also registers also interrupts also internals
transfer timers-builtins

( Initialize all timers, to mimic pre-2.0 behavior. )
0 0 timer_init_null
0 1 timer_init_null
1 0 timer_init_null
1 1 timer_init_null

( 0-4 -> two cells )
: t>nx ( t -- n x ) dup 2/ 1 and swap 1 and ;

create tmp 2 cells allot
: timer@ ( t -- lo hi ) t>nx tmp timer_get_counter_value throw tmp 2@ ;
: timer! ( lo hi t -- ) >r tmp 2! r> t>nx tmp timer_set_counter_value throw ;
: alarm@ ( t -- lo hi ) t>nx tmp timer_get_alarm_value throw tmp 2@ ;
: alarm! ( lo hi t -- ) >r tmp 2! r> t>nx tmp timer_set_alarm_value throw ;

: enable! ( v t )
   swap >r t>nx r> if timer_start else timer_pause then throw ;
: increase! ( v t ) swap >r t>nx r> timer_set_counter_mode throw ;
: autoreload! ( v t ) swap >r t>nx r> timer_set_auto_reload throw ;
: divider! ( v t ) swap >r t>nx r> timer_set_divider throw ;
: alarm-enable! ( v t ) swap >r t>nx r> timer_set_alarm throw ;

: int-enable! ( f t -- )
   swap >r t>nx r> if timer_enable_intr else timer_disable_intr then throw ;

: onalarm ( xt t ) swap >r t>nx r>
                   0 ESP_INTR_FLAG_EDGE timer_isr_callback_add throw ;
: interval ( xt usec t ) 0 over enable!
                         0 over int-enable!
                         80 over divider!
                         swap over 0 swap alarm!
                         dup >r onalarm r>
                         dup >r 0 0 r> timer!
                         1 over increase!
                         1 over autoreload!
                         1 over alarm-enable!
                         1 over int-enable!
                         1 swap enable! ;

only forth definitions
timers
| evaluate ;
)""";
