<?php
namespace Ds\Tests\Map;

trait containsValue
{
    public function containsValueDataProvider()
    {
        // initial, value, has
        return [
            [[],                1,      false],
            [['a' => 1],        1,      true],
            [['a' => 1],        2,      false],
            [['a' => null],     null,   true],
        ];
    }

    /**
     * @dataProvider containsValueDataProvider
     */
    public function testHasValue(array $initial, $value, bool $has)
    {
        $instance = $this->getInstance($initial);
        $this->assertEquals($has, $instance->containsValue($value));
    }

    public function testHasValueAfterRemoveAndPut()
    {
        $instance = $this->getInstance(['a' => 1]);
        $this->assertTrue($instance->containsValue(1));

        $instance->remove('a');
        $this->assertFalse($instance->containsValue(1));

        $instance->put('a', 1);
        $this->assertTrue($instance->containsValue(1));
    }
}
