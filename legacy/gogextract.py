#!/usr/bin/env python3
import io
import re
import shutil
import os
from os import path
import sys

# Constante
# Expression reguliere
# Recherche la chaine de caracteres (pas de despcialisaiton) : filesizes="xxxx"
# Et extrait le nombre correspondant a la taille (en mégabits) de de l'installeur
FILESIZE_RE = re.compile(r'filesizes="(\d+?)"')
# Recherche la chaine de caracteres : offset=`head -n xxxx "$0"
# Et extrait le nombre
OFFSET_RE = re.compile(r'offset=`head -n (\d+?) "\$0"')


# Controle du passage des arguments
# Un seul argument extrait tout dans le chemin
if len(sys.argv) == 2:
    input_path = sys.argv[1]
    output_path = "./"
elif len(sys.argv) == 3:
    input_path = sys.argv[1]
    output_path = sys.argv[2]
else:
    print("Usage: {} <input file> <output dir>".format(sys.argv[0]))
    exit(1)

# Ouvre le fichier en lecture binaire
game_bin = open(input_path, "rb")
os.makedirs(output_path, exist_ok=True)

# [legacy] Read the first 10kb so we can determine the script line number
# Lis les 10k permiers bits, "nous determinons le nombre de ligne du script ?"
beginning = game_bin.read(10240).decode("utf-8", errors="ignore")
offset_match = OFFSET_RE.search(beginning)
script_lines = int(offset_match.group(1))

# Read the number of lines to determine the script size
game_bin.seek(0, io.SEEK_SET)
for l in range(0, script_lines):
    game_bin.readline()
script_size = game_bin.tell()
print("Makeself script size:", script_size)

# Read the script
game_bin.seek(0, io.SEEK_SET)
script_bin = game_bin.read(script_size)
with open(path.join(output_path, "unpacker.sh"), "wb") as script_f:
    script_f.write(script_bin)
script = script_bin.decode("utf-8")

# Filesize is for the MojoSetup archive, not the actual game data
filesize_match = FILESIZE_RE.search(script)
filesize = int(filesize_match.group(1))
print("MojoSetup archive size:", filesize)

# Extract the setup archive
game_bin.seek(script_size, io.SEEK_SET)
with open(path.join(output_path, "mojosetup.tar.gz"), "wb") as setup_f:
    setup_f.write(game_bin.read(filesize))

# Extract the game data archive
dataoffset = script_size + filesize
game_bin.seek(dataoffset, io.SEEK_SET)
with open(path.join(output_path, "data.zip"), "wb") as datafile:
    shutil.copyfileobj(game_bin, datafile)
