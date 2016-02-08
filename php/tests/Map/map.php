<?php
namespace Ds\Tests\Sequence;

trait map
{
    public function mapDataProvider()
    {
        // values, callback
        return [

            // Test mapping an empty sequence produces an empty sequence.
            [[], function(){}],

            // Test basic mapping where integers are doubled.
            [[1, 2, 3], function ($k, $v) { return $v * 2; }],
        ];
    }


    /**
     * @dataProvider mapDataProvider
     */
    public function testMap(array $values, callable $callback)
    {
        $instance = $this->getInstance($values);

        $mapped = $instance->map($callback);
        $expected = array_map($callback, $values);

        $this->assertToArray($values, $instance);
        $this->assertEquals($expected, $mapped->toArray());
    }

    public function testMapCallbackThrowsException()
    {
        $instance = $this->getInstance([1, 2, 3]);
        $mapped = null;

        try {
            $mapped = $instance->map(function($value) {
                throw new \Exception();
            });
        } catch (\Exception $e) {
            $this->assertToArray([1, 2, 3], $instance);
            $this->assertNull($mapped);
            return;
        }

        $this->fail('Exception should have been caught');
    }

    public function testMapCallbackThrowsExceptionLaterOn()
    {
        $instance = $this->getInstance([1, 2, 3]);
        $mapped = null;

        try {
            $mapped = $instance->map(function($key, $value) {
                if ($value === 3) {
                    throw new \Exception();
                }
            });
        } catch (\Exception $e) {
            $this->assertToArray([1, 2, 3], $instance);
            $this->assertNull($mapped);
            return;
        }

        $this->fail('Exception should have been caught');
    }
}
