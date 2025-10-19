//超音波控制步進馬達
//預設馬達逆時針轉動，當監控視窗按下1時順時針運轉，按下2時逆時針運轉，按下3時停止運轉
//物體距離愈遠馬達旋轉愈慢，距離愈近旋轉愈快

int trigPin = 12;
int echoPin = 11;
int t; // 控制馬達轉速的延遲時間
float duration;
float cm;
float soundspeed = 0.034;
bool Active = true;
bool clockwise = false;    // 預設逆時針旋轉

void setup() {
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);

    Serial.println("輸入指令控制馬達：");
    Serial.println("1 = 順時針");
    Serial.println("2 = 逆時針");
    Serial.println("3 = 停止");
}

void loop() {
    // 超音波測距
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    cm = duration * soundspeed / 2;
    
    if(cm > 0 && cm < 200) {
        // 根據距離調整轉速：距離越近，轉速越快（延遲時間越短）
        t = map(cm, 2, 100, 1, 50); // 距離2-100cm對應延遲1-50ms
        t = constrain(t, 1, 50); // 限制延遲時間範圍
    } else {
        t = 50; // 超出範圍時使用最慢轉速
    }

    // 檢查電腦輸入
    if (Serial.available() > 0) {
        int command = Serial.parseInt();
        
        if(command == 1) {
            clockwise = true;
            Active = true;
            Serial.println("順時針運轉");
        } else if(command == 2) {
            clockwise = false;
            Active = true;
            Serial.println("逆時針運轉");
        } else if(command == 3) {
            Active = false;
            Serial.println("停止運轉");
            // 停止所有線圈
            digitalWrite(7, 0);
            digitalWrite(8, 0);
            digitalWrite(9, 0);
            digitalWrite(10, 0);
        }
    }

    // 控制步進馬達
    if(Active) {
        if(clockwise) {
            // 順時針旋轉
            digitalWrite(7, 0); 
            digitalWrite(8, 0); 
            digitalWrite(9, 0); 
            digitalWrite(10, 1); 
            delay(t);  
            
            digitalWrite(7, 0); 
            digitalWrite(8, 0); 
            digitalWrite(9, 1); 
            digitalWrite(10, 0); 
            delay(t);  
            
            digitalWrite(7, 0); 
            digitalWrite(8, 1); 
            digitalWrite(9, 0); 
            digitalWrite(10, 0); 
            delay(t); 
            
            digitalWrite(7, 1); 
            digitalWrite(8, 0); 
            digitalWrite(9, 0); 
            digitalWrite(10, 0); 
            delay(t);
        } else {
            // 逆時針旋轉
            digitalWrite(7, 1); 
            digitalWrite(8, 0); 
            digitalWrite(9, 0); 
            digitalWrite(10, 0); 
            delay(t);  
            
            digitalWrite(7, 0); 
            digitalWrite(8, 1); 
            digitalWrite(9, 0); 
            digitalWrite(10, 0); 
            delay(t); 
            
            digitalWrite(7, 0); 
            digitalWrite(8, 0); 
            digitalWrite(9, 1); 
            digitalWrite(10, 0); 
            delay(t); 
            
            digitalWrite(7, 0); 
            digitalWrite(8, 0); 
            digitalWrite(9, 0); 
            digitalWrite(10, 1); 
            delay(t);
        }
        
        // 顯示距離和轉速資訊
        Serial.print("Distance: ");
        Serial.print(cm);
        Serial.print(" cm, Speed delay: ");
        Serial.print(t);
        Serial.println(" ms");
    }
}
