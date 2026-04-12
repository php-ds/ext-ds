--TEST--
Set::filter - filters values with optional callback
--FILE--
<?php
// With callback
$s = new Ds\Set([1, 2, 3, 4, 5, 6]);
$even = $s->filter(fn($v) => $v % 2 === 0);
echo "even: " . $even->join(",") . "\n";
echo "even count: " . $even->count() . "\n";

// Original unchanged
echo "original: " . $s->join(",") . "\n";

// Without callback - removes falsy values
$s2 = new Ds\Set([0, 1, false, true, "", "hello", null, 2]);
$truthy = $s2->filter();
echo "truthy: " . $truthy->count() . "\n";
echo "truthy contains 1: " . var_export($truthy->contains(1), true) . "\n";
echo "truthy contains true: " . var_export($truthy->contains(true), true) . "\n";
echo "truthy contains 'hello': " . var_export($truthy->contains("hello"), true) . "\n";
echo "truthy contains 2: " . var_export($truthy->contains(2), true) . "\n";
echo "truthy contains 0: " . var_export($truthy->contains(0), true) . "\n";
echo "truthy contains false: " . var_export($truthy->contains(false), true) . "\n";

// Filter with string callback
$s3 = new Ds\Set(["abc", "a", "ab", "abcd", "abcde"]);
$long = $s3->filter(fn($v) => strlen($v) > 2);
echo "long strings: " . $long->join(",") . "\n";

// Filter empty set
$empty = new Ds\Set();
$result = $empty->filter(fn($v) => true);
echo "empty filter: " . $result->count() . "\n";

// Filter returning no matches
$s4 = new Ds\Set([1, 2, 3]);
$none = $s4->filter(fn($v) => $v > 10);
echo "no matches: " . $none->count() . "\n";

// Filter returning all matches
$all = $s4->filter(fn($v) => $v > 0);
echo "all match: " . $all->join(",") . "\n";

// Returns new Set
echo "new instance: " . var_export($even !== $s, true) . "\n";
?>
--EXPECT--
even: 2,4,6
even count: 3
original: 1,2,3,4,5,6
truthy: 4
truthy contains 1: true
truthy contains true: true
truthy contains 'hello': true
truthy contains 2: true
truthy contains 0: false
truthy contains false: false
long strings: abc,abcd,abcde
empty filter: 0
no matches: 0
all match: 1,2,3
new instance: true
