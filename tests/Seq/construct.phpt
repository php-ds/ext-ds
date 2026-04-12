--TEST--
Seq::__construct - create from various sources
--FILE--
<?php
// Empty constructor
$s = new Ds\Seq();
var_dump($s->count());
var_dump($s->toArray());

// From array
$s = new Ds\Seq([1, 2, 3]);
var_dump($s->toArray());

// From another Seq
$a = new Ds\Seq([4, 5, 6]);
$b = new Ds\Seq($a);
var_dump($b->toArray());

// Modifying original does not affect the copy
$a->push(7);
var_dump($b->toArray());

// From generator
function gen() {
    yield 10;
    yield 20;
    yield 30;
}
$s = new Ds\Seq(gen());
var_dump($s->toArray());

// From ArrayIterator
$s = new Ds\Seq(new ArrayIterator(["a", "b", "c"]));
var_dump($s->toArray());

// With mixed types
$s = new Ds\Seq([1, "two", 3.0, null, true, false]);
var_dump($s->count());

// From empty array
$s = new Ds\Seq([]);
var_dump($s->count());
var_dump($s->isEmpty());
?>
--EXPECT--
int(0)
array(0) {
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
  int(4)
  [1]=>
  int(5)
  [2]=>
  int(6)
}
array(3) {
  [0]=>
  int(4)
  [1]=>
  int(5)
  [2]=>
  int(6)
}
array(3) {
  [0]=>
  int(10)
  [1]=>
  int(20)
  [2]=>
  int(30)
}
array(3) {
  [0]=>
  string(1) "a"
  [1]=>
  string(1) "b"
  [2]=>
  string(1) "c"
}
int(6)
int(0)
bool(true)
