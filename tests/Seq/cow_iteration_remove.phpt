--TEST--
Seq: remove during iteration does not affect iterator
--FILE--
<?php
$seq = new Ds\Seq([1, 2, 3, 4, 5]);
$values = [];

foreach ($seq as $v) {
    $values[] = $v;
    if ($v === 2) {
        $seq->remove(0); // removes 1
    }
}

echo "Iterator saw: " . implode(",", $values) . "\n";
echo "Seq after: " . implode(",", $seq->toArray()) . "\n";
?>
--EXPECT--
Iterator saw: 1,2,3,4,5
Seq after: 2,3,4,5
