<h3>Français</h3>
<p>Bonjour,</p>
<p>Ce répertoire contient des outils à installer dans la carte ESP32 en utilisant l'éditeur VISUAL:</p>
<a href="https://esp32.arduino-forth.com/article/files_visualEditor">Visual editor: édition des fichiers sources dans ESP32forth</a>
<p>Une fois installés, ces fichiers sont accessibles en permanence, même après extinction et redémarrage de la carte
ESP32 et ESP32forth.</p>
<p>Dans vos développements, voici comment appeler et compiler le contenu de ces fichiers:</p>
<pre>DEFINED? macro: 
    invert [IF] include /spiffs/registersDefinition.txt [THEN]</pre>
<p>Ce code compilera le contenu du fichier <b>registersDefinition.txt</b> que vous aurez édité et sauvegardé dans le répertoire
<b>/spiffs/</b> sur la carte ESP32 avec l'éditeur VISUAL.</p>

<h3>English</h3>
<p>Hello,</p>
<p>This directory contains tools to install in the ESP32 board using the VISUAL editor:</p>
<a href="https://esp32.arduino-forth.com/article/files_visualEditor">Visual editor: editing source files in ESP32forth</a>
<p>Once installed, these files are permanently accessible, even after switching off and restarting the card
ESP32 and ESP32forth.</p>
<p>In your developments, here is how to call and compile the contents of these files:</p>
<pre>DEFINED? macro:
     invert [IF] include /spiffs/registersDefinition.txt [THEN]</pre>
<p>This code will compile the contents of the <b>registersDefinition.txt</b> file that you have edited and saved in the directory
<b>/spiffs/</b> on the ESP32 board with the VISUAL editor.</p>

<h3>Deutsch</h3>
<p>Hallo,</p>
<p>Dieses Verzeichnis enthält Tools, die mithilfe des VISUAL-Editors im ESP32-Board installiert werden können:</p>
<a href="https://esp32.arduino-forth.com/article/files_visualEditor">Visueller Editor: Quelldateien in ESP32forth bearbeiten</a>
<p>Nach der Installation sind diese Dateien dauerhaft zugänglich, auch nach dem Ausschalten und Neustarten der Karte
ESP32 und ESP32forth.</p>
<p>In Ihren Entwicklungen können Sie den Inhalt dieser Dateien wie folgt aufrufen und kompilieren:</p>
<pre>DEFINIERT? Makro:
     invertieren [IF] include /spiffs/registersDefinition.txt [THEN]</pre>
<p>Dieser Code kompiliert den Inhalt der Datei <b>registersDefinition.txt</b>, die Sie bearbeitet und im Verzeichnis gespeichert haben
<b>/spiffs/</b> auf dem ESP32-Board mit dem VISUAL-Editor.</p>

<h3>Español</h3>
<p>Hola,</p>
<p>Este directorio contiene herramientas para instalar en la placa ESP32 usando el editor VISUAL:</p>
<a href="https://esp32.arduino-forth.com/article/files_visualEditor">Editor visual: edición de archivos fuente en ESP32forth</a>
<p>Una vez instalados, se puede acceder a estos archivos de forma permanente, incluso después de apagar y reiniciar la tarjeta
ESP32 y ESP32 en adelante.</p>
<p>En sus desarrollos, aquí está cómo llamar y compilar el contenido de estos archivos:</p>
<pre>DEFINIDO? macro:
     invertir [SI] incluir /spiffs/registersDefinition.txt [ENTONCES]</pre>
<p>Este código compilará el contenido del archivo <b>registersDefinition.txt</b> que ha editado y guardado en el directorio
<b>/spiffs/</b> en la placa ESP32 con el editor VISUAL.</p>

<h3>中文</h3>
<p>你好，</p>
<p>此目录包含使用 VISUAL 编辑器安装在 ESP32 板上的工具：</p>
<a href="https://esp32.arduino-forth.com/article/files_visualEditor">可视化编辑器：在 ESP32forth 中编辑源文件</a>
<p>一旦安装，这些文件就可以永久访问，即使在关闭并重新启动卡后也是如此
ESP32 和 ESP32forth。</p>
<p>在你的开发中，下面是如何调用和编译这些文件的内容：</p>
<pre>定义？ 宏观：
     反转 [IF] include /spiffs/registersDefinition.txt [THEN]</pre>
<p>此代码将编译您已编辑并保存在目录中的 <b>registersDefinition.txt</b> 文件的内容
<b>/spiffs/</b> 在带有 VISUAL 编辑器的 ESP32 开发板上。</p>
