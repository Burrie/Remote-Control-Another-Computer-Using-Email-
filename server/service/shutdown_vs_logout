import os
import threading

def shutdown():
    os_type = check_os()
    if os_type == 'window':
        os.system("shutdown -s -t 0")
    else:
        os.system("shutdown -h now")

def logout():
    os_type = check_os()
    if os_type == 'window':
        os.system(f"shutdown -l")
    else:
        os.system(f"logout")

def check_os():
    # Hàm kiểm tra hệ điều hành
    pass

def shutdown_logout(function):
    actions = {
        "Shutdown": (shutdown, "Server will shutdown in 30s"),
        "Logout": (logout, "Server will logout in 30s")
    }
    
    result = None
    if function in actions:
        action, message = actions[function]
        result = f"{message}"
        threading.Timer(30, action).start()
    else:
        result = "Invalid function"
        
    return "<div class='mb-2'><b>Shutdown/Logout:</b> " + result + "</div>"
