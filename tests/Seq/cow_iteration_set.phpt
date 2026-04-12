--TEST--
Seq: set during iteration does not affect iterator
--FILE--
<?php
$seq = new Ds\Seq([1, 2, 3]);
$values = [];

foreach ($seq as $v) {
    $values[] = $v;
    if ($v === 1) {
        $seq->set(1, 99);
        $seq->set(2, 88);
    }
}

echo "Iterator saw: " . implode(",", $values) . "\n";
echo "Seq after: " . implode(",", $seq->toArray()) . "\n";
?>
--EXPECT--
Iterator saw: 1,2,3
Seq after: 1,99,88
