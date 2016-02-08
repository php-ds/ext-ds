<?php
namespace Ds\Tests\Pair;

trait __get
{
    public function testArrayAccessKey()
    {
        $pair = $this->getPair('a', 1);

        $this->assertEquals('a', $pair['key']);
        $this->assertEquals('a', $pair[0]);
    }

    public function testArrayAccessValue()
    {
        $pair = $this->getPair('a', 1);

        $this->assertEquals(1, $pair['value']);
        $this->assertEquals(1, $pair[1]);
    }

    public function testPropertyAccess()
    {
        $pair = $this->getPair('a', 1);

        $this->assertEquals('a', $pair->key);
        $this->assertEquals( 1,  $pair->value);
    }

    public function testIncorrectPropertyAccess()
    {
        $this->expectPropertyDoesNotExistException();
        $this->getPair('a', 1)->foo;
    }
}
