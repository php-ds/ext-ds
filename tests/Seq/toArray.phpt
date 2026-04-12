--TEST--
Seq::toArray - convert to PHP array
--FILE--
<?php
// Basic
$s = new Ds\Seq([1, 2, 3]);
var_dump($s->toArray());

// Empty
$s = new Ds\Seq();
var_dump($s->toArray());

// Keys are sequential 0-indexed
$s = new Ds\Seq(["a", "b", "c"]);
$arr = $s->toArray();
var_dump(array_keys($arr));

// Mixed types
$s = new Ds\Seq([1, "two", 3.0, null, true]);
var_dump($s->toArray());

// Preserves order after mutations
$s = new Ds\Seq([3, 1, 2]);
$s->push(4);
$s->shift();
var_dump($s->toArray());

// Returns a regular array (not reference)
$s = new Ds\Seq([1, 2, 3]);
$arr = $s->toArray();
$arr[0] = 99;
var_dump($s->get(0)); // unchanged

// Single element
$s = new Ds\Seq([42]);
var_dump($s->toArray());

// Result is a proper array
$s = new Ds\Seq([1, 2, 3]);
var_dump(is_array($s->toArray()));
?>
--EXPECT--
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
array(0) {
}
array(3) {
  [0]=>
  int(0)
  [1]=>
  int(1)
  [2]=>
  int(2)
}
array(5) {
  [0]=>
  int(1)
  [1]=>
  string(3) "two"
  [2]=>
  float(3)
  [3]=>
  NULL
  [4]=>
  bool(true)
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(4)
}
int(1)
array(1) {
  [0]=>
  int(42)
}
bool(true)
