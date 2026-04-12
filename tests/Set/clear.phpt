--TEST--
Set::clear - removes all values
--FILE--
<?php
$s = new Ds\Set([1, 2, 3, 4, 5]);
echo "before: " . $s->count() . "\n";

$s->clear();
echo "after clear: " . $s->count() . "\n";
echo "isEmpty: " . var_export($s->isEmpty(), true) . "\n";
echo "capacity: " . $s->capacity() . "\n";

// Clear on empty set
$s->clear();
echo "clear empty: " . $s->count() . "\n";

// Can add after clear
$s->add(10, 20);
echo "after re-add: " . $s->count() . "\n";
echo "values: " . $s->join(",") . "\n";
?>
--EXPECT--
before: 5
after clear: 0
isEmpty: true
capacity: 8
clear empty: 0
after re-add: 2
values: 10,20
