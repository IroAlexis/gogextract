# GOG Extract

The [project] is developped by [@Yepoleb]. I'm rewriting it in the C language with the objective of not having memory leaks.

Script for unpacking GOG Linux installers (don't compatible with DLCs).

Explanation of how it works is in his [blog post].

## Building

Just run `make`

## Usage

`./gogextract <input file>`

Output files will be named `unpacker.sh`, `mojosetup.tar.gz` and `data.zip`.

### Game standalone extraction
Here is the first draft for game standalone extraction :
* `unzip data.zip "data/noarch/*" -d /tmp/` the initial extraction is done in RAM (by default)
* `mv /tmp/data/noarch/* /path/to/game` we move the files that are interessing in the specified place

We take into account that one has to be careful with SSDs.

## Roadmap
* Add header(s) file(s)
* Divide the file (for improved maintainability)
* Bad behavior with the name of some games
* Game standalone extraction
* DLCs Support
* Use Meson build system

## License

Available in [LICENSE](LICENSE) file

[@Yepoleb]: https://github.com/Yepoleb
[project]: https://github.com/Yepoleb/gogextract 
[blog post]: https://yepoleb.github.io/blog/2016/10/09/how-the-gog-linux-installers-work/

