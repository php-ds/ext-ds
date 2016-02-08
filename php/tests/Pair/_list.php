<?php
namespace Ds\Tests\Pair;

trait _list
{
    public function testList()
    {
        $instance = $this->getPair('a', 1);
        list($key, $value) = $instance;

        $this->assertEquals('a', $key);
        $this->assertEquals( 1,  $value);
    }
}
