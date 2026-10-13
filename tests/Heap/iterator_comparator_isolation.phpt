--TEST--
Heap: nested and interleaved iterators use their own comparators
--FILE--
<?php
$min = new Ds\Heap([1, 2, 3, 4, 5], fn($a, $b) => $b <=> $a);
$max = new Ds\Heap([1, 2, 3, 4, 5], fn($a, $b) => $a <=> $b);

$outer = [];
$inner = [];
foreach ($min as $value) {
    $outer[] = $value;
    $row = [];
    foreach ($max as $other) {
        $row[] = $other;
    }
    $inner[] = $row;
}
echo json_encode($outer), "\n";
echo json_encode($inner), "\n";

$a = $min->getIterator();
$b = $max->getIterator();
$result = [];
while ($a->valid() && $b->valid()) {
    $result[] = [$a->current(), $b->current()];
    $a->next();
    $b->next();
}
echo json_encode($result), "\n";

// Creating an iterator inside another collection's comparator must not replace it.
$sequence = new Ds\Seq([3, 5, 1, 4, 2]);
$sequence->sort(function ($a, $b) use ($max) {
    $iterator = $max->getIterator();
    return $b <=> $a;
});
echo json_encode($sequence->toArray()), "\n";
?>
--EXPECT--
[1,2,3,4,5]
[[5,4,3,2,1],[5,4,3,2,1],[5,4,3,2,1],[5,4,3,2,1],[5,4,3,2,1]]
[[1,5],[2,4],[3,3],[4,2],[5,1]]
[5,4,3,2,1]
