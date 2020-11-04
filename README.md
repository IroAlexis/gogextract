# GOG Extract

The [project] is developped by [@Yepoleb]. I'm rewriting it in the C language with the objective of not having memory leaks.

Script for unpacking GOG Linux installers (don't compatible with DLCs).

Explanation of how it works is in his [blog post].

## Building GOG Extract
### Requirements:
* [libzip] for the game standalone extraction

### Building:
Inside the GOG Extract directory, run: `make` :frog:

## Usage

`./gogextract <input file>`

Output files will be named `unpacker.sh`, `mojosetup.tar.gz` and `data.zip`.

## Roadmap
* Game standalone extraction
* Implement arguments
* DLCs Support
* ArchLinux installation
* Use Meson build system
* Bad behavior with the name of some games

## License

Available in [LICENSE](LICENSE) file

[@Yepoleb]: https://github.com/Yepoleb
[project]: https://github.com/Yepoleb/gogextract 
[blog post]: https://yepoleb.github.io/blog/2016/10/09/how-the-gog-linux-installers-work/
[libzip]: https://libzip.org/

