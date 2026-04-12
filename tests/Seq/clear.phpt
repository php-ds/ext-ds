--TEST--
Seq::clear - empty the sequence
--FILE--
<?php
// Clear a populated seq
$s = new Ds\Seq([1, 2, 3, 4, 5]);
$s->clear();
var_dump($s->count());
var_dump($s->isEmpty());
var_dump($s->capacity()); // resets to MIN_CAPACITY
var_dump($s->toArray());

// Clear an already empty seq
$s = new Ds\Seq();
$s->clear();
var_dump($s->count());
var_dump($s->capacity());

// Clear then push
$s = new Ds\Seq([10, 20, 30]);
$s->clear();
$s->push(99);
var_dump($s->toArray());

// Clear a large seq resets capacity
$s = new Ds\Seq();
for ($i = 0; $i < 100; $i++) {
    $s->push($i);
}
echo $s->capacity() > 8 ? "true" : "false";
echo "\n";
$s->clear();
var_dump($s->capacity());
?>
--EXPECT--
int(0)
bool(true)
int(8)
array(0) {
}
int(0)
int(8)
array(1) {
  [0]=>
  int(99)
}
true
int(8)
