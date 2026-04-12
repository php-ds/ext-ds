--TEST--
Seq: unshift/pop/shift on clone does not affect original
--FILE--
<?php
$a = new Ds\Seq([1, 2, 3]);

$b = clone $a;
$b->unshift(0);
echo "After unshift on clone: a=" . implode(",", $a->toArray()) . " b=" . implode(",", $b->toArray()) . "\n";

$c = clone $a;
$c->pop();
echo "After pop on clone: a=" . implode(",", $a->toArray()) . " c=" . implode(",", $c->toArray()) . "\n";

$d = clone $a;
$d->shift();
echo "After shift on clone: a=" . implode(",", $a->toArray()) . " d=" . implode(",", $d->toArray()) . "\n";
?>
--EXPECT--
After unshift on clone: a=1,2,3 b=0,1,2,3
After pop on clone: a=1,2,3 c=1,2
After shift on clone: a=1,2,3 d=2,3
