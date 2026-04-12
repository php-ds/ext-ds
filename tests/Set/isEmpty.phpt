--TEST--
Set::isEmpty - checks if the set has no elements
--FILE--
<?php
// Empty set
$s = new Ds\Set();
echo "new: " . var_export($s->isEmpty(), true) . "\n";

// Non-empty set
$s->add(1);
echo "after add: " . var_export($s->isEmpty(), true) . "\n";

// After removing last element
$s->remove(1);
echo "after remove: " . var_export($s->isEmpty(), true) . "\n";

// Set with elements
$s2 = new Ds\Set([1, 2, 3]);
echo "with elements: " . var_export($s2->isEmpty(), true) . "\n";

// After clear
$s2->clear();
echo "after clear: " . var_export($s2->isEmpty(), true) . "\n";
?>
--EXPECT--
new: true
after add: false
after remove: true
with elements: false
after clear: true
