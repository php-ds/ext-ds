--TEST--
Heap::copy - creates independent copy via COW
--FILE--
<?php
$h = new Ds\Heap([5, 3, 8, 1, 9]);

// Copy creates a new heap
$c = $h->copy();
echo "original count: " . $h->count() . "\n";
echo "copy count: " . $c->count() . "\n";
echo "original peek: " . $h->peek() . "\n";
echo "copy peek: " . $c->peek() . "\n";

// Modifying copy does not affect original
$c->push(100);
echo "after push to copy:\n";
echo "  original count: " . $h->count() . "\n";
echo "  copy count: " . $c->count() . "\n";
echo "  original peek: " . $h->peek() . "\n";
echo "  copy peek: " . $c->peek() . "\n";

// Pop from original does not affect copy
$h->pop();
echo "after pop from original:\n";
echo "  original count: " . $h->count() . "\n";
echo "  copy count: " . $c->count() . "\n";

// Copy of empty heap
$e = new Ds\Heap();
$ec = $e->copy();
echo "empty copy count: " . $ec->count() . "\n";
echo "empty copy isEmpty: " . var_export($ec->isEmpty(), true) . "\n";

// Copy preserves comparator (min-heap)
$min = new Ds\Heap([3, 1, 4], fn($a, $b) => $b <=> $a);
$minCopy = $min->copy();
echo "min-heap copy peek: " . $minCopy->peek() . "\n";
$minCopy->push(0);
echo "min-heap copy peek after push(0): " . $minCopy->peek() . "\n";

// Multiple copies are independent
$a = new Ds\Heap([10, 20, 30]);
$b = $a->copy();
$c2 = $a->copy();
$b->push(50);
$c2->clear();
echo "multi-copy original: " . $a->count() . "\n";
echo "multi-copy b: " . $b->count() . "\n";
echo "multi-copy c: " . $c2->count() . "\n";
?>
--EXPECT--
original count: 5
copy count: 5
original peek: 9
copy peek: 9
after push to copy:
  original count: 5
  copy count: 6
  original peek: 9
  copy peek: 100
after pop from original:
  original count: 4
  copy count: 6
empty copy count: 0
empty copy isEmpty: true
min-heap copy peek: 1
min-heap copy peek after push(0): 0
multi-copy original: 3
multi-copy b: 4
multi-copy c: 0
