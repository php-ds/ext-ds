--TEST--
Set: remove during iteration does not affect iterator
--FILE--
<?php
$set = new Ds\Set([1, 2, 3, 4]);
$values = [];

foreach ($set as $v) {
    $values[] = $v;
    if ($v === 1) {
        $set->remove(2, 3);
    }
}

echo "Iterator saw: " . implode(",", $values) . "\n";
echo "Set after: " . implode(",", $set->toArray()) . "\n";
?>
--EXPECT--
Iterator saw: 1,2,3,4
Set after: 1,4
