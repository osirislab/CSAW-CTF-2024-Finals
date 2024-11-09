To solve this challenge, the participant must create a polyglot webp/js file to perform cross-site scripting.

- The user has the ability to upload a blog post with the following
    - image (webp only)
    - title
    - description
    - "script pack" - three different style packs to adjust style of blog post

- The image is verified via the Pillow library to make sure its a "webp" file. It does not, however, check for file extensions.

- Since the CSP has scripts src set to 'self', a participant is able to make a new post and point the script_pack src to their image 
(which is both a valid webp/js file)

- Once the post has been created, they can send the post link via the admin bot and exfiltrate the cookie to acces the flag page.


- To exploit:
1. Create a polyglot webp/js image. Participants will likely need to research the webp specification. Ultimately, they need something that passes 
the Pillow library check and can succesfully execute in a browser as the src of a script tag. 

2. Upload this image into a post. It will be given a random UUID identifier within the static/uploads folder. 

3. Send the post link to the admin bot to exfiltrate cookie.

4. Set the exec_cookie to view the flag page endpoint.


Provided Solver (create_webp_polyglot.py):
The script can be used to create a webp/js polyglot. The file be viewed as a valid webp or used within a script src.


Reference Materials:

1. webp container specification:
https://developers.google.com/speed/webp/docs/riff_container#:~:text=WebP%20is%20an%20image%20format,JPEG%2C%20GIF%2C%20and%20PNG.

2. Similar but different exploit: Bypassing CSP with a JPEG/JS polyglot:
https://portswigger.net/research/bypassing-csp-using-polyglot-jpegs

3. Creating a webp/bootable x86 polyglot:
https://research.h4x.cz/html/2023/2023-08-08--webp_polyglot_i-bootable_picture.html