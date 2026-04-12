--TEST--
Seq::shift - remove and return first element
--FILE--
<?php
// Basic shift
$s = new Ds\Seq([1, 2, 3]);
var_dump($s->shift());
var_dump($s->toArray());

// Shift until empty
$s = new Ds\Seq(["a", "b"]);
var_dump($s->shift());
var_dump($s->shift());
var_dump($s->isEmpty());

// Shift returns correct value
$s = new Ds\Seq([10, 20, 30, 40]);
var_dump($s->shift());
var_dump($s->shift());
var_dump($s->count());

// Shift null
$s = new Ds\Seq([null, 1, 2]);
var_dump($s->shift());

// Shift on empty throws UnderflowException
$s = new Ds\Seq();
try {
    $s->shift();
} catch (UnderflowException $e) {
    echo "UnderflowException\n";
}

// Shift on just-emptied seq throws too
$s = new Ds\Seq([1]);
$s->shift();
try {
    $s->shift();
} catch (UnderflowException $e) {
    echo "UnderflowException\n";
}

// Shift preserves remaining order
$s = new Ds\Seq([1, 2, 3, 4, 5]);
$s->shift();
$s->shift();
var_dump($s->toArray());
?>
--EXPECT--
int(1)
array(2) {
  [0]=>
  int(2)
  [1]=>
  int(3)
}
string(1) "a"
string(1) "b"
bool(true)
int(10)
int(20)
int(2)
NULL
UnderflowException
UnderflowException
array(3) {
  [0]=>
  int(3)
  [1]=>
  int(4)
  [2]=>
  int(5)
}
