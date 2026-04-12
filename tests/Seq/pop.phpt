--TEST--
Seq::pop - remove and return last element
--FILE--
<?php
// Basic pop
$s = new Ds\Seq([1, 2, 3]);
var_dump($s->pop());
var_dump($s->toArray());

// Pop until empty
$s = new Ds\Seq(["a", "b"]);
var_dump($s->pop());
var_dump($s->pop());
var_dump($s->isEmpty());

// Pop returns the correct value
$s = new Ds\Seq([10, 20, 30, 40]);
var_dump($s->pop());
var_dump($s->pop());
var_dump($s->count());

// Pop null
$s = new Ds\Seq([1, null]);
var_dump($s->pop());

// Pop on empty throws UnderflowException
$s = new Ds\Seq();
try {
    $s->pop();
} catch (UnderflowException $e) {
    echo "UnderflowException\n";
}

// Pop on just-emptied seq throws too
$s = new Ds\Seq([1]);
$s->pop();
try {
    $s->pop();
} catch (UnderflowException $e) {
    echo "UnderflowException\n";
}
?>
--EXPECT--
int(3)
array(2) {
  [0]=>
  int(1)
  [1]=>
  int(2)
}
string(1) "b"
string(1) "a"
bool(true)
int(40)
int(30)
int(2)
NULL
UnderflowException
UnderflowException
