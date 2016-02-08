<?php
namespace Ds\Tests\Stack;

trait isEmpty
{
    public function isEmptyDataProvider()
    {
        // values, is empty
        return [
            [[],         true],
            [['a'],     false],
        ];
    }

    /**
     * @dataProvider isEmptyDataProvider
     */
    public function testIsEmpty(array $values, bool $isEmpty)
    {
        $instance = $this->getInstance($values);
        $this->assertEquals($isEmpty, $instance->isEmpty());
    }

    public function testIsNotEmptyAfterPop()
    {
        $instance = $this->getInstance();
        $this->assertTrue($instance->isEmpty());

        $instance->push('a');
        $this->assertFalse($instance->isEmpty());

        $instance->pop();
        $this->assertTrue($instance->isEmpty());
    }
}
