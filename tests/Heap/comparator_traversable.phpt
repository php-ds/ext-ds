--TEST--
Heap: traversable input may operate on heaps with different comparators
--FILE--
<?php
function values(): Generator
{
    $other = new Ds\Heap([10, 20], fn($a, $b) => $a <=> $b);
    foreach ([3, 5, 1, 4, 2] as $value) {
        $other->toArray();
        yield $value;
    }
}

$min = fn($a, $b) => $b <=> $a;
echo json_encode((new Ds\Heap(values(), $min))->toArray()), "\n";
echo json_encode(Ds\heap(values(), $min)->toArray()), "\n";
?>
--EXPECT--
[1,2,3,4,5]
[1,2,3,4,5]
