--TEST--
Seq::filter - filter values with or without callback
--FILE--
<?php
// Without callback: removes falsy values
$s = new Ds\Seq([0, 1, "", "hello", null, true, false, 2, 0.0]);
$filtered = $s->filter();
var_dump($filtered->toArray());

// Original unchanged
var_dump($s->count());

// With callback
$s = new Ds\Seq([1, 2, 3, 4, 5, 6]);
$even = $s->filter(function($v) { return $v % 2 === 0; });
var_dump($even->toArray());

// Original unchanged
var_dump($s->toArray());

// Filter returns new Seq
$s = new Ds\Seq([1, 2, 3]);
$f = $s->filter(function($v) { return $v > 1; });
echo get_class($f) . "\n";

// Empty seq
$s = new Ds\Seq();
$filtered = $s->filter();
var_dump($filtered->toArray());

$filtered = $s->filter(function($v) { return true; });
var_dump($filtered->toArray());

// All elements filtered out
$s = new Ds\Seq([0, false, null, ""]);
$filtered = $s->filter();
var_dump($filtered->toArray());

// No elements filtered out
$s = new Ds\Seq([1, "a", true]);
$filtered = $s->filter();
var_dump($filtered->toArray());
?>
--EXPECT--
array(4) {
  [0]=>
  int(1)
  [1]=>
  string(5) "hello"
  [2]=>
  bool(true)
  [3]=>
  int(2)
}
int(9)
array(3) {
  [0]=>
  int(2)
  [1]=>
  int(4)
  [2]=>
  int(6)
}
array(6) {
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
  [5]=>
  int(6)
}
Ds\Seq
array(0) {
}
array(0) {
}
array(0) {
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  string(1) "a"
  [2]=>
  bool(true)
}
