#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// --- تحديد قنوات المحركات ---
const int PIN_R1_HIP  = 11; 
const int PIN_R3_KNEE = 15; 

const int PIN_L1_HIP  = 1; 
const int PIN_L3_KNEE = 0; 

const int PIN_R2_HIP  = 14; 
const int PIN_R4_KNEE = 12; 

const int PIN_L2_HIP  = 2; 
const int PIN_L4_KNEE = 3; 

// وضعية الاستعداد
const int STANDBY = 307;    
const int JOINT_DELAY = 60; 

// مقادير الحركة
const int HIP_SWING = 50; 
const int KNEE_LIFT = 60; 

// 🎯 --- نظام تصحيح الانحراف لليسار --- 🎯
int LEFT_HIP_OFFSET = 20;  // إعطاء الأرجل اليسرى خطوة أطول بـ 20 نبضة لمنع السحب لليسار
int RIGHT_HIP_OFFSET = 0;  // تصفير تعويض اليمين تماماً

// رفع إضافي لركب اليسار لمنع الاحتكاك بالأرض
const int EXTRA_LEFT_LIFT = 15; 

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); 
  
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50); 
  
  Serial.println("Spider Robot Booting... Anti-Left Drift Activated!");
  
  robotStandby();
  delay(2000); 
}

void loop() {
  moveForwardContinuous();
}

// ==========================================
//            دوال التحكم
// ==========================================

void moveJoint(int pin, int pulse) {
  pulse = constrain(pulse, 102, 512);
  pwm.setPWM(pin, 0, pulse);
}

void robotStandby() {
  int hips[] = {PIN_R1_HIP, PIN_L1_HIP, PIN_R2_HIP, PIN_L2_HIP};
  int knees[] = {PIN_R3_KNEE, PIN_L3_KNEE, PIN_R4_KNEE, PIN_L4_KNEE};
  
  for(int i = 0; i < 4; i++) {
    moveJoint(hips[i], STANDBY);
    delay(JOINT_DELAY); 
    moveJoint(knees[i], STANDBY);
    delay(JOINT_DELAY);
  }
}

void moveForwardContinuous() {
  // === الخطوة 1: تحريك الزوج القطري الأول (R1 و L2) ===
  // 1. ارفع الركب (مع إعطاء الركبة اليسرى رفعاً إضافياً لإنهاء الاحتكاك)
  moveJoint(PIN_R3_KNEE, STANDBY - KNEE_LIFT); delay(JOINT_DELAY);
  moveJoint(PIN_L4_KNEE, STANDBY + (KNEE_LIFT + EXTRA_LEFT_LIFT)); delay(JOINT_DELAY);
  
  // 2. قدم الأوراك للأمام (إعطاء الرجل اليسرى الدفعة التصحيحية)
  moveJoint(PIN_R1_HIP, STANDBY + HIP_SWING); delay(JOINT_DELAY);
  moveJoint(PIN_L2_HIP, (STANDBY - HIP_SWING) - LEFT_HIP_OFFSET); delay(JOINT_DELAY);
  
  // 3. نزل الركب لتثبيت الخطوة
  moveJoint(PIN_R3_KNEE, STANDBY); delay(JOINT_DELAY);
  moveJoint(PIN_L4_KNEE, STANDBY); delay(JOINT_DELAY);

  // === الخطوة 2: تحريك الزوج القطري الثاني (L1 و R2) ===
  // 1. ارفع الركب
  moveJoint(PIN_L3_KNEE, STANDBY + (KNEE_LIFT + EXTRA_LEFT_LIFT)); delay(JOINT_DELAY);
  moveJoint(PIN_R4_KNEE, STANDBY - KNEE_LIFT); delay(JOINT_DELAY);
  
  // 2. قدم الأوراك للأمام
  moveJoint(PIN_L1_HIP, (STANDBY - HIP_SWING) - LEFT_HIP_OFFSET); delay(JOINT_DELAY);
  moveJoint(PIN_R2_HIP, STANDBY + HIP_SWING); delay(JOINT_DELAY);
  
  // 3. نزل الركب لتثبيت الخطوة
  moveJoint(PIN_L3_KNEE, STANDBY); delay(JOINT_DELAY);
  moveJoint(PIN_R4_KNEE, STANDBY); delay(JOINT_DELAY);

  // === الخطوة 3: سحب الجسم للأمام ===
  moveJoint(PIN_R1_HIP, STANDBY); delay(JOINT_DELAY);
  moveJoint(PIN_L2_HIP, STANDBY); delay(JOINT_DELAY);
  moveJoint(PIN_L1_HIP, STANDBY); delay(JOINT_DELAY);
  moveJoint(PIN_R2_HIP, STANDBY); delay(JOINT_DELAY);
  
  delay(100); 
}