--TEST--
Heap: comparator callbacks cannot advance or rewind the same iterator
--FILE--
<?php
function checkReentry(string $outer, string $inner, bool $catchInside): void
{
    echo "$outer -> $inner, caught ", $catchInside ? 'inside' : 'outside', "\n";
    $iterator = null;
    $attempt = false;
    $heap = new Ds\Heap([1, 2, 3, 4, 5], static function ($a, $b) use (
        &$iterator, &$attempt, $inner, $catchInside
    ) {
        if ($attempt) {
            $attempt = false;
            if ($catchInside) {
                try {
                    $iterator->$inner();
                } catch (RuntimeException $e) {
                    echo $e->getMessage(), "\n";
                }
            } else {
                $iterator->$inner();
            }
        }
        return $a <=> $b;
    });
    $iterator = $heap->getIterator();
    if ($outer === 'next') {
        $iterator->rewind();
    }

    $attempt = true;
    try {
        $iterator->$outer();
    } catch (RuntimeException $e) {
        echo $e->getMessage(), "\n";
    }

    if ($catchInside) {
        // A rejected nested operation must not change the key or skip a value.
        echo $iterator->key(), ':', $iterator->current(), "\n";
        $iterator->next();
        echo $iterator->key(), ':', $iterator->current(), "\n";
    }

    // The guard must be released even when the comparator propagates an exception.
    echo json_encode(iterator_to_array($iterator)), "\n";
    echo json_encode($heap->toArray()), "\n";
    $iterator = null; // Break the reference held by the comparator.
}

foreach (['rewind', 'next'] as $outer) {
    foreach (['rewind', 'next'] as $inner) {
        checkReentry($outer, $inner, true);
        checkReentry($outer, $inner, false);
    }
}
?>
--EXPECT--
rewind -> rewind, caught inside
Cannot modify a heap iterator while it is executing
0:5
1:4
[5,4,3,2,1]
[5,4,3,2,1]
rewind -> rewind, caught outside
Cannot modify a heap iterator while it is executing
[5,4,3,2,1]
[5,4,3,2,1]
rewind -> next, caught inside
Cannot modify a heap iterator while it is executing
0:5
1:4
[5,4,3,2,1]
[5,4,3,2,1]
rewind -> next, caught outside
Cannot modify a heap iterator while it is executing
[5,4,3,2,1]
[5,4,3,2,1]
next -> rewind, caught inside
Cannot modify a heap iterator while it is executing
1:4
2:3
[5,4,3,2,1]
[5,4,3,2,1]
next -> rewind, caught outside
Cannot modify a heap iterator while it is executing
[5,4,3,2,1]
[5,4,3,2,1]
next -> next, caught inside
Cannot modify a heap iterator while it is executing
1:4
2:3
[5,4,3,2,1]
[5,4,3,2,1]
next -> next, caught outside
Cannot modify a heap iterator while it is executing
[5,4,3,2,1]
[5,4,3,2,1]
