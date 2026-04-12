--TEST--
Seq::contains - check for values using strict equality
--FILE--
<?php
$s = new Ds\Seq([1, "two", 3.0, null, true, false]);

// Single value checks
var_dump($s->contains(1));
var_dump($s->contains("two"));
var_dump($s->contains(3.0));
var_dump($s->contains(null));
var_dump($s->contains(true));
var_dump($s->contains(false));

// Strict: "1" !== 1
var_dump($s->contains("1"));

// Strict: 3 !== 3.0
var_dump($s->contains(3));

// Not present
var_dump($s->contains("missing"));

// Multiple args: all must be present
var_dump($s->contains(1, "two"));
var_dump($s->contains(1, "missing"));
var_dump($s->contains("missing", 1));

// Object identity
$obj1 = new stdClass();
$obj2 = new stdClass();
$s = new Ds\Seq([$obj1]);
var_dump($s->contains($obj1));
var_dump($s->contains($obj2));

// Empty seq
$s = new Ds\Seq();
var_dump($s->contains(1));
var_dump($s->contains(null));
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
