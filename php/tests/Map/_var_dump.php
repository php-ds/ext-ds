<?php
namespace Ds\Tests\Map;

trait _var_dump
{
    /**
     * @dataProvider pairsDataProvider
     */
    public function testVarDump(array $values, array $expected)
    {
        $instance = $this->getInstance($values);
        $this->assertInstanceDump($expected, $instance);
    }
}
