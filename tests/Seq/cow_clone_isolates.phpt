--TEST--
Seq: clone isolates — mutating clone does not affect original
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = clone $a;

$b->push(4);
$b->set(0, 99);
$b->unshift(0);
$b->pop();
$b->shift();
$b->remove(0);

var_dump($a->toArray()); // original untouched
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
