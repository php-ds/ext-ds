--TEST--
Nested: Set containing Seq values (by reference identity)
--FILE--
<?php
$s1 = new Ds\Seq([1, 2]);
$s2 = new Ds\Seq([3, 4]);
$set = new Ds\Set([$s1, $s2]);

echo "set count: " . $set->count() . "\n";

// Seq objects are unique by identity in a Set
$set->add($s1); // duplicate, should not increase count
echo "after re-add: " . $set->count() . "\n";

// A different Seq with same values is a different object
$s3 = new Ds\Seq([1, 2]);
$set->add($s3);
echo "after add s3: " . $set->count() . "\n";
?>
--EXPECT--
set count: 2
after re-add: 2
after add s3: 3
