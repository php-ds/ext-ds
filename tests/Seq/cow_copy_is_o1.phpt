--TEST--
Seq: copy() shares data (COW), mutating copy does not affect original
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = $a->copy();

$b->push(4);
$a->push(5);

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
  int(5)
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
