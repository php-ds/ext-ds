--TEST--
Seq::slice - return a sub-sequence
--FILE--
<?php
$s = new Ds\Seq(["a", "b", "c", "d", "e"]);

// Slice with offset and length
var_dump($s->slice(1, 3)->toArray());

// Slice from offset to end (no length)
var_dump($s->slice(2)->toArray());

// Slice from beginning with length
var_dump($s->slice(0, 2)->toArray());

// Negative offset (from end)
var_dump($s->slice(-2)->toArray());

// Negative offset with length
var_dump($s->slice(-3, 2)->toArray());

// Offset 0, length 0
var_dump($s->slice(0, 0)->toArray());

// Original unchanged
var_dump($s->toArray());

// Returns Ds\Seq
echo get_class($s->slice(0, 1)) . "\n";

// Entire sequence
var_dump($s->slice(0)->toArray());

// Length exceeding remaining
var_dump($s->slice(3, 100)->toArray());

// Empty seq
$s = new Ds\Seq();
var_dump($s->slice(0)->toArray());

// Single element
$s = new Ds\Seq([42]);
var_dump($s->slice(0, 1)->toArray());
var_dump($s->slice(0, 0)->toArray());
?>
--EXPECT--
array(3) {
  [0]=>
  string(1) "b"
  [1]=>
  string(1) "c"
  [2]=>
  string(1) "d"
}
array(3) {
  [0]=>
  string(1) "c"
  [1]=>
  string(1) "d"
  [2]=>
  string(1) "e"
}
array(2) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "b"
}
array(2) {
  [0]=>
  string(1) "d"
  [1]=>
  string(1) "e"
}
array(2) {
  [0]=>
  string(1) "c"
  [1]=>
  string(1) "d"
}
array(0) {
}
array(5) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "b"
  [2]=>
  string(1) "c"
  [3]=>
  string(1) "d"
  [4]=>
  string(1) "e"
}
Ds\Seq
array(5) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "b"
  [2]=>
  string(1) "c"
  [3]=>
  string(1) "d"
  [4]=>
  string(1) "e"
}
array(2) {
  [0]=>
  string(1) "d"
  [1]=>
  string(1) "e"
}
array(0) {
}
array(1) {
  [0]=>
  int(42)
}
array(0) {
}
