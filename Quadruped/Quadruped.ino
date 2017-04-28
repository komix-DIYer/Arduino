/*
 * Motion control program for a quadruped robot
 * by komix
 * 2014.02.22
 * Reference：http://www.thingiverse.com/thing:38159
 */

#include <Servo.h>
#include <PS2X_lib.h>

Servo frh;   int home_frh = 91 ;   // front right hip
Servo frl;   int home_frl = 12;   // front right leg
Servo flh;   int home_flh = 95;   // front left hip
Servo fll;   int home_fll =173;   // front left leg
Servo rlh;   int home_rlh = 90;   // rear left hip
Servo rll;   int home_rll = 27;   // rear left leg
Servo rrh;   int home_rrh = 90;   // rear right hip
Servo rrl;   int home_rrl =168;   // rear right leg
PS2X ps2x;

int error = 0;
byte type = 0;
byte vibrate = 0;
int do_flag = 0;



void setup(){
  
  Serial.begin(9600);
  
  do_flag = 1;
  
  frh.attach(2);  frh.write(home_frh);
  frl.attach(3);  frl.write(home_frl);
  flh.attach(4);  flh.write(home_flh);
  fll.attach(5);  fll.write(home_fll);
  rlh.attach(6);  rlh.write(home_rlh);
  rll.attach(7);  rll.write(home_rll);
  rrh.attach(8);  rrh.write(home_rrh);
  rrl.attach(9);  rrl.write(home_rrl);
  
  SetupPS2X();
  
}

void idle(){delay(300);}

void sitdown(){ // X 字型に足を投げ出して寝転ぶ
  for(int i=0;i<31;i++){
    frh.write(home_frh+i);
    flh.write(home_flh-i);
    rlh.write(home_rlh+i);
    rrh.write(home_rrh-i);
    frl.write(home_frl+3*i);
    fll.write(home_fll-3*i);
    rll.write(home_rll+3*i);
    rrl.write(home_rrl-3*i);
    delay(100);
  }
}

void standup(){
  for(int i=30;i>0;i--){
    frh.write(home_frh+i);
    flh.write(home_flh-i);
    rlh.write(home_rlh+i);
    rrh.write(home_rrh-i);
    frl.write(home_frl+3*i);
    fll.write(home_fll-3*i);
    rll.write(home_rll+3*i);
    rrl.write(home_rrl-3*i);
    delay(100);
  }
}

void stand(){
  frh.write(home_frh);
  frl.write(home_frl);
  flh.write(home_flh);
  fll.write(home_fll);
  rlh.write(home_rlh);
  rll.write(home_rll);
  rrh.write(home_rrh);
  rrl.write(home_rrl);
}

void forward(){
  frl.write(home_frl+30);
  rll.write(home_rll+30);
  delay(100);
  frh.write(home_frh+30);
  rrh.write(home_rrh-30);
  flh.write(home_flh);
  rlh.write(home_rlh);
  delay(100);
  frl.write(home_frl);
  rll.write(home_rll);
  idle();
  
  fll.write(home_fll-30);
  rrl.write(home_rrl-30);
  delay(100);
  frh.write(home_frh);
  rrh.write(home_rrh);
  flh.write(home_flh-30);
  rlh.write(home_rlh+30);
  delay(100);
  fll.write(home_fll);
  rrl.write(home_rrl);
  idle();
}

void backward(){
  frl.write(home_frl+30);
  rll.write(home_rll+30);
  delay(100);
  frh.write(home_frh);
  rrh.write(home_rrh);
  flh.write(home_flh-30);
  rlh.write(home_rlh+30);
  delay(100);
  frl.write(home_frl);
  rll.write(home_rll);
  idle();
  
  fll.write(home_fll-30);
  rrl.write(home_rrl-30);
  delay(100);
  frh.write(home_frh+30);
  rrh.write(home_rrh-30);
  flh.write(home_flh);
  rlh.write(home_rlh);
  delay(100);
  fll.write(home_fll);
  rrl.write(home_rrl);
  idle();
}

void rightturn(){
  fll.write(home_fll-30);
  rrl.write(home_rrl-30);
  delay(100);
  frh.write(home_frh+30);
  rrh.write(home_rrh-30);
  flh.write(home_flh-30);
  rlh.write(home_rlh+30);
  delay(100);
  fll.write(home_fll);
  rrl.write(home_rrl);
  idle();
  
  frl.write(home_frl+30);
  rll.write(home_rll+30);
  delay(100);
  frh.write(home_frh);
  rrh.write(home_rrh);
  flh.write(home_flh);
  rlh.write(home_rlh);
  delay(100);
  frl.write(home_frl);
  rll.write(home_rll);
  idle();
}

void leftturn(){
  frl.write(home_frl+30);
  rll.write(home_rll+30);
  delay(100);
  frh.write(home_frh+30);
  rrh.write(home_rrh-30);
  flh.write(home_flh-30);
  rlh.write(home_rlh+30);
  delay(100);
  frl.write(home_frl);
  rll.write(home_rll);
  idle();
  
  fll.write(home_fll-30);
  rrl.write(home_rrl-30);
  delay(100);
  frh.write(home_frh);
  rrh.write(home_rrh);
  flh.write(home_flh);
  rlh.write(home_rlh);
  delay(100);
  fll.write(home_fll);
  rrl.write(home_rrl);
  idle();
}

void SetupPS2X(){
  
  error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  
  if(error == 0){
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
  }
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  // Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }
}

void GetPS2X(){
  if(error == 1) return; // skip loop if no controller found
  if(type  == 2){ // Guitar Hero Controller
  }else{ //DualShock Controller
    ps2x.read_gamepad(false, vibrate); // read controller and set large motor to spin at 'vibrate' speed
  }
  delay(50);
}



void loop(){
  GetPS2X();

  if(ps2x.ButtonPressed(PSB_L1) && do_flag==-1){
    standup();
    do_flag = 1;
  }
  
  if(ps2x.ButtonPressed(PSB_L2) && do_flag==1){
    sitdown();
    do_flag=-1;
  }
  
  if(do_flag==1){
          if(ps2x.Analog(PSS_LY)<  5){ forward();   // 左スティック上で前進
    }else if(ps2x.Analog(PSS_LY)>250){ backward();  // 左スティック下で後退
    }else if(ps2x.Analog(PSS_LX)>250){ rightturn(); // 左スティック右で右旋回
    }else if(ps2x.Analog(PSS_LX)<  5){ leftturn();  // 左スティック左で左旋回
    }else{
      stand();
    }
  }
}

