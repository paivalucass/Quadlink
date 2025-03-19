from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='communication',
            namespace='rcpilot',
            executable='communication',
            name='communication_node'
        )
    ])
    
    
    
    