--TEST--
Seq::rotate - rotate elements left or right
--FILE--
<?php
// Positive rotation: left (first elements go to end)
$s = new Ds\Seq(["a", "b", "c"]);
$s->rotate(1);
var_dump($s->toArray());

// rotate(2)
$s = new Ds\Seq(["a", "b", "c"]);
$s->rotate(2);
var_dump($s->toArray());

// Full rotation returns to original
$s = new Ds\Seq(["a", "b", "c"]);
$s->rotate(3);
var_dump($s->toArray());

// More than size wraps around
$s = new Ds\Seq(["a", "b", "c"]);
$s->rotate(4); // same as rotate(1)
var_dump($s->toArray());

// Negative rotation: right (last elements go to front)
$s = new Ds\Seq(["a", "b", "c"]);
$s->rotate(-1);
var_dump($s->toArray());

$s = new Ds\Seq(["a", "b", "c"]);
$s->rotate(-2);
var_dump($s->toArray());

// Rotate 0: no change
$s = new Ds\Seq([1, 2, 3]);
$s->rotate(0);
var_dump($s->toArray());

// Empty seq
$s = new Ds\Seq();
$s->rotate(1);
var_dump($s->toArray());

// Single element
$s = new Ds\Seq([42]);
$s->rotate(1);
var_dump($s->toArray());

// Two elements
$s = new Ds\Seq([1, 2]);
$s->rotate(1);
var_dump($s->toArray());

$s = new Ds\Seq([1, 2]);
$s->rotate(-1);
var_dump($s->toArray());
?>
--EXPECT--
array(3) {
  [0]=>
  string(1) "b"
  [1]=>
  string(1) "c"
  [2]=>
  string(1) "a"
}
array(3) {
  [0]=>
  string(1) "c"
  [1]=>
  string(1) "a"
  [2]=>
  string(1) "b"
}
array(3) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "b"
  [2]=>
  string(1) "c"
}
array(3) {
  [0]=>
  string(1) "b"
  [1]=>
  string(1) "c"
  [2]=>
  string(1) "a"
}
array(3) {
  [0]=>
  string(1) "c"
  [1]=>
  string(1) "a"
  [2]=>
  string(1) "b"
}
array(3) {
  [0]=>
  string(1) "b"
  [1]=>
  string(1) "c"
  [2]=>
  string(1) "a"
}
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
array(1) {
  [0]=>
  int(42)
}
array(2) {
  [0]=>
  int(2)
  [1]=>
  int(1)
}
array(2) {
  [0]=>
  int(2)
  [1]=>
  int(1)
}
