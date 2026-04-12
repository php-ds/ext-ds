--TEST--
Seq: rotate on clone does not affect original
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3, 4]);
$b = clone $a;

$b->rotate(2);

var_dump($a->toArray());
var_dump($b->toArray());
?>
--EXPECT--
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
  int(3)
  [1]=>
  int(4)
  [2]=>
  int(1)
  [3]=>
  int(2)
}
