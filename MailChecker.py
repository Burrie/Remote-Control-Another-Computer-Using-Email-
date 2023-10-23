import imaplib
import time
import email

server = 'imap.gmail.com'
account = 'mangmaytinhremotecontrol@gmail.com'
password = 'lmlx vrwx cwym hvqz'

imap = imaplib.IMAP4_SSL(host = server, port = 993)
    
imap.login(account, password)

imap.select("Inbox")

#Select unseen message in Inbox to read
cmd = 'start'
while cmd != 'quit':
    res, mailIds = imap.search(None, '(UNSEEN)')

    #Try to read email
    try:
        for id in mailIds[0].decode().split():
            res, mailData = imap.fetch(id, '(RFC822)')
            message = email.message_from_bytes(mailData[0][1])

            #Get message
            for part in message.walk():
                if(part.get_content_type() == 'text/plain'):
                    cmd = part.as_string().splitlines()[-1]
                    print(cmd)

    except Exception as e:
        print("Error type: {}, Error: {}".format(type(e).__name__, e))
        
    time.sleep(0.75)

imap.close()

imap.logout()