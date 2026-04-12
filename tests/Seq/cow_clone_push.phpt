--TEST--
Seq: push on clone does not affect original
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = clone $a;

$b->push(4, 5, 6);

var_dump($a->count());
var_dump($b->count());
var_dump($a->toArray());
var_dump($b->toArray());
?>
--EXPECT--
int(3)
int(6)
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
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
