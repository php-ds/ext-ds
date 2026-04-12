--TEST--
Seq: multiple clones share data, each separates independently
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);
$b = clone $a;
$c = clone $a;
$d = clone $b;

$b->push(4);
$c->push(5);
$d->push(6);

var_dump($a->toArray());
var_dump($b->toArray());
var_dump($c->toArray());
var_dump($d->toArray());
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
  int(6)
}
