<?php
namespace Ds\Tests\Pair;

trait _isset
{
    public function testArrayAccessIssetKeyString()
    {
        $pair = $this->getPair('a', 1);
        $this->assertTrue (isset($pair['key']));
        $this->assertFalse(isset($pair['?']));

        $pair = $this->getPair(null, 1);
        $this->assertFalse(isset($pair['key']));
    }

    public function testArrayAccessIssetValueString()
    {
        $pair = $this->getPair('a', 1);
        $this->assertTrue (isset($pair['value']));
        $this->assertFalse(isset($pair['?']));

        $pair = $this->getPair('a', null);
        $this->assertFalse(isset($pair['value']));
    }

    public function testArrayAccessIssetKey()
    {
        $pair = $this->getPair('a', 1);
        $this->assertTrue(isset($pair[0]));

        $pair = $this->getPair(null, 1);
        $this->assertFalse(isset($pair[0]));
    }

    public function testArrayAccessIssetValue()
    {
        $pair = $this->getPair('a', 1);
        $this->assertTrue(isset($pair[1]));

        $pair = $this->getPair('a', null);
        $this->assertFalse(isset($pair[1]));
    }

    public function testPropertyIssetKey()
    {
        $pair = $this->getPair('a', 1);
        $this->assertTrue (isset($pair->key));
        $this->assertFalse(isset($pair->nope));

        $pair = $this->getPair(null, 1);
        $this->assertFalse (isset($pair->key));
    }

    public function testPropertyIssetValue()
    {
        $pair = $this->getPair('a', 1);
        $this->assertTrue (isset($pair->value));
        $this->assertFalse(isset($pair->nope));

        $pair = $this->getPair('a', null);
        $this->assertFalse (isset($pair->value));
    }

    public function testPropertyExists()
    {
        $pair = $this->getPair('a', 1);
        $this->assertTrue(property_exists($pair, 'key'));
        $this->assertTrue(property_exists($pair, 'value'));

        $this->assertFalse(property_exists($pair, 'foo'));
        $this->assertFalse(property_exists($pair, 'bar'));

        $this->assertFalse(property_exists($pair,  0));
        $this->assertFalse(property_exists($pair,  1));
        $this->assertFalse(property_exists($pair, '0'));
        $this->assertFalse(property_exists($pair, '1'));
    }
}
