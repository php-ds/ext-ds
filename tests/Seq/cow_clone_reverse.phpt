--TEST--
Seq: reverse on clone does not affect original
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = clone $a;

$b->reverse();

var_dump($a->toArray());
var_dump($b->toArray());
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
array(3) {
  [0]=>
  int(3)
  [1]=>
  int(2)
  [2]=>
  int(1)
}
