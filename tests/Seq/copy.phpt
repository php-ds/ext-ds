--TEST--
Seq::copy - shallow copy of the sequence
--FILE--
<?php
// Basic copy
$a = new Ds\Seq([1, 2, 3]);
$b = $a->copy();

var_dump($a->toArray());
var_dump($b->toArray());

// Modifying copy does not affect original
$b->push(4);
var_dump($a->toArray());
var_dump($b->toArray());

// Modifying original does not affect copy
$a->push(5);
var_dump($a->toArray());
var_dump($b->toArray());

// Copy of empty seq
$a = new Ds\Seq();
$b = $a->copy();
var_dump($b->isEmpty());

$b->push(1);
var_dump($a->isEmpty());
var_dump($b->count());

// Copy is a Ds\Seq
$a = new Ds\Seq([1]);
$b = $a->copy();
echo get_class($b) . "\n";

// Shallow copy: objects are shared
$obj = new stdClass();
$obj->x = 1;
$a = new Ds\Seq([$obj]);
$b = $a->copy();
$obj->x = 2;
var_dump($a->get(0)->x);
var_dump($b->get(0)->x);
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
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
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
  int(4)
}
bool(true)
bool(true)
int(1)
Ds\Seq
int(2)
int(2)
