Arguments
=========

`-c` or `--config`: Specify config file path. Default is config-unicue.xml.

`-s` or `--silent`: Slient Mode. No GUI.

`-i` or `--input`: Specify an input file path. If no -i or --input, will read the first argument as input file path.

`-o` or `--output`: Specify output file path. Only used under Slient Mode.
If it is not provided,
  * `SilentModeOverwrite` is set as true, will use input file path as output file path.
  * `SilentModeOverwrite` is set as false, will use template defined inside config file to fabricate the  output file path.

`--inencoding`: encoding for input file. Only used under Slient Mode. Only used when the file has no BOM. See `charmap-anisong.xml` (the default charmap config file in `config-unicue.xml`) for detail. Valid name is inside `<name></name>` tag, for example, `Shift-JIS`. (Notice it is case-sensitive). If it is not provided, unicue will find the most possible encoding automatically.

`--outencoding`: encoding for output file. Only used under Slient Mode. Valid name is `utf-8`, `utf-8-nobom`, `utf-16` (short for `utf-16-le`), `utf-16-le` and `utf-16-be`. If it is not provided, will use the setting inside config file.

Configuration inside config file (only for Silent Mode)
=======================================================

`SilentModeForceConvert`: if it is true, and even though no matched encoding is found for input file, unicue will use local codepage to force convert the file. Dangerous if SilentModeOverwrite is set as true and backup is closed, you may lost your file! Recommend set as false.

`SilentModeOverwrite`: See usage of `-o`/`--output`.
 
`SilentModeBackup`: If it set as true, when output file path is same as input file path, unicue will backup the file with extension name `.bak`.
