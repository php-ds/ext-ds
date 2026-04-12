--TEST--
Seq: set on clone does not affect original
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = clone $a;

$b->set(0, 99);
$b->set(1, 88);
$b->set(2, 77);

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
  int(99)
  [1]=>
  int(88)
  [2]=>
  int(77)
}
