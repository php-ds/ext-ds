<?php
namespace Ds\Tests\Set;

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

    public function testIsNotEmptyAfterRemove()
    {
        $instance = $this->getInstance();
        $this->assertTrue($instance->isEmpty());

        $instance->add('a');
        $this->assertFalse($instance->isEmpty());

        $instance->remove('a');
        $this->assertTrue($instance->isEmpty());
    }
}
