--TEST--
Seq::reverse - reverse in-place
--FILE--
<?php
// Basic reverse
$s = new Ds\Seq([1, 2, 3, 4, 5]);
$s->reverse();
var_dump($s->toArray());

// Two elements
$s = new Ds\Seq(["a", "b"]);
$s->reverse();
var_dump($s->toArray());

// Single element
$s = new Ds\Seq([42]);
$s->reverse();
var_dump($s->toArray());

// Empty seq
$s = new Ds\Seq();
$s->reverse();
var_dump($s->toArray());

// Double reverse restores original
$s = new Ds\Seq([1, 2, 3]);
$s->reverse();
$s->reverse();
var_dump($s->toArray());

// Even number of elements
$s = new Ds\Seq([1, 2, 3, 4]);
$s->reverse();
var_dump($s->toArray());

// Reverse modifies in-place (returns void)
$s = new Ds\Seq([1, 2, 3]);
$result = $s->reverse();
var_dump($result);
var_dump($s->toArray());
?>
--EXPECT--
array(5) {
  [0]=>
  int(5)
  [1]=>
  int(4)
  [2]=>
  int(3)
  [3]=>
  int(2)
  [4]=>
  int(1)
}
array(2) {
  [0]=>
  string(1) "b"
  [1]=>
  string(1) "a"
}
array(1) {
  [0]=>
  int(42)
}
array(0) {
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
array(4) {
  [0]=>
  int(4)
  [1]=>
  int(3)
  [2]=>
  int(2)
  [3]=>
  int(1)
}
NULL
array(3) {
  [0]=>
  int(3)
  [1]=>
  int(2)
  [2]=>
  int(1)
}
