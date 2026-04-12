--TEST--
Seq::isEmpty - check if the sequence is empty
--FILE--
<?php
// Empty
$s = new Ds\Seq();
var_dump($s->isEmpty());

// Not empty
$s = new Ds\Seq([1]);
var_dump($s->isEmpty());

// After push
$s = new Ds\Seq();
$s->push(1);
var_dump($s->isEmpty());

// After pop to empty
$s = new Ds\Seq([1]);
$s->pop();
var_dump($s->isEmpty());

// After clear
$s = new Ds\Seq([1, 2, 3]);
$s->clear();
var_dump($s->isEmpty());

// Multiple elements
$s = new Ds\Seq([1, 2, 3, 4, 5]);
var_dump($s->isEmpty());
?>
--EXPECT--
bool(true)
bool(false)
bool(false)
bool(true)
bool(true)
bool(false)
