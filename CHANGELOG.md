# Change Log
All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## [1.0.4] - 2016-08-01
### Fixed
- `unserialize` memory leak when failed to unserialize.
- `htable` bucket copy macro didn't copy the bucket's "next"

### Fixed
- ds_htable_put_next bucket rehash fix (see docs #4)

## [1.0.3] - 2016-08-01
### Added
- `Set::merge`

### Fixed
- ds_htable_put_next bucket rehash fix (see docs #4)
- 
## [1.0.2] - 2016-07-31
### Added
- `Map::putAll`
