--TEST--
Seq::count - number of elements
--FILE--
<?php
// Empty
$s = new Ds\Seq();
var_dump($s->count());
var_dump(count($s));

// After push
$s->push(1);
var_dump($s->count());
$s->push(2, 3);
var_dump($s->count());

// After pop
$s->pop();
var_dump($s->count());

// After shift
$s->shift();
var_dump($s->count());

// After clear
$s->clear();
var_dump($s->count());

// Constructed with values
$s = new Ds\Seq([1, 2, 3, 4, 5]);
var_dump($s->count());
var_dump(count($s));

// After remove
$s->remove(2);
var_dump($s->count());
?>
--EXPECT--
int(0)
int(0)
int(1)
int(3)
int(2)
int(1)
int(0)
int(5)
int(5)
int(4)
