<?php
namespace Ds\Tests\Deque;

trait remove
{
    /**
     * Deque has a few edge cases that don't exist for other sequences. These
     * occur when the head of the deque wraps around, ie. h > t.
     */
    public function testRemoveExtended()
    {
        $instance = $this->getInstance();

        /* HEAD WRAPPED AROUND, TAIL = 0 */
        // The head of the deque will wrap around if all items are unshifted.

        $instance->unshift('c'); // [_, _, _, _, _, _, _, c] tail = 0, head = 3
        $instance->unshift('b'); // [_, _, _, _, _, _, b, c] tail = 0, head = 2
        $instance->unshift('a'); // [_, _, _, _, _, a, b, c] tail = 0, head = 1

        // The sequence is now a, b, c
        $this->assertEquals('b', $instance->remove(1)); // [..., _, _, a, c]
        $this->assertEquals('a', $instance->remove(0)); // [..., _, _, _, c]
        $this->assertEquals('c', $instance->remove(0)); // [..., _, _, _, _]
        $this->assertTrue($instance->isEmpty());

        /* HEAD WRAPPED AROUND, TAIL > 0 */
        $instance = $this->getInstance();

        $instance->unshift('b'); // [_, _, ..., _, b] tail = 0, head = 3
        $instance->unshift('a'); // [_, _, ..., a, b] tail = 0, head = 2
        $instance->push('c');    // [c, _, ..., a, b] tail = 1, head = 2

        // The sequence is now a, b, c
        $this->assertEquals('b', $instance->remove(1)); // [c, _, ..., _, a]
        $this->assertEquals('a', $instance->remove(0)); // [c, _, ..., _, _]
        $this->assertEquals('c', $instance->remove(0)); // [_, _, ..., _, _]
        $this->assertTrue($instance->isEmpty());

        /* HEAD NOT WRAPPED, TAIL > 0 */
        $instance = $this->getInstance();

        $instance->push('a'); // [a, _, _, ..., _] tail = 1, head = 0
        $instance->push('b'); // [a, b, _, ..., _] tail = 2, head = 0
        $instance->push('c'); // [a, b, c, ..., _] tail = 3, head = 0

        // The sequence is now a, b, c
        $this->assertEquals('b', $instance->remove(1)); // [a, c, ..., _, _]
        $this->assertEquals('a', $instance->remove(0)); // [c, _, ..., _, _]
        $this->assertEquals('c', $instance->remove(0)); // [_, _, ..., _, _]
        $this->assertTrue($instance->isEmpty());
    }
}
