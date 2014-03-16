#!/usr/bin/env python
import cgi
import urllib
import cgitb; cgitb.enable()  # for troubleshooting
import os
from hashlib import sha256
import hmac
from collections import defaultdict

SHARED_SECRET_KEY = 'BESURETODRINKYOUROVALTINE'
def authenticate_with_codepuppy(token, forward_url, verify_url):
  sunetid = os.environ['REMOTE_USER']
  h = hmac.new(SHARED_SECRET_KEY, "{0},{1}".format(token, sunetid), sha256)
  url = "{0}?token={1}&user_system_id={2}&hmac={3}&forward_url={4}"
  token = urllib.quote(token)
  sunetid = urllib.quote(sunetid)
  hmac_param = urllib.quote(h.hexdigest())
  forward_url = urllib.quote(forward_url)
  url = url.format(verify_url, token, sunetid, hmac_param, forward_url)
  print("Location:{0}".format(url))
  print("")

def main():
  form = cgi.FieldStorage()
  token = form.getvalue("token")
  forward_url = form.getvalue("forward_url")
  verify_url = form.getvalue("verify_url")
  if token == None or forward_url == None or verify_url == None:
    print("Content-Type: text/plain")
    print("")
    print("unauthorized user plz go")
    return
  authenticate_with_codepuppy(token, forward_url, verify_url)

if __name__ == '__main__':
  main()
