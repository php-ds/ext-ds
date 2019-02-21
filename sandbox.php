<?php

$a = new Vector();

$a[] = "a";
$a[] = "b";
$a[] = "c";

/**
 * Simple iteration, no writes.
 */
printf("---\n");
foreach ($a as $k => $v) {
    printf("iteration of A: key=%s, val=%s\n", $k, $v);
}

/**
 * Simple iteration, except we write to the object being iterated. The traversal
 * should NOT include the values being added.
 */
printf("---\n");
foreach ($a as $k => $v) {
    printf("iteration of A: key=%s, val=%s\n", $k, $v);
    $a[] = uniqid();
}

/**
 *
 */
printf("---\n");
var_dump($a);
