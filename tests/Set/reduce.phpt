--TEST--
Set::reduce - reduces set to a single value
--FILE--
<?php
$s = new Ds\Set([1, 2, 3, 4, 5]);

// Sum with initial value
$sum = $s->reduce(fn($carry, $value) => $carry + $value, 0);
echo "sum: $sum\n";

// Sum without initial value (starts as null)
$sum2 = $s->reduce(fn($carry, $value) => ($carry ?? 0) + $value);
echo "sum no initial: $sum2\n";

// Product
$product = $s->reduce(fn($carry, $value) => $carry * $value, 1);
echo "product: $product\n";

// Concatenation
$s2 = new Ds\Set(["a", "b", "c"]);
$str = $s2->reduce(fn($carry, $value) => $carry . $value, "");
echo "concat: $str\n";

// With initial value
$result = $s->reduce(fn($carry, $value) => $carry + $value, 100);
echo "with initial 100: $result\n";

// Empty set with initial value
$empty = new Ds\Set();
$result2 = $empty->reduce(fn($carry, $value) => $carry + $value, 42);
echo "empty with initial: $result2\n";

// Empty set without initial value returns null
$result3 = $empty->reduce(fn($carry, $value) => $carry + $value);
echo "empty no initial: " . var_export($result3, true) . "\n";

// Collect into array
$collected = $s->reduce(function($carry, $value) {
    $carry[] = $value * 2;
    return $carry;
}, []);
echo "collected: " . implode(",", $collected) . "\n";
?>
--EXPECT--
sum: 15
sum no initial: 15
product: 120
concat: abc
with initial 100: 115
empty with initial: 42
empty no initial: NULL
collected: 2,4,6,8,10
