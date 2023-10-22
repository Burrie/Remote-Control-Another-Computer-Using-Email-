import imaplib
import time
import email

server = 'imap.gmail.com'
account = 'mangmaytinhremotecontrol@gmail.com'
password = 'lmlx vrwx cwym hvqz'

imap = imaplib.IMAP4_SSL(host = server, port = 993)
    
imap.login(account, password)

imap.select("Inbox")

str = ""

while True:
    _, mailIDs = imap.search(None, 'UNSEEN')
    
    for id in mailIDs[0].decode().split():
        responses, mailData = imap.fetch(id, '(BODY)')
        message = email.message_from_bytes(mailData[0][1])
        
        for part in message.walk():
            if part.get_content_type() == "text/plain":
                str = part.as_string()
    
    time.sleep(0.5)

imap.close()

imap.logout()