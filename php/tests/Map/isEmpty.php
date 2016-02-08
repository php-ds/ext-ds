<?php
namespace Ds\Tests\Map;

trait isEmpty
{
    public function isEmptyDataProvider()
    {
        // values, is empty
        return [
            [[],              true],
            [['a' => 1],     false],
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

        $instance->put('a', 1);
        $this->assertFalse($instance->isEmpty());

        $instance->remove('a');
        $this->assertTrue($instance->isEmpty());
    }
}
