<?php
namespace Ds\Tests\Sequence;

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
