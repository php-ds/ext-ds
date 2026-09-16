--TEST--
Set: getIterator returns an independent InternalIterator
--FILE--
<?php
$collection = new Ds\Set([10, 20, 30]);
$expected = iterator_to_array($collection);
$iterator = $collection->getIterator();
var_dump($iterator instanceof InternalIterator);
var_dump($iterator instanceof Iterator);
var_dump($iterator !== $collection);

$other = $collection->getIterator();
var_dump($iterator !== $other);
$iterator->rewind();
$other->rewind();
$iterator->next();
var_dump($other->key() === array_key_first($expected));
var_dump($other->current() === reset($expected));

$result = [];
while ($other->valid()) {
    $result[$other->key()] = $other->current();
    $other->next();
}
var_dump($result === $expected);
var_dump(iterator_to_array($collection->getIterator()) === $expected);
var_dump($collection->count() === 3);

$empty = (new Ds\Set())->getIterator();
$empty->rewind();
var_dump($empty->valid());
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(false)
