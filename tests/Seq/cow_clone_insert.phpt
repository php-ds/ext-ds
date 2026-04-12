--TEST--
Seq: insert on clone does not affect original
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = clone $a;

$b->insert(1, 10, 20);

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
array(5) {
  [0]=>
  int(1)
  [1]=>
  int(10)
  [2]=>
  int(20)
  [3]=>
  int(2)
  [4]=>
  int(3)
}
