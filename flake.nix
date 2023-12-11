{
	outputs = { self, nixpkgs }:
	let
		system = "x86_64-linux";
		pkgs = import nixpkgs { inherit system; };
	in
	{
		devShells.${system}.default = pkgs.mkShell {
			buildInputs = with pkgs; [ 
				clang
				cmake
			];
			shellHook = ''
				export CC="${pkgs.clang}/bin/clang"
				export CXX="${pkgs.clang}/bin/clang++"
			'';
		};

		packages.${system}.default = pkgs.stdenv.mkDerivation rec {
			name = "project";
			src = ./.;
			buildInputs = with pkgs; [
				clang
				cmake
			];
			buildPhase = ''
				cmake .
				make
			'';
			installPhase = ''
				mkdir -p $out/bin
				mv project $out/bin/
			'';
			shellHook = ''
				export CC="${pkgs.clang}/bin/clang"
				export CXX="${pkgs.clang}/bin/clang++"
			'';
	  };

		apps.${system}.default = {
			type = "app";
			program = "${self.packages.${system}.default}/bin/project";
		};
	};
}