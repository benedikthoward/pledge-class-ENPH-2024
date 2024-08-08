#include "map_encoding.h"
#include "globals.h"

String toLowerCase(const String &str) {
    String lowerStr = str;
    lowerStr.toLowerCase();
    return lowerStr;
}

bool containsIgnoreCase(const String &str, const String &substr) {
    String lowerStr = toLowerCase(str);
    String lowerSubstr = toLowerCase(substr);
    return lowerStr.indexOf(lowerSubstr) != -1;
}

// Constructor for Path
Path::Path(String start, String end)
    : done(false) {
    if (containsIgnoreCase(start, "Start")) {
        if (containsIgnoreCase(end, "Cheese")) {
            // Move up 1s, move to right counter, move slow down along right counter
            moves_series[0] = new Move(5, false, false, true); 
            moves_series[1] = new Left_to_right(true);
            moves_series[2] = new Along_right_counter(false, 1, true);
        } else if (containsIgnoreCase(end, "Tomato")) {
            moves_series[0] = new Move(3, false, false, true); 
            moves_series[1] = new Right_to_left(true);
            moves_series[2] = new Along_left_counter(false, 1, true);
            // Define moves for Start to Tomato
        } else if (containsIgnoreCase(end, "Cutting")) {
            // Define moves for Start to Cutting
        } else if (containsIgnoreCase(end, "Serving_area")) {
            // Define moves for Start to Serving_area
        } else if (containsIgnoreCase(end, "Cooktop")) {
            // Define moves for Start to Cooktop
        } else if (containsIgnoreCase(end, "Plates")) {
            // Move up 1s, move to left counter, move along counter passing 2 markings fast, slow to the next marking
        } else if (containsIgnoreCase(end, "Lettuce")) {
            // Define moves for Start to Lettuce
        }
    } else if (containsIgnoreCase(start, "Cheese")) {
        if (containsIgnoreCase(end, "Tomato")) {
            // Define moves for Cheese to Tomato
        } else if (containsIgnoreCase(end, "Cutting")) {
            // Define moves for Cheese to Cutting
        } else if (containsIgnoreCase(end, "Serving_area")) {
            // Define moves for Cheese to Serving_area
        } else if (containsIgnoreCase(end, "Cooktop")) {
            // Define moves for Cheese to Cooktop
        } else if (containsIgnoreCase(end, "Plates")) {
            // Move up 1s along right counter, move to left counter, move along counter passing 2 markings fast, slow to the next marking
            moves_series[0] = new Move(3, true, false, true); 
            moves_series[1] = new Right_to_left(false);
            moves_series[2] = new Along_left_counter(true, 3, true);
        } else if (containsIgnoreCase(end, "Lettuce")) {
            // Define moves for Cheese to Lettuce
        }
    } else if (containsIgnoreCase(start, "Tomato")) {
        if (containsIgnoreCase(end, "Cheese")) {
            // Define moves for Tomato to Cheese
        } else if (containsIgnoreCase(end, "Cutting")) {
            // Define moves for Tomato to Cutting
        } else if (containsIgnoreCase(end, "Serving_area")) {
            // Define moves for Tomato to Serving_area
        } else if (containsIgnoreCase(end, "Cooktop")) {
            // Define moves for Tomato to Cooktop
        } else if (containsIgnoreCase(end, "Plates")) {
            // Define moves for Tomato to Plates
        } else if (containsIgnoreCase(end, "Lettuce")) {
            // Define moves for Tomato to Lettuce
            moves_series[0] = new Along_left_counter(true, 2, false);
            moves_series[1] = new Left_to_right(false); 
            moves_series[2] = new Along_right_counter(true, 1, true);
        }
    } else if (containsIgnoreCase(start, "Cutting")) {
        if (containsIgnoreCase(end, "Cheese")) {
            // Define moves for Cutting to Cheese
        } else if (containsIgnoreCase(end, "Tomato")) {
            // Define moves for Cutting to Tomato
        } else if (containsIgnoreCase(end, "Serving_area")) {
            // Define moves for Cutting to Serving_area
        } else if (containsIgnoreCase(end, "Cooktop")) {
            // Define moves for Cutting to Cooktop
        } else if (containsIgnoreCase(end, "Plates")) {
            // Define moves for Cutting to Plates
        } else if (containsIgnoreCase(end, "Lettuce")) {
            // Define moves for Cutting to Lettuce
        }
    } else if (containsIgnoreCase(start, "Serving_area")) {
        if (containsIgnoreCase(end, "Cheese")) {
            // Define moves for Serving_area to Cheese
        } else if (containsIgnoreCase(end, "Tomato")) {
            // Define moves for Serving_area to Tomato
        } else if (containsIgnoreCase(end, "Cutting")) {
            // Define moves for Serving_area to Cutting
        } else if (containsIgnoreCase(end, "Cooktop")) {
            // Define moves for Serving_area to Cooktop
        } else if (containsIgnoreCase(end, "Plates")) {
            // Define moves for Serving_area to Plates
        } else if (containsIgnoreCase(end, "Lettuce")) {
            // Define moves for Serving_area to Lettuce
        }
    } else if (containsIgnoreCase(start, "Cooktop")) {
        if (containsIgnoreCase(end, "Cheese")) {
            // Define moves for Cooktop to Cheese
        } else if (containsIgnoreCase(end, "Tomato")) {
            // Define moves for Cooktop to Tomato
        } else if (containsIgnoreCase(end, "Cutting")) {
            // Define moves for Cooktop to Cutting
        } else if (containsIgnoreCase(end, "Serving_area")) {
            // Define moves for Cooktop to Serving_area
        } else if (containsIgnoreCase(end, "Plates")) {
            // Define moves for Cooktop to Plates
        } else if (containsIgnoreCase(end, "Lettuce")) {
            // Define moves for Cooktop to Lettuce
        }
    } else if (containsIgnoreCase(start, "Plates")) {
        if (containsIgnoreCase(end, "Cheese")) {
            // Define moves for Plates to Cheese
        } else if (containsIgnoreCase(end, "Tomato")) {
            // Define moves for Plates to Tomato
        } else if (containsIgnoreCase(end, "Cutting")) {
            // Define moves for Plates to Cutting
        } else if (containsIgnoreCase(end, "Serving_area")) {
            // Move down 1 marking fast, from left counter to right counter, search for beacon()
            moves_series[0] = new Move(3, false, true, false);
            moves_series[1] = new Left_to_right(false); 
            moves_series[2] = new Along_right_counter(true, 1, true);
            moves_series[3] = new Move(10, true, false, false);
        } else if (containsIgnoreCase(end, "Cooktop")) {
            // Define moves for Plates to Cooktop
        } else if (containsIgnoreCase(end, "Lettuce")) {
            // Define moves for Plates to Lettuce
        }
    } else if (containsIgnoreCase(start, "Lettuce")) {
        if (containsIgnoreCase(end, "Cheese")) {
            // Define moves for Lettuce to Cheese
        } else if (containsIgnoreCase(end, "Tomato")) {
            // Define moves for Lettuce to Tomato
        } else if (containsIgnoreCase(end, "Cutting")) {
            // Define moves for Lettuce to Cutting
        } else if (containsIgnoreCase(end, "Serving_area")) {
            // Define moves for Lettuce to Serving_area
        } else if (containsIgnoreCase(end, "Cooktop")) {
            // Define moves for Lettuce to Cooktop
        } else if (containsIgnoreCase(end, "Plates")) {
            // Define moves for Lettuce to Plates
            moves_series[0] = new Move(3, true, false, false);
            moves_series[1] = new Right_to_left(true); 
            moves_series[2] = new Along_left_counter(true, 1, true);
        }
    }
}

// Destructor for Path to clean up dynamically allocated memory
Path::~Path() {
    for (int i = 0; i < 6; ++i) {
        delete moves_series[i];
    }
}

void Path::execute() {
    bool is_done = true;
    for (int i = 0; i < 6; i++) {
        if (moves_series[i] != nullptr) {
            if (!moves_series[i]->done) {
                is_done = false;
                moves_series[i]->execute();
                break; // Execute only the first move that is not done
            }
        }
    }

    done = is_done;
    if(done){
        Serial3.println("DONE; ISAT: "+ end_move);
        begin_move = end_move;
        for (int i = 0; i < 4; i++) {
            motorSpeeds[i] = 0;
        }
    }
    
}
