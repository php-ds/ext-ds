# Change Log
All notable changes to this project will be documented in this file.
This project follows [Semantic Versioning](http://semver.org/).

## [1.1.3] - UNRELEASED
### Fixed
- [x] Memory leaks during map when callback throws an exception.
- [x] Memory leaks when structures contain themselves.

## Added
- [ ] Show version info in `phpinfo()`

### Improvements
- [x] `Map::map` and `Map::filter` are now slightly faster.

## [1.1.2] - 2016-08-05
### Fixed
- Many, many memory leaks.
- Added memory checks to CI.
- Performance improvements.
- JSON dependency now handled correctly (thanks @nikic).

## [1.1.1] - 2016-08-04
### Fixed
- Multiple memory leaks where objects were not free'd correctly. #30

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
