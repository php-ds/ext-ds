<?php
namespace Ds\Tests\Queue;

trait _empty
{
    public function testArrayAccessEmpty()
    {
        $set = $this->getInstance();
        $this->expectArrayAccessUnsupportedException();
        empty($set['a']);
    }
}
