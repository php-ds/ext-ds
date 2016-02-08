<?php
namespace Ds\Tests\Pair;

trait __set
{
    public function testPropertySetKey()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        $pair->key = 'b';
        $this->assertEquals('a', $pair->key);
    }

    public function testPropertySetValue()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        $pair->value = 2;
        $this->assertEquals(1, $pair->value);
    }

    public function testIndexSetKey()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        $pair[0] = 'b';
        $this->assertEquals('a', $pair[0]);
    }

    public function testIndexSetValue()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        $pair[1] = 2;
        $this->assertEquals(1, $pair[1]);
    }

    public function testIndexSetKeyString()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        $pair['key'] = 'b';
        $this->assertEquals('a', $pair['key']);
    }

    public function testIndexSetValueString()
    {
        $pair = $this->getPair('a', 1);
        $this->expectImmutableException();
        $pair['value'] = 2;
        $this->assertEquals(1, $pair['value']);
    }
}
