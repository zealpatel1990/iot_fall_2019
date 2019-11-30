void test(int val){ 
  int last_value = servo_height.read();
  Serial.println(servo_height.read()); 
  while (last_value!=val){
      servo_height.write(last_value);
      delay(20);
      if (last_value>val) {
        last_value-=1;
      }
      else {
        last_value+=1;
      }
  }
}
