<h1>Manage OLED display SSD1306 with ESP32Forth</h1>
<h2>SSD1306 OLED 128x32 wiring</h2>
<img src="https://github.com/MPETREMANN11/ESP32forth/blob/main/displays/SSD1306wiring.jpg"/>

<h2>Essential words</h2>

<h4>dispInit ( -- )</h4>
<p>Initialize transmission between ESP32 and SSD1306 thru I2C port. And set display parameters.</p>

<h4>dispInvert ( -- )</h4>
<p>Invert display.</p>

<h4>dispNormal ( -- )</h4>
<p>Set normal display.</p>

<h4>dispOff ( -- )</h4>
<p>Turn display OFF</p>

<h4>dispOn ( -- )</h4>
<p>Turn display ON</p>

<h4>OledCLS ( -- )</h4>
<p>Clear content of OLED screen</p>

<h4>OledInvert ( -- )</h4>
<p>Toggle display between normal/inverse</p>

<h4>setContrast ( [0..254] -- )</h4>
<p>Set display contrast.</h4>

<h4>Wire.send ( c -- )</h4>
<p>Send a byte [00..255] to OLED display</p>

