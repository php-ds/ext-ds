--TEST--
Seq: clear on clone does not affect original
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = clone $a;

$b->clear();

var_dump($a->count());
var_dump($b->count());
var_dump($a->toArray());
?>
--EXPECT--
int(3)
int(0)
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
