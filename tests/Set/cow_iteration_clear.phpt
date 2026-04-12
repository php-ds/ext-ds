--TEST--
Set: clear during iteration does not affect iterator
--FILE--
<?php
$set = new Ds\Set([1, 2, 3]);
$values = [];

foreach ($set as $v) {
    $values[] = $v;
    if ($v === 1) {
        $set->clear();
    }
}

echo "Iterator saw: " . implode(",", $values) . "\n";
echo "Set count: " . $set->count() . "\n";
?>
--EXPECT--
Iterator saw: 1,2,3
Set count: 0
