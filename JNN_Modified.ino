//Declaring all the variables to be used.....

int fl,l,c,r,fr;
int flm,lm,cm,rm,frm;
int bfl,bl,bc,br,bfr;
int wfl,wl,wc,wr,wfr;

int path[5];
int prev_location = 2,cur_location = 2;
/*fl = far left sensor reading
        l = left sensor reading
        c = center sensor reading
        r = right sensor reading
        fr = far right sensor reading

        Prefix w indicates sensor reading when placed over a white surface
        For Example, wc = reading of center sensor when placed over a white surface

        Prefix b indicates sensor reading when placed over a black surface
        For Example, bc = reading of center sensor when placed over a black surface

        Suffix m indicates that the variable stores average value of black and white reading
        For Example, variable cm stores average value of center sensor readings when placed
        over a black and then a white surface (order of placing doesn't matter, cm will
        store the average value and will be used as a threshold to determine whether the
        sensor is over a white or black background)

        Please NOTE -   The bot follows a BLACK LINE on a WHITE BACKGROUND.....
                        The line-following code is written with the assumption that sensors are
                        calibrated (physically) in such a manner that they read a high when they detect black
                        and a low when they detect white.
                        If the sensors are calibrated differently or are uncalibrated (meaning they read when
                        a high when kept over a white surface and a low when kept over a black surface), then
                        all the > s in the line-following part of the code must be changed to < s and vice versa.
 */


//Assigning pins to the sensor input variables - The variable names are self explanatory.....
//#define far_left_sensor A3
#define left_sensor A6
#define center_sensor A5
#define right_sensor A1
//#define far_right_sensor A0


//Assigning pins to the motor output variables - The variable names are self explanatory.....
#define left_enable 9
#define left_dir 4
#define left_speed 5
#define right_enable 10
#define right_dir 2
#define right_speed 6


#define LED 13


//Setup starts here.....
void setup() {
        //Setting all sensor pins to input....
        //pinMode (far_left_sensor,INPUT);    //farleft sensor
        pinMode (left_sensor,INPUT);    //left sensor
        pinMode (center_sensor,INPUT);    //center sensor
        pinMode (right_sensor,INPUT);    //right sensor
        //pinMode (far_right_sensor,INPUT);    //farright sensor

        //Setting all motor driver pins to output....
        pinMode (left_enable,OUTPUT);    //left motor enable - 10
        pinMode (left_dir,OUTPUT);    //left motor direction - 4
        pinMode (left_speed,OUTPUT);    //left motor speed - 5
        pinMode (right_enable,OUTPUT);    //right motor enable - 9
        pinMode (right_dir,OUTPUT);    //right motor direction - 2
        pinMode (right_speed,OUTPUT);    //right motor speed - 3
        pinMode (LED,OUTPUT);    //Led


        //Calibration starts here.....
        digitalWrite(LED,LOW);
        delay(1000);
        digitalWrite(LED,HIGH);
        //Once LED turns on, place sensors over a black surface.....
        delay(2000);
        bl = analogRead (A1);
        bc = analogRead (A5);
        br = analogRead (A6);
        //bfl = analogRead (A0);
        //bfr = analogRead (A3);
        delay(2000);
        digitalWrite(LED,LOW);
        //Once LED turns off, remove sensors from black surface and wait for LED to turn.....
        delay(2000);
        digitalWrite(LED,HIGH);
        //Once LED turns on, place sensors over a white surface.....
        delay(2000);
        wl = analogRead (A1);
        wc = analogRead (A5);
        wr = analogRead (A6);
        //wfl = analogRead (A0);
        //wfr = analogRead (A3);
        delay(2000);
        digitalWrite(LED,LOW);
        //Once LED turns off, place the bot at the starting of the maze and wait for 3+ seconds.....
        delay(3000);
        //Actual Calibration calculation is done here.....
        //flm  = (bfl+wfl)/2;
        lm = (bl+wl)/2;
        cm = (wc+bc)/2;
        rm = (wr+br)/2;
        //frm = (bfr+wfr)/2;
        //End of Calibration.....

        //Setting the enable pins of the motor driver to high.....
        digitalWrite(9,HIGH);
        digitalWrite(10,HIGH);

        //Now, the bot can begin its motion...
        Serial.begin(9600);
}

//Loop starts here.....
void loop() 
{
        //Line following starts here.....
        read_sensors();
        if(cur_location == 0){//000
            read_sensors();
            while(cur_location == 0){
                spin();
                read_sensors();
            }
        }
        else if(cur_location == 1){//001
        	right();
        }
        else if(cur_location == 2){//010
        	straight();
        }
        else if(cur_location == 3){//011
        	right();
        }
        else if(cur_location == 4){//100
        	left();
        }
        else if(cur_location == 5){//101

        }
        else if(cur_location == 6){//110
        	left();
        }
        else{//111
        	if(prev_location == 111){
        		stop();
        	}
        	else{
        		straight();
        	}
        }
}


//read_sensors function starts here - Function name is self explanatory.....
void read_sensors(){

		prev_location = cur_location;	
        //fl = analogRead(far_left_sensor);
        l = analogRead(left_sensor);
        c = analogRead(center_sensor);
        r = analogRead(right_sensor);
        //fr = analogRead(far_right_sensor);

        //path[4] = fl>flm?1:0;
        //path[3] = l>lm?1:0;
        path[2] = l>lm?1:0;
        path[1] = c>cm?1:0;
        path[0] = r>rm?1:0;
        cur_location = path[2]*4 + path[1]*2 + path[0]*1;
}
void stop(){
        digitalWrite(right_dir,LOW);
        digitalWrite(left_dir,HIGH);
        analogWrite(left_speed,0);
        analogWrite(right_speed,0);
        digitalWrite(LED,HIGH);
        stop();
}
void spin(){
	    digitalWrite(right_dir,HIGH);
	    digitalWrite(left_dir,HIGH);
	    analogWrite(right_speed,100);
	    analogWrite(left_speed,100);
}
void right(){
	    digitalWrite(right_dir,LOW);
	    digitalWrite(left_dir,LOW);
	    analogWrite(right_speed,0);
	    analogWrite(left_speed,300);
}
void straight(){
	    digitalWrite(right_dir,HIGH);
	    digitalWrite(left_dir,LOW);
	    analogWrite(right_speed,200);
	    analogWrite(left_speed,300); 
}
void left(){
	    digitalWrite(right_dir,HIGH);
	    digitalWrite(left_dir,HIGH);
	    analogWrite(right_speed,200);
	    analogWrite(left_speed,0);
}
