--TEST--
Seq::reversed - return new reversed Seq
--FILE--
<?php
// Basic reversed
$s = new Ds\Seq([1, 2, 3, 4, 5]);
$r = $s->reversed();
var_dump($r->toArray());

// Original unchanged
var_dump($s->toArray());

// Returns Ds\Seq
echo get_class($r) . "\n";

// Single element
$s = new Ds\Seq([42]);
var_dump($s->reversed()->toArray());

// Two elements
$s = new Ds\Seq(["a", "b"]);
var_dump($s->reversed()->toArray());

// Empty seq
$s = new Ds\Seq();
var_dump($s->reversed()->toArray());

// Reversed of reversed equals original
$s = new Ds\Seq([1, 2, 3]);
var_dump($s->reversed()->reversed()->toArray());
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
array(5) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
  [3]=>
  int(4)
  [4]=>
  int(5)
}
Ds\Seq
array(1) {
  [0]=>
  int(42)
}
array(2) {
  [0]=>
  string(1) "b"
  [1]=>
  string(1) "a"
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
