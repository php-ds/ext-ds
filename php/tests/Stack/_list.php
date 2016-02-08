<?php
namespace Ds\Tests\Stack;

trait _list
{
    public function testList()
    {
        $instance = $this->getInstance(['a', 'b', 'c']);
        $this->expectListNotSupportedException();
        list($a, $b, $c) = $instance;
    }
}
