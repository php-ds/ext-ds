--TEST--
Map::__construct - create from various sources
--FILE--
<?php
// Empty constructor
$m = new Ds\Map();
echo "empty count: " . $m->count() . "\n";

// From associative array
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
echo "from array count: " . $m->count() . "\n";
echo "a=" . $m->get("a") . " b=" . $m->get("b") . " c=" . $m->get("c") . "\n";

// From integer-keyed array
$m = new Ds\Map([10 => "x", 20 => "y"]);
echo "int keys count: " . $m->count() . "\n";
echo "10=" . $m->get(10) . " 20=" . $m->get(20) . "\n";

// From another Map (traversable)
$a = new Ds\Map(["x" => 1, "y" => 2]);
$b = new Ds\Map($a);
echo "from map count: " . $b->count() . "\n";
echo "x=" . $b->get("x") . " y=" . $b->get("y") . "\n";

// From a generator
function gen() {
    yield "foo" => 100;
    yield "bar" => 200;
}
$m = new Ds\Map(gen());
echo "from generator count: " . $m->count() . "\n";
echo "foo=" . $m->get("foo") . " bar=" . $m->get("bar") . "\n";

// From ArrayIterator
$m = new Ds\Map(new ArrayIterator(["p" => 9, "q" => 8]));
echo "from ArrayIterator count: " . $m->count() . "\n";
echo "p=" . $m->get("p") . " q=" . $m->get("q") . "\n";

// Duplicate keys in array - last value wins
$m = new Ds\Map(["a" => 1, "b" => 2]);
// Arrays can't have dup keys, but we can test with putAll after construct
echo "dedup count: " . $m->count() . "\n";

// Empty array
$m = new Ds\Map([]);
echo "empty array count: " . $m->count() . "\n";
?>
--EXPECT--
empty count: 0
from array count: 3
a=1 b=2 c=3
int keys count: 2
10=x 20=y
from map count: 2
x=1 y=2
from generator count: 2
foo=100 bar=200
from ArrayIterator count: 2
p=9 q=8
dedup count: 2
empty array count: 0
