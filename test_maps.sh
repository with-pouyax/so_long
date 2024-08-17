#!/bin/bash

# Directory containing the map files
MAP_DIR="./maps"

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# Loop through each .ber file in the directory
for map in "$MAP_DIR"/*; do
    map_name=$(basename "$map")
    
    # Skip the `valid-valid-simple-*` files and `ber.ber.ber`
    if [[ "$map_name" == valid-valid-simple-* || "$map_name" == ber.ber.ber ]]; then
        continue
    fi

    # Determine if the map should be valid or invalid based on its name
    if [[ "$map_name" == valid* || "$map_name" == a.ber || "$map_name" == test.ber ]]; then
        expected="valid"
    else
        expected="invalid"
    fi
    
    # Run the so_long program with the map
    ./so_long "$map" > /dev/null 2>&1
    result=$?

    if [ $result -eq 0 ]; then
        outcome="valid"
    else
        outcome="invalid"
    fi

    if [ "$outcome" == "$expected" ]; then
        echo -e "$map_name: ${GREEN}Handled Correctly${NC} (${YELLOW}$outcome${NC})"
    else
        echo -e "$map_name: ${RED}Handled Incorrectly${NC} (${YELLOW}$outcome${NC})"
    fi
done
