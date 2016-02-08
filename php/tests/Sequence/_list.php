<?php
namespace Ds\Tests\Sequence;

trait _list
{
    public function testList()
    {
        $instance = $this->getInstance(['a', 'b', 'c']);
        list($a, $b, $c) = $instance;

        $this->assertEquals('a', $a);
        $this->assertEquals('b', $b);
        $this->assertEquals('c', $c);
    }
}
