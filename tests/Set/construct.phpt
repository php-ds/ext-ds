--TEST--
Set::__construct - creates a set from various inputs
--FILE--
<?php
// Empty constructor
$s = new Ds\Set();
echo "empty count: " . $s->count() . "\n";
echo "empty isEmpty: " . var_export($s->isEmpty(), true) . "\n";

// From array
$s = new Ds\Set([1, 2, 3]);
echo "from array count: " . $s->count() . "\n";
echo "from array first: " . $s->first() . "\n";
echo "from array last: " . $s->last() . "\n";

// Deduplicates
$s = new Ds\Set([1, 2, 2, 3, 3, 3]);
echo "dedup count: " . $s->count() . "\n";
echo "dedup values: " . $s->join(",") . "\n";

// From another Set (via iterator)
$a = new Ds\Set([10, 20, 30]);
$b = new Ds\Set($a);
echo "from set count: " . $b->count() . "\n";
echo "from set values: " . $b->join(",") . "\n";

// From generator
function gen() {
    yield 'a';
    yield 'b';
    yield 'c';
    yield 'a'; // duplicate
}
$s = new Ds\Set(gen());
echo "from generator count: " . $s->count() . "\n";
echo "from generator values: " . $s->join(",") . "\n";

// Mixed types are distinct
$s = new Ds\Set([0, false, "", null, "0"]);
echo "mixed falsy count: " . $s->count() . "\n";

// From ArrayIterator
$s = new Ds\Set(new ArrayIterator([4, 5, 6, 5]));
echo "from ArrayIterator count: " . $s->count() . "\n";
echo "from ArrayIterator values: " . $s->join(",") . "\n";
?>
--EXPECT--
empty count: 0
empty isEmpty: true
from array count: 3
from array first: 1
from array last: 3
dedup count: 3
dedup values: 1,2,3
from set count: 3
from set values: 10,20,30
from generator count: 3
from generator values: a,b,c
mixed falsy count: 5
from ArrayIterator count: 3
from ArrayIterator values: 4,5,6
