<?php
namespace Ds\Tests\Set;

trait _var_dump
{
    /**
     * @dataProvider basicDataProvider
     */
    public function testVarDump(array $values, array $expected)
    {
        $instance = $this->getInstance($values);
        $this->assertInstanceDump($expected, $instance);
    }
}
