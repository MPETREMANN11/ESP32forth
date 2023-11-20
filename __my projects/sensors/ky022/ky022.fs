\ *********************************************************************
\ KY 022 Ifra Red Control
\    Filename:      ky022.fs
\    Date:          15 nov 2023
\    Updated:       18 nov 2023
\    File Version:  0.0
\    MCU:           ESP32-WROOM-32
\    Forth:         ESP32forth all versions 7.x++
\    Copyright:     Marc PETREMANN
\    GNU General Public License
\ *********************************************************************


\ chercher ES¨32 IR remote library:  
\   https://esp32io.com/tutorials/esp32-ir-remote-control
\   https://github.com/ExploreEmbedded/ESP32_RMT/blob/master/rmt.h
\   https://github.com/espressif/esp-idf/tree/f3704f027/examples/peripherals/rmt_nec_tx_rx
\  MMMmmmm... à décortiquer:
\   https://github.com/DjordjeMandic/Universal-IR-Blaster-TV-B-Gone/tree/main



\ Parameters Common to Transmit and Receive Mode
\ • Selection of a GPIO pin number on the input or output of the RMT - rmt_set_gpio()
\ • Number of memory blocks allocated for the incoming or outgoing data - rmt_set_mem_pd()
\ • Setting of the clock divider - rmt_set_clk_div()
\ • Selection of the clock source, note that currently one clock source is supported, the APB clock which is 80Mhz
\ - rmt_set_source_clk()

\ Receive Mode Parameters
\ • The filter setting - rmt_set_rx_filter()
\ • The receiver threshold setting - rmt_set_rx_idle_thresh()
\ • Whether the transmitter or receiver is entitled to access RMT’s memory - rmt_set_memory_owner(),
\ selection is from rmt_mem_owner_t

\ //3. set RMT receiver
\  * void rmt_rx_init()
\  * {
\  *     rmt_config_t rmt_rx;
\  *     rmt_rx.channel = 1;                        OK
\  *     rmt_rx.gpio_num = 19;                      OK
\  *     rmt_rx.clk_div = 100;                      OK
\  *     rmt_rx.mem_block_num = 1;                  OK
\  *     rmt_rx.rmt_mode = RMT_MODE_RX;             OK
\  *     rmt_rx.rx_config.filter_en = true;
\  *     rmt_rx.rx_config.filter_ticks_thresh = 100;
\  *     rmt_rx.rx_config.idle_threshold = 0xffff;
\  *     rmt_config(&rmt_rx);
\  *
\  *     //install system RMT driver.
\  *     rmt_driver_install(rmt_rx.channel, 1000, RMT_INTR_NUM);
\  * }




RECORDFILE /spiffs/ky022.fs
DEFINED? esp_errors [IF]   \ test if esp_err.fs loaded
    0 to NODEBUG
[ELSE]
    : .esp_error (  error -- )
        POSTPONE drop ; immediate
[THEN]

DEFINED? rmt 0= [IF]
    ABORT" rmt vocabulary not defined"
[THEN]

\ global parameters constants
2 constant RMT_RX_CHANNEL \ interval [0..7]
1 constant RMT_MODE_RX
0 constant RMT_MODE_TX
0 constant INVERT_SIG
0 constant PD_EN \ 0 = normal mode; 1 = memory in low power state

0 value DIV_CNT     \ RMT counter clock divider
\ DEFINED? DECODE_LG  [IF] 
    80 to DIV_CNT   \  input clock 80 MHz => output clk 1 MHz
\ [THEN]

1 constant RX_FILTER_EN   \ 1 enable | 0 disable filter
1 constant RX_MEM_BLOCK_NUM

\ reception buffer
create RX_BUFFER  512 allot

$3FF56800 constant RMT_RAM_ADDRESS_BASE
64 32 *   constant RMT_RAM_BLOCK_SIZE

0 constant RMT_RX_BUF_SIZE
0 constant RMT_INTR_NUM

1 constant RX_IDX_RST

rmt
\ init GPIO used by KY-022
: ky022.gpio.init ( -- )
    RMT_RX_CHANNEL RMT_MODE_RX IR_RECEIVE_PIN INVERT_SIG rmt_set_gpio .esp_error
  ;

\ set RMT memory power mode
: ky022.setMem.init ( -- )
    RMT_RX_CHANNEL PD_EN rmt_set_mem_pd .esp_error
  ;

\ Set RMT clock divider
: ky022.setClkDiv.init ( -- )
    RMT_RX_CHANNEL DIV_CNT rmt_set_clk_div .esp_error
  ;

\ set RMT filter
: ky022.setFilter.init ( -- )
    RMT_RX_CHANNEL RX_FILTER_EN 1 rmt_set_rx_filter .esp_error
  ;

\ set RMT memory bloc number
: ky022.setBlkNum.init ( -- )
    RMT_RX_CHANNEL RX_MEM_BLOCK_NUM rmt_set_mem_block_num .esp_error
  ;

: ky022.init ( -- )
    ky022.gpio.init 
    ky022.setMem.init
    ky022.setClkDiv.init
    ky022.setFilter.init
    ky022.setBlkNum.init
  ;

: ky022.begin ( -- )
    ky022.init
    RMT_RX_CHANNEL RMT_RX_BUF_SIZE RMT_INTR_NUM rmt_driver_install .esp_error
    RMT_RX_CHANNEL RX_IDX_RST rmt_rx_start .esp_error
  ;



forth

<EOF>
