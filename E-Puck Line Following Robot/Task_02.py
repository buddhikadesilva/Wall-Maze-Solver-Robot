from controller import Robot, DistanceSensor, Motor

def run_robot(robot):
    time_step=32
    max_speed=6.28
    count=0
    stopMotors=0
    
    # Motors
    left_motor = robot.getDevice('left wheel motor')
    right_motor = robot.getDevice('right wheel motor')
    left_motor.setPosition(float('inf'))
    right_motor.setPosition(float('inf'))
    left_motor.setVelocity(0.0)
    right_motor.setVelocity(0.0)
    
    # IR
    left_ir = robot.getDevice('gs0')
    left_ir.enable(time_step)
    middle_ir = robot.getDevice('gs1')
    middle_ir.enable(time_step)
    right_ir = robot.getDevice('gs2')
    right_ir.enable(time_step)
        
    # Step simulation    
    while robot.step(time_step) != -1: 
        # read ir sensor
        left_ir_value = left_ir.getValue()
        middle_ir_value = middle_ir.getValue()
        right_ir_value = right_ir.getValue()
        print('left: {} middle: {} right: {}'.format(left_ir_value,middle_ir_value,right_ir_value))
        # print('left: {}  right: {}'.format(left_ir_value,right_ir_value))
        
        left_speed = max_speed * 0.25
        right_speed = max_speed * 0.25
        
        if(count>100):
            count= count+1
            right_speed = 5
            left_speed = 0
            print("==============+++++++++================")
            print(count)
            if(count>172):
                right_speed = 1
                left_speed = -1
                if(count>230):
                    right_speed = 3
                    left_speed = 3
                    if(count>242):
                        print("==============++ Stop ++================")
                        stopMotors = 1
            
        else:
            if((left_ir_value<700 and middle_ir_value<700 and right_ir_value<700) or (left_ir_value>700 and right_ir_value<700)):
                print("Go right")
                left_speed = 6
                right_speed = 0
                count= count+1
                print(count)
                
            elif(left_ir_value>700 and middle_ir_value<700 and right_ir_value>700):
                print("Go forword")
                left_speed = 3
                right_speed = 3
                count=0
                
            elif(left_ir_value>700 and middle_ir_value>700 and right_ir_value>700):
                print("Go Reverse")
                left_speed = -max_speed * 0.25
                count=0
                
            elif((left_ir_value<700 and middle_ir_value<700 and right_ir_value<700) or (right_ir_value>700 and left_ir_value<700)):
                print("Go left") 
                print(count)
                left_speed = 0
                right_speed = 6
                count= count+1
                print(count)
            
        
        if(stopMotors==1):       
            left_motor.setVelocity(0)
            right_motor.setVelocity(0)
        else:
            left_motor.setVelocity(left_speed)
            right_motor.setVelocity(right_speed)
          

if __name__ == '__main__':
    my_robot = Robot()
    run_robot(my_robot)