--TEST--
Heap: custom comparator with objects and strings
--FILE--
<?php
// Compare objects by a property (priority queue style)
class Task {
    public function __construct(
        public string $name,
        public int $priority
    ) {}
}

$h = new Ds\Heap([], fn(Task $a, Task $b) => $a->priority <=> $b->priority);
$h->push(new Task("low", 1));
$h->push(new Task("high", 10));
$h->push(new Task("medium", 5));
$h->push(new Task("critical", 20));
$h->push(new Task("normal", 3));

$result = [];
while ($h->count() > 0) {
    $task = $h->pop();
    $result[] = $task->name . "(" . $task->priority . ")";
}
echo "by priority: " . implode(", ", $result) . "\n";

// Compare strings by length
$h2 = new Ds\Heap([], fn($a, $b) => strlen($a) <=> strlen($b));
$h2->push("a", "hello", "hi", "hey", "wonderful");
$result2 = [];
while ($h2->count() > 0) {
    $result2[] = $h2->pop();
}
echo "by length: " . implode(", ", $result2) . "\n";

// Reverse string comparison (Z before A)
$h3 = new Ds\Heap([], fn($a, $b) => $b <=> $a);
$h3->push("alpha", "gamma", "beta", "delta");
$result3 = [];
while ($h3->count() > 0) {
    $result3[] = $h3->pop();
}
echo "reverse alpha: " . implode(", ", $result3) . "\n";

// Compare by absolute value
$h4 = new Ds\Heap([], fn($a, $b) => abs($a) <=> abs($b));
$h4->push(-10, 3, -5, 1, 8);
$result4 = [];
while ($h4->count() > 0) {
    $result4[] = $h4->pop();
}
echo "by abs value: " . implode(", ", $result4) . "\n";

// Compare arrays by sum
$h5 = new Ds\Heap([], fn($a, $b) => array_sum($a) <=> array_sum($b));
$h5->push([1, 2], [10, 20], [5, 5]);
$result5 = [];
while ($h5->count() > 0) {
    $arr = $h5->pop();
    $result5[] = "[" . implode(",", $arr) . "]=" . array_sum($arr);
}
echo "by array sum: " . implode(", ", $result5) . "\n";
?>
--EXPECT--
by priority: critical(20), high(10), medium(5), normal(3), low(1)
by length: wonderful, hello, hey, hi, a
reverse alpha: alpha, beta, delta, gamma
by abs value: -10, 8, -5, 3, 1
by array sum: [10,20]=30, [5,5]=10, [1,2]=3
