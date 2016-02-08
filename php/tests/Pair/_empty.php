<?php
namespace Ds\Tests\Pair;

trait _empty
{
    public function testArrayAccessEmptyKey()
    {
        $pair = $this->getPair('a', 1);
        $this->assertFalse(empty($pair['key']));
        $this->assertTrue (empty($pair['?']));

        $pair = $this->getPair(false, 1);
        $this->assertTrue(empty($pair['key']));
    }

    public function testArrayAccessEmptyValue()
    {
        $pair = $this->getPair('a', 1);
        $this->assertFalse(empty($pair['value']));
        $this->assertTrue (empty($pair['?']));

        $pair = $this->getPair('a', false);
        $this->assertTrue(empty($pair['value']));
    }

    public function testArrayAccessEmptyKeyUsingIndex()
    {
        $pair = $this->getPair('a', 1);
        $this->assertFalse(empty($pair[0]));

        $pair = $this->getPair(false, 1);
        $this->assertTrue(empty($pair[0]));
    }

    public function testArrayAccessEmptyValueUsingIndex()
    {
        $pair = $this->getPair('a', 1);
        $this->assertFalse(empty($pair[1]));

        $pair = $this->getPair('a', false);
        $this->assertTrue(empty($pair[1]));
    }

    public function testPropertyEmptyKey()
    {
        $pair = $this->getPair('a', 1);
        $this->assertFalse(empty($pair->key));
        $this->assertTrue (empty($pair->nope));

        $pair = $this->getPair(false, 1);
        $this->assertTrue (empty($pair->key));
    }

    public function testPropertyEmptyValue()
    {
        $pair = $this->getPair('a', 1);
        $this->assertFalse(empty($pair->value));
        $this->assertTrue (empty($pair->nope));

        $pair = $this->getPair('a', false);
        $this->assertTrue (empty($pair->value));
    }
}
