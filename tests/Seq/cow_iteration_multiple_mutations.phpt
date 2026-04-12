--TEST--
Seq: multiple mutations during single iteration — iterator stable
--FILE--
<?php
$s = new Ds\Seq([1, 2, 3, 4, 5]);
$result = [];
foreach ($s as $v) {
    $result[] = $v;
    // Mutate on every iteration
    $s->push($v * 10);
}
echo "iterated: " . implode(",", $result) . "\n";
echo "final count: " . $s->count() . "\n";
echo "final: " . implode(",", $s->toArray()) . "\n";
?>
--EXPECT--
iterated: 1,2,3,4,5
final count: 10
final: 1,2,3,4,5,10,20,30,40,50
