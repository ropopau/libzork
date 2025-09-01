# LIBZORK

Dynamic library that can execute zork-like game in yaml format.

## Quickstart

### Build

```bash
mkdir -p build
cd build
cmake ..
make
```

### Execution

This project includes zorkxplorer, which builds the main program and manages the type of runner to use.
There are two types of runners available:

- Choice Runner – Displays all possible options and waits for you to select one.

- Smart Runner – Waits for you to type a word, then picks the option that best matches the available choices.



```bash
# Example with the choice runner (default) 
./zorkxplorer/zorkxplorer --story ../tests/stories/dynamic/story.yml

# With the smart runner
./zorkxplorer/zorkxplorer --story ../tests/stories/dynamic/story.yml --smart ../tests/synonyms.yml
```
