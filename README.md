# optionator
University team project - pricing options in C++

## Building the Code

You can build the code manually - we provide `CMakeLists.txt` for that. Alternatively 
(and preferably), you would be using `nix` to build the `optionator`. `nix` is 
a powerful tool for producing reproducible build environments declaratively. Please follow 
the [official download instruction](https://nixos.org/download) to install it on your platform.

All necessary dependencies will be installed automatically via `nix`, their versions will be 
consistent and defined in a declarative development environment to avoid the *works-on-my-machine*
effect.

Once `nix` is installed on your system, simply run the following command to build the project:

```{bash}
nix build
```

### Running `optionator`

Use the below snippet to run the project:

```{bash}
nix run
```

