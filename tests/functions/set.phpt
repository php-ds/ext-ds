--TEST--
\Ds\set() functional constructor
--FILE--
<?php
// Empty
$s = \Ds\set();
echo "empty: " . $s->count() . "\n";
var_dump($s instanceof Ds\Set);

// From array
$s = \Ds\set([1, 2, 3, 2, 1]);
echo "unique count: " . $s->count() . "\n";
echo "contains 1: " . var_export($s->contains(1), true) . "\n";
echo "contains 2: " . var_export($s->contains(2), true) . "\n";
echo "contains 3: " . var_export($s->contains(3), true) . "\n";

// From generator
function gen() { yield 10; yield 20; yield 10; }
$s = \Ds\set(gen());
echo "gen count: " . $s->count() . "\n";
?>
--EXPECT--
empty: 0
bool(true)
unique count: 3
contains 1: true
contains 2: true
contains 3: true
gen count: 2
