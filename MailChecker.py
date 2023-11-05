import imaplib
import time
import email

server = 'imap.gmail.com'
account = 'mangmaytinhremotecontrol@gmail.com'
password = 'lmlx vrwx cwym hvqz'

imap = imaplib.IMAP4_SSL(host = server, port = 993)
    
imap.login(account, password)

#Check mail and implement
def CheckAndDo(cmd):
    if(cmd == 'list applications'):
        print('applications: app A, app B, app C')
    elif(cmd == 'list processes'):  
        print('processes: pA, pB, pC')
    elif(cmd == 'shut down'):  
        print('shutdown')
    elif(cmd == 'keylogger'):  
        print('keylogger: A, B, C, D')
    elif(cmd == 'screenshot'):  
        print('photo')
    else:
        print(cmd)

#Select unseen message in Inbox to read
cmd = 'start'
while cmd != 'quit':
    imap.select("Inbox")
    res, mailIds = imap.search(None, '(UNSEEN)')
    print(res)

    #Try to read email
    for id in mailIds[0].decode().split():
        res, mailData = imap.fetch(id, '(RFC822)')
        message = email.message_from_string(mailData[0][1].decode())

        #Get message
        for part in message.walk():
            if(part.get_content_type() == 'text/plain'):
                cmd = part.as_string().splitlines()[-1]
                CheckAndDo(cmd)
                    
    
    imap.close()
    
    time.sleep(0.75)

print("Byee")

imap.logout()