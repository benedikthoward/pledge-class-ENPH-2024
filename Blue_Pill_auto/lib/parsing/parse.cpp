
#include "parse.h"
#include "globals.h"
#include "map_encoding.h"
/**
 * this parses all of the incoming serial data
 * change of variables, etc...
 * 
 */

/**
 * Position Positions[8] = {
    Position(String("Start"), right, 1, 6, tape), 
    Position(String("Cheese"), right, 1, 6, tape), 
    Position(String("Tomato"), left, 1, 6, tape),
    Position(String("Cutting"), left, 2, 24, tape),
    Position(String("Serving_area"), right, 1, 48, IR_beacon),
    Position(String("Cooktop"), left, 3, 72, tape),
    Position(String("Plates"), left, 3, 90, tape),
    Position(String("Lettuce"), right, 3, 90, tape)
};
Position* goal;
 */

String parse(String message) { //TODO
    if(message.equalsIgnoreCase("run")){
        run = true;
        return "BP recieved: run";
    }else if(message.equalsIgnoreCase("stop")){
        run = false;
        return "BP recieved: stop";
    }else if(message.substring(0,5).equalsIgnoreCase("GOTO:")){
        end_move = message.substring(5);
        
        return "GOTO"+end_move;
    }
    else if(message.substring(0,4).equalsIgnoreCase("move")){

    }
    else if(message.substring(0,5).equalsIgnoreCase("ISAT:")){
        begin_move = message.substring(5,message.length()-1);
        end_move = begin_move;
        return ("BP; ISAT: " + begin_move);
    }
    return "";
}
