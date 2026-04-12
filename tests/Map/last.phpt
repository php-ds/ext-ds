--TEST--
Map::last - get last pair
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$pair = $m->last();
echo "type: " . get_class($pair) . "\n";
echo "key: " . $pair->key . "\n";
echo "value: " . $pair->value . "\n";

// Last of single element
$m2 = new Ds\Map(["only" => 42]);
$pair2 = $m2->last();
echo "single key: " . $pair2->key . "\n";
echo "single value: " . $pair2->value . "\n";

// Last does not remove the pair
echo "count after last: " . $m->count() . "\n";

// Last of empty throws UnderflowException
try {
    $empty = new Ds\Map();
    $empty->last();
    echo "ERROR: should have thrown\n";
} catch (UnderflowException $e) {
    echo "empty: " . $e->getMessage() . "\n";
}

// Last reflects insertion order
$m3 = new Ds\Map();
$m3->put("a", 1);
$m3->put("z", 26);
$pair3 = $m3->last();
echo "insertion order: " . $pair3->key . "=" . $pair3->value . "\n";

// Last after adding more elements
$m3->put("m", 13);
$pair4 = $m3->last();
echo "after add: " . $pair4->key . "=" . $pair4->value . "\n";
?>
--EXPECT--
type: Ds\Pair
key: c
value: 3
single key: only
single value: 42
count after last: 3
empty: Unexpected empty state
insertion order: z=26
after add: m=13
