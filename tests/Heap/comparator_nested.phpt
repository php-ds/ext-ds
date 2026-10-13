--TEST--
Heap: nested heap operations preserve each caller's comparator
--FILE--
<?php
$inner = new Ds\Heap([1, 2, 3, 4, 5], fn($a, $b) => $a <=> $b);
$compare = function ($a, $b) use ($inner) {
    $inner->toArray();
    return $b <=> $a;
};
$heap = new Ds\Heap([3, 5, 1, 4, 2], $compare);
echo json_encode($heap->toArray()), "\n";

// A temporary heap's callback must not be retained by the surrounding operation.
$heap = new Ds\Heap([3, 5, 1, 4, 2], function ($a, $b) {
    $temporary = new Ds\Heap([10, 20], fn($x, $y) => $x <=> $y);
    $temporary->toArray();
    return $b <=> $a;
});
echo json_encode($heap->toArray()), "\n";

// An exception caught inside the comparator must not replace its callback state.
$throw = false;
$throwing = new Ds\Heap([1, 2, 3], function ($a, $b) use (&$throw) {
    if ($throw) {
        throw new RuntimeException('nested comparison');
    }
    return $a <=> $b;
});
$throw = true;
$heap = new Ds\Heap([3, 5, 1, 4, 2], function ($a, $b) use ($throwing) {
    try {
        $throwing->toArray();
    } catch (RuntimeException $e) {
    }
    return $b <=> $a;
});
echo json_encode($heap->toArray()), "\n";
$heap->push(7, 0, 6);
echo json_encode($heap), "\n";
echo json_encode($heap->__serialize()), "\n";
$result = [];
while (!$heap->isEmpty()) {
    $result[] = $heap->pop();
}
echo json_encode($result), "\n";

$heap = new Ds\Heap([], $compare);
$heap->__unserialize([3, 5, 1, 4, 2]);
echo json_encode($heap->toArray()), "\n";
echo json_encode(Ds\heap([3, 5, 1, 4, 2], $compare)->toArray()), "\n";

// Heap operations must also leave the comparator of a surrounding sort intact.
$sequence = new Ds\Seq([3, 5, 1, 4, 2]);
$sequence->sort(function ($a, $b) use ($inner) {
    $inner->toArray();
    return $b <=> $a;
});
echo json_encode($sequence->toArray()), "\n";

// Sorts inside a heap comparator must not affect the heap's ordering either.
$heap = new Ds\Heap([3, 5, 1, 4, 2], function ($a, $b) {
    $sequence = new Ds\Seq([3, 1, 2]);
    $sequence->sort(fn($x, $y) => $x <=> $y);
    return $b <=> $a;
});
echo json_encode($heap->toArray()), "\n";
?>
--EXPECT--
[1,2,3,4,5]
[1,2,3,4,5]
[1,2,3,4,5]
[0,1,2,3,4,5,6,7]
[0,1,2,3,4,5,6,7]
[0,1,2,3,4,5,6,7]
[1,2,3,4,5]
[1,2,3,4,5]
[5,4,3,2,1]
[1,2,3,4,5]
