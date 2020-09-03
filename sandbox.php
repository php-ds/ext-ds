<?php

$vector = new \Ds\Vector();
$vector->push(new DateTime());

$sequence = $vector->map(function (DateTime $_dateTime) { return $_dateTime->format(DateTime::ISO8601);});

$sequence->push(new DateTime());
$sequence->push(new DateTime());
	
$sequence->push(new DateTime());
$sequence->push(new DateTime());

$sequence->push(new DateTime());
$sequence->push(new DateTime());
	
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());
$sequence->push(new DateTime());

var_dump($sequence);

echo 'done';