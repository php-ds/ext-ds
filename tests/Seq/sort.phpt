--TEST--
Seq::sort - sort in-place
--FILE--
<?php
// Default ascending sort
$s = new Ds\Seq([3, 1, 4, 1, 5, 9, 2, 6]);
$s->sort();
var_dump($s->toArray());

// Custom comparator: descending
$s = new Ds\Seq([3, 1, 4, 1, 5]);
$s->sort(function($a, $b) { return $b - $a; });
var_dump($s->toArray());

// String sort
$s = new Ds\Seq(["banana", "apple", "cherry"]);
$s->sort();
var_dump($s->toArray());

// Already sorted
$s = new Ds\Seq([1, 2, 3]);
$s->sort();
var_dump($s->toArray());

// Reverse sorted
$s = new Ds\Seq([3, 2, 1]);
$s->sort();
var_dump($s->toArray());

// Single element
$s = new Ds\Seq([42]);
$s->sort();
var_dump($s->toArray());

// Empty seq
$s = new Ds\Seq();
$s->sort();
var_dump($s->toArray());

// Sort returns void
$s = new Ds\Seq([3, 1, 2]);
$result = $s->sort();
var_dump($result);

// Two elements
$s = new Ds\Seq([2, 1]);
$s->sort();
var_dump($s->toArray());
?>
--EXPECT--
array(8) {
  [0]=>
  int(1)
  [1]=>
  int(1)
  [2]=>
  int(2)
  [3]=>
  int(3)
  [4]=>
  int(4)
  [5]=>
  int(5)
  [6]=>
  int(6)
  [7]=>
  int(9)
}
array(5) {
  [0]=>
  int(5)
  [1]=>
  int(4)
  [2]=>
  int(3)
  [3]=>
  int(1)
  [4]=>
  int(1)
}
array(3) {
  [0]=>
  string(5) "apple"
  [1]=>
  string(6) "banana"
  [2]=>
  string(6) "cherry"
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
array(1) {
  [0]=>
  int(42)
}
array(0) {
}
NULL
array(2) {
  [0]=>
  int(1)
  [1]=>
  int(2)
}
