--TEST--
Heap: iteration with custom min-heap comparator
--FILE--
<?php
$h = new Ds\Heap([3, 1, 4, 1, 5], function($a, $b) { return $b <=> $a; });
$result = [];
foreach ($h as $value) {
    $result[] = $value;
}
echo "iterated: " . implode(",", $result) . "\n";
echo "count after: " . $h->count() . "\n";
?>
--EXPECT--
iterated: 1,1,3,4,5
count after: 5
