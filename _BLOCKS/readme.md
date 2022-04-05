<h3>Put the FORTH code in a file</h3>

<p>At startup, ESP32Forth has a full vocabulary. However, during development,
    it happens to resort to words used permanently.</p>
    <p>To permanently have these definitions, you have several solutions:</p>
    <ul>
        <li>integrate these definitions at the beginning of your FORTH code. But this lengthens your code
        under development;</li>
        <li>use the include directive from <a href='article/installation_myFFshell'>myFFshell</a></li>
        <li>modify ESP32Forth source code and integrate these definitions, recompile ESP32Forth and upload
        this custom version. The disadvantage is that it will be necessary to modify each new version of
        ESP32Forth.</li>
    </ul>

   <h3>Prepare your FORTH code</h3>
    <p>The solution we propose uses block files. It consists of
     put the source code of our definitions in a file located on our ESP32Forth board.</p>
    <p>Let's take the code from <a href="listing/page/text/strings">strings management for ESP32forth</a>.</p>
    <p>This code will be rewritten by removing all comment lines and
     compacting the FORTH code.</p>
     
     <h3>Load code into a block</h3>
    <p>This source code will be loaded into a FORTH source code block. In order not to saturate
    the ESP32Forth interpreter, this code will be transferred several times, by copy-paste.</p>

<h3>Compilation of block content</h3>
    <p>Unplug the ESP32 board. Plug it back in. If saving source code in an edit block
    was successful, we can find this source code like this: <code>editor l</code></p>
    <p>What should display the content of our block.</p>

    <p>To compile this code, just type the block number, 0 for our example,
    followed by <code>load</code>. It is easy to verify that the content of the block
    was successfully compiled by typing <code>words</code>.</p>
    <p>To compile the contents of this block automatically when ESP32Forth starts,
    we will put this loading command in the file <i>/spiffs/autoexec.fs</i>
    like this:</p>

<code>s" 1 load"  s" /spiffs/autoexec.fs " dump-file </code>
    <p>The system will automatically attempt to mount SPIFFS filesystem at /spiffs. It 
    will then at start attempt to load <i>/spiffs/autoexec.fs</i>.</p>
    <p>One way this feature can be used to load a bloc.</p>
    <p>If your FORTH code is spread over several contiguous blocks, for example
    on blocks 0-5 run <code>0 5 thru</code>.</p>
    <p>Loading FORTH source code from blocks is extremely fast. That
    possibility makes it possible to add definitions without requiring their transfer from
    repeatedly by the terminal.</p>
