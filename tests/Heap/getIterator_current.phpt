--TEST--
Heap: iterator owns its current value after the source heap is cleared
--FILE--
<?php
$heap = new Ds\Heap([(object) ['value' => 1]]);
$iterator = $heap->getIterator();
$heap->clear();
unset($heap);

// Releasing a current() result must not free the iterator's current value.
echo json_encode($iterator->current()), "\n";
echo json_encode($iterator->current()), "\n";

// A returned value must also survive destruction of the iterator.
$value = $iterator->current();
$reference = WeakReference::create($value);
unset($iterator);
echo json_encode($value), "\n";
unset($value);
var_dump($reference->get());

// Advancing and destroying the iterator must release all owned values.
$values = [(object) ['value' => 1], (object) ['value' => 2]];
$references = array_map(WeakReference::create(...), $values);
$heap = new Ds\Heap($values);
$iterator = $heap->getIterator();
unset($values);
$heap->clear();
while ($iterator->valid()) {
    echo json_encode($iterator->current()), "\n";
    $iterator->next();
}
var_dump($iterator->current(), $iterator->key());
unset($iterator);
foreach ($references as $reference) {
    var_dump($reference->get());
}

// An iterator can be destroyed before it has ever been rewound.
$heap = new Ds\Heap([new stdClass()]);
$reference = WeakReference::create($heap->peek());
$iterator = $heap->getIterator();
unset($heap, $iterator);
var_dump($reference->get());
?>
--EXPECT--
{"value":1}
{"value":1}
{"value":1}
NULL
{"value":2}
{"value":1}
NULL
NULL
NULL
NULL
NULL
