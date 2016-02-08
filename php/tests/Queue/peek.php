<?php
namespace Ds\Tests\Queue;

trait peek
{
    public function peekDataProvider()
    {
        // initial, returned, expected result
        return [
            [['a'],         'a'],
            [['a', 'b'],    'a'],
        ];
    }

    /**
     * @dataProvider peekDataProvider
     */
    public function testPeek($initial, $returned)
    {
        $instance = $this->getInstance($initial);

        $value = $instance->peek();

        $this->assertToArray($initial, $instance);
        $this->assertEquals($returned, $value);
    }

    public function testPeekNotAllowedWhenEmpty()
    {
        $instance = $this->getInstance();
        $this->expectEmptyNotAllowedException();
        $instance->peek();
    }
}
