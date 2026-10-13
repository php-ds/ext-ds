--TEST--
Heap: iterator rewind restores the original snapshot
--FILE--
<?php
$heap = new Ds\Heap([1, 3, 2]);
$iterator = $heap->getIterator();

// Mutation before the first rewind must not change the iterator's snapshot.
$heap->clear();
$heap->push(99);

$iterator->rewind();
echo $iterator->key(), ':', $iterator->current(), "\n";
$iterator->rewind();
echo $iterator->key(), ':', $iterator->current(), "\n";
$iterator->next();
echo $iterator->key(), ':', $iterator->current(), "\n";
$iterator->rewind();
echo $iterator->key(), ':', $iterator->current(), "\n";

// foreach rewinds the same iterator, including after it has been exhausted.
echo json_encode(iterator_to_array($iterator)), "\n";
echo json_encode(iterator_to_array($iterator)), "\n";
echo json_encode($heap->toArray()), "\n";

// Rewinding must preserve the custom comparator's ordering too.
$heap = new Ds\Heap([3, 1, 2], fn($a, $b) => $b <=> $a);
$iterator = $heap->getIterator();
echo json_encode(iterator_to_array($iterator)), "\n";
echo json_encode(iterator_to_array($iterator)), "\n";

$iterator = (new Ds\Heap())->getIterator();
$iterator->rewind();
$iterator->rewind();
var_dump($iterator->valid());
var_dump($iterator->current(), $iterator->key());
?>
--EXPECT--
0:3
0:3
1:2
0:3
[3,2,1]
[3,2,1]
[99]
[1,2,3]
[1,2,3]
bool(false)
NULL
NULL
