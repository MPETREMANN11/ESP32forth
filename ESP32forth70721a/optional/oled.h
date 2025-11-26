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
 * ESP32forth Oled v7.0.7.21
 * Revision: 9ae74fa18335b0378a98bd8c693b468cc1265ee5
 */

// You will need to install these libraries from the Library Manager:
//   Adafruit SSD1306
//   Adafruit GFX Library
//   Adafruit BusIO

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

static Adafruit_SSD1306 *oled_display = 0;

#define OPTIONAL_OLED_VOCABULARY V(oled)
#define OPTIONAL_OLED_SUPPORT \
  XV(internals, "oled-source", OLED_SOURCE, \
      PUSH oled_source; PUSH sizeof(oled_source) - 1) \
  YV(oled, OledAddr, PUSH &oled_display) \
  YV(oled, OledNew, oled_display = new Adafruit_SSD1306(n2, n1, &Wire, n0); DROPn(3)) \
  YV(oled, OledDelete, delete oled_display) \
  YV(oled, OledBegin, n0 = oled_display->begin(n1, n0); NIP) \
  YV(oled, OledHOME, oled_display->setCursor(0,0); DROP) \
  YV(oled, OledCLS, oled_display->clearDisplay()) \
  YV(oled, OledTextc, oled_display->setTextColor(n0); DROP) \
  YV(oled, OledPrintln, oled_display->println(c0); DROP) \
  YV(oled, OledNumln, oled_display->println(n0); DROP) \
  YV(oled, OledNum, oled_display->print(n0); DROP) \
  YV(oled, OledDisplay, oled_display->display()) \
  YV(oled, OledPrint, oled_display->write(c0); DROP) \
  YV(oled, OledInvert, oled_display->invertDisplay(n0); DROP) \
  YV(oled, OledTextsize, oled_display->setTextSize(n0); DROP) \
  YV(oled, OledSetCursor, oled_display->setCursor(n1,n0); DROPn(2)) \
  YV(oled, OledPixel, oled_display->drawPixel(n2, n1, n0); DROPn(2)) \
  YV(oled, OledDrawL, oled_display->drawLine(n4, n3, n2, n1, n0); DROPn(4)) \
  YV(oled, OledCirc, oled_display->drawCircle(n3,n2, n1, n0); DROPn(3)) \
  YV(oled, OledCircF, oled_display->fillCircle(n3, n2, n1, n0); DROPn(3)) \
  YV(oled, OledRect, oled_display->drawRect(n4, n3, n2, n1, n0); DROPn(4)) \
  YV(oled, OledRectF, oled_display->fillRect(n4, n3, n2, n1, n0); DROPn(3)) \
  YV(oled, OledRectR, oled_display->drawRoundRect(n5, n4, n3, n2, n1, n0); DROPn(5)) \
  YV(oled, OledRectRF, oled_display->fillRoundRect(n5, n4, n3, n2, n1, n0 ); DROPn(5))

const char oled_source[] = R"""(
vocabulary oled   oled definitions
transfer oled-builtins
DEFINED? OledNew [IF]
128 value WIDTH
 64 value HEIGHT
-1 constant OledReset
0 constant BLACK
1 constant WHITE
1 constant SSD1306_EXTERNALVCC
2 constant SSD1306_SWITCHCAPVCC
: OledInit
  OledAddr @ 0= if
    WIDTH HEIGHT OledReset OledNew
    SSD1306_SWITCHCAPVCC $3C OledBegin drop
  then
  OledCLS
  2 OledTextsize  ( Draw 2x Scale Text )
  WHITE OledTextc  ( Draw white text )
  0 0 OledSetCursor  ( Start at top-left corner )
  z" *Esp32forth*" OledPrintln OledDisplay
;
[THEN]
forth definitions
)""";
