<?php
namespace Ds\Tests\Pair;

trait _unset
{
    public function testArrayAccessUnsetKey()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        unset($pair['key']);
        $this->assertEquals('a', $pair->key);
    }

    public function testArrayAccessUnsetValue()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        unset($pair['value']);
        $this->assertEquals(1, $pair->value);
    }

    public function testArrayAccessUnsetKeyUsingIndex()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        unset($pair[0]);
        $this->assertEquals('a', $pair->key);
    }

    public function testArrayAccessUnsetValueUsingIndex()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        unset($pair[1]);
        $this->assertEquals(1, $pair->value);
    }

    public function testPropertyUnsetKey()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        unset($pair->key);
        $this->assertEquals('a', $pair->key);
    }

    public function testPropertyUnsetValue()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        unset($pair->value);
        $this->assertEquals(1, $pair->value);
    }
}
