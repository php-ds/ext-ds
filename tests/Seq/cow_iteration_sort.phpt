--TEST--
Seq: sort during iteration does not affect iterator
--FILE--
<?php
$seq = new Ds\Seq([3, 1, 2]);
$values = [];

foreach ($seq as $v) {
    $values[] = $v;
    if ($v === 3) {
        $seq->sort();
    }
}

echo "Iterator saw: " . implode(",", $values) . "\n";
echo "Seq after: " . implode(",", $seq->toArray()) . "\n";
?>
--EXPECT--
Iterator saw: 3,1,2
Seq after: 1,2,3
