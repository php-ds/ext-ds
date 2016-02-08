<?php
namespace Ds\Tests\Deque;

trait insert
{
    /**
     * Deque has a few edge cases that don't exist for other sequences. These
     * occur when the head of the deque wraps around, ie. h > t.
     */
    public function testInsertExtended()
    {
        $instance = $this->getInstance();

        // The head of the deque will wrap around if all items are unshifted.

        $instance->unshift('c'); // [_, ..., _, _, c] tail = 0, head = 3
        $instance->unshift('b'); // [_, ..., _, b, c] tail = 0, head = 2
        $instance->unshift('a'); // [_, ..., a, b, c] tail = 0, head = 1

        $instance->insert(1, ...['x']); // [_, _, _, _, a, x, b, c]
        $this->assertToArray(['a', 'x', 'b', 'c'], $instance);

        $instance->insert(0, ...['y']); // [_, _, _, y, a, x, b, c]
        $this->assertToArray(['y', 'a', 'x', 'b', 'c'], $instance);

        $instance->insert(4, ...['z']); // [_, _, _, y, a, x, b, z, c]
        $this->assertToArray(['y', 'a', 'x', 'b', 'z', 'c'], $instance);

        $instance->insert(6, ...['#']); // [_, _, y, a, x, b, z, c, #]
        $this->assertToArray(['y', 'a', 'x', 'b', 'z', 'c', '#'], $instance);
    }

    public function testInsertAtBoundaryWithMoreOnTheLeft()
    {
        $instance = $this->getInstance();
        $instance->push(3, 4, 5, 6);
        $instance->unshift(1, 2);

        // [3, 4, 5, 6, _, _, 1, 2]
        //
        // Inserting at index 2 should determine that the right partition
        // should be moved to the left instead of the left to the right.
        $instance->insert(2, 'x');

        $this->assertToArray([1, 2, 'x', 3, 4, 5, 6], $instance);
    }

    public function testInsertAtBoundaryWithMoreOnTheRight()
    {
        $instance = $this->getInstance();
        $instance->push(5, 6);
        $instance->unshift(1, 2, 3, 4);

        // [5, 6, _, _, 1, 2, 3, 4]
        //
        // Inserting at index 4 should determine that the left partition
        // should be moved to the right instead of the right to the left.
        $instance->insert(4, 'x');

        $this->assertToArray([1, 2, 3, 4, 'x', 5, 6], $instance);
    }

    public function testInsertAtBoundaryWithEqualOnBothSides()
    {
        $instance = $this->getInstance();
        $instance->push(4, 5, 6);
        $instance->unshift(1, 2, 3);

        // [4, 5, 6, _, _, 1, 2, 3]
        //
        // Inserting at index 3 should choose to move either partition.
        $instance->insert(3, 'x');

        $this->assertToArray([1, 2, 3, 'x', 4, 5, 6], $instance);
    }
}
