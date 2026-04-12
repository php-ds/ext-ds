--TEST--
Seq: clear during iteration does not affect iterator
--FILE--
<?php
$seq = new Ds\Seq([1, 2, 3]);
$values = [];

foreach ($seq as $v) {
    $values[] = $v;
    if ($v === 1) {
        $seq->clear();
    }
}

echo "Iterator saw: " . implode(",", $values) . "\n";
echo "Seq count after: " . $seq->count() . "\n";
?>
--EXPECT--
Iterator saw: 1,2,3
Seq count after: 0
