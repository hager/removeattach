   removeattach - Remove all attachments from a MIME encoded message

   PREPREQUISITES: glib-2.0, gmime-2.4, zlib

   TO BUILD: gcc `pkg-config --cflags --libs glib-2.0` `pkg-config --cflags --libs gmime-2.4` removeattach.c -o removeattach 

   TO RUN: pipe mime encoded e-mail message from procmailrc to
   removeattach.

   SAMPLE .procmailrc entry for Gmail illegal attachment bounce entries.

:0c:
*
{
        # Filer attachments from Gmail illegal attachment bounces
        :0Bf
	* 552-5.7.0
        |/usr/bin/removeattach 

	# Forward mail to gmail
        :0
	! user@example.com
}
