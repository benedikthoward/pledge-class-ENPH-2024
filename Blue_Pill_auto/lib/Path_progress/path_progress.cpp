#include "path_progress.h"
#include "globals.h"
#include "Arduino.h"
#include "motor_control.h"

Left_to_right::Left_to_right(bool start)
: right_crossed(start), right_sensors_crossed(0),left_crossed(false), left_sensors_crossed(0){
}

void Left_to_right::execute() {
    if (!right_crossed) {
        check_right_sensors();
    } else if (right_crossed&&!left_crossed) {
        check_left_sensors();
    }
    correct_motor_speeds();
    if(left_crossed && right_crossed && (switch_states[1] || switch_states[3])){ // if both sides crossed and  one of the switches on the right side clicked, succesfully crossed
        done = true;
        for(int i = 0; i<4;i++){
            motorSpeeds[i] = 0;
        }
    }

}

void Left_to_right::check_left_sensors() {
    if (analogRead(sensor_pins_left[1]) > Reflectance_threshold) {
            left_sensors_crossed = 1; // On tape
        } else {
            if (left_sensors_crossed == 1) {
                left_sensors_crossed = 2; // Crossed tape
            }
            else if (left_sensors_crossed == 0) {
                left_sensors_crossed = 0; // Before tape
            }
        }
    left_crossed = (left_sensors_crossed==2);
}

void Left_to_right::check_right_sensors() {
    if (analogRead(sensor_pins_right[1]) > Reflectance_threshold) {
            right_sensors_crossed = 1; // On tape
        } else {
            if (right_sensors_crossed == 1) {
                right_sensors_crossed = 2; // Crossed tape
            }
            else if (right_sensors_crossed == 0) {
                right_sensors_crossed = 0; // Before tape
            }
        }
    right_crossed = (right_sensors_crossed==2);
}

void Left_to_right::correct_motor_speeds() {
    // 1. If none of right side sensors have crossed, then set motor speeds to standard LTR speeds
    if (!right_crossed&&!left_crossed) {
        for (int i = 0; i < 4; i++) {
            motorSpeeds[i] = stdMotorSpeedsLTR[i];
        }
    } 
    // // 2. If all right side sensors have crossed but the left have not yet, then set the motor speeds to standard LTR speeds
    else if (right_crossed && !left_crossed) {
        for (int i = 0; i < 4; i++) {
            motorSpeeds[i] = stdMotorSpeedsLTR[i];
        }
    }

    // 3. If all right side sensors have crossed and left side sensors have crossed, slow motors down
    else if(right_crossed && left_crossed) {
        for (int i = 0; i < 4; i++) {
            motorSpeeds[i] = slowMotorSpeedsLTR[i];
        }
    }

    // 4. if all the sensors have crossed and one of the counter detection switches was pressed set to 0 and finish
    else if((right_crossed && left_crossed)&&(switch_states[1]||switch_states[3])){
        for (int i = 0; i < 4; i++) {
            motorSpeeds[i] = slowMotorSpeedsLTR[i];
        }
        done = true;
    }
}

Right_to_left::Right_to_left(bool start)
: right_crossed(false), right_sensors_crossed(0),left_crossed(start), left_sensors_crossed(0){
}

void Right_to_left::execute() {
    if (!left_crossed) {
        check_left_sensors();
    } else if (left_crossed && !right_crossed) {
        check_right_sensors();
    }
    correct_motor_speeds();
    if(left_crossed && right_crossed && (switch_states[0] && switch_states[2])){
        done = true;
        for (int i = 0; i < 4; i++) {
            motorSpeeds[i] = 0;
        }
    }
}

void Right_to_left::check_left_sensors() {
    if (analogRead(sensor_pins_left[1]) > Reflectance_threshold) {
            left_sensors_crossed = 1; // On tape
        } else {
            if (left_sensors_crossed == 1) {
                left_sensors_crossed = 2; // Crossed tape
            }
            else if (left_sensors_crossed == 0) {
                left_sensors_crossed = 0; // Before tape
            }
        }
    left_crossed = (left_sensors_crossed==2);
}

void Right_to_left::check_right_sensors() {
    if (analogRead(sensor_pins_right[1]) > Reflectance_threshold) {
        right_sensors_crossed = 1; // On tape
    } else {
        if (right_sensors_crossed == 1) {
            right_sensors_crossed = 2; // Crossed tape
        }
        else if (right_sensors_crossed == 0) {
            right_sensors_crossed = 0; // Before tape
        }
    }
    right_crossed = (right_sensors_crossed==2);
}

void Right_to_left::correct_motor_speeds() {
    // 1. If neither the left or the right sensors have crossed, then set motor speeds to standard RTL speeds
    if (!left_crossed&&!right_crossed) {
        for (int i = 0; i < 4; i++) {
            motorSpeeds[i] = stdMotorSpeedsRTL[i];
        }
    } 
    // right has crossed but left has not... should never happen...
    else if (!left_crossed && right_crossed){
        for (int i = 0; i < 4; i++) {
            motorSpeeds[i] = stdMotorSpeedsRTL[i];
        }
        right_crossed = false;
        right_sensors_crossed = 0;
    }
    // // 2. If left sensors have crossed but the right have not yet, then set the motor speeds to standard RTL speeds
    else if (left_crossed && !right_crossed) {
        for (int i = 0; i < 4; i++) {
            motorSpeeds[i] = stdMotorSpeedsRTL[i];
        }
    }

    // 3. If all right side sensors have crossed and left side sensors have crossed, slow motors down
    else if(right_crossed && left_crossed) {
        for (int i = 0; i < 4; i++) {
            motorSpeeds[i] = slowMotorSpeedsRTL[i];
        }
    }

    // 4. if all the sensors have crossed and one of the counter detection switches was pressed set to 0 and finish
    else if((right_crossed && left_crossed)&&(switch_states[0]||switch_states[2])){
        for (int i = 0; i < 4; i++) {
            motorSpeeds[i] = slowMotorSpeedsRTL[i];
        }
        done = true;
    }
}

Along_right_counter::Along_right_counter(bool forward, int tape_markings, bool on_end)
    :tape_markings(tape_markings), forward(forward), on_end(on_end), done_count(0){

    right_sensors_on[0] = 0; 
    right_sensors_on[1] = 0; 
    right_sensors_on[2] = 0; 

    right_sensors_num_crossed[0] = 0;
    right_sensors_num_crossed[1] = 0;
    right_sensors_num_crossed[2] = 0;
} 

void Along_right_counter::execute() {
    check_right_sensors();
    if (forward) { 
        if(!on_end){
            // 1. If foremost left sensor is on desired tape marking, run motors in reverse to stop
            if (right_sensors_num_crossed[2] >= (tape_markings-1) && right_sensors_on[2] == 1) { 
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] =stdMotorSpeedsBackwardRightAC[i]; //stdMotorSpeedsBackward[i];
                }
                done_count = 0;
            }

            // 2. If middle sensor is on desired tape marking, stop motors
            else if (right_sensors_num_crossed[1] >= (tape_markings-1) && right_sensors_on[1] == 1) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = 0;
                }
                done_count++;
                if(done_count>=50){
                    done = true;
                }
            } 

            // 3. If middle sensor has over exceeded desired tape marking, go slowly in reverse
            else if (right_sensors_num_crossed[1] == tape_markings) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] =  slowMotorSpeedsBackwardRightAC[i]; // slowMotorSpeedsBackward[i];
                }
                done_count = 0;
            }

            // 4. If backmost sensor is on desired tape marking, go slowly in reverse
            else if (right_sensors_num_crossed[0] >= (tape_markings-1) && right_sensors_on[0] == 1) { 
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = slowMotorSpeedsBackwardRightAC[i];
                }
                done_count = 0;
            }

            // 5. If backmost sensor has crossed desired tape marking, go slowly in reverse
            else if (right_sensors_num_crossed[0] >= tape_markings) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = slowMotorSpeedsBackwardRightAC[i];
                }
                done_count = 0;
            }

            // 6. If no sensor has crossed the last tape marking, set motor speeds to standard AC_LEFT speeds
            else if (right_sensors_num_crossed[0] <= (tape_markings-1) &&
                right_sensors_num_crossed[1] <= (tape_markings-1) &&
                right_sensors_num_crossed[2] <= (tape_markings-1)) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsForwardRightAC[i]; // stdMotorSpeedsForward[i] ?
                }
            }
        }else{
            if(right_sensors_num_crossed[2]< tape_markings-1){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsForwardRightAC[i];
                }
            }
            else if(right_sensors_num_crossed[2]== tape_markings-1){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = slowMotorSpeedsForwardRightAC[i];
                }
            }
            else if((right_sensors_num_crossed[2]== tape_markings-1&&right_sensors_on[2]==1)||right_sensors_num_crossed[2]== tape_markings){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsBackward[i];
                }
                updateMotorSpeed();
                delay(50);
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = 0;
                }
                done = true;
            }
        }

    } else { // BACKWWARD
        if(!on_end){
            // 1. If backmost left sensor is on desired tape marking, run motors in reverse to stop
            if (right_sensors_num_crossed[0] >= (tape_markings-1) && right_sensors_on[0] == 1) { 
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsForwardRightAC[i];
                }
                done_count = 0;
            }

            // 2. If middle sensor is on desired tape marking, stop motors
            else if (right_sensors_num_crossed[1] >= (tape_markings-1) && right_sensors_on[1] == 1) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = 0;
                }
                done_count++;
                if(done_count>=50){
                    done = true;
                }
            } 

            // 3. If middle sensor has over exceeded desired tape marking, go slowly in reverse
            else if (right_sensors_num_crossed[1] == tape_markings) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] =  slowMotorSpeedsForwardRightAC[i]; 
                }
                done_count = 0;
            }

            // 4. If foremost sensor is on desired tape marking, go slowly in reverse
            else if (right_sensors_num_crossed[2] >= (tape_markings-1) && right_sensors_on[2] == 1) { 
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = slowMotorSpeedsForwardRightAC[i];
                }
                done_count = 0;
            }

            // 5. If foremost sensor has crossed desired tape marking, go slowly in reverse
            else if (right_sensors_num_crossed[2] >= tape_markings) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = slowMotorSpeedsForwardRightAC[i];
                }
                done_count = 0;
            }

            // 6. If no sensor has crossed the last tape marking, set motor speeds to standard AC_LEFT speeds
            else if (right_sensors_num_crossed[0] <= (tape_markings-1) &&
                right_sensors_num_crossed[1] <= (tape_markings-1) &&
                right_sensors_num_crossed[2] <= (tape_markings-1)) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsBackwardRightAC[i];
                }
            }
        }
        else{
            if(right_sensors_num_crossed[0]< tape_markings-1){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsBackwardRightAC[i];
                }
            }
            else if(right_sensors_num_crossed[0]== tape_markings-1){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = slowMotorSpeedsBackwardRightAC[i];
                }
            }
            else if((right_sensors_num_crossed[0]== tape_markings-1&&right_sensors_on[0]==1)||right_sensors_num_crossed[0]== tape_markings){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsForward[i];
                }
                updateMotorSpeed();
                delay(50);
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = 0;
                }
                done = true;
            }
        }
    }
}

void Along_right_counter::check_right_sensors() {
    for (int i = 0; i < 3; i++) {
        bool on = analogRead(sensor_pins_right[i]) > Reflectance_threshold;
        if (on && right_sensors_on[i] == 0) {
            right_sensors_on[i] = 1; // Just touched tape
        } else if (on && right_sensors_on[i] == 1) {
            // nothing, still on tape
        } else if (!on && right_sensors_on[i] == 1) {
            right_sensors_on[i] = 0; // Just left tape
            right_sensors_num_crossed[i]++;
        }
    }
}


/*
LEFT COUNTER
*/

Along_left_counter::Along_left_counter(bool forward, int tape_markings, bool on_end)
    :tape_markings(tape_markings), forward(forward), on_end(on_end), done_count(0){

    left_sensors_on[0] = 0; 
    left_sensors_on[1] = 0; 
    left_sensors_on[2] = 0; 

    left_sensors_num_crossed[0] = 0;
    left_sensors_num_crossed[1] = 0;
    left_sensors_num_crossed[2] = 0;
}

void Along_left_counter::execute() {
    check_left_sensors();
    if (forward) {
        if(!on_end){
            // 1. If foremost left sensor is on desired tape marking, run motors in reverse to stop
            if (left_sensors_num_crossed[2] >= (tape_markings-1) && left_sensors_on[2] == 1) { 
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = 0; // slowMotorSpeedsBackwardLeftAC[i]; //stdMotorSpeedsBackward[i];
                }
                done_count = 0;
            }

            // 2. If middle sensor is on desired tape marking, stop motors
            else if (left_sensors_num_crossed[1] >= (tape_markings-1) && left_sensors_on[1] == 1) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = 0;
                }
                done_count++;
                if(done_count>=50){
                    done = true;
                }
            } 

            // 3. If middle sensor has over exceeded desired tape marking, go slowly in reverse
            else if (left_sensors_num_crossed[1] == tape_markings) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] =  slowMotorSpeedsBackwardLeftAC[i]; // slowMotorSpeedsBackward[i];
                }
                done_count = 0;
            }

            // 6. If no sensor has crossed the last tape marking, set motor speeds to standard AC_LEFT speeds
            else if (left_sensors_num_crossed[0] <= (tape_markings-1) &&
                left_sensors_num_crossed[1] <= (tape_markings-1) &&
                left_sensors_num_crossed[2] <= (tape_markings-1)) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsForwardLeftAC[i]/1.25; // stdMotorSpeedsForward[i] ?
                }

            }
        }else{
            if(left_sensors_num_crossed[2]< tape_markings-1){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsForwardLeftAC[i];
                }
            }
            else if(left_sensors_num_crossed[2]== tape_markings-1){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = slowMotorSpeedsForwardLeftAC[i];
                }
            }
            else if((left_sensors_num_crossed[2]== tape_markings-1&&left_sensors_on[2]==1)||left_sensors_num_crossed[2]== tape_markings){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsBackward[i]*2;
                }
                updateMotorSpeed();
                delay(50);
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = 0;
                }
                done = true;
            }
        }
        
    }else { // backward
        if(!on_end){
            // 1. If backmost left sensor is on desired tape marking, run motors in reverse to stop
            if (left_sensors_num_crossed[0] >= (tape_markings-1) && left_sensors_on[0] == 1) { 
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsForwardLeftAC[i];
                }
                done_count = 0;
            }

            // 2. If middle sensor is on desired tape marking, stop motors
            else if (left_sensors_num_crossed[1] >= (tape_markings-1) && left_sensors_on[1] == 1) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = 0;
                }
                done_count++;
                if(done_count>=50){
                    done = true;
                }
            } 

            // 3. If middle sensor has over exceeded desired tape marking, go slowly in reverse
            else if (left_sensors_num_crossed[1] == tape_markings) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] =  slowMotorSpeedsForwardLeftAC[i]; 
                }
                done_count = 0;
            }

            // 4. If foremost sensor is on desired tape marking, go slowly in reverse
            else if (left_sensors_num_crossed[2] >= (tape_markings-1) && left_sensors_on[2] == 1) { 
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = slowMotorSpeedsForwardLeftAC[i];
                }
                done_count = 0;
            }

            // 5. If foremost sensor has crossed desired tape marking, go slowly in reverse
            else if (left_sensors_num_crossed[2] >= tape_markings) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = slowMotorSpeedsForwardLeftAC[i];
                }
                done_count = 0;
            }

            // 6. If no sensor has crossed the last tape marking, set motor speeds to standard AC_LEFT speeds
            else if (left_sensors_num_crossed[0] <= (tape_markings-1) &&
                left_sensors_num_crossed[1] <= (tape_markings-1) &&
                left_sensors_num_crossed[2] <= (tape_markings-1)) {
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsBackwardLeftAC[i];
                }
            }
        }else{
            if(left_sensors_num_crossed[0]< tape_markings-1){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsBackwardLeftAC[i];
                }
            }
            else if(left_sensors_num_crossed[0]== tape_markings-1){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = slowMotorSpeedsBackwardLeftAC[i];
                }
            }
            else if((left_sensors_num_crossed[0]== tape_markings-1&&left_sensors_on[0]==1)||left_sensors_num_crossed[0]== tape_markings){
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = stdMotorSpeedsForward[i];
                }
                updateMotorSpeed();
                delay(50);
                for (int i = 0; i < 4; i++) {
                    motorSpeeds[i] = 0;
                }
                done = true;
            }
        }
    }
}

void Along_left_counter::check_left_sensors() {
    for (int i = 0; i < 3; i++) {
        bool on = analogRead(sensor_pins_left[i]) >= Reflectance_threshold;
        if (on && left_sensors_on[i] == 0) {
            left_sensors_on[i] = 1; // Just touched tape
        } else if (on && left_sensors_on[i] == 1) {
            // nothing, still on tape
        } else if (!on && left_sensors_on[i] == 1) {
            left_sensors_on[i] = 0; // Just left tape
            left_sensors_num_crossed[i]++;
        }
    }
}

Move::Move(int inches, bool along_right, bool along_left, bool forward)
    :inches(inches), along_right_counter(along_right), along_left_counter(along_left), forward(forward), started(false){
    start_time = 0;
}

void Move::execute(){
    if(!started){
        start_time = millis();
        started = true;

    }
    if((millis())>start_time+100*inches){
        this->done = true;
        for (int i = 0; i <= 3; i++) {
            if(forward){
                motorSpeeds[i] = stdMotorSpeedsBackward[i];
            }else{
                motorSpeeds[i] = stdMotorSpeedsForward[i];
            }
        }
        delay(15);
        for (int i = 0; i <= 3; i++) {
                motorSpeeds[i] = 0;
        }
    }else if(along_left_counter){
        if(forward){
            for (int i = 0; i <= 3; i++) {
                motorSpeeds[i] = stdMotorSpeedsForwardLeftAC[i];
            }
        }else{
            for (int i = 0; i <= 3; i++) {
                motorSpeeds[i] = stdMotorSpeedsBackwardLeftAC[i];
            }
        }
    }else if(along_right_counter){
        if(forward){
            for (int i = 0; i <= 3; i++) {
                motorSpeeds[i] = stdMotorSpeedsForwardRightAC[i];
            }
        }else{
            for (int i = 0; i <= 3; i++) {
                motorSpeeds[i] = stdMotorSpeedsBackwardRightAC[i];
            }
            Serial3.println("I am not being dumb...");
        }
    }else{
        if(forward){
            for (int i = 0; i <= 3; i++) {
                motorSpeeds[i] = stdMotorSpeedsForward[i];
            }
        }else{
            for (int i = 0; i <= 3; i++) {
                motorSpeeds[i] = stdMotorSpeedsBackward[i];
            }
        }
    }
}