<?php
namespace Ds\Tests\Sequence;

trait pop
{
    public function popDataProvider()
    {
        // initial, expected, returned
        return [
            [['a'],         [],     'a'],
            [['a', 'b'],    ['a'],  'b'],
        ];
    }

    /**
     * @dataProvider popDataProvider
     */
    public function testPop($initial, array $expected, $returned)
    {
        $instance = $this->getInstance($initial);

        $result = $instance->pop();

        $this->assertToArray($expected, $instance);
        $this->assertEquals($returned, $result);
        $this->assertEquals(count($initial) - 1, count($instance));

        //
        $instance = $this->getInstance();
        $this->expectEmptyNotAllowedException();
        $instance->pop();
    }

    public function testPopAll()
    {
        $instance = $this->getInstance(range(1, self::MANY));

        while ( ! $instance->isEmpty()) {
            $instance->pop();
        }

        $this->assertEquals(count($instance), 0);
    }
}
