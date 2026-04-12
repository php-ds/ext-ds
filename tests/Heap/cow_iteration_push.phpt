--TEST--
Heap: push during iteration — iterator uses its own clone
--FILE--
<?php
$heap = new Ds\Heap();
$heap->push(3, 1, 2);

$values = [];
foreach ($heap as $v) {
    $values[] = $v;
    if ($v === 3) {
        $heap->push(99);
    }
}

// Destructive iteration pops in order from a clone
echo "Iterator saw: " . implode(",", $values) . "\n";
echo "Heap count: " . $heap->count() . "\n";
echo "Heap peek: " . $heap->peek() . "\n";
?>
--EXPECT--
Iterator saw: 3,2,1
Heap count: 4
Heap peek: 99
