--TEST--
Seq::push - append one or more values
--FILE--
<?php
// Push single value
$s = new Ds\Seq();
$s->push(1);
var_dump($s->toArray());

// Push multiple values
$s = new Ds\Seq();
$s->push(1, 2, 3);
var_dump($s->toArray());

// Push onto existing
$s = new Ds\Seq([1, 2]);
$s->push(3);
var_dump($s->toArray());

// Push various types
$s = new Ds\Seq();
$s->push(1, "two", 3.0, null, true, false);
var_dump($s->count());
var_dump($s->get(0));
var_dump($s->get(1));
var_dump($s->get(3));

// Push triggers capacity growth
$s = new Ds\Seq();
for ($i = 0; $i < 9; $i++) {
    $s->push($i);
}
var_dump($s->count());
echo $s->capacity() >= 9 ? "capacity grew" : "error";
echo "\n";

// Push preserves order
$s = new Ds\Seq();
$s->push("a");
$s->push("b");
$s->push("c");
echo $s->join("") . "\n";
?>
--EXPECT--
array(1) {
  [0]=>
  int(1)
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
int(6)
int(1)
string(3) "two"
NULL
int(9)
capacity grew
abc
