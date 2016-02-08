<?php
namespace Ds\Tests\Map;

trait containsKey
{
    public function containsKeyDataProvider()
    {
        // initial, key, has
        return [
            [[],            'a',    false],
            [['a' => 1],    'a',    true],
            [['a' => 1],    'b',    false],
        ];
    }

    /**
     * @dataProvider containsKeyDataProvider
     */
    public function testHasKey(array $initial, $key, bool $has)
    {
        $instance = $this->getInstance($initial);
        $this->assertEquals($has, $instance->containsKey($key));
    }

    public function testHasKeyAfterRemoveAndPut()
    {
        $instance = $this->getInstance(['a' => 1]);
        $this->assertTrue($instance->containsKey('a'));

        $instance->remove('a');
        $this->assertFalse($instance->containsKey('a'));

        $instance->put('a', 1);
        $this->assertTrue($instance->containsKey('a'));
    }
}
