--TEST--
Map::first - get first pair
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$pair = $m->first();
echo "type: " . get_class($pair) . "\n";
echo "key: " . $pair->key . "\n";
echo "value: " . $pair->value . "\n";

// First of single element
$m2 = new Ds\Map(["only" => 42]);
$pair2 = $m2->first();
echo "single key: " . $pair2->key . "\n";
echo "single value: " . $pair2->value . "\n";

// First does not remove the pair
echo "count after first: " . $m->count() . "\n";

// First of empty throws UnderflowException
try {
    $empty = new Ds\Map();
    $empty->first();
    echo "ERROR: should have thrown\n";
} catch (UnderflowException $e) {
    echo "empty: " . $e->getMessage() . "\n";
}

// First reflects insertion order
$m3 = new Ds\Map();
$m3->put("z", 26);
$m3->put("a", 1);
$pair3 = $m3->first();
echo "insertion order: " . $pair3->key . "=" . $pair3->value . "\n";
?>
--EXPECT--
type: Ds\Pair
key: a
value: 1
single key: only
single value: 42
count after first: 3
empty: Unexpected empty state
insertion order: z=26
