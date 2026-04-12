--TEST--
Set::copy - creates a shallow copy
--FILE--
<?php
$a = new Ds\Set([1, 2, 3]);
$b = $a->copy();

echo "a count: " . $a->count() . "\n";
echo "b count: " . $b->count() . "\n";
echo "equal values: " . var_export($a->toArray() === $b->toArray(), true) . "\n";

// Modifying copy does not affect original
$b->add(4);
echo "a after b add: " . $a->count() . "\n";
echo "b after b add: " . $b->count() . "\n";

// Modifying original does not affect copy
$a->remove(1);
echo "a after remove: " . $a->count() . "\n";
echo "b after a remove: " . $b->count() . "\n";

// Copy of empty set
$empty = new Ds\Set();
$copy = $empty->copy();
echo "empty copy count: " . $copy->count() . "\n";
echo "empty copy isEmpty: " . var_export($copy->isEmpty(), true) . "\n";

// Copy is a new instance
echo "same instance: " . var_export($a === $b, true) . "\n";

// Shallow: objects are shared
$obj = new stdClass();
$obj->value = 1;
$s1 = new Ds\Set([$obj]);
$s2 = $s1->copy();
$obj->value = 2;
$arr = $s2->toArray();
echo "shared object: " . $arr[0]->value . "\n";
?>
--EXPECT--
a count: 3
b count: 3
equal values: true
a after b add: 3
b after b add: 4
a after remove: 2
b after a remove: 4
empty copy count: 0
empty copy isEmpty: true
same instance: false
shared object: 2
