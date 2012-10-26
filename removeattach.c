
/* 
   removeattach - Remove all attachments from a MIME encoded message

   PREPREQUISITES: glib-2.0, gmime-2.4, zlib

   TO BUILD: gcc `pkg-config --cflags --libs glib-2.0` `pkg-config --cflags --libs gmime-2.4` removeattach.c -o removeattach 

   TO RUN: pipe mime encoded e-mail message from procmailrc to
   removeattach.

*/

#include <stdio.h>
#include <glib.h>
#include <gmime/gmime.h>

int is_attachment(GMimeObject *part) {
  if (g_mime_object_get_content_disposition_parameter(part, "filename") ||
      g_mime_object_get_content_type_parameter(part, "name"))
    return 1;
  else
    return 0;
}

int remove_part(GMimeObject *parent, GMimeObject *part) {
  int attachment = is_attachment(part);

  if (attachment && GMIME_IS_MULTIPART(parent)) {
    g_mime_multipart_remove((GMimeMultipart*)parent, part);
    return 1;
  }
  else if (attachment && GMIME_IS_MESSAGE(parent)) { // I'm not sure if this could ever occur or even makes sense.
    GMimePart *new_part;
    new_part = g_mime_part_new_with_type("text", "plain");
    g_mime_part_set_content_object(new_part,
				   g_mime_data_wrapper_new_with_stream(g_mime_stream_mem_new_with_buffer("Attachment Removed.", 19),
								       GMIME_CONTENT_ENCODING_DEFAULT));
    g_mime_message_set_mime_part((GMimeMessage*)parent, (GMimeObject*)new_part);
    g_object_unref(part);
  }
  else if (GMIME_IS_MESSAGE_PART(part)) {
    remove_message(g_mime_message_part_get_message((GMimeMessagePart*)part));
  }

  return 0;
}

void remove_multipart(GMimeMultipart *multipart) {
  int i=0;

  while (i < g_mime_multipart_get_count(multipart)) {
    int removed = remove_part((GMimeObject*)multipart, g_mime_multipart_get_part(multipart, i));
    if (!removed)
      i++;
  }
}

void remove_message(GMimeMessage *message) {
  GMimeObject *part;

  part = g_mime_message_get_mime_part(message);

  if (GMIME_IS_MULTIPART(part))
    remove_multipart((GMimeMultipart*)part);
  else
    remove_part((GMimeObject*)message, part);

}

int main(int argc, char **argv) {
  GByteArray *garray;
  GMimeStream *stream;
  GMimeParser *parse;
  GMimeMessage *message;
  char buf[1];
  int len;

  g_mime_init(0);

  garray = g_byte_array_new();
  
  while (!feof(stdin)) {
    len = fread(buf, 1, 1, stdin);
    if (len == 1)
      g_byte_array_append(garray, buf, len);
  }

  stream = g_mime_stream_mem_new_with_byte_array(garray);
  parse = g_mime_parser_new_with_stream(stream);
  message = g_mime_parser_construct_message(parse);

  remove_message(message);

  printf(g_mime_object_to_string((GMimeObject*)message));
  
  g_object_unref(message);
  g_object_unref(parse);
  g_object_unref(stream);

  return 0;
}
