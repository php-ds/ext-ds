<?php
namespace Ds\Tests\Pair;

trait _var_dump
{
    public function testVarDump()
    {
        $instance = $this->getPair('a', 1);
        $expected = [
            'key'   => 'a',
            'value' =>  1
        ];

        $this->assertInstanceDump($expected, $instance);
    }
}
