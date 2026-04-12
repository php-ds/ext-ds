--TEST--
Seq::unshift - prepend one or more values
--FILE--
<?php
// Single unshift
$s = new Ds\Seq([2, 3]);
$s->unshift(1);
var_dump($s->toArray());

// Multiple unshift
$s = new Ds\Seq([4, 5]);
$s->unshift(1, 2, 3);
var_dump($s->toArray());

// Unshift onto empty
$s = new Ds\Seq();
$s->unshift(1, 2, 3);
var_dump($s->toArray());

// Unshift single onto empty
$s = new Ds\Seq();
$s->unshift(1);
var_dump($s->toArray());

// Multiple unshift calls preserve order
$s = new Ds\Seq([3]);
$s->unshift(2);
$s->unshift(1);
var_dump($s->toArray());

// Unshift various types
$s = new Ds\Seq(["end"]);
$s->unshift(null, true, 42);
var_dump($s->toArray());

// Returns void
$s = new Ds\Seq([1]);
$result = $s->unshift(0);
var_dump($result);

// Unshift triggers capacity growth when needed
$s = new Ds\Seq([1, 2, 3, 4, 5, 6, 7, 8]);
$s->unshift(0);
var_dump($s->count());
echo $s->capacity() >= 9 ? "capacity grew" : "error";
echo "\n";
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
array(4) {
  [0]=>
  NULL
  [1]=>
  bool(true)
  [2]=>
  int(42)
  [3]=>
  string(3) "end"
}
NULL
int(9)
capacity grew
