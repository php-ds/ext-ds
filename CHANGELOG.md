# Change Log
All notable changes to this project will be documented in this file.
This project follows [Semantic Versioning](http://semver.org/).

## [1.2.6] - 2018-05-24
### Fixed
- Fixed not clearing memory after buffer reallocation. #114

## [1.2.5] - 2018-03-13
### Fixed
- Buffer outflow during deserialization of objects. #111

## [1.2.4] - 2017-11-29
### Fixed
- Empty `PriorityQueue` causing segfault on `gc_collect_cycles`. #106

## [1.2.3] - 2017-08-16
### Fixed
- Memory allocation bug on ppc64. @remicollet #88

## [1.2.2] - 2017-08-08
### Fixed
- Segfault in ds_htable_lookup_bucket_by_hash. @gnoddep #86

## [1.2.1] - 2017-08-03
### Changed
- Minor capacity adjustments.

## [1.2.0] - 2017-07-22
### Changed
- Vector's minimum and default capacity down from 10 to 8.
- Map and Set's minimum and default capacity down from 16 to 8.
- Hash function of arrays is now the length of the array, so O(1).

## [1.1.10] - 2017-06-22
### Fixed
- Using a key as reference not working correctly with array access. #86

## [1.1.9] - 2017-04-26
### Fixed
- Iterating over an implicit iterator (not variable). #82

## [1.1.8] - 2017-03-24
### Fixed
- PriorityQueue automatic truncate bug. #78
- Bugs related to unserialization. #77

## [1.1.7] - 2017-02-11
### Fixed
- Preliminary support for PHP 7.2

## [1.1.6] - 2016-09-03
### Fixed
- `Map` truncating to capacity less than the minimum.

## [1.1.5] - 2016-09-01
### Fixed
- `phpinfo` header
- `ds_htable_put_distinct` wasn't rehashing the correct bucket pointer (Fixes #53)
- Memory leaks.

## [1.1.4] - 2016-08-09
### Fixed
- `Vector` and `Deque` rotate crashing when empty (mod zero).

## [1.1.3] - 2016-08-08
### Fixed
- Memory leaks during map when callback throws an exception.
- Memory leaks when structures contain themselves.
- Module dependencies.

## Added
- Version info in `phpinfo()`

### Improved
- `Map::map` and `Map::filter` are now slightly faster.

## [1.1.2] - 2016-08-05
### Fixed
- Many, many memory leaks.
- Added memory checks to CI.
- Performance improvements.
- JSON dependency now handled correctly (thanks @nikic).

## [1.1.1] - 2016-08-04
### Fixed
- Multiple memory leaks where objects were not free'd correctly.

## [1.1.0] - 2016-08-04
### Added
- `Pair::copy`

## [1.0.4] - 2016-08-01
### Fixed
- `unserialize` memory leak when failed to unserialize.
- `htable` bucket copy macro didn't copy the bucket's "next".

## [1.0.3] - 2016-08-01
### Added
- `Set::merge`

### Fixed
- ds_htable_put_next bucket rehash fix
-
## [1.0.2] - 2016-07-31
### Added
- `Map::putAll`
