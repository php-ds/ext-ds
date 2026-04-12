--TEST--
Map::pairs - get all pairs as a Seq
--FILE--
<?php
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$pairs = $m->pairs();

echo "type: " . get_class($pairs) . "\n";
echo "count: " . $pairs->count() . "\n";

// Each element is a Ds\Pair
$p0 = $pairs->get(0);
echo "pair type: " . get_class($p0) . "\n";
echo "pair 0: " . $p0->key . "=" . $p0->value . "\n";

$p1 = $pairs->get(1);
echo "pair 1: " . $p1->key . "=" . $p1->value . "\n";

$p2 = $pairs->get(2);
echo "pair 2: " . $p2->key . "=" . $p2->value . "\n";

// Empty map
$empty = new Ds\Map();
$emptyPairs = $empty->pairs();
echo "empty pairs: " . $emptyPairs->count() . "\n";

// Pairs is independent - modifying seq doesn't affect map
$m2 = new Ds\Map(["x" => 1]);
$p = $m2->pairs();
echo "map count: " . $m2->count() . "\n";

// Pairs preserves insertion order
$m3 = new Ds\Map();
$m3->put("z", 26);
$m3->put("a", 1);
$m3->put("m", 13);
$pairs3 = $m3->pairs();
echo "order: " . $pairs3->get(0)->key . "," . $pairs3->get(1)->key . "," . $pairs3->get(2)->key . "\n";
?>
--EXPECT--
type: Ds\Seq
count: 3
pair type: Ds\Pair
pair 0: a=1
pair 1: b=2
pair 2: c=3
empty pairs: 0
map count: 1
order: z,a,m
