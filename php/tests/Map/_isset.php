<?php
namespace Ds\Tests\Map;

trait _isset
{

    public function issetDataProvider()
    {
        // initial, key, isset
        return [
            [['a' => 0    ], 'a', true ],
            [['a' => 1    ], 'a', true ],
            [['a' => false], 'a', true ],
            [['a' => null ], 'a', false],
            [[            ], 'a', false],
        ];
    }

    /**
     * @dataProvider issetDataProvider
     */
    public function testArrayAccessIsset(array $initial, $key, bool $isset)
    {
        $instance = $this->getInstance();

        foreach ($initial as $key => $value) {
            $instance->put($key, $value);
        }

        $this->assertEquals($isset, isset($instance[$key]));
    }

    /**
     * @dataProvider issetDataProvider
     */
    public function testArrayAccessIssetByReference(array $initial, $key, bool $isset)
    {
        $instance = $this->getInstance([$initial]);
        $this->assertEquals($isset, isset($instance[0][$key]));
    }
}
