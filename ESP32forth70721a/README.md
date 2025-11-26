<h1>ESP32Forth version 7.0.7.21a</h1>
<p>This version is derived from <a href="https://eforth.storage.googleapis.com/releases/ESP32forth-7.0.7.21.zip">Brad NELSON's version 7.0.7.21</a>.</p>
<p>The changes only affect the version for ESP32.</p>
<h2>Evolutions</h2>
<ul>
  <li>Modifications:
    <ul>
      <li>recognition of binary numbers prefixed by <b>%</b></li>
      <li>rewriting the word dump</li>
    </ul>
  </li>
  <li>Vocabulary removal <code>EDIT</code></li>
  <li>Added the <b>spi-voc.h</b> module</li>
</ul>

<h2>Modifications</h2>
<h3>rewriting the word dump</h3>
<p>The word <code>dump</code> has been rewritten to provide a more readable output:</p>
<code>--> here 20 dump
--addr---  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  ------chars-----
3FFE-E140  C0 EB 98 F3 B6 68 96 E5 40 36 95 F8 A7 8D 68 A3  .....h..@6....h.
3FFE-E150  1E 1B BE AE F1 79 1A 40 13 CD 85 85 D5 B5 B2 28  .....y.@.......(</code>
