--TEST--
Heap::serialize/unserialize - round-trip preserves heap property
--FILE--
<?php
// Serialize and unserialize a max-heap
$h = new Ds\Heap([3, 1, 4, 1, 5, 9, 2, 6]);
$s = serialize($h);
$h2 = unserialize($s);

echo "original count: " . $h->count() . "\n";
echo "unserialized count: " . $h2->count() . "\n";

// Verify heap property is preserved
$result = [];
while ($h2->count() > 0) {
    $result[] = $h2->pop();
}
echo "popped: " . implode(",", $result) . "\n";

// Original is not affected
echo "original still has: " . $h->count() . "\n";

// Serialize empty heap
$empty = new Ds\Heap();
$es = serialize($empty);
$empty2 = unserialize($es);
echo "empty count: " . $empty2->count() . "\n";
echo "empty isEmpty: " . var_export($empty2->isEmpty(), true) . "\n";

// Serialize single element
$single = new Ds\Heap([42]);
$s2 = serialize($single);
$single2 = unserialize($s2);
echo "single peek: " . $single2->peek() . "\n";

// Serialize heap with strings
$strHeap = new Ds\Heap(["cherry", "apple", "banana"]);
$s3 = serialize($strHeap);
$strHeap2 = unserialize($s3);
echo "string peek: " . $strHeap2->peek() . "\n";

// Unserialized heap can be modified
$h3 = new Ds\Heap([1, 2, 3]);
$h4 = unserialize(serialize($h3));
$h4->push(100);
echo "modified unserialized peek: " . $h4->peek() . "\n";
echo "original unchanged peek: " . $h3->peek() . "\n";
?>
--EXPECT--
original count: 8
unserialized count: 8
popped: 9,6,5,4,3,2,1,1
original still has: 8
empty count: 0
empty isEmpty: true
single peek: 42
string peek: cherry
modified unserialized peek: 100
original unchanged peek: 3
