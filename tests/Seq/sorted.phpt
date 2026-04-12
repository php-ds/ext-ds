--TEST--
Seq::sorted - return new sorted Seq
--FILE--
<?php
// Default ascending
$s = new Ds\Seq([3, 1, 4, 1, 5]);
$sorted = $s->sorted();
var_dump($sorted->toArray());

// Original unchanged
var_dump($s->toArray());

// Returns Ds\Seq
echo get_class($sorted) . "\n";

// Custom comparator: descending
$s = new Ds\Seq([3, 1, 4, 1, 5]);
$sorted = $s->sorted(function($a, $b) { return $b - $a; });
var_dump($sorted->toArray());

// String sort
$s = new Ds\Seq(["cherry", "apple", "banana"]);
$sorted = $s->sorted();
var_dump($sorted->toArray());

// Empty seq
$s = new Ds\Seq();
$sorted = $s->sorted();
var_dump($sorted->toArray());

// Single element
$s = new Ds\Seq([42]);
var_dump($s->sorted()->toArray());

// Already sorted
$s = new Ds\Seq([1, 2, 3]);
var_dump($s->sorted()->toArray());
?>
--EXPECT--
array(5) {
  [0]=>
  int(1)
  [1]=>
  int(1)
  [2]=>
  int(3)
  [3]=>
  int(4)
  [4]=>
  int(5)
}
array(5) {
  [0]=>
  int(3)
  [1]=>
  int(1)
  [2]=>
  int(4)
  [3]=>
  int(1)
  [4]=>
  int(5)
}
Ds\Seq
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
array(0) {
}
array(1) {
  [0]=>
  int(42)
}
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
