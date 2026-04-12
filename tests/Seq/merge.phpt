--TEST--
Seq::merge - return new Seq with values appended
--FILE--
<?php
// Merge array
$s = new Ds\Seq([1, 2, 3]);
$merged = $s->merge([4, 5, 6]);
var_dump($merged->toArray());

// Original unchanged
var_dump($s->toArray());

// Merge returns Ds\Seq
echo get_class($merged) . "\n";

// Merge with empty array
$s = new Ds\Seq([1, 2]);
$merged = $s->merge([]);
var_dump($merged->toArray());

// Merge onto empty seq
$s = new Ds\Seq();
$merged = $s->merge([1, 2, 3]);
var_dump($merged->toArray());

// Merge with traversable
$s = new Ds\Seq([1, 2]);
$merged = $s->merge(new ArrayIterator([3, 4]));
var_dump($merged->toArray());

// Merge with another Seq
$a = new Ds\Seq([1, 2]);
$b = new Ds\Seq([3, 4]);
$merged = $a->merge($b);
var_dump($merged->toArray());

// Merge mixed types
$s = new Ds\Seq([1]);
$merged = $s->merge(["a", null, true]);
var_dump($merged->toArray());
?>
--EXPECT--
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
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
Ds\Seq
array(2) {
  [0]=>
  int(1)
  [1]=>
  int(2)
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
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
  [3]=>
  int(4)
}
array(4) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
  [3]=>
  int(4)
}
array(4) {
  [0]=>
  int(1)
  [1]=>
  string(1) "a"
  [2]=>
  NULL
  [3]=>
  bool(true)
}
