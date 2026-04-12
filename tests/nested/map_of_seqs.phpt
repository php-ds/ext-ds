--TEST--
Nested: Map with Seq values
--FILE--
<?php
$m = new Ds\Map();
$m->put("primes", new Ds\Seq([2, 3, 5, 7]));
$m->put("evens", new Ds\Seq([2, 4, 6, 8]));

echo "primes: " . implode(",", $m->get("primes")->toArray()) . "\n";
echo "evens: " . implode(",", $m->get("evens")->toArray()) . "\n";

// Mutate a value through the map
$m->get("primes")->push(11);
echo "primes after: " . implode(",", $m->get("primes")->toArray()) . "\n";
?>
--EXPECT--
primes: 2,3,5,7
evens: 2,4,6,8
primes after: 2,3,5,7,11
