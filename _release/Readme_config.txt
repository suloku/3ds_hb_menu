Up to 20 additional paths can be defined. Any more will be ignored.
Paths must start with /.
The order in HBLauncher is sequential.

You don't need to define the /3ds/ folder, it is always the default folder.

Example:

<folders>
	<path>/Homebrew Games/</path>
	<path>/Save Utilities/</path>
	<path>/Emulators/</path>
</folders>

Important note: homebrew that uses hardcoded paths
to the /3ds/ directory won't work on other directories

Config values:
--------------
 - mix_files:
	· 0 = OFF Stray .3dsx files are sorted after homebrew with folder and icon
	· 1 = ON all homebrew is sorted alphabetically
 - case_sensitive (see example)
	· 0 = OFF : capital and lower case letters alike
	· 1 = ON : Capital letters before lower case
 - disable_RF
	· 0 = OFF : region free entry will appear
	· 1 = ON : disables the region free entry

note: the region free entry can be toogled with DPad UP + R.
note2: when the config file is saved, region free setting will be saved as well.
note 3: currently config file is only saved if there are changes to favorites.

Case_sensitive sorting example:

	OFF					ON

	blargsnes			GameYob
	GameYob				SavedataManager
	ftbrony				blargsnes
	SavedataManager		ftbrony
	svdt				svdt

 - current_theme: stores the current theme being used from themes.xml file.

 - menu_pos: 
	· remember_menu = 0 = always start on /3ds/ folder, first entry
	· remember_menu = 1 = start on last folder and entry.
	· Note: this can be toogled on/off by holding Dup+L, so there's really no need to manually edit it.
	· Note: the folder and entry atributes are only there to store the last folder and entry.


Favorites:
----------
They can be set and removed from inside the HBLauncher. Press SELECT.
Press X to enter/exit the favorites menu.
Up to 20 favorites can be added.

Favorites can be manually defined in the config file, example:

IMPORTANT: unlike folders, don't add the last '/' character

		<fav>/3ds/_EMU/GameYob/GameYob.3dsx</fav>
		<fav>/3ds/_EMU/PokeMini</fav>
		<fav>/3ds/_UTIL/CTRXPLORER</fav>
		<fav>/3ds/_UTIL/CTRXplorer/boot.3dsx</fav>
		<fav>/3ds/ftbrony/boot.3dsx</fav>

____________
suloku '15