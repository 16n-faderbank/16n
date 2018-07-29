# 16n electronics

Documentation for the 16n faderbank.

## Directory contents

* `16n.sch` and `16n.brd` are EAGLE 9.1.1 format schematic and board layout for the project.
* `16n_schematic.pdf` is the schematic in PDF format.
* `bom-csv.csv` is a CSV rendition of a BOM for the project. The parts are all highly generic - any brand of 0805 resistors will do, for instance. For more specific recommendations, the [live BOM at Octopart][octobom] has links to specific components, designed around things easily purchased from Mouser, though note that stock of one manufacturer's parts may not be guaranteed, and you might need to edit your basket.
* `gerbers` contains RS-274x format Gerber files for the current version of the board. These files are named per OSHpark standards, though OSHpark is an expensive place to make a 16n - you may wish to rename before you send them to another manufacturer. Do not assume your fab house can just make these - if your fab house has specific CAM rules, you should generate your own Gerbers from EAGLE.

---

[octobom]: https://octopart.com/bom-tool/nlacEI7z