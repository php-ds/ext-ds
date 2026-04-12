--TEST--
Seq::find - find index of a value using strict comparison
--FILE--
<?php
$s = new Ds\Seq(["a", "b", "c", "d"]);

// Found
var_dump($s->find("a"));
var_dump($s->find("c"));
var_dump($s->find("d"));

// Not found returns false
var_dump($s->find("z"));

// Strict comparison
$s = new Ds\Seq([1, 2, 3]);
var_dump($s->find(1));
var_dump($s->find("1")); // strict: string !== int
var_dump($s->find(true)); // strict: bool !== int

// Find null
$s = new Ds\Seq([null, 1, 2]);
var_dump($s->find(null));
var_dump($s->find(false)); // strict: false !== null

// Find returns first occurrence
$s = new Ds\Seq([1, 2, 1, 2]);
var_dump($s->find(2));

// Empty seq
$s = new Ds\Seq();
var_dump($s->find(1));

// Object identity
$obj1 = new stdClass();
$obj2 = new stdClass();
$s = new Ds\Seq([$obj1, $obj2]);
var_dump($s->find($obj1));
var_dump($s->find($obj2));
var_dump($s->find(new stdClass()));
?>
--EXPECT--
int(0)
int(2)
int(3)
bool(false)
int(0)
bool(false)
bool(false)
int(0)
bool(false)
int(1)
bool(false)
int(0)
int(1)
bool(false)
