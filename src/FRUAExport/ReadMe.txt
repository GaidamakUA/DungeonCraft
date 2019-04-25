
These files should all be in the same folder,

FRUAExport.exe 
default_item.dat 
default_items.dat 
default_monsters.dat 
default_spells.dat

The procedure for handling defaults varies a bit for spells, monsters, items so here are the current steps taken by
FRUAExport.

Items:

Search for an existing FRUA items.dat and item.dat files. If they exist they will fully define all items and we use
the data in the files. If the files don't exist, use the default_item.dat and default_items.dat files that are
provided with FRUAExport. These two files contain a full set of default FRUA item definitions. The file search order
is <FRUAFolder>/DISK1, then <DesignFolder>, then the local folder ".".

Monsters:

We read the default_monsters.dat file first to get a definition for all default FRUA monsters, then go searching for
monster*.dat files which will replace default settings. The file search order is <DesignFolder>, then the local
folder ".".

Spells:

We read the spell definitions from the FRUA binary file called ckit.exe. If for some reason that file cannot be
located then we fall back to using the full set of default definitions in the default_spells.dat file. The file
search order is <FRUAFolder>, then the local folder ".".

The DesignFolder is a required command line parameter. The parameter is indicated using '-d'. The FRUAFolder is an
optional command line parameter, and it defaults to the local folder "." if not specified. The parameter is indicated
using '-u'.

Example invoke:

Export a design using the command line, specifying the design folder to parse.

c:\UA\FRUAExport -d .\TestDesign.dsn

Export a design using the command line, specifying the design folder to parse, and specifying the FRUA folder
location

c:\UA\FRUAExport -d .\TestDesign.dsn -u c:\UA

