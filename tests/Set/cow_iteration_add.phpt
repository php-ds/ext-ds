--TEST--
Set: add during iteration does not affect iterator
--FILE--
<?php
$set = new Ds\Set([1, 2, 3]);
$values = [];

foreach ($set as $v) {
    $values[] = $v;
    if ($v === 1) {
        $set->add(99, 100);
    }
}

echo "Iterator saw: " . implode(",", $values) . "\n";
echo "Set count: " . $set->count() . "\n";
echo "Set contains 99: " . var_export($set->contains(99), true) . "\n";
?>
--EXPECT--
Iterator saw: 1,2,3
Set count: 5
Set contains 99: true
