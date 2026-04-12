--TEST--
Seq::first - get the first element
--FILE--
<?php
// Basic
$s = new Ds\Seq([10, 20, 30]);
var_dump($s->first());

// Single element
$s = new Ds\Seq(["only"]);
var_dump($s->first());

// Does not remove the element
$s = new Ds\Seq([1, 2, 3]);
$s->first();
var_dump($s->count());

// After shift + push, first should update
$s = new Ds\Seq([1, 2, 3]);
$s->shift();
var_dump($s->first());

// With null as first
$s = new Ds\Seq([null, 1, 2]);
var_dump($s->first());

// Empty throws UnderflowException
$s = new Ds\Seq();
try {
    $s->first();
} catch (UnderflowException $e) {
    echo "UnderflowException\n";
}
?>
--EXPECT--
int(10)
string(4) "only"
int(3)
int(2)
NULL
UnderflowException
