--TEST--
Map::reduce - reduce map to a single value
--FILE--
<?php
// Sum all values
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3]);
$sum = $m->reduce(function($carry, $key, $value) {
    return $carry + $value;
}, 0);
echo "sum: $sum\n";

// Concatenate key=value pairs
$str = $m->reduce(function($carry, $key, $value) {
    return $carry . ($carry ? "," : "") . "$key=$value";
}, "");
echo "concat: $str\n";

// Without initial value (null initial)
$result = $m->reduce(function($carry, $key, $value) {
    return ($carry ?? 0) + $value;
});
echo "no initial: $result\n";

// Reduce with initial value that's not zero
$product = $m->reduce(function($carry, $key, $value) {
    return $carry * $value;
}, 1);
echo "product: $product\n";

// Empty map with initial value
$empty = new Ds\Map();
$result = $empty->reduce(function($carry, $key, $value) {
    return $carry + $value;
}, 42);
echo "empty with initial: $result\n";

// Empty map without initial value
$result2 = $empty->reduce(function($carry, $key, $value) {
    return $carry;
});
echo "empty no initial: " . var_export($result2, true) . "\n";

// Reduce using key
$m2 = new Ds\Map(["a" => 1, "bb" => 2, "ccc" => 3]);
$keyLen = $m2->reduce(function($carry, $key, $value) {
    return $carry + strlen($key);
}, 0);
echo "key lengths: $keyLen\n";
?>
--EXPECT--
sum: 6
concat: a=1,b=2,c=3
no initial: 6
product: 6
empty with initial: 42
empty no initial: NULL
key lengths: 6
