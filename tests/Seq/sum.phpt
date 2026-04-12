--TEST--
Seq::sum - sum of all values
--FILE--
<?php
// Integer sum
$s = new Ds\Seq([1, 2, 3, 4, 5]);
var_dump($s->sum());

// Float sum
$s = new Ds\Seq([1.5, 2.5, 3.0]);
var_dump($s->sum());

// Mixed int and float
$s = new Ds\Seq([1, 2.5, 3]);
var_dump($s->sum());

// Empty seq
$s = new Ds\Seq();
var_dump($s->sum());

// Single element
$s = new Ds\Seq([42]);
var_dump($s->sum());

// With negative numbers
$s = new Ds\Seq([-1, 0, 1]);
var_dump($s->sum());

// Large values
$s = new Ds\Seq([1000000, 2000000, 3000000]);
var_dump($s->sum());

// With zero
$s = new Ds\Seq([0, 0, 0]);
var_dump($s->sum());

// Boolean and string coercion
$s = new Ds\Seq([true, false, "5"]);
var_dump($s->sum());
?>
--EXPECT--
int(15)
float(7)
float(6.5)
int(0)
int(42)
int(0)
int(6000000)
int(0)
int(6)
