--TEST--
\Ds\seq() functional constructor
--FILE--
<?php
// Empty
$s = \Ds\seq();
echo "empty: " . $s->count() . "\n";
var_dump($s instanceof Ds\Seq);

// From array
$s = \Ds\seq([1, 2, 3]);
echo "array: " . implode(",", $s->toArray()) . "\n";

// From traversable (generator)
function gen() { yield "a"; yield "b"; yield "c"; }
$s = \Ds\seq(gen());
echo "gen: " . implode(",", $s->toArray()) . "\n";

// From another Ds type
$s2 = \Ds\seq($s);
echo "from seq: " . implode(",", $s2->toArray()) . "\n";
?>
--EXPECT--
empty: 0
bool(true)
array: 1,2,3
gen: a,b,c
from seq: a,b,c
