<h2>FOOD - Forth Object Oriented Dialect</h2>

<h3>Classes, sub-classes, super-classes</h3>

<h4>classes ( -- )</h4>
<p>Display list of defined classes</p>

<h4>.active ( -- )</h4>
<p>Display active class</p>

<h4>class ( comp: -- <class-name> | exec: -- )</h4>
<p>Define new class</p>
<pre>
class data
class array
classes
  \ display: data array
data
.active   \ display: active class: data
array
.active   \ display: active class: array
</pre>
