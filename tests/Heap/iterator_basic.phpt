--TEST--
Heap: foreach iteration is destructive (pops from clone)
--FILE--
<?php
$h = new Ds\Heap([3, 1, 4, 1, 5]);
$result = [];
foreach ($h as $value) {
    $result[] = $value;
}
// Iteration yields in max-heap order
echo "iterated: " . implode(",", $result) . "\n";
// Original is NOT consumed (iterator uses a clone)
echo "count after: " . $h->count() . "\n";
echo "peek: " . $h->peek() . "\n";
?>
--EXPECT--
iterated: 5,4,3,1,1
count after: 5
peek: 5
