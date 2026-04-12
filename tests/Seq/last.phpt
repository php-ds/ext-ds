--TEST--
Seq::last - get the last element
--FILE--
<?php
// Basic
$s = new Ds\Seq([10, 20, 30]);
var_dump($s->last());

// Single element
$s = new Ds\Seq(["only"]);
var_dump($s->last());

// Does not remove
$s = new Ds\Seq([1, 2, 3]);
$s->last();
var_dump($s->count());

// After push, last should update
$s = new Ds\Seq([1, 2, 3]);
$s->push(4);
var_dump($s->last());

// After pop, last should update
$s = new Ds\Seq([1, 2, 3]);
$s->pop();
var_dump($s->last());

// With null as last
$s = new Ds\Seq([1, 2, null]);
var_dump($s->last());

// Empty throws UnderflowException
$s = new Ds\Seq();
try {
    $s->last();
} catch (UnderflowException $e) {
    echo "UnderflowException\n";
}
?>
--EXPECT--
int(30)
string(4) "only"
int(3)
int(4)
int(2)
NULL
UnderflowException
