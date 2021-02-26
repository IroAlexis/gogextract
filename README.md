# GOG Extract

The [project] is developped by [@Yepoleb] and it was a script for unpacking GOG Linux installers.
Explanation of how it works is in his [blog post].

I'm rewriting it in the C language with the objective of not having memory leaks and adding the new functionnalities

## Building GOG Extract
### Requirements:
* [pkgconf] (*make*)
* [libzip] for the game standalone extraction

### Building:
Inside the GOG Extract directory, run:
```shell
$ make check
$ make
```

## Usage
### Basic usage
`gogextract gog_installer.sh`

Extracts next files `unpacker.sh`, `mojosetup.tar.gz` and `data.zip`. The `data.zip` file contains the game data and others things the GOG Linux installer needs.

### Usage
`gogextract -g /path/to/dir gog_installer.sh`

Extracts only game data needs in `/path/to/dir`. You can just launch `/path/to/dir/start.sh` for enjoy the game.

## Roadmap
* ArchLinux installation
* Bad behavior with the name of some games

## Acknowledgements
* [@Yepoleb], for his authorization and creating the project : gogextract
* @dysliked, for his precious help
* [@mobius], for the libzip [simple example]

## License

Available in [LICENSE](LICENSE) file

[@Yepoleb]: https://github.com/Yepoleb
[project]: https://github.com/Yepoleb/gogextract 
[blog post]: https://yepoleb.github.io/blog/2016/10/09/how-the-gog-linux-installers-work/
[pkgconf]: https://git.sr.ht/~kaniini/pkgconf
[libzip]: https://libzip.org/
[@mobius]: https://github.com/mobius
[simple example]: https://gist.github.com/mobius/1759816

