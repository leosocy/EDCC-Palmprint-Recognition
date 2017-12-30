# Code structure

## API

- The implementation of the APIs provided in EDCC.h.

## Pub

- Some common functions and macros.

## IO

- Load the configuration file of the EDCC algorithm `loadConfig` (configuration file in JSON format).
- Loads palmprint training set `loadPalmprintGroup`.
- Load palmprint training set feature library `loadPalmprintFeatureData`.
- Save the palmprint training set signature library `savePalmprintFeatureData`.

## Check

- Check the legitimacy of the EDCC algorithm `checkConfigValid`.
- Check the training set palmprint legality checkPalmprintGroupValid`.
- Check signature library validation `checkPalmprintFeatureData`.

## Core

- `Palmprint`
- `EDCCoding`
- `GaborFilter`
- `PalmprintCode`

## Match

- Two palm prints encode point-to-point matches `matchP2P`.