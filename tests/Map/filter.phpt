--TEST--
Map::filter - filter pairs by callback or truthiness
--FILE--
<?php
// Filter with callback
$m = new Ds\Map(["a" => 1, "b" => 2, "c" => 3, "d" => 4]);
$even = $m->filter(function($key, $value) {
    return $value % 2 === 0;
});
echo "even count: " . $even->count() . "\n";
echo "has b: " . var_export($even->hasKey("b"), true) . "\n";
echo "has d: " . var_export($even->hasKey("d"), true) . "\n";
echo "has a: " . var_export($even->hasKey("a"), true) . "\n";

// Filter using key in callback
$m2 = new Ds\Map(["apple" => 1, "avocado" => 2, "banana" => 3]);
$aKeys = $m2->filter(function($key, $value) {
    return str_starts_with($key, "a");
});
echo "a-keys count: " . $aKeys->count() . "\n";
echo "has apple: " . var_export($aKeys->hasKey("apple"), true) . "\n";
echo "has avocado: " . var_export($aKeys->hasKey("avocado"), true) . "\n";

// Filter without callback - filter by value truthiness
$m3 = new Ds\Map(["a" => 0, "b" => "", "c" => null, "d" => false, "e" => 1, "f" => "hello", "g" => []]);
$truthy = $m3->filter();
echo "truthy count: " . $truthy->count() . "\n";
echo "has e: " . var_export($truthy->hasKey("e"), true) . "\n";
echo "has f: " . var_export($truthy->hasKey("f"), true) . "\n";
echo "has a: " . var_export($truthy->hasKey("a"), true) . "\n";

// Filter returns new map
$orig = new Ds\Map(["a" => 1, "b" => 2]);
$filtered = $orig->filter(function($k, $v) { return $v > 1; });
$orig->put("c", 3);
echo "filtered count: " . $filtered->count() . "\n";
echo "orig count: " . $orig->count() . "\n";

// Filter empty map
$empty = new Ds\Map();
$result = $empty->filter(function($k, $v) { return true; });
echo "empty filter: " . $result->count() . "\n";

// Filter where nothing matches
$m4 = new Ds\Map(["a" => 1, "b" => 2]);
$none = $m4->filter(function($k, $v) { return false; });
echo "no match: " . $none->count() . "\n";
?>
--EXPECT--
even count: 2
has b: true
has d: true
has a: false
a-keys count: 2
has apple: true
has avocado: true
truthy count: 2
has e: true
has f: true
has a: false
filtered count: 1
orig count: 3
empty filter: 0
no match: 0
